/*
 * Photo Scenery Maker
 *
 * Copyright (C) 2003 Takuya Murakami
 *
 * MForm.h : Main form
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

#ifndef MFormH
#define MFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>

#include "Project.h"
#include <ToolWin.hpp>

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE managed components
        TMainMenu *MainMenu;
        TMenuItem *F1;
	TMenuItem *MenuQuit;
        TStatusBar *StatusBar;
        TMenuItem *C1;
        TMenuItem *SetCPoint;
	TScrollBox *ScrollBox;
	TMenuItem *ExecCorrection;
	TMenuItem *MenuOption;
	TMenuItem *H1;
	TMenuItem *MenuAbout;
	TMenuItem *MenuSCGen;
	TMenuItem *MenuOpenPrj;
	TMenuItem *MenuPrjSaveAs;
	TMenuItem *MenuPrjSave;
	TMenuItem *N5;
	TOpenDialog *OpenPrjDialog;
	TSaveDialog *SavePrjDialog;
	TMenuItem *MenuPrjProperty;
	TMenuItem *N1;
	TMenuItem *MenuNewPrj;
	TMenuItem *V1;
	TMenuItem *MenuViewSummer;
	TMenuItem *MenuViewSpring;
	TMenuItem *MenuViewFall;
	TMenuItem *MenuViewWinter;
	TMenuItem *MenuViewHSWinter;
	TMenuItem *MenuViewLightmap;
	TMenuItem *MenuViewAlpha;
	TPaintBox *PaintBox;
	TTimer *Timer;

	void __fastcall SetCPointClick(TObject *Sender);
	void __fastcall OnMouseDown(TObject *Sender, TMouseButton Button,
	  TShiftState Shift, int X, int Y);
	void __fastcall OnMouseMove(TObject *Sender, TShiftState Shift, int X,
	  int Y);
	void __fastcall OnMouseUp(TObject *Sender, TMouseButton Button,
	  TShiftState Shift, int X, int Y);
	void __fastcall ExecCorrectionClick(TObject *Sender);
	void __fastcall MenuQuitClick(TObject *Sender);
	void __fastcall MenuOptionClick(TObject *Sender);
	void __fastcall MenuAboutClick(TObject *Sender);
	void __fastcall MenuOpenPrjClick(TObject *Sender);
	void __fastcall MenuPrjSaveClick(TObject *Sender);
	void __fastcall MenuPrjSaveAsClick(TObject *Sender);
	void __fastcall MenuSCGenClick(TObject *Sender);
	void __fastcall MenuPrjPropertyClick(TObject *Sender);
	void __fastcall MenuNewPrjClick(TObject *Sender);
	void __fastcall MenuViewSummerClick(TObject *Sender);
	void __fastcall MenuViewSpringClick(TObject *Sender);
	void __fastcall MenuViewFallClick(TObject *Sender);
	void __fastcall MenuViewWinterClick(TObject *Sender);
	void __fastcall MenuViewHSWinterClick(TObject *Sender);
	void __fastcall MenuViewLightmapClick(TObject *Sender);
	void __fastcall MenuViewAlphaClick(TObject *Sender);
	void __fastcall PaintBoxPaint(TObject *Sender);
	void __fastcall OnTimer(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// ÉÜÅ[ÉUÅ[êÈåæ
	bool isCpSpecifing;
        int CpSpecifing;
        ControlPoint cp[2];

	PSMProject	*proj;

	bool isDragging;
	int DragX, DragY;
	int MoveX, MoveY;

	Graphics::TBitmap	*bitmap;
	int curBmpIdx;

	void CalcParameters(void);
//	void Gauss(double k[3][4]);

public:		// User decl.
        __fastcall TMainForm(TComponent* Owner);
	void UpdateMenu(void);
	void SetProgress(int perc);

protected:
	void StartCpSpecify(void);
	void ChangeBmp(int bmpidx);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
