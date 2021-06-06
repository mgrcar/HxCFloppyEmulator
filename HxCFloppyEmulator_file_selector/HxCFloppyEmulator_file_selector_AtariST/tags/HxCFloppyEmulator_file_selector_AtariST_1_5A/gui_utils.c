/*
//
// Copyright (C) 2009, 2010, 2011 Jean-Fran�ois DEL NERO
//
// This file is part of the HxCFloppyEmulator file selector.
//
// HxCFloppyEmulator file selector may be used and distributed without restriction
// provided that this copyright statement is not removed from the file and that any
// derivative work contains the original copyright notice and the associated
// disclaimer.
//
// HxCFloppyEmulator file selector is free software; you can redistribute it
// and/or modify  it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// HxCFloppyEmulator file selector is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HxCFloppyEmulator file selector; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mint/osbind.h>
#include <time.h>
#include <vt52.h>
#include <stdarg.h>


#include "graphx/data_bmp_hxc2001logo_bmp.h"
#include "graphx/data_bmp_font_bmp.h"
#include "graphx/data_bmp_font8x8_bmp.h"
#include "graphx/data_bmp_sdhxcfelogo_bmp.h"

#include "gui_utils.h"

#include "conf.h"

unsigned char * screen_buffer;
unsigned char * screen_buffer_aligned;
unsigned char * screen_buffer_backup;
unsigned char * screen_buffer_backup_aligned;
unsigned char color;
unsigned long old_physical_adr;

unsigned char NUMBER_OF_FILE_ON_DISPLAY;// 19-5 //19 -240
unsigned short SCREEN_YRESOL;


	#define BLACK 0x002           /*  RGB values for the four colors used.   */
	#define RED   0xFFF
	#define GREEN 0x0f0
	#define BLUE  0x00f


	#define WIDTH  SCREEN_XRESOL /* 640 pixels wide (high resolution)                */
	//#define HEIGHT SCREEN_YRESOL /* 200 lines high (non interlaced NTSC display)     */
	#define DEPTH    2 /* 1 BitPlanes should be used, gives eight colours. */
	#define COLOURS  2 /* 2^1 = 2                                          */


	#define BLACK 0x002           /*  RGB values for the four colors used.   */
	#define RED   0xFFF
	#define GREEN 0x0f0
	#define BLUE  0x00f


  static unsigned long colortable[] = {
								0x002, 0xFFF, 0x0f0, 0x00f,
								0x000, 0xFFF, 0x0f0, 0x00f,
								0xFFF, 0x000, 0x0f0, 0x00f,
								0x030, 0xFFF, 0x0f0, 0x00f,
								0x300, 0xFFF, 0x0f0, 0x00f,
								0x303, 0xFFF, 0x0f0, 0x00f,
								0x999, 0x000, 0x0f0, 0x00f,
								0xFFF, 0x343, 0x0f0, 0x00f,
								0xF33, 0xFFF, 0x0f0, 0x00f,
								0xF0F, 0xFFF, 0x0f0, 0x00f,
								0xFFF, 0x0F0, 0x0f0, 0x00f,
								0xFF0, 0xFFF, 0x0f0, 0x00f,
								0x000, 0xF00, 0x0f0, 0x00f,
								0x000, 0x0F0, 0x0f0, 0x00f,
								0x000, 0x00F, 0x0f0, 0x00f,
								0x004, 0xFFF, 0x0f0, 0x00f,

};





void display_sprite(unsigned char * membuffer, bmaptype * sprite,unsigned short x, unsigned short y)
{
  unsigned short i,j,k,l,x_offset,base_offset;
  unsigned short *ptr_src;
  unsigned short *ptr_dst;

  ptr_dst=(unsigned short*)membuffer;
  ptr_src=(unsigned short*)&sprite->data[0];

   k=0;
   l=0;
   base_offset=((y*160)+ (((x>>2)&(~0x3))))/2;
   for(j=0;j<(sprite->Ysize);j++)
   {
     l=base_offset +(80*j);
     for(i=0;i<(sprite->Xsize/16);i++)
     {
       ptr_dst[l]=ptr_src[k];
       l++;
       ptr_dst[l]=ptr_src[k];
       l++;
       k++;
     }
   }

}

void print_char(unsigned char * membuffer, bmaptype * font,unsigned short x, unsigned short y,unsigned char c)
{
  unsigned short j,k,l,c1;
  unsigned short *ptr_src;
  unsigned short *ptr_dst;

  ptr_dst=(unsigned short*)membuffer;
  ptr_src=(unsigned short*)&font->data[0];
   x=(x>>3) & (~0x1);
  // x=((x&(~0x1))<<1)+(x&1);//  0 1   2 3

   l=(y*80)+ x;
   k=((c>>4)*(16*16))+(c&0xF);
   for(j=0;j<16;j++)
   {
    ptr_dst[l]  =ptr_src[k];
    ptr_dst[l+1]=ptr_src[k];
    k=k+(16);
    l=l+(80);
   }

}

void print_char8x8(unsigned char * membuffer, bmaptype * font,unsigned short x, unsigned short y,unsigned char c)
{
  unsigned short j,k,l,c1;
  unsigned char *ptr_src;
  unsigned char *ptr_dst;

  ptr_dst=(unsigned char*)membuffer;
  ptr_src=(unsigned char*)&font->data[0];

  x=x>>3;
  x=((x&(~0x1))<<1)+(x&1);//  0 1   2 3
  l=(y*160)+ (x);
  k=((c>>4)*(8*8*2))+(c&0xF);
  for(j=0;j<8;j++)
  {
    ptr_dst[l]  =ptr_src[k];
    ptr_dst[l+2]=ptr_src[k];
    k=k+(16);
    l=l+(160);
  }

}
/*void print_char8x8(unsigned char * membuffer, bmaptype * font,unsigned short x, unsigned short y,unsigned char c)
{
	unsigned short j,k,l,c1;
	unsigned char *ptr_src;
	unsigned char *ptr_dst;

	ptr_dst=(unsigned char*)membuffer;
	ptr_src=(unsigned char*)&font->data[0];

	x=x>>3;
	//x=((x&(~0x1))<<1)+(x&1);//  0 1   2 3
	ptr_dst=ptr_dst + ((y*80)+ x);
	ptr_src=ptr_src + (((c>>4)*(8*8*2))+(c&0xF));
	for(j=0;j<8;j++)
	{
		*ptr_dst=*ptr_src;
		ptr_src=ptr_src+16;
		ptr_dst=ptr_dst+80;
	}

}
*/


void print_str(unsigned char * membuffer,char * buf,unsigned short x_pos,unsigned short y_pos,unsigned char font)
{
	unsigned short i;
	i=0;
	
	switch(font)
	{
	case 8:
		while(buf[i])
		{
			if(x_pos<=(SCREEN_XRESOL-8))
			{
				print_char8x8(membuffer,bitmap_font8x8_bmp,x_pos,y_pos,buf[i]);
				x_pos=x_pos+8;
			}
			i++;
		}
	break;
	case 16:
		while(buf[i])
		{
			if(x_pos<=(SCREEN_XRESOL-16))
			{
				print_char(membuffer,bitmap_font_bmp,x_pos,y_pos,buf[i]);
				x_pos=x_pos+16;
			}
			i++;
		}
	break;
	}
}

int hxc_printf(unsigned char mode,unsigned short x_pos,unsigned short y_pos,char * chaine, ...)
{
      char temp_buffer[1024];

      va_list marker;
      va_start( marker, chaine );

      vsnprintf(temp_buffer,1024,chaine,marker);
      switch(mode)
      {
        case 0:
        print_str(screen_buffer_aligned,temp_buffer,x_pos,y_pos,8);
        break;
        case 1:
        print_str(screen_buffer_aligned,temp_buffer,(SCREEN_XRESOL-(strlen(temp_buffer)*8))/2,y_pos,8);
        break;
        case 2:
        print_str(screen_buffer_aligned,temp_buffer,(SCREEN_XRESOL-(strlen(temp_buffer)*8)),y_pos,8);
        break;
        case 4:
        print_str(screen_buffer_aligned,temp_buffer,x_pos,y_pos,16);
        break;
        case 5:
        print_str(screen_buffer_aligned,temp_buffer,(SCREEN_XRESOL-(strlen(temp_buffer)*16))/2,y_pos,16);
        break;
        case 6:
        print_str(screen_buffer_aligned,temp_buffer,(SCREEN_XRESOL-(strlen(temp_buffer)*16)),y_pos,16);
        break;
      }

      va_end( marker );

      return 0;
}

void h_line(unsigned short y_pos,unsigned short val)
{
  unsigned short *ptr_dst;
  unsigned short i,ptroffset;

  ptr_dst=(unsigned short*)screen_buffer_aligned;
  ptroffset=80* y_pos;

  for(i=0;i<80;i++)
  {
     ptr_dst[ptroffset+i]=val;
  }

}
void box(unsigned short x_p1,unsigned short y_p1,unsigned short x_p2,unsigned short y_p2,unsigned short fillval,unsigned char fill)
{
	unsigned short *ptr_dst;
	unsigned short i,j,ptroffset,x_size;


	ptr_dst=(unsigned short*)screen_buffer_aligned;

	x_size=((x_p2-x_p1)/16)*2;

	for(j=0;j<(y_p2-y_p1);j++)
	{
		for(i=0;i<x_size;i++)
		{
			ptr_dst[ptroffset+i]=fillval;
		}
		ptroffset=80* (y_p1+j);
	}
}

void clear_line(unsigned short y_pos,unsigned short val)
{
	unsigned char i;
	for(i=0;i<8;i++)
		h_line(y_pos+i,val);
}

void invert_line(unsigned short y_pos)
{
	unsigned char i,j;
	unsigned short *ptr_dst;
	unsigned short ptroffset;

 for(j=0;j<8;j++)
 {
  ptr_dst=(unsigned short*)screen_buffer_aligned;
  ptroffset=80* (y_pos+j);

  for(i=0;i<80;i++)
  {
     ptr_dst[ptroffset+i]=ptr_dst[ptroffset+i]^0xFFFF;
  }
 }
}

void restore_box()
{
	memcpy(&screen_buffer_aligned[160*70],screen_buffer_backup_aligned, (8*1000) + 256);
}

int hxc_printf_box(unsigned char mode,char * chaine, ...)
{
	char temp_buffer[1024];
	int str_size;
	unsigned short i;

	memcpy(screen_buffer_backup_aligned,&screen_buffer_aligned[160*70], (8*1000) + 256);

	va_list marker;
	va_start( marker, chaine );

	vsnprintf(temp_buffer,1024,chaine,marker);

	str_size=strlen(temp_buffer) * 8;
	str_size=str_size+(4*8);

	for(i=0;i< str_size;i=i+8)
	{
        print_char8x8(screen_buffer_aligned,bitmap_font8x8_bmp,((SCREEN_XRESOL-str_size)/2)+i,80-8,8);
	}
	print_char8x8(screen_buffer_aligned,bitmap_font8x8_bmp,((SCREEN_XRESOL-str_size)/2)+(i-8),80-8,3);
	print_char8x8(screen_buffer_aligned,bitmap_font8x8_bmp,((SCREEN_XRESOL-str_size)/2),80-8,2);

	for(i=0;i< str_size;i=i+8)
	{
        print_char8x8(screen_buffer_aligned,bitmap_font8x8_bmp,((SCREEN_XRESOL-str_size)/2)+i,80,' ');
	}

	print_str(screen_buffer_aligned,temp_buffer,((SCREEN_XRESOL-str_size)/2)+(2*8),80,8);
	print_char8x8(screen_buffer_aligned,bitmap_font8x8_bmp,((SCREEN_XRESOL-str_size)/2)+(i-8),80,7);
	print_char8x8(screen_buffer_aligned,bitmap_font8x8_bmp,((SCREEN_XRESOL-str_size)/2),80,6);

	for(i=0;i< str_size;i=i+8)
	{
        print_char8x8(screen_buffer_aligned,bitmap_font8x8_bmp,((SCREEN_XRESOL-str_size)/2)+i,80+8,9);
	}
	print_char8x8(screen_buffer_aligned,bitmap_font8x8_bmp,((SCREEN_XRESOL-str_size)/2)+(i-8),80+8,5);
	print_char8x8(screen_buffer_aligned,bitmap_font8x8_bmp,((SCREEN_XRESOL-str_size)/2),80+8,4);

	va_end( marker );
}

void init_buffer()
{
	int i;
	display_sprite(screen_buffer_aligned, bitmap_hxc2001logo_bmp,(SCREEN_XRESOL-bitmap_hxc2001logo_bmp->Xsize), (SCREEN_YRESOL-bitmap_hxc2001logo_bmp->Ysize));
	display_sprite(screen_buffer_aligned, bitmap_sdhxcfelogo_bmp,(SCREEN_XRESOL-bitmap_sdhxcfelogo_bmp->Xsize)/2, (SCREEN_YRESOL-bitmap_sdhxcfelogo_bmp->Ysize));

	h_line(SCREEN_YRESOL-34,0xFFFF) ;
	h_line(SCREEN_YRESOL-((48+(3*8))+2),0xFFFF) ;
	h_line(8,0xFFFF) ;

	hxc_printf(0,0,SCREEN_YRESOL-(8*1),"Ver %s",VERSIONCODE);

	hxc_printf(1,0,0,"SDCard HxC Floppy Emulator Manager for Atari ST");
	h_line(SCREEN_YRESOL-(48+20)+24-2,0xFFFF) ;
	hxc_printf(1,0,SCREEN_YRESOL-(48+20)+24,">>>Press HELP key for the function key list<<<");

	i=1;
	hxc_printf(1,0,HELP_Y_POS+(i*8), "SDCard HxC Floppy Emulator file selector for Atari ST");
	i++;
	hxc_printf(1,0,HELP_Y_POS+(i*8), "(c) 2006-2011 HxC2001 / Jean-Francois DEL NERO");
	i++;
	hxc_printf(1,0,HELP_Y_POS+(i*8), "Check for updates on :");
	i++;
	hxc_printf(1,0,HELP_Y_POS+(i*8), "http://hxc2001.free.fr/floppy_drive_emulator/");
	i++;
	hxc_printf(1,0,HELP_Y_POS+(i*8), "Email : hxc2001@free.fr");
	i++;
	hxc_printf(1,0,HELP_Y_POS+(i*8), "V%s - %s",VERSIONCODE,DATECODE);


}

void initpal()
{
	volatile unsigned short * ptr;
	
	ptr=(unsigned short *)0xFF8240;
	*ptr=colortable[((color&0xF)*4)+0];
	ptr=(unsigned short *)0xFF8242;
	*ptr=colortable[((color&0xF)*4)+2];
	ptr=(unsigned short *)0xFF8244;
	*ptr=colortable[((color&0xF)*4)+3];
	ptr=(unsigned short *)0xFF8246;
	*ptr=colortable[((color&0xF)*4)+1];	

}

void set_color_scheme(unsigned char colorm)
{
	color=colorm;
    Supexec(initpal);
}

int init_display()
{
	unsigned short loop,yr;
	unsigned short k,i;

	screen_buffer_backup_aligned=(unsigned char*)malloc(16*1024);

	SCREEN_YRESOL=200;
	NUMBER_OF_FILE_ON_DISPLAY=19-5;// 19-5 //19 -240

	old_physical_adr=(unsigned long)Physbase();
	screen_buffer=(unsigned char*)malloc((32*1024) + 256);
	memset(screen_buffer,0,(32*1024) + 256);
	screen_buffer_aligned = (unsigned char*)(((unsigned long)screen_buffer| 0xff)+1);

        screen_buffer_backup=(unsigned char*)malloc((8*1000) + 256);
	memset(screen_buffer_backup,0,(8*1000) + 256);
	screen_buffer_backup_aligned = (unsigned char*)(((unsigned long)screen_buffer_backup| 0xff)+1);

	Blitmode(1);
	
    Setscreen( -1, screen_buffer_aligned, 1 );
	color=0;
    Supexec(initpal);

	init_buffer();
}



