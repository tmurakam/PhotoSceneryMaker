//---------------------------------------------------------------------------

#ifndef ProjectH
#define ProjectH

#include "Transform.h"

//---------------------------------------------------------------------------
// Photo Scenery Maker プロジェクト管理クラス

enum {
	BM_SUMMER = 0,
	BM_SPRING,
	BM_FALL,
	BM_WINTER,
	BM_HSWINTER,
	BM_LIGHTMAP,
	BM_ALPHA,
	BM_MAX
};

#define	SEASON_SUFFIX	{"su", "sp", "fa", "wi", "hw", "lm", "al"}
#define	SEASON_NAME	{"summer", "spring", "fall", \
	"mild winter", "harsh winter", "lightmap", "summer"}

class PSMProject
{
private:
	bool modified;

	// Source information
	AnsiString BmpFiles[BM_MAX];
	AnsiString BmpKey[BM_MAX];
	bool hasSeason;

	// Transform information
	Transform trans;

	// Output information
	AnsiString outdir;
	AnsiString basefile;

	// project info
	AnsiString	prjpath;
 
	Transform *readTransform(void)		{ return &trans; }

	void writeHasSeason(bool s)		{ hasSeason = s; modified = true; }
	void writeOutDir(AnsiString d)		{ outdir = d; modified = true; }
	void writeBaseFile(AnsiString b)	{ basefile = b; modified = true; }

public:
	PSMProject();

	void LoadFromFile(AnsiString file);
	void SaveToFile(AnsiString file = "");

	AnsiString BmpFile(int idx = BM_SUMMER)	{ return BmpFiles[idx]; }
	void SetBmpFile(int idx, AnsiString b)	{ BmpFiles[idx] = b; modified = true; }

	__property bool Modified = {read=modified, write=modified};
	__property Transform * Trans = {read=readTransform};

	__property bool HasSeason = {read=hasSeason, write=writeHasSeason};
	__property AnsiString OutDir = {read=outdir, write=writeOutDir};
	__property AnsiString BaseFile = {read=basefile, write=writeBaseFile};
};

#endif
