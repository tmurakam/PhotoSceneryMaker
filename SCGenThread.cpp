/*
 * Photo Scenery Maker
 *
 * Copyright (C) 2003 Takuya Murakami
 *
 * SCGenFrm.cpp : Scenery generation thread
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

#include "gnugettext.hpp"

#include "SCGenThread.h"
#include "SCGenFrm.h"
#include "OptDlg.h"
#include "targa.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
const char *SeasonSuffix[] = SEASON_SUFFIX;
const char *SeasonName[] = SEASON_NAME;

//---------------------------------------------------------------------------

// 注意：異なるスレッドが所有する VCL のメソッド/関数/プロパティを
//       別のスレッドの中から扱う場合，排他処理の問題が発生します。
//
//       メインスレッドの所有するオブジェクトに対しては Synchronize
//       メソッドを使う事ができます。他のオブジェクトを参照するため
//       のメソッドをスレッドクラスに追加し，Synchronize メソッドの
//       引数として渡します。
//
//       たとえば UpdateCaption を以下のように定義し、
//
//      void __fastcall SCGenThread::UpdateCaption()
//      {
//        Form1->Caption = "スレッドから書き換えました";
//      }
//
//       Execute メソッドの中で Synchronize メソッドに渡すことでメイ
//       ンスレッドが所有する Form1 の Caption プロパティを安全に変
//       更できます。
//
//      Synchronize(UpdateCaption);
//
//---------------------------------------------------------------------------

__fastcall SCGenThread::SCGenThread(PSMProject *proj, int sw)
	: TThread(true)
{
	Sw = sw;
	Proj = proj;
}
//---------------------------------------------------------------------------
void __fastcall SCGenThread::Execute()
{
	try {
		if (Sw & EX_MAKEINF) MakeInf();
		if (!Terminated && Sw & EX_RESAMPLE) Resample();
		if (!Terminated && Sw & EX_MRGALPHA) MergeAlpha();
		if (!Terminated && Sw & EX_CONVTEX) ConvTex();
		if (!Terminated && Sw & EX_GENBGL) GenBgl();

		if (Terminated) {
			ResultMsg = _("Interrupted");
		} else {
			ResultMsg = _("Done");
		}
	}
	catch (Exception &e) {
		//ShowMessage(e.Message);
		ResultMsg = e.Message;
	}

	Synchronize(ShowMsg);
}

void __fastcall SCGenThread::ShowMsg(void)
{
	Application->MessageBox(ResultMsg.c_str(), "Result", MB_OK);
}

//---------------------------------------------------------------------------
// INF ファイル作成
void SCGenThread::MakeInf(void)
{
	AnsiString msg = _("Creating inf files...");
	SetStatusMsg(msg);

	// 出力ディレクトリを作成
	MkDir(Proj->OutDir);
	MkDir(Proj->OutDir + "\\TmpBmp");
	MkDir(Proj->OutDir + "\\TmpAlpha");

	if (!Proj->HasSeason) {
		MakeInf(BM_SUMMER);
		MakeInf(BM_ALPHA);
	} else {
		for (int i = 0; i < BM_MAX; i++) {
			MakeInf(i);
		}
	}
}

// INF ファイル名決定
AnsiString SCGenThread::InfFileName(int season)
{
	AnsiString path = Proj->OutDir;
	path.sprintf("%s\\%s.inf", Proj->OutDir, SeasonSuffix[season]);

	return path;
}

// BMP パス決定
AnsiString SCGenThread::BmpPath(int season)
{
	AnsiString path;
	if (season == BM_ALPHA) {
		path.sprintf("%s\\TmpAlpha", Proj->OutDir);
	} else {
		path.sprintf("%s\\TmpBmp", Proj->OutDir);
	}
	return path;
}

// 一個の INF ファイルを生成する
void SCGenThread::MakeInf(int season)
{
	AnsiString inf = InfFileName(season);

	AnsiString BmpFile = Proj->BmpFile(season);
	if (BmpFile.IsEmpty()) {
		return;
	}

	// inf ファイルを作成
	FILE *fp;
	fp = fopen(inf.c_str(), "w");
	if (!fp) {
		AnsiString msg, fmt;
		fmt = _("Can't open inf file : %s");
		msg.sprintf(fmt.c_str(), inf.c_str());
		throw Exception(msg);
	}

	Transform *trans = Proj->Trans;

	// Source セクション
	fprintf(fp, "[Source]\n");
	fprintf(fp, "\tType = CUSTOM\n");
	fprintf(fp, "\tSourceDir = \"%s\"\n", ExtractFileDir(BmpFile).c_str());
	fprintf(fp, "\tSourceFile = \"%s\"\n", ExtractFileName(BmpFile).c_str());
	fprintf(fp, "\tLat = %.24f\n", trans->Base.lat.deg);
	fprintf(fp, "\tLon = %.24f\n", trans->Base.lon.deg);
	fprintf(fp, "\tNumOfCellsPerLine = %d\n", trans->Width);
	fprintf(fp, "\tNumOfLines = %d\n", trans->Height);
	fprintf(fp, "\tCellXdimensionDeg = %.24f\n", trans->Resolution.x);
	fprintf(fp, "\tCellYdimensionDeg = %.24f\n", trans->Resolution.y);

	// Destination
	fprintf(fp, "\n[Destination]\n");
	fprintf(fp, "\tDestDir = \"%s\"\n", BmpPath(season).c_str());
	fprintf(fp, "\tDestBaseFileName = \"%s\"\n", Proj->BaseFile.c_str());
	fprintf(fp, "\tUseSourceDimensions = 1\n");
	if (Proj->HasSeason) {
		fprintf(fp, "\twithseasons = 1\n");
		fprintf(fp, "\tseason = %s\n", SeasonName[season]);
	}
		
	fclose(fp);
}
//---------------------------------------------------------------------------
// Execute generic commands
int SCGenThread::ExecCmd(AnsiString cmdline, AnsiString desc)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	int ret = 0;

	memset(&si, 0, sizeof(si));
	si.cb=sizeof(si);

	ret = CreateProcess(NULL, cmdline.c_str(),NULL,NULL,FALSE,
		BELOW_NORMAL_PRIORITY_CLASS,
		NULL,NULL,&si,&pi);

	if (ret == 0) {
		// execution failed
		LPTSTR buf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			LANG_USER_DEFAULT,
//			MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			(LPTSTR)&buf,
			0,
			NULL );

		AnsiString msg; 
		msg.sprintf("%s : %s", desc, buf);
		LocalFree(buf);

		throw Exception(msg);
	}


	for (;;) {
		int st = WaitForSingleObject(pi.hProcess, 200);
		if (st == WAIT_OBJECT_0) break;

		if (Terminated) {
			TerminateProcess(pi.hProcess, 0);
		}
	}
		
	CloseHandle(pi.hProcess);

	return 0;
}

//---------------------------------------------------------------------------
// resample 実行
void SCGenThread::Resample(void)
{
	for (int i = 0; i < BM_MAX; i++) {
		if (Terminated) return;

		if (i != BM_SUMMER && i != BM_ALPHA && !Proj->HasSeason) {
			continue;
		}
		if (i == BM_ALPHA && !Proj->HasAlpha) {
			continue;
		}

		AnsiString fmt = _("Resampling : %s...");
		AnsiString msg;
		msg.sprintf(fmt.c_str(), SeasonName[i]);
		SetStatusMsg(msg);

		// 作業用ディレクトリにchdir する
		AnsiString bmppath = BmpPath(i);
		ChDir(bmppath);

		// resample を実行する
		AnsiString inf = InfFileName(i);
	
		AnsiString cmdline;
		cmdline.sprintf("%s\\resample.exe %s",
				OptionDlg->GetSDKPath(), inf);
		if (ExecCmd(cmdline, "resample.exe") != 0) {
			AnsiString title = _("Error");
			AnsiString mes = _("Some errors occured.");
			Application->MessageBox(mes.c_str(), title.c_str(), MB_OK);
		}
	}

	// tmf ファイルを移動する
	AnsiString tmf;
	tmf.sprintf("%s\\%s.tmf", BmpPath(BM_SUMMER), Proj->BaseFile);
	AnsiString newtmf;
	newtmf.sprintf("%s\\%s.tmf", Proj->OutDir, Proj->BaseFile);

	DeleteFile(newtmf.c_str());
	MoveFile(tmf.c_str(), newtmf.c_str());

	// いらないファイルを削除
	tmf.sprintf("%s\\%s.tmf", BmpPath(BM_ALPHA), Proj->BaseFile);
	DeleteFile(tmf.c_str());
}
//---------------------------------------------------------------------------
// Alpha テクスチャのマージ
void SCGenThread::MergeAlpha(void)
{
	AnsiString msg = _("Merging Alpha Textures...");
	SetStatusMsg(msg);

	// texture ディレクトリ生成
	AnsiString texdir;
	texdir.sprintf("%s\\texture", Proj->OutDir);
	MkDir(texdir);

	// Targa ファイルを生成する
	AnsiString bmpdir = BmpPath(BM_SUMMER);
	AnsiString alphadir = BmpPath(BM_ALPHA);

	AnsiString searchfile;
	searchfile.sprintf("%s\\*su.bmp", bmpdir);

	AnsiString alphafile, bmpfiles[BM_MAX], tgafiles[BM_MAX], pre;

	alphafile = "";
	
	TSearchRec rec;
	int ret = FindFirst(searchfile, faAnyFile & ~faDirectory, rec);
	int count = 1;
	while (ret == 0) {
		if (Terminated) return;

		// Show current status
		AnsiString st;
		st.sprintf("Merging textures : %d", count++);
		SetStatusMsg(st);

		// ファイル名の最初の部分
		int len = rec.Name.Length();
		pre = rec.Name.SubString(1, len - 6);

		// alpha テクスチャ名
		if (Proj->HasAlpha) {
			alphafile.sprintf("%s\\%ssu.bmp", alphadir, pre);
		}

		// bmp ファイル名
		for (int i = 0; i < BM_MAX; i++) {
			if (i == BM_ALPHA) continue;
			bmpfiles[i].sprintf("%s\\%s%s.bmp", bmpdir, pre, SeasonSuffix[i]);
			tgafiles[i].sprintf("%s\\%s%s.tga", texdir, pre, SeasonSuffix[i]);
		}

		MergeAlphaTextures(bmpfiles, alphafile, tgafiles, Proj->HasSeason);

		ret = FindNext(rec);
	}
	FindClose(rec);
}

//---------------------------------------------------------------------------
// テクスチャ変換
void SCGenThread::ConvTex(void)
{
	AnsiString msg = _("Converting Textures...");
	SetStatusMsg(msg);

	AnsiString Imagetool;
	Imagetool.sprintf("%s\\imagetool.exe", OptionDlg->GetImagetoolPath());

	AnsiString cmdline;
	cmdline.sprintf("\"%s\" -DXT1 -e bmp -terrainphoto \"%s\\texture\\*.tga\"",
		Imagetool, Proj->OutDir);

	ExecCmd(cmdline, "imagetool.exe");
}
//---------------------------------------------------------------------------
// BGL 生成
void SCGenThread::GenBgl(void)
{
	AnsiString msg = _("Generating BGL files...");
	SetStatusMsg(msg);

	AnsiString outdir = Proj->OutDir;
	AnsiString basefile = Proj->BaseFile;

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
	ExecCmd(cmdline, "tmfcompress.exe");
	if (Terminated) return;

	// BGL に変換する
	cmdline.sprintf("%s\\tmf2bgl.exe %s %s", sdkpath, tmfc, bgl);
	ExecCmd(cmdline, "tmf2bgl.exe");
}

//---------------------------------------------------------------------------
// BGL 生成
void SCGenThread::SetStatusMsg(AnsiString msg)
{
	SCGenForm->StatusBar->SimpleText = msg;
}
