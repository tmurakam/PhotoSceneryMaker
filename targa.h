//---------------------------------------------------------------------------

#ifndef targaH
#define targaH
//---------------------------------------------------------------------------
/* tga ƒwƒbƒ_ */
typedef struct {
	unsigned char   id;
	unsigned char   map;
	unsigned char   format;
	unsigned char   mapinfo[5];
	unsigned short  x;
	unsigned short  y;
	unsigned short  width;
	unsigned short  height;
	unsigned char   depth;
	unsigned char   xxx;
} TGAHEADER;

typedef struct {
	unsigned long   fpos;
	unsigned long   dpos;
	char            str[18];
} TGATRAILER;


/* external function */
void MergeAlphaTextures(AnsiString bmpfiles[],
	AnsiString alphafile, AnsiString tgafiles[]);

#endif
