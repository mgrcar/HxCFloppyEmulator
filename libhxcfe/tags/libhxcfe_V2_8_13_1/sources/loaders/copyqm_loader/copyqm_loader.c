/*
//
// Copyright (C) 2006-2018 Jean-Fran?ois DEL NERO
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
// File : copyqm_loader.c
// Contains: CopyQm floppy image loader
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

#include "copyqm_loader.h"
#include "crctable.h"

#include "libhxcadaptor.h"

int CopyQm_libIsValidDiskFile(HXCFE_IMGLDR * imgldr_ctx,char * imgfile)
{
	int pathlen;
	int i;
	char * fileheader;
	unsigned char checksum;
	FILE * f;

	imgldr_ctx->hxcfe->hxc_printf(MSG_DEBUG,"CopyQm_libIsValidDiskFile");
	if(imgfile)
	{
		pathlen=strlen(imgfile);
		if(pathlen!=0)
		{
			f=hxc_fopen(imgfile,"rb");
			if(f==NULL)
			{
				imgldr_ctx->hxcfe->hxc_printf(MSG_ERROR,"CopyQm_libIsValidDiskFile : Cannot open %s !",imgfile);
				return HXCFE_ACCESSERROR;
			}

			fileheader=(char*)malloc(QM_HEADER_SIZE+1);
			hxc_fread( fileheader, QM_HEADER_SIZE, f );
			if(ftell(f)==QM_HEADER_SIZE)
			{

				if ( fileheader[0] != 'C' || fileheader[1] != 'Q' )
				{
					imgldr_ctx->hxcfe->hxc_printf(MSG_DEBUG,"CopyQm_libIsValidDiskFile : bad header tag !");
					hxc_fclose(f);
					free(fileheader);
					return HXCFE_BADFILE;
				}

				checksum=0;
				/* Check the header checksum */
				for ( i = 0; i < QM_HEADER_SIZE; ++i )
				{
					checksum= checksum + (unsigned char)(fileheader[i]);
				}
				if ( checksum != 0 )
				{
					imgldr_ctx->hxcfe->hxc_printf(MSG_DEBUG,"CopyQm_libIsValidDiskFile : bad header checksum !");
					hxc_fclose(f);
					free(fileheader);
					return HXCFE_BADFILE;
				}

				imgldr_ctx->hxcfe->hxc_printf(MSG_DEBUG,"CopyQm_libIsValidDiskFile : it's an copyqm file!");
				hxc_fclose(f);
				free(fileheader);
				return HXCFE_VALIDFILE;
			}

			imgldr_ctx->hxcfe->hxc_printf(MSG_DEBUG,"CopyQm_libIsValidDiskFile : bad header tag !");
			hxc_fclose(f);
			free(fileheader);
     		return HXCFE_BADFILE;
		}
	}

	return HXCFE_BADPARAMETER;
}



int CopyQm_libLoad_DiskFile(HXCFE_IMGLDR * imgldr_ctx,HXCFE_FLOPPY * floppydisk,char * imgfile,void * parameters)
{
	FILE * f;
	unsigned int   filesize;
	int   i,j;
	unsigned int   file_offset;
	unsigned char* flatimg;
	int gap3len,interleave;
	int rpm;
	int sectorsize;
	unsigned char  checksum;
	uint32_t  crc,crc32,comentlen;
	unsigned char* fileheader;
	int  trackformat;
	size_t curwritepos;
	char *comment_buf;
	size_t image_size;
	int res;
	HXCFE_CYLINDER* currentcylinder;

	imgldr_ctx->hxcfe->hxc_printf(MSG_DEBUG,"CopyQm_libLoad_DiskFile %s",imgfile);

	f=hxc_fopen(imgfile,"rb");
	if(f==NULL)
	{
		imgldr_ctx->hxcfe->hxc_printf(MSG_ERROR,"CopyQm_libLoad_DiskFile : Cannot open %s !",imgfile);
		return HXCFE_ACCESSERROR;
	}

	filesize = hxc_fgetsize(f);

	if( filesize )
	{
		fileheader=(unsigned char*)malloc(QM_HEADER_SIZE+1);
		hxc_fread( fileheader, QM_HEADER_SIZE, f );

		checksum=0;
		/* Check the header checksum */
		for ( i = 0; i < QM_HEADER_SIZE; ++i )
		{
			checksum = checksum + (unsigned char)(fileheader[i]);
		}
		if ( checksum != 0 )
		{
			imgldr_ctx->hxcfe->hxc_printf(MSG_ERROR,"bad header checksum !");
			free(fileheader);
			hxc_fclose(f);
			return HXCFE_BADFILE;
		}
		if ( fileheader[0] != 'C' || fileheader[1] != 'Q' )
		{
			imgldr_ctx->hxcfe->hxc_printf(MSG_ERROR,"bad header tag !");
			free(fileheader);
			hxc_fclose(f);
			return HXCFE_BADFILE;
		}


		/* I'm guessing sector size is at 3. Expandqm thinks 7 */
		sectorsize = get_u16( fileheader, 0x03 );
		/* Number of sectors 0x0B-0x0C, strange number for non-blind, often 116 */
		//qm_self->qm_h_nbr_sectors = get_u16( header, 0x0b );

		/* Number of sectors per track */
		floppydisk->floppySectorPerTrack= get_u16( fileheader, 0x10 );
		/* Number of heads */
		floppydisk->floppyNumberOfSide= get_u16( fileheader, 0x12 );
		/* Blind or not */
		//qm_self->qm_h_blind = fileheader[0x58];
		/* Density - 0 is DD, 1 means HD */
		if(fileheader[0x59])
		{
			floppydisk->floppyBitRate=500000;
		}
		else
		{
			floppydisk->floppyBitRate=250000;
		}
		/* Number of used tracks */
		floppydisk->floppyNumberOfTrack = fileheader[0x5a];
		/* Number of total tracks */
		floppydisk->floppyNumberOfTrack = fileheader[0x5b];
		/* CRC 0x5c - 0x5f */
		crc32 = get_u32( fileheader, 0x5c );
		/* Length of comment */
		comentlen = get_u16( fileheader, 0x6f );
		/* 0x71 is first sector number - 1 */
		//qm_self->qm_h_secbase = (signed char)(fileheader[0x71]);
		/* 0x74 is interleave, I think. Normally 1, but 0 for old copyqm */
		interleave = fileheader[0x74];
		/* 0x75 is skew. Normally 0. Negative number for alternating sides */
		//qm_self->qm_h_skew = header[0x75];

		if (comentlen)
		{
			comment_buf = malloc(1 + comentlen);
			/* If malloc fails, ignore it - comments aren't essential */
			if (comment_buf)
			{
				res = fseek( f, QM_HEADER_SIZE, SEEK_SET );
				if ( !(res < 0) )
				{
					res = hxc_fread(comment_buf, comentlen, f);
					if ( !res )
					{
						comment_buf[comentlen] = 0;
						imgldr_ctx->hxcfe->hxc_printf(MSG_INFO_1,"Disk info:\n%s",comment_buf);
						free(comment_buf);
					}
				}
			}
		}

		/* Write position in the memory image */
		curwritepos = 0;
		/* FIXME: Use the used tracks instead of the total tracks to detect */
		/*        that there is the correct amount of data in the image     */
		image_size = (size_t)floppydisk->floppySectorPerTrack *
				(size_t)floppydisk->floppyNumberOfSide *
				(size_t)floppydisk->floppyNumberOfTrack * sectorsize;
		/* Set the position after the header and comment */
		res = fseek( f, QM_HEADER_SIZE + comentlen, SEEK_SET );

		crc=0xFFFFFFFF;
		if ( res < 0 )
		{
			imgldr_ctx->hxcfe->hxc_printf(MSG_ERROR,"file corrupt!");
			free(fileheader);
			hxc_fclose(f);
			return HXCFE_FILECORRUPTED;
		}

		/* Alloc memory for the image */
		flatimg = malloc( image_size );
		if ( ! flatimg )
		{
			imgldr_ctx->hxcfe->hxc_printf(MSG_ERROR,"malloc error!");
			free(fileheader);
			hxc_fclose(f);
			return HXCFE_INTERNALERROR;
		}

		/* Start reading */
		/* Note that it seems like each track starts a new block */

			while ( curwritepos < image_size )
			{
				/* Read the length */
				unsigned char lengthBuf[2];
				res = hxc_fread( lengthBuf, 2, f );
				if ( res )
				{
					if ( feof( f ) )
					{
						/* End of file - fill with f6 - do not update CRC for these */
						memset( flatimg + curwritepos, 0xf6, image_size - curwritepos );
						curwritepos += image_size - curwritepos;
					}
					else
					{
						free(flatimg);
						free(fileheader);
			            hxc_fclose(f);
						return HXCFE_FILECORRUPTED;
					}
				}
				else
				{
					int length = get_i16( lengthBuf, 0 );
					if ( length < 0 )
					{
						/* Negative number - next byte is repeated (-length) times */
						int c = fgetc( f );
						if ( c == EOF ) return HXCFE_FILECORRUPTED;
						/* Copy the byte into memory and update the offset */
						memset( flatimg + curwritepos, c, -length );
						curwritepos -= length;
						/* Update CRC */
						for( ; length != 0; ++length )
						{
							drv_qm_update_crc( &crc, (unsigned char)c );
							//crc = crc32r_table[(c ^ (unsigned char)crc) & 0x3f ] ^ (crc >> 8);
						}
					}
					else
					{
						if ( length != 0 )
						{
							/* Positive number - length different characters */
							res = hxc_fread( flatimg + curwritepos, length, f );
							/* Update CRC (and write pos) */
							while ( length-- )
							{
								drv_qm_update_crc( &crc,
									flatimg[curwritepos++] );
								//crc = crc32r_table[(flatimg[curwritepos++] ^ (unsigned char)crc) & 0x3f ] ^ (crc >> 8);
							}
							if ( res )
							{
								free(flatimg);
								free(fileheader);
								hxc_fclose(f);
								return HXCFE_FILECORRUPTED;
							}
						}
					}
				}
			}

			/* Compare the CRCs */
			/* The CRC is zero on old images so it cannot be checked then */
			/*	if ( crc32 ) {
			if ( crc32!= crc ) {
			return -1;
			}
			}*/
		free(fileheader);
		hxc_fclose(f);

		gap3len=255;
		interleave=1;

		floppydisk->floppyiftype=GENERIC_SHUGART_DD_FLOPPYMODE;
		floppydisk->tracks=(HXCFE_CYLINDER**)malloc(sizeof(HXCFE_CYLINDER*)*floppydisk->floppyNumberOfTrack);
		trackformat=IBMFORMAT_DD;
		rpm=300; // normal rpm

		imgldr_ctx->hxcfe->hxc_printf(MSG_INFO_1,"%d tracks, %d Side(s), %d %d-bytes sector per tracks, interleave %d, GAP3:%d, %dkbits/s\n",floppydisk->floppyNumberOfTrack,floppydisk->floppyNumberOfSide,floppydisk->floppySectorPerTrack,sectorsize,interleave,gap3len,floppydisk->floppyBitRate/1000);

		for(j=0;j<floppydisk->floppyNumberOfTrack;j++)
		{
			floppydisk->tracks[j]=allocCylinderEntry(rpm,floppydisk->floppyNumberOfSide);
			currentcylinder=floppydisk->tracks[j];

			for(i=0;i<floppydisk->floppyNumberOfSide;i++)
			{
				hxcfe_imgCallProgressCallback(imgldr_ctx,(j<<1) | (i&1),floppydisk->floppyNumberOfTrack*2);

				file_offset=(sectorsize*(j*floppydisk->floppySectorPerTrack*floppydisk->floppyNumberOfSide))+
							(sectorsize*(floppydisk->floppySectorPerTrack)*i);

				currentcylinder->sides[i]=tg_generateTrack(&flatimg[file_offset],sectorsize,floppydisk->floppySectorPerTrack,(unsigned char)j,(unsigned char)i,1,interleave,(unsigned char)(0),floppydisk->floppyBitRate,currentcylinder->floppyRPM,trackformat,gap3len,0,2500 | NO_SECTOR_UNDER_INDEX,-2500);
			}
		}

		imgldr_ctx->hxcfe->hxc_printf(MSG_INFO_1,"track file successfully loaded and encoded!");
		free(flatimg);

	return HXCFE_NOERROR;
	}

	imgldr_ctx->hxcfe->hxc_printf(MSG_ERROR,"file size=%d !?",filesize);
	return HXCFE_FILECORRUPTED;
}

int CopyQm_libGetPluginInfo(HXCFE_IMGLDR * imgldr_ctx,uint32_t infotype,void * returnvalue)
{

	static const char plug_id[]="COPYQM";
	static const char plug_desc[]="COPYQM IMG Loader";
	static const char plug_ext[]="dsk";

	plugins_ptr plug_funcs=
	{
		(ISVALIDDISKFILE)	CopyQm_libIsValidDiskFile,
		(LOADDISKFILE)		CopyQm_libLoad_DiskFile,
		(WRITEDISKFILE)		0,
		(GETPLUGININFOS)	CopyQm_libGetPluginInfo
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


