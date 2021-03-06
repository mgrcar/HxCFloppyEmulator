/*
//
// Copyright (C) 2006 - 2013 Jean-Fran?ois DEL NERO
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

#pragma pack(1)

typedef struct picfileformatheader_
{
	unsigned char HEADERSIGNATURE[8];
	unsigned char formatrevision;
	unsigned char number_of_track;
	unsigned char number_of_side;
	unsigned char track_encoding;
	unsigned short bitRate;
	unsigned short floppyRPM;
	unsigned char floppyinterfacemode;
	unsigned char write_protected;
	unsigned short track_list_offset;
	unsigned char write_allowed;
	unsigned char single_step;
	unsigned char track0s0_altencoding;
	unsigned char track0s0_encoding;
	unsigned char track0s1_altencoding;
	unsigned char track0s1_encoding;
}picfileformatheader;


typedef struct pictrack_
{
	unsigned short offset;
	unsigned short track_len;
}pictrack;

#pragma pack()

#pragma pack(1)

typedef struct picfileformatextheader_
{
	unsigned char HEADERSIGNATURE[8];
	unsigned char formatrevision;
	unsigned char number_of_track;
	unsigned char number_of_side;
	unsigned char track_encoding;
	unsigned short bitRate;
	unsigned short floppyRPM;
	unsigned char floppyinterfacemode;
	unsigned char write_protected;
	unsigned short track_list_offset;
	unsigned char write_allowed;
	unsigned char single_step;
	unsigned char track0s0_altencoding;
	unsigned char track0s0_encoding;
	unsigned char track0s1_altencoding;
	unsigned char track0s1_encoding;
}picfileformatextheader;


typedef struct picexttrack_
{
	unsigned short offset;
	unsigned short track_len;
}picexttrack;

#pragma pack()

