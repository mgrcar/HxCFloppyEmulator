/*
//
// Copyright (C) 2006-2014 Jean-Fran?ois DEL NERO
//
// This file is part of the HxCFloppyEmulator library
//
// HxCFloppyEmulator may be used and distributed without restriction provided
// that this copyright statement is not removed from the file and that any
// derivative work contains the original copyright notice and the associated
// disclaimer.
//
// HxCFloppyEmulator is free software; you can redistribute it
// and/or modify  it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// HxCFloppyEmulator is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HxCFloppyEmulator; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
*/
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-----------H----H--X----X-----CCCCC----22222----0000-----0000------11----------//
//----------H----H----X-X-----C--------------2---0----0---0----0--1--1-----------//
//---------HHHHHH-----X------C----------22222---0----0---0----0-----1------------//
//--------H----H----X--X----C----------2-------0----0---0----0-----1-------------//
//-------H----H---X-----X---CCCCC-----222222----0000-----0000----1111------------//
//-------------------------------------------------------------------------------//
//----------------------------------------------------- http://hxc2001.free.fr --//
///////////////////////////////////////////////////////////////////////////////////
// File : flppcm_loader.c
// Contains: FLP PC Magazine floppy image loader
//
// Written by:	DEL NERO Jean Francois
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "libhxcfe.h"

#include "floppy_loader.h"
#include "floppy_utils.h"

#include "flppcm_loader.h"

#include "flppcmfileformat.h"

#include "libhxcadaptor.h"


int FLPPCM_libIsValidDiskFile(HXCFLOPPYEMULATOR* floppycontext,char * imgfile)
{
	int filesize;
	FILE *f;
	flp_header_t flp_header;

	floppycontext->hxc_printf(MSG_DEBUG,"FLPPCM_libIsValidDiskFile");

	if(hxc_checkfileext(imgfile,"flp"))
	{
		filesize=hxc_getfilesize(imgfile);
		if(filesize<0)
		{
			floppycontext->hxc_printf(MSG_ERROR,"FLPPCM_libIsValidDiskFile : Cannot open %s !",imgfile);
			return HXCFE_ACCESSERROR;
		}

		memset(&flp_header,0,sizeof(flp_header_t));
		f=hxc_fopen(imgfile,"rb");
		if(f==NULL)
		{
			return HXCFE_ACCESSERROR;
		}
		fread(&flp_header,sizeof(flp_header_t),1,f);
		hxc_fclose(f);

		if(strncmp((char*)flp_header.hsign,"PCM",sizeof(flp_header.hsign)))
		{
			floppycontext->hxc_printf(MSG_DEBUG,"FLPPCM_libIsValidDiskFile : non FLP file - bad header !");
			return HXCFE_BADFILE;
		}

		floppycontext->hxc_printf(MSG_DEBUG,"FLPPCM_libIsValidDiskFile : FLP file !");
		return HXCFE_VALIDFILE;
	}
	else
	{
		floppycontext->hxc_printf(MSG_DEBUG,"FLPPCM_libIsValidDiskFile : non FLP file !");
		return HXCFE_BADFILE;
	}

	return HXCFE_BADPARAMETER;
}

int FLPPCM_libLoad_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppydisk,char * imgfile,void * parameters)
{

	FILE * f;
	unsigned int i,j;
	unsigned int file_offset;
	unsigned char* trackdata;
	unsigned char gap3len,interleave;
	unsigned short rpm;
	unsigned short sectorsize;
	unsigned char trackformat;
	CYLINDER* currentcylinder;
	flp_header_t flp_header;

	floppycontext->hxc_printf(MSG_DEBUG,"FLPPCM_libLoad_DiskFile %s",imgfile);

	f=hxc_fopen(imgfile,"rb");
	if(f==NULL)
	{
		floppycontext->hxc_printf(MSG_ERROR,"Cannot open %s !",imgfile);
		return HXCFE_ACCESSERROR;
	}

	memset(&flp_header,0,sizeof(flp_header_t));

	fread(&flp_header,sizeof(flp_header_t),1,f);

	if(!strncmp((char*)flp_header.hsign,"PCM",sizeof(flp_header.hsign)))
	{
		sectorsize = flp_header.sectorsize;
		floppydisk->floppyNumberOfTrack = flp_header.nbtracks;
		floppydisk->floppyNumberOfSide = (unsigned char)flp_header.nbsides;
		floppydisk->floppySectorPerTrack = flp_header.nbsectors;
		floppydisk->floppyBitRate = 250000;
		floppydisk->floppyiftype = IBMPC_DD_FLOPPYMODE;
		if( floppydisk->floppySectorPerTrack >= 15 && sectorsize >=512 )
		{
			floppydisk->floppyBitRate = 500000;
			floppydisk->floppyiftype=IBMPC_HD_FLOPPYMODE;
		}

		rpm = 300;
		gap3len = 0xFF;
		interleave = 1;

		floppydisk->tracks=(CYLINDER**)malloc(sizeof(CYLINDER*)*floppydisk->floppyNumberOfTrack);

		floppycontext->hxc_printf(MSG_DEBUG,"rpm %d bitrate:%d track:%d side:%d sector:%d",rpm,floppydisk->floppyBitRate,floppydisk->floppyNumberOfTrack,floppydisk->floppyNumberOfSide,floppydisk->floppySectorPerTrack);

		trackdata=(unsigned char*)malloc(sectorsize*floppydisk->floppySectorPerTrack);

		trackformat=IBMFORMAT_DD;

		for(j=0;j<floppydisk->floppyNumberOfTrack;j++)
		{
			floppydisk->tracks[j]=allocCylinderEntry(rpm,floppydisk->floppyNumberOfSide);
			currentcylinder=floppydisk->tracks[j];

			for(i=0;i<floppydisk->floppyNumberOfSide;i++)
			{
				file_offset=(sectorsize*(j*floppydisk->floppySectorPerTrack*floppydisk->floppyNumberOfSide))+
							(sectorsize*(floppydisk->floppySectorPerTrack)*i)+
							sizeof(flp_header_t);

				fseek (f , file_offset , SEEK_SET);
				fread(trackdata,sectorsize*floppydisk->floppySectorPerTrack,1,f);

				currentcylinder->sides[i]=tg_generateTrack(trackdata,sectorsize,floppydisk->floppySectorPerTrack,(unsigned char)j,(unsigned char)i,1,interleave,(unsigned char)(0),floppydisk->floppyBitRate,currentcylinder->floppyRPM,trackformat,gap3len,0,2500|REVERTED_INDEX,-2500);
			}
		}

		free(trackdata);

		floppycontext->hxc_printf(MSG_INFO_1,"track file successfully loaded and encoded!");

		hxc_fclose(f);
		return HXCFE_NOERROR;
	}

	floppycontext->hxc_printf(MSG_ERROR,"Bad FLP header");
	hxc_fclose(f);

	return HXCFE_BADFILE;
}

int FLPPCM_libGetPluginInfo(HXCFLOPPYEMULATOR* floppycontext,unsigned long infotype,void * returnvalue)
{
	static const char plug_id[]="FLP_IMG";
	static const char plug_desc[]="FLP PC Magazine image Loader";
	static const char plug_ext[]="flp";

	plugins_ptr plug_funcs=
	{
		(ISVALIDDISKFILE)	FLPPCM_libIsValidDiskFile,
		(LOADDISKFILE)		FLPPCM_libLoad_DiskFile,
		(WRITEDISKFILE)		0,
		(GETPLUGININFOS)	FLPPCM_libGetPluginInfo
	};

	return libGetPluginInfo(
			floppycontext,
			infotype,
			returnvalue,
			plug_id,
			plug_desc,
			&plug_funcs,
			plug_ext
			);
}

