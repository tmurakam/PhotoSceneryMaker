/*
 * Photo Scenery Maker
 *
 * Copyright (C) 2003 Takuya Murakami
 *
 * MForm.cpp : Main Form
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

#include "gnugettext.hpp"

#include "MForm.h"
#include "LLDlg.h"
#include "OptDlg.h"
#include "AboutDlg.h"
#include "SCGenFrm.h"
#include "PJForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
//
// Constructor
//
const crMove = 5;

__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	proj = new PSMProject;

	isCpSpecifing = false;
	isDragging = false;

	bitmap = NULL;
	curBmpIdx = BM_SUMMER;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	TranslateComponent(this);

	UpdateMenu();

	HICON h = ::LoadCursor(HInstance, "MOVECURSOR");
	Screen->Cursors[crMove] = h;
}

//---------------------------------------------------------------------------
//
// Quit
//
void __fastcall TMainForm::MenuQuitClick(TObject *Sender)
{
	delete(proj);
	Application->Terminate();
}

//---------------------------------------------------------------------------
// Create new project
void __fastcall TMainForm::MenuNewPrjClick(TObject *Sender)
{
	delete proj;
	proj = new PSMProject;
	MenuPrjPropertyClick(Sender);
}
//---------------------------------------------------------------------------
// Open project
void __fastcall TMainForm::MenuOpenPrjClick(TObject *Sender)
{
	if (!OpenPrjDialog->Execute()) return;

	proj->LoadFromFile(OpenPrjDialog->FileName);

	// BMP ファイル読み直し
	ChangeBmp(curBmpIdx, true);
}
//---------------------------------------------------------------------------
// Save project
void __fastcall TMainForm::MenuPrjSaveClick(TObject *Sender)
{
	proj->SaveToFile();
	UpdateMenu();
}
//---------------------------------------------------------------------------
// Save project as new file
void __fastcall TMainForm::MenuPrjSaveAsClick(TObject *Sender)
{
	if (!SavePrjDialog->Execute()) return;
	proj->SaveToFile(SavePrjDialog->FileName);
	UpdateMenu();
}

//---------------------------------------------------------------------------
// Specify project property
void __fastcall TMainForm::MenuPrjPropertyClick(TObject *Sender)
{
	PrjForm->LoadData(proj);
	if (PrjForm->ShowModal() == mrOk) {
		PrjForm->UpdateData(proj);
		ChangeBmp(curBmpIdx, true);
		UpdateMenu();
	}

}

//---------------------------------------------------------------------------
// Update menu items
void TMainForm::UpdateMenu(void)
{
	MenuPrjSave->Enabled = false;
	MenuPrjSaveAs->Enabled = false;
	SetCPoint->Enabled = false;
	ExecCorrection->Enabled = false;
	MenuSCGen->Enabled = false;

	if (bitmap && !isCpSpecifing) {
		SetCPoint->Enabled = true;
		ExecCorrection->Enabled = true;
		MenuPrjSave->Enabled = proj->Modified;
		MenuPrjSaveAs->Enabled = true;
		MenuSCGen->Enabled = true;
	}

	MenuViewSummer->Checked   = (curBmpIdx == BM_SUMMER);
	MenuViewSpring->Checked   = (curBmpIdx == BM_SPRING);
	MenuViewFall->Checked     = (curBmpIdx == BM_FALL);
	MenuViewWinter->Checked   = (curBmpIdx == BM_WINTER);
	MenuViewHSWinter->Checked = (curBmpIdx == BM_HSWINTER);
	MenuViewLightmap->Checked = (curBmpIdx == BM_LIGHTMAP);
	MenuViewAlpha->Checked    = (curBmpIdx == BM_ALPHA);
}

//---------------------------------------------------------------------------
// Start specify control points for calibration
void __fastcall TMainForm::SetCPointClick(TObject *Sender)
{
	isCpSpecifing = true;
	
	CpSpecifing = 0;
	StartCpSpecify();

	PaintBox->Cursor = crCross;
}
//---------------------------------------------------------------------------
// Start specify control points for calibration
void TMainForm::StartCpSpecify(void)
{
	AnsiString tmp;
	AnsiString fmt = _("Specify #%d correction point");
	tmp.sprintf(fmt.c_str(), CpSpecifing + 1);
	StatusBar->Panels->Items[0]->Text = tmp;
}

//---------------------------------------------------------------------------
// Mouse Event : Down
void __fastcall TMainForm::OnMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	// drag
	if (!isCpSpecifing) {
		// ドラッグ開始
		isDragging = true;
		POINT p;
		GetCursorPos(&p);

		DragX = p.x;
		DragY = p.y;

		MoveX = MoveY = 0;
		
		Timer->Enabled = true;

		// Drag 中のカーソルを変更
		// なぜか PaintBox->Cursor の変更ではうまくいかない、、、
		Screen->Cursor = crMove;
		return;
	}

	ControlPoint *c = &cp[CpSpecifing];

	c->v.x = X;
	c->v.y = Y;

	// 緯度経度を入力する
	LatLonDlg->ShowModal();
	c->p.lat.SetStr(LatLonDlg->LatEdit->Text);
	c->p.lon.SetStr(LatLonDlg->LonEdit->Text);

	CpSpecifing ++;
	if (CpSpecifing < 2) {
		StartCpSpecify();
	} else {
		StatusBar->Panels->Items[0]->Text = _("All control points specified");
		isCpSpecifing = false;
		UpdateMenu();

		PaintBox->Cursor = crDefault;

		// 補正パラメータ計算
		proj->Trans->CalcParameters(cp);
		proj->Modified = true;

		PaintBox->Invalidate();
		UpdateMenu();
	}

}
//---------------------------------------------------------------------------
// Mouse Event : Move
void __fastcall TMainForm::OnMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	AnsiString tmp;
        tmp.sprintf("(%d,%d)", X, Y);
	StatusBar->Panels->Items[1]->Text = tmp;

	LatLon ll = proj->Trans->CalcLatLon(X, Y);
	tmp.sprintf("%s %s", ll.lat.GetStr(), ll.lon.GetStr());
	StatusBar->Panels->Items[2]->Text = tmp;

	// drag
	if (isDragging) {
		POINT p;
		GetCursorPos(&p);
		MoveX = p.x - DragX;
		MoveY = p.y - DragY;
	}
}

//---------------------------------------------------------------------------
// Mouse Event : Up
void __fastcall TMainForm::OnMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (isDragging) {
		isDragging = false;
		Timer->Enabled = false;
		Screen->Cursor = crDefault;
	}
}
//---------------------------------------------------------------------------
// Scroll texture
void __fastcall TMainForm::OnTimer(TObject *Sender)
{
	if (!isDragging) return;

	ScrollBox->HorzScrollBar->Position += MoveX * 0.5;
	ScrollBox->VertScrollBar->Position += MoveY * 0.5;
}

//---------------------------------------------------------------------------
// Texture Correction
void __fastcall TMainForm::ExecCorrectionClick(TObject *Sender)
{
#if 0
	// 境界補正
	int cx, cy;
	proj->Trans->CalcOptSize(bitmap->Width, bitmap->Height, &cx, &cy);
	AnsiString query;
	query.sprintf("補正後の画像サイズは %d x %d から"
		" %d x %d ピクセルになります\nよろしいですか？",
		bitmap->Width, bitmap->Height, cx, cy);
	if (Application->MessageBox(query.c_str(), "確認", MB_ICONQUESTION | MB_YESNO)
		!= IDYES) {
		return;
	}

	// 変換実行
	StatusBar->Panels->Items[0]->Text = "処理中です。しばらくお待ちください、、、";
	TPicture *pict = proj->Trans->TransImage(Image->Picture);
	Image->Picture = pict;

	UpdateMenu();
	StatusBar->Panels->Items[0]->Text = "補正完了";

	proj->Modified = true;
#endif
}
//---------------------------------------------------------------------------
// Show option dialog
void __fastcall TMainForm::MenuOptionClick(TObject *Sender)
{
	if (OptionDlg->ShowModal() != mrOk) return;
}
//---------------------------------------------------------------------------
// Show scenery generation dialog
void __fastcall TMainForm::MenuSCGenClick(TObject *Sender)
{
	SCGenForm->SetParam(proj, bitmap->Width, bitmap->Height);
	SCGenForm->ShowModal();
}
//---------------------------------------------------------------------------
// Show about(version) dialog
void __fastcall TMainForm::MenuAboutClick(TObject *Sender)
{
	AboutDialog->ShowModal();
}
//---------------------------------------------------------------------------
// Show progress on status bar
void TMainForm::SetProgress(int perc)
{
	AnsiString tmp;
	tmp.sprintf("処理中: %d%%", perc);
	StatusBar->Panels->Items[0]->Text = tmp;
}
//---------------------------------------------------------------------------
// Switch seasonal bitmaps
void TMainForm::ChangeBmp(int bmpidx, bool reload)
{
	curBmpIdx = bmpidx;
	
	if (bitmap) {
		delete bitmap;
	}
	bitmap = new Graphics::TBitmap;
	bitmap->LoadFromFile(proj->BmpFile(curBmpIdx));

	// 描画領域のサイズを修正
	proj->Trans->Width = bitmap->Width;
	proj->Trans->Height = bitmap->Height;

	PaintBox->Width = bitmap->Width;
	PaintBox->Height = bitmap->Height;

	PaintBox->Invalidate();

	UpdateMenu();
}
//---------------------------------------------------------------------------
// Display texture
void __fastcall TMainForm::PaintBoxPaint(TObject *Sender)
{
	if (!bitmap) return;
	
	// ビットマップ描画
	PaintBox->Canvas->Draw(0, 0, bitmap);

	// グリッド描画
	PaintBox->Canvas->Pen->Color = clWhite;
	PaintBox->Canvas->Pen->Width = 1;
        PaintBox->Canvas->Pen->Style = psDashDot;

	Transform *trans = proj->Trans;
	
	LatLon nw = trans->CalcLatLon(0, 0);
	LatLon se = trans->CalcLatLon(bitmap->Width - 1, bitmap->Height - 1);

	int min, max, i;

	// 緯線
	const double latbound = 90.0 / 8196.0;	// LOD 13
	min = (int)(se.lat.deg / latbound - 0.5);
	max = (int)(nw.lat.deg / latbound - 0.5) + 1;
	for (i = min; i <= max; i++) {
		int y = trans->CalcY((i + 0.5) * latbound);
		PaintBox->Canvas->MoveTo(0, y);
		PaintBox->Canvas->LineTo(bitmap->Width - 1, y);
	}

	// 経線
	const double lonbound = 120.0 / 8196.0;	// LOD 13
	min = (int)(nw.lon.deg / lonbound - 0.5);
	max = (int)(se.lon.deg / lonbound - 0.5) + 1;
	for (i = min; i <= max; i++) {
		int x = trans->CalcX((i + 0.5) * lonbound);
		PaintBox->Canvas->MoveTo(x, 0);
		PaintBox->Canvas->LineTo(x, bitmap->Height - 1);
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::MenuViewSummerClick(TObject *Sender)
{
	ChangeBmp(BM_SUMMER);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuViewSpringClick(TObject *Sender)
{
	ChangeBmp(BM_SPRING);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuViewFallClick(TObject *Sender)
{
	ChangeBmp(BM_FALL);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuViewWinterClick(TObject *Sender)
{
	ChangeBmp(BM_WINTER);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuViewHSWinterClick(TObject *Sender)
{
	ChangeBmp(BM_HSWINTER);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuViewLightmapClick(TObject *Sender)
{
	ChangeBmp(BM_LIGHTMAP);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::MenuViewAlphaClick(TObject *Sender)
{
	ChangeBmp(BM_ALPHA);
}
//---------------------------------------------------------------------------


