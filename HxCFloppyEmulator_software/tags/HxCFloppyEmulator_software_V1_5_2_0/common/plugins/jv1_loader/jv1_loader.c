/*
//
// Copyright (C) 2006, 2007, 2008, 2009 Jean-Fran?ois DEL NERO
//
// This file is part of HxCFloppyEmulator.
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
// File : JV1_DiskFile.c
// Contains: JV1 TRS80 floppy image loader and plugins interfaces
//
// Written by:	DEL NERO Jean Francois
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "hxc_floppy_emulator.h"
#include "internal_floppy.h"
#include "floppy_loader.h"
#include "floppy_utils.h"

#include "../common/crc.h"
#include "../common/iso_ibm_track.h"

#include "jv1_loader.h"

#include "../common/os_api.h"



int JV1_libIsValidDiskFile(HXCFLOPPYEMULATOR* floppycontext,char * imgfile)
{
	int pathlen;
	char * filepath;
	floppycontext->hxc_printf(MSG_DEBUG,"JV1_libIsValidDiskFile %s",imgfile);
	if(imgfile)
	{
		pathlen=strlen(imgfile);
		if(pathlen!=0)
		{
			filepath=malloc(pathlen+1);
			if(filepath!=0)
			{
				sprintf(filepath,"%s",imgfile);
				strlower(filepath);

				if(strstr( filepath,".jv1" )!=NULL)
				{
					floppycontext->hxc_printf(MSG_DEBUG,"JV1 file !");
					free(filepath);
					return LOADER_ISVALID;
				}
				else
				{
					floppycontext->hxc_printf(MSG_DEBUG,"non JV1 file !");
					free(filepath);
					return LOADER_BADFILE;
				}
			}
		}
	}

	return LOADER_BADPARAMETER;
}



int JV1_libLoad_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppydisk,char * imgfile,void * parameters)
{

	FILE * f;
	unsigned int filesize;
	unsigned int i,j,k;
	unsigned int file_offset;
	char* trackdata;
	int tracklen;
	int gap3len,interleave,rpm;
	int sectorsize;
	int bitrate;

	SECTORCONFIG* sectorconfig;
	CYLINDER* currentcylinder;
	SIDE* currentside;

	floppycontext->hxc_printf(MSG_DEBUG,"JV1_libLoad_DiskFile %s",imgfile);

	f=fopen(imgfile,"rb");
	if(f==NULL)
	{
		floppycontext->hxc_printf(MSG_ERROR,"Cannot open %s !",imgfile);
		return LOADER_ACCESSERROR;
	}

	fseek (f , 0 , SEEK_END);
	filesize=ftell(f);
	fseek (f , 0 , SEEK_SET);

	if(filesize!=0)
	{

			sectorsize=256; // JV1 file support only 256bytes/sector floppies.

            bitrate=250000;
            rpm=300;
            interleave=3;
            gap3len=15;
            floppydisk->floppyNumberOfSide=1;
            floppydisk->floppySectorPerTrack=10;
            floppydisk->floppyNumberOfTrack=filesize/( floppydisk->floppyNumberOfSide*floppydisk->floppySectorPerTrack*256);

			floppydisk->floppyBitRate=bitrate;
			floppydisk->floppyiftype=GENERIC_SHUGART_DD_FLOPPYMODE;
			floppydisk->tracks=(CYLINDER**)malloc(sizeof(CYLINDER*)*floppydisk->floppyNumberOfTrack);

			floppycontext->hxc_printf(MSG_DEBUG,"rpm %d bitrate:%d track:%d side:%d sector:%d",rpm,bitrate,floppydisk->floppyNumberOfTrack,floppydisk->floppyNumberOfSide,floppydisk->floppySectorPerTrack);
			
			sectorconfig=(SECTORCONFIG*)malloc(sizeof(SECTORCONFIG)*floppydisk->floppySectorPerTrack);
			memset(sectorconfig,0,sizeof(SECTORCONFIG)*floppydisk->floppySectorPerTrack);
		
			tracklen=(bitrate/(rpm/60))/4;
			trackdata=(unsigned char*)malloc(sectorsize*floppydisk->floppySectorPerTrack);

			for(j=0;j<floppydisk->floppyNumberOfTrack;j++)
			{

				floppydisk->tracks[j]=(CYLINDER*)malloc(sizeof(CYLINDER));
				currentcylinder=floppydisk->tracks[j];

				currentcylinder->number_of_side=floppydisk->floppyNumberOfSide;
				currentcylinder->sides=(SIDE**)malloc(sizeof(SIDE*)*currentcylinder->number_of_side);
				memset(currentcylinder->sides,0,sizeof(SIDE*)*currentcylinder->number_of_side);


				for(i=0;i<floppydisk->floppyNumberOfSide;i++)
				{

					floppydisk->tracks[j]->floppyRPM=rpm;

					floppydisk->tracks[j]->sides[i]=malloc(sizeof(SIDE));
					memset(floppydisk->tracks[j]->sides[i],0,sizeof(SIDE));
					currentside=floppydisk->tracks[j]->sides[i];

					currentside->number_of_sector=floppydisk->floppySectorPerTrack;
					currentside->tracklen=tracklen;

					currentside->databuffer=malloc(currentside->tracklen);
					memset(currentside->databuffer,0,currentside->tracklen);

					currentside->flakybitsbuffer=0;

					currentside->timingbuffer=0;
					currentside->bitrate=bitrate;
					
					currentside->track_encoding=ISOIBM_FM_ENCODING;

					currentside->indexbuffer=malloc(currentside->tracklen);
					memset(currentside->indexbuffer,0,currentside->tracklen);

					file_offset=(sectorsize*(j*currentside->number_of_sector*floppydisk->floppyNumberOfSide))+
						(sectorsize*(currentside->number_of_sector)*i);


					fseek (f , file_offset , SEEK_SET);

					fread(trackdata,sectorsize*currentside->number_of_sector,1,f);

					memset(sectorconfig,0,sizeof(SECTORCONFIG)*floppydisk->floppySectorPerTrack);
					for(k=0;k<floppydisk->floppySectorPerTrack;k++)
					{
						sectorconfig[k].sectorsize=256;
						sectorconfig[k].cylinder=j;
						sectorconfig[k].head=i;
						sectorconfig[k].sector=k;
						if(j==17)
						{
							sectorconfig[k].use_alternate_datamark=1;
							sectorconfig[k].alternate_datamark=0xFA;
						}
					}


					BuildISOTrack(floppycontext,IBMFORMAT_SD,currentside->number_of_sector,1,sectorsize,j,i,gap3len,trackdata,currentside->databuffer,&currentside->tracklen,interleave,0,sectorconfig);

					currentside->tracklen=currentside->tracklen*8;

					fillindex(currentside->tracklen-1,currentside,2500,TRUE,1);

				}
			}

			free(sectorconfig);
			floppycontext->hxc_printf(MSG_INFO_1,"track file successfully loaded and encoded!");

			fclose(f);
			return LOADER_NOERROR;
	}

	floppycontext->hxc_printf(MSG_ERROR,"file size=%d !?",filesize);
	fclose(f);
	return LOADER_BADFILE;
}


