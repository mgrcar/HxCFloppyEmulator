/*
//
// Copyright (C) 2006-2017 Jean-Fran�ois DEL NERO
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"

#include "internal_libhxcfe.h"
#include "tracks/track_generator.h"
#include "libhxcfe.h"

#include "xml_loader.h"

#include "tracks/sector_extractor.h"

#include "libhxcadaptor.h"

void gettracktype(HXCFE_SECTORACCESS* ss,int track,int side,int * nbsect,int *firstsectid,char * format,int *sectorsize)
{
	int i;
	HXCFE_SECTCFG** sca;
	int32_t nb_sectorfound;

	*sectorsize = 512;
	*firstsectid = 1;
	sprintf(format,"IBM_MFM");
	*nbsect = 0;

	sca = hxcfe_getAllTrackSectors(ss,track,side,ISOIBM_MFM_ENCODING,&nb_sectorfound);
	if(nb_sectorfound)
	{
		*sectorsize = sca[0]->sectorsize;
		*firstsectid = 0xFF;
		*nbsect = nb_sectorfound;
		for(i=0;i<nb_sectorfound;i++)
		{

			sprintf(format,"IBM_MFM");
			if(sca[i]->sector<*firstsectid)
				*firstsectid = sca[i]->sector;
			hxcfe_freeSectorConfig  (ss,sca[i]);
		}
		free(sca);
	}

	sca = hxcfe_getAllTrackSectors(ss,track,side,ISOIBM_FM_ENCODING,&nb_sectorfound);
	if(nb_sectorfound)
	{
		*sectorsize = sca[0]->sectorsize;
		*firstsectid = 0xFF;
		*nbsect = nb_sectorfound;
		for(i=0;i<nb_sectorfound;i++)
		{
			sprintf(format,"IBM_FM");
			if(sca[i]->sector<*firstsectid)
				*firstsectid = sca[i]->sector;
			hxcfe_freeSectorConfig  (ss,sca[i]);
		}
		free(sca);
	}


}

typedef struct sect_offset_
{
	HXCFE_SECTCFG* ss;
	uint32_t offset;
}sect_offset;

static void exchange(sect_offset **  table, int a, int b)
{
    sect_offset * temp;
	temp = table[a];
    table[a] = table[b];
    table[b] = temp;
}

static void quickSort(sect_offset ** table, int start, int end)
{
    int left = start-1;
    int right = end+1;
    const int pivot = table[start]->ss->sector;

    if(start >= end)
        return;

    while(1)
    {
        do right--; while(table[right]->ss->sector > pivot);
        do left++; while(table[left]->ss->sector < pivot);

        if(left < right)
            exchange(table, left, right);
        else break;
    }

    quickSort(table, start, right);
    quickSort(table, right+1, end);
}

int XML_libWrite_DiskFile(HXCFE_IMGLDR* imgldr_ctx,HXCFE_FLOPPY * floppy,char * filename)
{
	int i,j,k,s;
	FILE * xmlfile;
	int fileoffset;
	int32_t nb_sectorfound;
	int nbsect,firstsectid,sectorsize,imagesize;
	char trackformat[32];
	HXCFE_SECTORACCESS* ss;
	HXCFE_SECTCFG** sca;
	sect_offset ** sorted_sectoffset,**sectoffset;

	imgldr_ctx->hxcfe->hxc_printf(MSG_INFO_1,"Write XML file %s...",filename);

	fileoffset = 0;

	xmlfile=hxc_fopen(filename,"w+");
	if(xmlfile)
	{
		imagesize = hxcfe_getFloppySize(imgldr_ctx->hxcfe,floppy,0);

		ss=hxcfe_initSectorAccess(imgldr_ctx->hxcfe,floppy);
		if(ss)
		{
			fprintf(xmlfile,"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
			fprintf(xmlfile,"<!-- HxC Floppy Emulator Disk Layout -->\n");

			fprintf(xmlfile,"<disk_layout>\n");

			fprintf(xmlfile,"\t<disk_layout_name>AUTOGENERATEDLAYOUT</disk_layout_name>\n");
			fprintf(xmlfile,"\t<disk_layout_description>Auto Generated Disk Layout</disk_layout_description>\n");
			fprintf(xmlfile,"\t<prefered_file_extension>img</prefered_file_extension>\n");
			fprintf(xmlfile,"\t<interface_mode>GENERIC_SHUGART_DD_FLOPPYMODE</interface_mode>\n");

			fprintf(xmlfile,"\t<file_size>%d</file_size>\n",imagesize);

			fprintf(xmlfile,"\t<layout>\n");

			fprintf(xmlfile,"\t\t<number_of_track>%d</number_of_track>\n",floppy->floppyNumberOfTrack);
			fprintf(xmlfile,"\t\t<number_of_side>%d</number_of_side>\n",floppy->floppyNumberOfSide);

			gettracktype(ss,0,0,&nbsect,&firstsectid,(char*)&trackformat,&sectorsize);

			fprintf(xmlfile,"\t\t<format>%s</format>\n",trackformat);

			fprintf(xmlfile,"\t\t<start_sector_id>%d</start_sector_id>\n",firstsectid);
			fprintf(xmlfile,"\t\t<sector_per_track>%d</sector_per_track>\n",nbsect);

			fprintf(xmlfile,"\t\t<sector_size>%d</sector_size>\n",sectorsize);

			fprintf(xmlfile,"\t\t<formatvalue>%d</formatvalue>\n",0x00);

			fprintf(xmlfile,"\t\t<gap3>%d</gap3>\n",0xFF);

			if(floppy->floppyBitRate!=-1)
			{
				fprintf(xmlfile,"\t\t<bitrate>%d</bitrate>\n",floppy->floppyBitRate);
			}
			else
			{
				fprintf(xmlfile,"\t\t<bitrate>%d</bitrate>\n",(int)((float)(floppy->tracks[0]->sides[0]->tracklen/2) * ( (float)floppy->tracks[0]->floppyRPM / (float)60)));
			}

			fprintf(xmlfile,"\t\t<pregap>%d</pregap>\n",0);
			fprintf(xmlfile,"\t\t<rpm>%d</rpm>\n",floppy->tracks[0]->floppyRPM);

			fprintf(xmlfile,"\t\t<track_list>\n");


			for(j=0;j<floppy->floppyNumberOfTrack;j++)
			{
				for(i=0;i<floppy->floppyNumberOfSide;i++)
				{
					fprintf(xmlfile,"\t\t\t<track track_number=\"%.2d\" side_number=\"%d\">\n",j,i);
					fprintf(xmlfile,"\t\t\t\t<data_offset>0x%.6X</data_offset>\n",fileoffset);

					gettracktype(ss,j,i,&nbsect,&firstsectid,(char*)&trackformat,&sectorsize);
					fprintf(xmlfile,"\t\t\t\t<format>%s</format>\n",trackformat);

					sca = hxcfe_getAllTrackISOSectors(ss,j,i,&nb_sectorfound);
					fprintf(xmlfile,"\t\t\t\t<sector_list>\n");

					if(sca && nb_sectorfound)
					{
						sectoffset = malloc(sizeof(sect_offset*) * nb_sectorfound);
						sorted_sectoffset = malloc(sizeof(sect_offset*) * nb_sectorfound);
						if(sorted_sectoffset && sectoffset)
						{
							memset(sectoffset,0,sizeof(sect_offset*) * nb_sectorfound);
							memset(sorted_sectoffset,0,sizeof(sect_offset*) * nb_sectorfound);
							for(s=0;s<nb_sectorfound;s++)
							{
								sorted_sectoffset[s] = malloc(sizeof(sect_offset));
								sorted_sectoffset[s]->ss = sca[s];
								sorted_sectoffset[s]->offset = 0;
							}

							memcpy(sectoffset,sorted_sectoffset,sizeof(sect_offset*) * nb_sectorfound);

							quickSort(sorted_sectoffset, 0, nb_sectorfound - 1);

							for(s=0;s<nb_sectorfound;s++)
							{
								sorted_sectoffset[s]->offset = fileoffset;
								fileoffset += sca[s]->sectorsize;
							}

							free(sorted_sectoffset);

							for(s=0;s<nb_sectorfound;s++)
							{

								fprintf(xmlfile,"\t\t\t\t\t<sector sector_id=\"%d\" sector_size=\"%d\">\n",sca[s]->sector,sca[s]->sectorsize);
								if(sca[s]->fill_byte_used)
								{
									fprintf(xmlfile,"\t\t\t\t\t\t<data_fill>0x%.2X</data_fill>\n",sca[s]->fill_byte);
								}
								else
								{
									if(sca[s]->input_data)
									{
										fprintf(xmlfile,"\t\t\t\t\t\t<sector_data>");
										k=0;
										do
										{
											fprintf(xmlfile,"%.2X",sca[s]->input_data[k]);
											k++;
										}while(k<(int)sca[s]->sectorsize);

										fprintf(xmlfile,"</sector_data>\n");
									}
								}

								if(sca[s]->head != i)
								{
									fprintf(xmlfile,"\t\t\t\t\t\t<side_id>0x%.2X</side_id>\n",sca[s]->head);
								}

								if(sca[s]->cylinder != j)
								{
									fprintf(xmlfile,"\t\t\t\t\t\t<track_id>0x%.2X</track_id>\n",sca[s]->cylinder);
								}

								if(sca[s]->use_alternate_datamark)
								{
									fprintf(xmlfile,"\t\t\t\t\t\t<datamark>0x%.2X</datamark>\n",sca[s]->alternate_datamark);
								}

								fprintf(xmlfile,"\t\t\t\t\t\t<data_offset>0x%.6X</data_offset>\n",(unsigned int)sectoffset[s]->offset);
								fprintf(xmlfile,"\t\t\t\t\t</sector>\n");

								hxcfe_freeSectorConfig  (ss,sca[s]);
							}

							for(s=0;s<nb_sectorfound;s++)
							{
								free(sectoffset[s]);
							}
							free(sectoffset);
						}

						free(sca);
					}

					fprintf(xmlfile,"\t\t\t\t</sector_list>\n");

					fprintf(xmlfile,"\t\t\t</track>\n");
				}
			}
			hxcfe_deinitSectorAccess(ss);
		}

		fprintf(xmlfile,"\t\t</track_list>\n");
		fprintf(xmlfile,"\t</layout>\n");
		fprintf(xmlfile,"</disk_layout>\n");

		hxc_fclose(xmlfile);
	}

	return 0;
}
