/*
 * Photo Scenery Maker
 *
 * Copyright (C) 2003 Takuya Murakami
 *
 * SCGenFrm.cpp : Scenery generation dialog
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
#include <IniFiles.hpp>
#include <SysUtils.hpp>
#include <stdio.h>

#include "gnugettext.hpp"

#include "Transform.h"

#include "SCGenFrm.h"
#include "Util.h"
#include "OptDlg.h"
#include "Exec.h"
#include "targa.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSCGenForm *SCGenForm;

const char *SeasonSuffix[] = SEASON_SUFFIX;
const char *SeasonName[] = SEASON_NAME;

//---------------------------------------------------------------------------
__fastcall TSCGenForm::TSCGenForm(TComponent* Owner)
	: TForm(Owner)
{
	proj = NULL;
}

//---------------------------------------------------------------------------
void __fastcall TSCGenForm::FormCreate(TObject *Sender)
{
	TranslateComponent(this);
}

//---------------------------------------------------------------------------
void TSCGenForm::SetParam(PSMProject *prj, int width, int height)
{
	proj = prj;
	
	Width = width;
	Height = height;
}

//---------------------------------------------------------------------------
// INF ファイル作成
void __fastcall TSCGenForm::ButtonMakeInfClick(TObject *Sender)
{
	// 出力ディレクトリを作成
	MkDir(proj->OutDir);
	MkDir(proj->OutDir + "\\TmpBmp");
	MkDir(proj->OutDir + "\\TmpAlpha");

	if (!proj->HasSeason) {
		MakeInf(BM_SUMMER);
		MakeInf(BM_ALPHA);
	} else {
		for (int i = 0; i < BM_MAX; i++) {
			MakeInf(i);
		}
	}
}

//---------------------------------------------------------------------------
// INF ファイル名決定
AnsiString TSCGenForm::InfFileName(int season)
{
	AnsiString path = proj->OutDir;
	path.sprintf("%s\\%s.inf", proj->OutDir, SeasonSuffix[season]);

	return path;
}

// BMP パス決定
AnsiString TSCGenForm::BmpPath(int season)
{
	AnsiString path;
	if (season == BM_ALPHA) {
		path.sprintf("%s\\TmpAlpha", proj->OutDir);
	} else {
		path.sprintf("%s\\TmpBmp", proj->OutDir);
	}
	return path;
}

//---------------------------------------------------------------------------
// 一個の INF ファイルを生成する
void TSCGenForm::MakeInf(int season)
{
	AnsiString inf = InfFileName(season);

	AnsiString BmpFile = proj->BmpFile(season);
	if (BmpFile.IsEmpty()) {
		return;
	}

	// inf ファイルを作成
	FILE *fp;
	fp = fopen(inf.c_str(), "w");

	Transform *trans = proj->Trans;

	// Source セクション
	fprintf(fp, "[Source]\n");
	fprintf(fp, "\tType = CUSTOM\n");
	fprintf(fp, "\tSourceDir = \"%s\"\n", ExtractFileDir(BmpFile).c_str());
	fprintf(fp, "\tSourceFile = \"%s\"\n", ExtractFileName(BmpFile).c_str());
	fprintf(fp, "\tLat = %.24f\n", trans->Base.lat.deg);
	fprintf(fp, "\tLon = %.24f\n", trans->Base.lon.deg);
	fprintf(fp, "\tNumOfCellsPerLine = %d\n", Width);
	fprintf(fp, "\tNumOfLines = %d\n", Height);
	fprintf(fp, "\tCellXdimensionDeg = %.24f\n", trans->Resolution.x);
	fprintf(fp, "\tCellYdimensionDeg = %.24f\n", trans->Resolution.y);

	// Destination
	fprintf(fp, "\n[Destination]\n");
	fprintf(fp, "\tDestDir = \"%s\"\n", BmpPath(season).c_str());
	fprintf(fp, "\tDestBaseFileName = \"%s\"\n", proj->BaseFile.c_str());
	fprintf(fp, "\tUseSourceDimensions = 1\n");
	if (proj->HasSeason) {
		fprintf(fp, "\twithseasons = 1\n");
		fprintf(fp, "\tseason = %s\n", SeasonName[season]);
	}
		
	fclose(fp);
}

int TSCGenForm::ExecCmd(AnsiString cmdline)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	memset(&si, 0, sizeof(si));
	si.cb=sizeof(si);

	CreateProcess(NULL, cmdline.c_str(),NULL,NULL,FALSE,
		BELOW_NORMAL_PRIORITY_CLASS,
		NULL,NULL,&si,&pi);

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);

	return 0;
}

//---------------------------------------------------------------------------
// resample 実行
void __fastcall TSCGenForm::ButtonResampleClick(TObject *Sender)
{
	for (int i = 0; i < BM_MAX; i++) {
		if (!proj->HasSeason &&
		    i != BM_SUMMER && i != BM_ALPHA) {
			continue;
		}

		// 作業用ディレクトリにchdir する
		AnsiString bmppath = BmpPath(i);
		ChDir(bmppath);

		// resample を実行する
		AnsiString inf = InfFileName(i);
	
		AnsiString cmdline;
		cmdline.sprintf("%s\\resample.exe %s",
				OptionDlg->GetSDKPath(), inf);
		if (ExecCmd(cmdline) != 0) {
			AnsiString title = _("Error");
			AnsiString mes = _("Some errors occured.");
			Application->MessageBox(mes.c_str(), title.c_str(), MB_OK);
		}
	}

	// tmf ファイルを移動する
	AnsiString tmf;
	tmf.sprintf("%s\\%s.tmf", BmpPath(BM_SUMMER), proj->BaseFile);
	AnsiString newtmf;
	newtmf.sprintf("%s\\%s.tmf", proj->OutDir, proj->BaseFile);

	DeleteFile(newtmf.c_str());
	MoveFile(tmf.c_str(), newtmf.c_str());

	// いらないファイルを削除
	tmf.sprintf("%s\\%s.tmf", BmpPath(BM_ALPHA), proj->BaseFile);
	DeleteFile(tmf.c_str());
}
//---------------------------------------------------------------------------
// Alpha テクスチャのマージ
void __fastcall TSCGenForm::ButtonMergeAlphaClick(TObject *Sender)
{
	ProcessState(true);

	// texture ディレクトリ生成
	AnsiString texdir;
	texdir.sprintf("%s\\texture", proj->OutDir);
	MkDir(texdir);

	// Targa ファイルを生成する
	AnsiString bmpdir = BmpPath(BM_SUMMER);
	AnsiString alphadir = BmpPath(BM_ALPHA);

	AnsiString searchfile;
	searchfile.sprintf("%s\\*su.bmp", bmpdir);

	AnsiString alphafile, bmpfiles[BM_MAX], tgafiles[BM_MAX], pre;

	TSearchRec rec;
	int ret = FindFirst(searchfile, faAnyFile & ~faDirectory, rec);
	int count = 1;
	while (Processing && ret == 0) {
		AnsiString st;
		st.sprintf("Merging textures : %d", count++);
		StatusBar->SimpleText = st;
		Application->ProcessMessages();

		// ファイル名の最初の部分
		int len = rec.Name.Length();
		pre = rec.Name.SubString(1, len - 6);

		// alpha テクスチャ名
		alphafile.sprintf("%s\\%ssu.bmp", alphadir, pre);

		// bmp ファイル名
		for (int i = 0; i < BM_MAX; i++) {
			if (i == BM_ALPHA) continue;
			bmpfiles[i].sprintf("%s\\%s%s.bmp", bmpdir, pre, SeasonSuffix[i]);
			tgafiles[i].sprintf("%s\\%s%s.tga", texdir, pre, SeasonSuffix[i]);
		}
		MergeAlphaTextures(bmpfiles, alphafile, tgafiles);

		ret = FindNext(rec);
	}
	FindClose(rec);

	ProcessState(false);
}

//---------------------------------------------------------------------------
// テクスチャ変換
void __fastcall TSCGenForm::ButtonConvertTextureClick(TObject *Sender)
{
	AnsiString Imagetool;
	Imagetool.sprintf("%s\\imagetool.exe", OptionDlg->GetImagetoolPath());

	AnsiString cmdline;
	cmdline.sprintf("\"%s\" -DXT1 -e bmp -terrainphoto \"%s\\texture\\*.tga\"",
		Imagetool, proj->OutDir);

	ExecCmd(cmdline);
}
//---------------------------------------------------------------------------
// BGL 生成
void __fastcall TSCGenForm::ButtonGenBGLClick(TObject *Sender)
{
	AnsiString outdir = proj->OutDir;
	AnsiString basefile = proj->BaseFile;

	// BGL 出力ディレクトリ生成
	AnsiString scdir;
	scdir.sprintf("%s\\scenery", outdir);
	MkDir(scdir);

	// BGL ファイル名生成
	AnsiString bgl;
	bgl.sprintf("%s\\scenery\\%s.bgl", outdir, basefile);

	// 中間ファイル名生成
	AnsiString tmf, tmfc;
	tmf.sprintf("%s\\%s.tmf", outdir, basefile);
	tmfc.sprintf("%s\\%s-cmp.tmf", outdir, basefile);

	AnsiString sdkpath;
	sdkpath = OptionDlg->GetSDKPath();

	// compress する
	AnsiString cmdline;
	cmdline.sprintf("%s\\tmfcompress.exe %s %s", sdkpath, tmf, tmfc);
	ExecCmd(cmdline);

	// BGL に変換する
	cmdline.sprintf("%s\\tmf2bgl.exe %s %s", sdkpath, tmfc, bgl);
	ExecCmd(cmdline);
}
//---------------------------------------------------------------------------
// 全プロセス実行
void __fastcall TSCGenForm::ButtonDoAllClick(TObject *Sender)
{
	ButtonMakeInfClick(Sender);
	ButtonResampleClick(Sender);
	ButtonMergeAlphaClick(Sender);
	ButtonConvertTextureClick(Sender);
	ButtonGenBGLClick(Sender);

	Application->MessageBox("Done", "Done", MB_OK);
}

//---------------------------------------------------------------------------
void TSCGenForm::ProcessState(bool start)
{
	Processing = start;

	ButtonDoAll->Enabled = !start;
	ButtonMakeInf->Enabled = !start;
	ButtonResample->Enabled = !start;
	ButtonMergeAlpha->Enabled = !start;
	ButtonConvertTexture->Enabled = !start;
	ButtonGenBGL->Enabled = !start;
	ButtonClose->Enabled = !start;

	ButtonStop->Enabled = start;

	if (start == false) {
		StatusBar->SimpleText = "Ready";
	}
}
//---------------------------------------------------------------------------
void __fastcall TSCGenForm::ButtonStopClick(TObject *Sender)
{
	// 処理停止
	ProcessState(false);
}
//---------------------------------------------------------------------------


