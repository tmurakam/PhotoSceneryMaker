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
#include <Registry.hpp>

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
	// Load window position from registry
	try {
		TRegistry *reg = new TRegistry;
		reg->RootKey = HKEY_CURRENT_USER;
		reg->OpenKey(REG_KEY, true);
	
		int top, left, width, height;
		top = reg->ReadInteger("WindowTop");
		left = reg->ReadInteger("WindowLeft");
		width = reg->ReadInteger("WindowWidth");
		height = reg->ReadInteger("WindowHeight");

		Top = top;
		Left = left;
		Width = width;
		Height = height;
	}
	catch (const Exception &e) {
		// do nothing
	}

	TranslateComponent(this);

	UpdateMenu();

	HICON h = ::LoadCursor(HInstance, "MOVECURSOR");
	Screen->Cursors[crMove] = h;

	// Load initial file
	if (ParamCount() > 0) {
		proj->LoadFromFile(ParamStr(1));
		ChangeBmp(curBmpIdx, true);
	}
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::FormShow(TObject *Sender)
{
	TranslateAllForms();
}

//---------------------------------------------------------------------------
void TMainForm::TranslateAllForms(void)
{
	TranslateComponent(this);
	TranslateComponent(LatLonDlg);
	TranslateComponent(OptionDlg);
	TranslateComponent(AboutDialog);
	TranslateComponent(SCGenForm);
	TranslateComponent(PrjForm);
}

//---------------------------------------------------------------------------
//
// Quit
//
void __fastcall TMainForm::MenuQuitClick(TObject *Sender)
{
	if (!CheckSave()) return;

	delete(proj);
	Application->Terminate();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	CanClose = CheckSave();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// save windows position
	try {
		TRegistry *reg = new TRegistry;
		reg->RootKey = HKEY_CURRENT_USER;
		reg->OpenKey(REG_KEY, true);

		reg->WriteInteger("WindowTop", Top);
		reg->WriteInteger("WindowLeft", Left);
		reg->WriteInteger("WindowWidth", Width);
		reg->WriteInteger("WindowHeight", Height);
	}
	catch (Exception &e) {
		// do nothing
	}
}

//---------------------------------------------------------------------------
// Create new project
void __fastcall TMainForm::MenuNewPrjClick(TObject *Sender)
{
	if (!CheckSave()) return;
	
	delete proj;
	proj = new PSMProject;
	MenuPrjPropertyClick(Sender);
}
//---------------------------------------------------------------------------
// Open project
void __fastcall TMainForm::MenuOpenPrjClick(TObject *Sender)
{
	if (!CheckSave()) return;
	if (!OpenPrjDialog->Execute()) return;

	proj->LoadFromFile(OpenPrjDialog->FileName);

	// Reload BMP file
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
// Check save
bool TMainForm::CheckSave(void)
{
	if (proj->Modified) {
		int res;
		AnsiString title = _("Confirmation");
		AnsiString msg = _("Project is modified. Do you want to save?");
		res = Application->MessageBox(msg.c_str(), title.c_str(),
			MB_YESNOCANCEL | MB_ICONQUESTION);

		if (res == IDCANCEL) return false;
		if (res == IDYES) {
			proj->SaveToFile();
		}
	}
	return true;
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
		// Start dragging
		isDragging = true;
		POINT p;
		GetCursorPos(&p);

		DragX = p.x;
		DragY = p.y;

		MoveX = MoveY = 0;
		
		Timer->Enabled = true;

		// Modify dragging cursor
		// Note: PaintBox->Cursor modification does not work...
		Screen->Cursor = crMove;
		return;
	}

	// Specify new control point.

	ControlPoint *c = &cp[CpSpecifing];

	c->v.x = X;
	c->v.y = Y;

	// Show lat/lon
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

		// Calculate calibration parameters
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
	int cx, cy;
	proj->Trans->CalcOptSize(bitmap->Width, bitmap->Height, &cx, &cy);

	AnsiString fmt, mes1, mes2, mes;
	fmt = _("Optimal bitmap size is %d x %d.\n");
	mes1.sprintf(fmt.c_str(), cx, cy);
	mes2 = _("You can resize the bitmap with some image editing tools.");

	mes = mes1 + mes2;
	Application->MessageBox(mes.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
}
//---------------------------------------------------------------------------
// Show option dialog
void __fastcall TMainForm::MenuOptionClick(TObject *Sender)
{
	if (OptionDlg->ShowModal() != mrOk) return;
	TranslateAllForms();
}
//---------------------------------------------------------------------------
// Show scenery generation dialog
void __fastcall TMainForm::MenuSCGenClick(TObject *Sender)
{
	SCGenForm->SetParam(proj);
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
	tmp.sprintf("Processing: %d%%", perc);
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
	try {
		bitmap->LoadFromFile(proj->BmpFile(curBmpIdx));

		proj->Trans->Width = bitmap->Width;
		proj->Trans->Height = bitmap->Height;

		PaintBox->Width = bitmap->Width;
		PaintBox->Height = bitmap->Height;

		PaintBox->Invalidate();
	}
	catch (Exception &e) {
		AnsiString msg = _("Can't open bitmap file.");
		Application->MessageBox(msg.c_str(), "Error", MB_OK | MB_ICONERROR);	

		delete bitmap;
		bitmap = NULL;
	}

	UpdateMenu();
}
//---------------------------------------------------------------------------
// Display texture
void __fastcall TMainForm::PaintBoxPaint(TObject *Sender)
{
	if (!bitmap) return;
	
	// Draw Bitmap
	PaintBox->Canvas->Draw(0, 0, bitmap);

	// Draw grids
	PaintBox->Canvas->Pen->Color = clWhite;
	PaintBox->Canvas->Pen->Width = 1;
        PaintBox->Canvas->Pen->Style = psDashDot;

	Transform *trans = proj->Trans;
	
	LatLon nw = trans->CalcLatLon(0, 0);
	LatLon se = trans->CalcLatLon(bitmap->Width - 1, bitmap->Height - 1);

	int min, max, i;

	// Latitude line
	const double latbound = 90.0 / 8196.0;	// LOD 13
	min = (int)(se.lat.deg / latbound - 0.5);
	max = (int)(nw.lat.deg / latbound - 0.5) + 1;
	for (i = min; i <= max; i++) {
		int y = trans->CalcY((i + 0.5) * latbound);
		PaintBox->Canvas->MoveTo(0, y);
		PaintBox->Canvas->LineTo(bitmap->Width - 1, y);
	}

	// Longitude line
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


void __fastcall TMainForm::MenuManualClick(TObject *Sender)
{
	ShowHtml("manual");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuTutorialClick(TObject *Sender)
{
	ShowHtml("tutorial");
}

void TMainForm::ShowHtml(AnsiString prefix)
{
	AnsiString html;

	html = ExtractFilePath(Application->ExeName);
	html += "doc\\";
	html += prefix;
	html += ".";
	html += OptionDlg->GetLangCode();
	html += ".html";

	ShellExecute(this->Handle, "open", html.c_str(),
		NULL, NULL, SW_SHOW);
}



//---------------------------------------------------------------------------

