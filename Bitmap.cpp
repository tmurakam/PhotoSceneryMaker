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

bool __fastcall TBitmap2::GetEmpty(void) { return hBitmap == NULL ? true : false; };
int __fastcall TBitmap2::GetHeight(void) { return height; };
int __fastcall TBitmap2::GetWidth(void) { return width; };

// dummy funcs...
void __fastcall TBitmap2::SetHeight(int Height) {};
void __fastcall TBitmap2::SetWidth(int Width) {};
void __fastcall TBitmap2::LoadFromStream(TStream *stream) {};
void __fastcall TBitmap2::SaveToStream(TStream *stream) {};
void __fastcall TBitmap2::LoadFromClipboardFormat(Word AFormat, unsigned AData,
		HPALETTE APalette) {};
void __fastcall TBitmap2::SaveToClipboardFormat(Word &AFormat, unsigned &AData,
		HPALETTE &APalette) {};


void __fastcall TBitmap2::LoadFromFile(const AnsiString FileName)
{
	if (hBitmap) DeleteObject(hBitmap);

	// Open bitmap file
	TFileStream *stream = new TFileStream(FileName,
		fmOpenRead | fmShareDenyWrite);

#if 0
	// Load to memory
	__int64 sz = stream->Size;
	char *dib = new char[sz];
	stream->ReadBuffer(dib, sz);
	delete stream;

	BITMAPFILEHEADER *bf = (BITMAPFILEHEADER *)dib;
	BITMAPINFOHEADER *bi = (BITMAPINFOHEADER *)(bf + 1);
	width = bi->biWidth;
	height = bi->biHeight;

	char *bits = dib + bf->bfOffBits;

	// Create DDB
	HDC hDC = GetDC(0);
	HDC MemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateDIBitmap(MemDC, bi, CBM_INIT, bits,
		(BITMAPINFO *)bi, DIB_RGB_COLORS);
	DeleteDC(MemDC);
	ReleaseDC(0, hDC);
	
	delete dib;
#else
	BITMAPFILEHEADER 	bf;
	BITMAPINFOHEADER	bi;

	stream->ReadBuffer(&bf, sizeof(bf));
	stream->ReadBuffer(&bi, sizeof(bi));

	if (bi.biBitCount != 24) {
		// oops
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

	//fseek(fp, bf.bfOffBits, SEEK_SET);
	stream->Seek(bf.bfOffBits,soFromBeginning);
	//int len = fread(bits, 1, size, fp);
	stream->ReadBuffer(bits, size);

	//fclose(fp);
	delete stream;
#endif
}

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
