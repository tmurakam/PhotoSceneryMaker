/*
 * Photo Scenery Maker
 *
 * Copyright (C) 2003 Takuya Murakami
 *
 * Bitmap.cpp : Alternate bitmap class
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "Bitmap.h"
//--------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall TBitmap2::TBitmap2(void)
{
	hBitmap = NULL;
	height = width = 0;
}

__fastcall TBitmap2::~TBitmap2()
{
	if (hBitmap) DeleteObject(hBitmap);
}

bool __fastcall TBitmap2::GetEmpty(void)
{
	return hBitmap == NULL ? true : false;
}

int __fastcall TBitmap2::GetHeight(void)
{
	return height;
}

int __fastcall TBitmap2::GetWidth(void)
{
	return width;
}

//
// Load bitmap from file
//
void __fastcall TBitmap2::LoadFromFile(const AnsiString FileName)
{
	if (hBitmap) DeleteObject(hBitmap);

	// Open bitmap file
	TFileStream *stream = new TFileStream(FileName,
		fmOpenRead | fmShareDenyWrite);

	BITMAPFILEHEADER 	bf;
	BITMAPINFOHEADER	bi;

	stream->ReadBuffer(&bf, sizeof(bf));
	stream->ReadBuffer(&bi, sizeof(bi));

	if (bf.bfType != 'B' | ('M' << 8) || bi.biBitCount != 24) {
		delete stream;
		AnsiString msg = _("Bitmap must be 24bit bmp file.");
		throw Exception(msg);
	}
	width = bi.biWidth;
	height = bi.biHeight;

	HDC dc = GetDC(NULL);
	VOID *bits;
	hBitmap = CreateDIBSection(dc, (BITMAPINFO *)&bi,
		DIB_RGB_COLORS, &bits, 0, 0);
	ReleaseDC(NULL, dc);

	DWORD size = ((bi.biWidth * bi.biBitCount + 31) / 32)
	  * 4 * bi.biHeight;

	stream->Seek(bf.bfOffBits,soFromBeginning);
	stream->ReadBuffer(bits, size);

	delete stream;
}

//
// Draw function to TCanvas
//
void __fastcall TBitmap2::Draw(TCanvas *canvas, const TRect &rect)
{
	HDC hDC = CreateCompatibleDC(canvas->Handle);
	SelectObject(hDC, hBitmap);
	int ret = BitBlt(canvas->Handle,
		rect.Left, rect.Top,
		rect.Width(), rect.Height(),
		hDC,
		0, 0,
		canvas->CopyMode);
	DeleteDC(hDC);
}


//
// Not implemented...
//
void __fastcall TBitmap2::SetHeight(int Height) {};
void __fastcall TBitmap2::SetWidth(int Width) {};
void __fastcall TBitmap2::LoadFromStream(TStream *stream) {};
void __fastcall TBitmap2::SaveToStream(TStream *stream) {};
void __fastcall TBitmap2::LoadFromClipboardFormat(Word AFormat, unsigned AData,
		HPALETTE APalette) {};
void __fastcall TBitmap2::SaveToClipboardFormat(Word &AFormat, unsigned &AData,
		HPALETTE &APalette) {};

