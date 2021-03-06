/*
//
// Copyright (C) 2006-2017 Jean-Fran?ois DEL NERO
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
// File : krz_loader.c
// Contains: Kurzweil KRZ floppy image loader
//
// Written by:	DEL NERO Jean Francois
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"

#include "internal_libhxcfe.h"
#include "tracks/track_generator.h"
#include "libhxcfe.h"

#include "floppy_loader.h"
#include "floppy_utils.h"

#include "krz_loader.h"

#include "libhxcadaptor.h"

#include "../fat12floppy_loader/fat12.h"

extern unsigned char msdos_bootsector;

int KRZ_libIsValidDiskFile(HXCFE_IMGLDR * imgldr_ctx,char * imgfile)
{
	int filesize;

	imgldr_ctx->hxcfe->hxc_printf(MSG_DEBUG,"KRZ_libIsValidDiskFile");

	if( hxc_checkfileext(imgfile,"krz"))
	{

		filesize=hxc_getfilesize(imgfile);
		if(filesize<0)
		{
			imgldr_ctx->hxcfe->hxc_printf(MSG_ERROR,"KRZ_libIsValidDiskFile : Cannot open %s !",imgfile);
			return HXCFE_ACCESSERROR;
		}

		imgldr_ctx->hxcfe->hxc_printf(MSG_DEBUG,"KRZ_libIsValidDiskFile : Kurzweil KRZ file !");
		return HXCFE_VALIDFILE;
	}
	else
	{
		imgldr_ctx->hxcfe->hxc_printf(MSG_DEBUG,"KRZ_libIsValidDiskFile : non Kurzweil KRZ file !");
		return HXCFE_BADFILE;
	}
}



int KRZ_libLoad_DiskFile(HXCFE_IMGLDR * imgldr_ctx,HXCFE_FLOPPY * floppydisk,char * imgfile,void * parameters)
{
	int i,j;
	unsigned int file_offset;
	unsigned char * flatimg;
	int gap3len,interleave,skew;
	int trackformat;
	int rpm;
	int sectorsize;
	int numberofcluster;
	uint32_t   fatposition;
	uint32_t   rootposition;
	uint32_t   dataposition;
	int pcbootsector;
	int dksize;
	HXCFE_CYLINDER* currentcylinder;

	FATCONFIG fatconfig;

	imgldr_ctx->hxcfe->hxc_printf(MSG_DEBUG,"krz_libLoad_DiskFile %s",imgfile);

	floppydisk->floppyNumberOfTrack=80;
	floppydisk->floppyNumberOfSide=2;
	floppydisk->floppySectorPerTrack=18;
	floppydisk->floppyBitRate=500000;
	rpm=300;
	pcbootsector=1;
	skew=0;
	gap3len=84;
	interleave=1;
	sectorsize=512;
	trackformat=IBMFORMAT_DD;

	dksize=floppydisk->floppyNumberOfTrack*
			(floppydisk->floppySectorPerTrack*floppydisk->floppyNumberOfSide*512);


	imgldr_ctx->hxcfe->hxc_printf(MSG_INFO_1,"floppy size:%dkB, %d tracks, %d side(s), %d sectors/track, rpm:%d",dksize/1024,floppydisk->floppyNumberOfTrack,floppydisk->floppyNumberOfSide,floppydisk->floppySectorPerTrack,rpm);

	flatimg=(unsigned char*)malloc(dksize);
	if(flatimg!=NULL)
	{
		memset(flatimg,0,dksize);
		if(pcbootsector)
			memcpy(flatimg,&msdos_bootsector,512);


		fatconfig.sectorsize=512;
		fatconfig.clustersize=2;
		fatconfig.reservedsector=1;
		fatconfig.numberoffat=2;
		fatconfig.numberofrootentries=224;// 7secteurs = 32 *512
		fatconfig.nbofsector=(floppydisk->floppyNumberOfTrack*
								(floppydisk->floppySectorPerTrack*floppydisk->floppyNumberOfSide));
		fatconfig.nbofsectorperfat=((fatconfig.nbofsector-(fatconfig.reservedsector+(fatconfig.numberofrootentries/32)))/((fatconfig.sectorsize*8)/12))+1;
		//sprintf(&flatimg[CHSTOADR(0,0,0)+3],"HXC.EMU");

		*( (unsigned short*) &flatimg[0x0B])=fatconfig.sectorsize; //Nombre d'octets par secteur
		*( (unsigned char*)  &flatimg[0x0D])=fatconfig.clustersize; //Nombre de secteurs par cluster (1, 2, 4, 8, 16, 32, 64 ou 128).
		*( (unsigned short*) &flatimg[0x0E])=fatconfig.reservedsector; //Nombre de secteur r?serv? en comptant le secteur de boot (32 par d?faut pour FAT32, 1 par d?faut pour FAT12/16).
		*( (unsigned char*)  &flatimg[0x10])=fatconfig.numberoffat; //Nombre de FATs sur le disque (2 par d?faut).
		*( (unsigned short*) &flatimg[0x11])=fatconfig.numberofrootentries; //Taille du r?pertoire racine (0 par d?faut pour FAT32).
		*( (unsigned short*) &flatimg[0x13])=fatconfig.nbofsector; //Nombre total de secteur 16-bit (0 par d?faut pour FAT32).

		if(	floppydisk->floppyBitRate==250000)
		{
			*( (unsigned char*)  &flatimg[0x15])=0xF9; //Type de disque (0xF8 pour les disques durs, 0xF0 pour les disquettes). 0xF9 Double sided, 80 tracks per side, 9 sectors per track
		}
		else
		{
			*( (unsigned char*)  &flatimg[0x15])=0xF0; //Type de disque (0xF8 pour les disques durs, 0xF0 pour les disquettes). 0xF9 Double sided, 80 tracks per side, 9 sectors per track
		}

		*( (unsigned short*) &flatimg[0x16])=fatconfig.nbofsectorperfat; //Taille d'une FAT en secteurs (0 par d?faut pour FAT32).
		*( (unsigned short*) &flatimg[0x18])=floppydisk->floppySectorPerTrack; //Sectors per track
		*( (unsigned short*) &flatimg[0x1a])=floppydisk->floppyNumberOfSide; //Number of heads.
		*( (unsigned short*) &flatimg[0x1FE])=0xAA55;//End of sector marker (0x55 0xAA)

		fatposition=fatconfig.sectorsize*fatconfig.reservedsector;
		rootposition=((fatconfig.reservedsector)+(fatconfig.numberoffat*fatconfig.nbofsectorperfat))*fatconfig.sectorsize;
		dataposition=(((fatconfig.reservedsector)+(fatconfig.numberoffat*fatconfig.nbofsectorperfat))*fatconfig.sectorsize)+(32*fatconfig.numberofrootentries);

		numberofcluster=(fatconfig.nbofsector-(dataposition/fatconfig.sectorsize))/fatconfig.clustersize;

		if(ScanFileAndAddToFAT(imgldr_ctx->hxcfe,imgfile,0,&flatimg[fatposition],&flatimg[rootposition],&flatimg[dataposition],0,&fatconfig,numberofcluster))
		{
			return HXCFE_BADFILE;
		}
		memcpy(&flatimg[((fatconfig.reservedsector)+(fatconfig.nbofsectorperfat))*fatconfig.sectorsize],&flatimg[fatposition],fatconfig.nbofsectorperfat*fatconfig.sectorsize);


	}
	else
	{
		return HXCFE_INTERNALERROR;
	}


	floppydisk->floppyiftype=IBMPC_HD_FLOPPYMODE;
	floppydisk->tracks=(HXCFE_CYLINDER**)malloc(sizeof(HXCFE_CYLINDER*)*floppydisk->floppyNumberOfTrack);

	for(j=0;j<floppydisk->floppyNumberOfTrack;j++)
	{
		floppydisk->tracks[j]=allocCylinderEntry(rpm,floppydisk->floppyNumberOfSide);
		currentcylinder=floppydisk->tracks[j];

		for(i=0;i<floppydisk->floppyNumberOfSide;i++)
		{
			hxcfe_imgCallProgressCallback(imgldr_ctx, (j<<1) + (i&1),floppydisk->floppyNumberOfTrack*2);

			file_offset=(sectorsize*(j*floppydisk->floppySectorPerTrack*floppydisk->floppyNumberOfSide))+
						(sectorsize*(floppydisk->floppySectorPerTrack)*i);

			currentcylinder->sides[i]=tg_generateTrack(&flatimg[file_offset],sectorsize,floppydisk->floppySectorPerTrack,(unsigned char)j,(unsigned char)i,1,interleave,(unsigned char)(((j<<1)|(i&1))*skew),floppydisk->floppyBitRate,currentcylinder->floppyRPM,trackformat,gap3len,0,2500|NO_SECTOR_UNDER_INDEX,-2500);
		}
	}

	free(flatimg);

	imgldr_ctx->hxcfe->hxc_printf(MSG_INFO_1,"track file successfully loaded and encoded!");
	return HXCFE_NOERROR;
}

int KRZ_libGetPluginInfo(HXCFE_IMGLDR * imgldr_ctx,uint32_t infotype,void * returnvalue)
{

	static const char plug_id[]="KURZWEIL_KRZ";
	static const char plug_desc[]="KURZWEIL KRZ Loader";
	static const char plug_ext[]="krz";

	plugins_ptr plug_funcs=
	{
		(ISVALIDDISKFILE)	KRZ_libIsValidDiskFile,
		(LOADDISKFILE)		KRZ_libLoad_DiskFile,
		(WRITEDISKFILE)		0,
		(GETPLUGININFOS)	KRZ_libGetPluginInfo
	};

	return libGetPluginInfo(
			imgldr_ctx,
			infotype,
			returnvalue,
			plug_id,
			plug_desc,
			&plug_funcs,
			plug_ext
			);
}



