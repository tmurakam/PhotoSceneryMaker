//---------------------------------------------------------------------------

#ifndef bitmapH
#define bitmapH

#include <stdio.h>

//---------------------------------------------------------------------------
class TBitmap2 : public TGraphic
{
private:
	int height;
	int width;

	HBITMAP hBitmap;

public:
	__fastcall TBitmap2(void);
	__fastcall ~TBitmap2();
	
	void __fastcall LoadFromFile(const AnsiString FileName);
	void __fastcall Draw(TCanvas *canvas, const TRect &rect);

	bool __fastcall GetEmpty(void);
	int __fastcall GetHeight(void);
	int __fastcall GetWidth(void);
	void __fastcall SetHeight(int Height);
	void __fastcall SetWidth(int Width);
	void __fastcall LoadFromStream(TStream *stream);
	void __fastcall SaveToStream(TStream *stream);
	void __fastcall LoadFromClipboardFormat(Word AFormat, unsigned AData,
		HPALETTE APalette);
	void __fastcall SaveToClipboardFormat(Word &AFormat, unsigned &AData,
		HPALETTE &APalette);
	void __fastcall SetPalette(HPALETTE Value) {};

};

//---------------------------------------------------------------------------
#endif
