//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <IniFiles.hpp>

#include "Project.h"

//---------------------------------------------------------------------------
PSMProject::PSMProject()
{
	modified = false;

	BmpKey[BM_SUMMER]   = "BMPSummer";
	BmpKey[BM_SPRING]   = "BMPSpring";
	BmpKey[BM_FALL]     = "BMPFall";
	BmpKey[BM_WINTER]   = "BMPWinter";
	BmpKey[BM_HSWINTER] = "BMPHSWinter";
	BmpKey[BM_LIGHTMAP] = "BMPLightmap";
	BmpKey[BM_ALPHA]    = "BMPAlpha";
}

void PSMProject::LoadFromFile(AnsiString prjfile)
{
	prjpath = prjfile;

	TIniFile *ini = new TIniFile(prjpath);
	hasSeason = ini->ReadBool("Source", "HasSeason", false);

	for (int i = 0; i < BM_MAX; i++) {
		BmpFiles[i] = ini->ReadString("Source", BmpKey[i], "");

	}

	LatLon base;
	base.lat.deg = ini->ReadFloat("Source", "Latitude", 0);
	base.lon.deg = ini->ReadFloat("Source", "Longitude", 0);
	trans.Base = base;

	XYparam res;
	res.x = ini->ReadFloat("Source", "Xresolution", 0.0001);
	res.y = ini->ReadFloat("Source", "Yresolution", 0.0001);
	trans.Resolution = res;

	outdir   = ini->ReadString("Output", "Directory", "");
	basefile = ini->ReadString("Output", "BaseFile", "");

	delete(ini);

	modified = false;
}

void PSMProject::SaveToFile(AnsiString prjfile)
{
	if (!prjfile.IsEmpty()) {
		prjpath = prjfile;
	}

	TIniFile *ini = new TIniFile(prjpath);
	ini->WriteBool("Source", "HasSeason", hasSeason);

	for (int i = 0; i < BM_MAX; i++) {
		ini->WriteString("Source", BmpKey[i], BmpFiles[i]);
	}

	LatLon base = trans.Base;
	ini->WriteFloat("Source", "Latitude", base.lat.deg);
	ini->WriteFloat("Source", "Longitude", base.lon.deg);

	XYparam res = trans.Resolution;
	ini->WriteFloat("Source", "Xresolution", res.x);
	ini->WriteFloat("Source", "Yresolution", res.y);

	ini->WriteString("Output", "Directory", outdir);
	ini->WriteString("Output", "BaseFile",  basefile);

	delete(ini);

	modified = false;
}
	
#pragma package(smart_init)
