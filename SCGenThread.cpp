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

// ���ӁF�قȂ�X���b�h�����L���� VCL �̃��\�b�h/�֐�/�v���p�e�B��
//       �ʂ̃X���b�h�̒����爵���ꍇ�C�r�������̖�肪�������܂��B
//
//       ���C���X���b�h�̏��L����I�u�W�F�N�g�ɑ΂��Ă� Synchronize
//       ���\�b�h���g�������ł��܂��B���̃I�u�W�F�N�g���Q�Ƃ��邽��
//       �̃��\�b�h���X���b�h�N���X�ɒǉ����CSynchronize ���\�b�h��
//       �����Ƃ��ēn���܂��B
//
//       ���Ƃ��� UpdateCaption ���ȉ��̂悤�ɒ�`���A
//
//      void __fastcall SCGenThread::UpdateCaption()
//      {
//        Form1->Caption = "�X���b�h���珑�������܂���";
//      }
//
//       Execute ���\�b�h�̒��� Synchronize ���\�b�h�ɓn�����ƂŃ��C
//       ���X���b�h�����L���� Form1 �� Caption �v���p�e�B�����S�ɕ�
//       �X�ł��܂��B
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
// INF �t�@�C���쐬
void SCGenThread::MakeInf(void)
{
	AnsiString msg = _("Creating inf files...");
	SetStatusMsg(msg);

	// �o�̓f�B���N�g�����쐬
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

// INF �t�@�C��������
AnsiString SCGenThread::InfFileName(int season)
{
	AnsiString path = Proj->OutDir;
	path.sprintf("%s\\%s.inf", Proj->OutDir, SeasonSuffix[season]);

	return path;
}

// BMP �p�X����
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

// ��� INF �t�@�C���𐶐�����
void SCGenThread::MakeInf(int season)
{
	AnsiString inf = InfFileName(season);

	AnsiString BmpFile = Proj->BmpFile(season);
	if (BmpFile.IsEmpty()) {
		return;
	}

	// inf �t�@�C�����쐬
	FILE *fp;
	fp = fopen(inf.c_str(), "w");
	if (!fp) {
		AnsiString msg, fmt;
		fmt = _("Can't open inf file : %s");
		msg.sprintf(fmt.c_str(), inf.c_str());
		throw Exception(msg);
	}

	Transform *trans = Proj->Trans;

	// Source �Z�N�V����
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
// resample ���s
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

		// ��Ɨp�f�B���N�g����chdir ����
		AnsiString bmppath = BmpPath(i);
		ChDir(bmppath);

		// resample �����s����
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

	// tmf �t�@�C�����ړ�����
	AnsiString tmf;
	tmf.sprintf("%s\\%s.tmf", BmpPath(BM_SUMMER), Proj->BaseFile);
	AnsiString newtmf;
	newtmf.sprintf("%s\\%s.tmf", Proj->OutDir, Proj->BaseFile);

	DeleteFile(newtmf.c_str());
	MoveFile(tmf.c_str(), newtmf.c_str());

	// ����Ȃ��t�@�C�����폜
	tmf.sprintf("%s\\%s.tmf", BmpPath(BM_ALPHA), Proj->BaseFile);
	DeleteFile(tmf.c_str());
}
//---------------------------------------------------------------------------
// Alpha �e�N�X�`���̃}�[�W
void SCGenThread::MergeAlpha(void)
{
	AnsiString msg = _("Merging Alpha Textures...");
	SetStatusMsg(msg);

	// texture �f�B���N�g������
	AnsiString texdir;
	texdir.sprintf("%s\\texture", Proj->OutDir);
	MkDir(texdir);

	// Targa �t�@�C���𐶐�����
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

		// �t�@�C�����̍ŏ��̕���
		int len = rec.Name.Length();
		pre = rec.Name.SubString(1, len - 6);

		// alpha �e�N�X�`����
		if (Proj->HasAlpha) {
			alphafile.sprintf("%s\\%ssu.bmp", alphadir, pre);
		}

		// bmp �t�@�C����
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
// �e�N�X�`���ϊ�
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
// BGL ����
void SCGenThread::GenBgl(void)
{
	AnsiString msg = _("Generating BGL files...");
	SetStatusMsg(msg);

	AnsiString outdir = Proj->OutDir;
	AnsiString basefile = Proj->BaseFile;

	// BGL �o�̓f�B���N�g������
	AnsiString scdir;
	scdir.sprintf("%s\\scenery", outdir);
	MkDir(scdir);

	// BGL �t�@�C��������
	AnsiString bgl;
	bgl.sprintf("%s\\scenery\\%s.bgl", outdir, basefile);

	// ���ԃt�@�C��������
	AnsiString tmf, tmfc;
	tmf.sprintf("%s\\%s.tmf", outdir, basefile);
	tmfc.sprintf("%s\\%s-cmp.tmf", outdir, basefile);

	AnsiString sdkpath;
	sdkpath = OptionDlg->GetSDKPath();

	// compress ����
	AnsiString cmdline;
	cmdline.sprintf("%s\\tmfcompress.exe %s %s", sdkpath, tmf, tmfc);
	ExecCmd(cmdline, "tmfcompress.exe");
	if (Terminated) return;

	// BGL �ɕϊ�����
	cmdline.sprintf("%s\\tmf2bgl.exe %s %s", sdkpath, tmfc, bgl);
	ExecCmd(cmdline, "tmf2bgl.exe");
}

//---------------------------------------------------------------------------
// BGL ����
void SCGenThread::SetStatusMsg(AnsiString msg)
{
	SCGenForm->StatusBar->SimpleText = msg;
}
