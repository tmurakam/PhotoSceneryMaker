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
#include "SCGenThread.h"

#include "Util.h"
#include "OptDlg.h"
#include "targa.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSCGenForm *SCGenForm;

//---------------------------------------------------------------------------
__fastcall TSCGenForm::TSCGenForm(TComponent* Owner)
	: TForm(Owner)
{
	proj = NULL;
	thr = NULL;
}

//---------------------------------------------------------------------------
void __fastcall TSCGenForm::FormCreate(TObject *Sender)
{
	TranslateComponent(this);
}

//---------------------------------------------------------------------------
void TSCGenForm::SetParam(PSMProject *prj)
{
	proj = prj;
}

//---------------------------------------------------------------------------
// Start Thread
void TSCGenForm::StartThread(int sw)
{
	thr = new SCGenThread(proj, sw);
	thr->OnTerminate = OnThreadTerminate;

	ProcessState(true);
	thr->Resume();
}

//---------------------------------------------------------------------------
// Thread Terminate Handler
void __fastcall TSCGenForm::OnThreadTerminate(TObject *Sender)
{
	thr = NULL;

	ProcessState(false);
	Application->MessageBox("Done", "Done", MB_OK);
}

//---------------------------------------------------------------------------
// INF ファイル作成
void __fastcall TSCGenForm::ButtonMakeInfClick(TObject *Sender)
{
	StartThread(EX_MAKEINF);
}

//---------------------------------------------------------------------------
// resample 実行
void __fastcall TSCGenForm::ButtonResampleClick(TObject *Sender)
{
	StartThread(EX_RESAMPLE);
}

//---------------------------------------------------------------------------
// Alpha テクスチャのマージ
void __fastcall TSCGenForm::ButtonMergeAlphaClick(TObject *Sender)
{
	StartThread(EX_MRGALPHA);
}

//---------------------------------------------------------------------------
// テクスチャ変換
void __fastcall TSCGenForm::ButtonConvertTextureClick(TObject *Sender)
{
	StartThread(EX_CONVTEX);
}
//---------------------------------------------------------------------------
// BGL 生成
void __fastcall TSCGenForm::ButtonGenBGLClick(TObject *Sender)
{
	StartThread(EX_GENBGL);
}
//---------------------------------------------------------------------------
// 全プロセス実行
void __fastcall TSCGenForm::ButtonDoAllClick(TObject *Sender)
{
	StartThread(EX_MAKEINF | EX_RESAMPLE | EX_MRGALPHA | EX_CONVTEX | EX_GENBGL);
}

//---------------------------------------------------------------------------
void TSCGenForm::ProcessState(bool start)
{
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
	if (thr) {
		thr->Terminate();
	}
}
//---------------------------------------------------------------------------
