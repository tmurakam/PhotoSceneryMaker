//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "gnugettext.hpp"

#include "PJForm.h"
#include "Util.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPrjForm *PrjForm;
//---------------------------------------------------------------------------
__fastcall TPrjForm::TPrjForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPrjForm::FormCreate(TObject *Sender)
{
	TranslateComponent(this);
}
//---------------------------------------------------------------------------
void TPrjForm::LoadData(PSMProject *proj)
{
	EditBmpSummer->Text   = proj->BmpFile(BM_SUMMER);
	EditBmpSpring->Text   = proj->BmpFile(BM_SPRING);
	EditBmpFall->Text     = proj->BmpFile(BM_FALL);
	EditBmpWinter->Text   = proj->BmpFile(BM_WINTER);
	EditBmpHSWinter->Text = proj->BmpFile(BM_HSWINTER);
	EditBmpLightmap->Text = proj->BmpFile(BM_LIGHTMAP);
	EditBmpAlpha->Text    = proj->BmpFile(BM_ALPHA);

	CheckSeason->Checked = proj->HasSeason;

	EditOutDir->Text = proj->OutDir;
	EditBaseFile->Text = proj->BaseFile;
}

void TPrjForm::UpdateData(PSMProject *proj)
{
	proj->SetBmpFile(BM_SUMMER,   EditBmpSummer->Text);
	proj->SetBmpFile(BM_SPRING,   EditBmpSpring->Text);
	proj->SetBmpFile(BM_FALL,     EditBmpFall->Text);
	proj->SetBmpFile(BM_WINTER,   EditBmpWinter->Text);
	proj->SetBmpFile(BM_HSWINTER, EditBmpHSWinter->Text);
	proj->SetBmpFile(BM_LIGHTMAP, EditBmpLightmap->Text);
	proj->SetBmpFile(BM_ALPHA,    EditBmpAlpha->Text);

	proj->HasSeason = CheckSeason->Checked;

	proj->OutDir = EditOutDir->Text;
	proj->BaseFile = EditBaseFile->Text;
}

//---------------------------------------------------------------------------
// フォルダ選択
void TPrjForm::RefFolder(TEdit *edit)
{
	AnsiString f = SelectFolder(edit->Text, Handle);
	if (f.IsEmpty()) return;

	edit->Text = f;
}

void __fastcall TPrjForm::ButtonRefOutDirClick(TObject *Sender)
{
	RefFolder(EditOutDir);
}

//---------------------------------------------------------------------------
// BMPファイル選択
void TPrjForm::RefBmpFile(TEdit *edit)
{
	if (OpenDialog->Execute()) {
		edit->Text = OpenDialog->FileName;
	}
}

//---------------------------------------------------------------------------
void __fastcall TPrjForm::ButtonRefBmpSummerClick(TObject *Sender)
{
	RefBmpFile(EditBmpSummer);
}
//---------------------------------------------------------------------------

void __fastcall TPrjForm::ButtonRefBmpSpringClick(TObject *Sender)
{
	RefBmpFile(EditBmpSpring);
}
//---------------------------------------------------------------------------

void __fastcall TPrjForm::ButtonRefBmpFallClick(TObject *Sender)
{
	RefBmpFile(EditBmpFall);
}
//---------------------------------------------------------------------------

void __fastcall TPrjForm::ButtonRefBmpWinterClick(TObject *Sender)
{
	RefBmpFile(EditBmpWinter);
}
//---------------------------------------------------------------------------

void __fastcall TPrjForm::ButtonRefBmpHSWinterClick(TObject *Sender)
{
	RefBmpFile(EditBmpHSWinter);
}
//---------------------------------------------------------------------------

void __fastcall TPrjForm::ButtonRefBmpLightmapClick(TObject *Sender)
{
	RefBmpFile(EditBmpLightmap);
}
//---------------------------------------------------------------------------

void __fastcall TPrjForm::ButtonRefBmpAlphaClick(TObject *Sender)
{
	RefBmpFile(EditBmpAlpha);
}
//---------------------------------------------------------------------------


