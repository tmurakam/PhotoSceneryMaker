//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "gnugettext.hpp"

#include "AboutDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutDialog *AboutDialog;
//---------------------------------------------------------------------------
__fastcall TAboutDialog::TAboutDialog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutDialog::LabelURLClick(TObject *Sender)
{
	AnsiString url = LabelURL->Caption;
	ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}
//---------------------------------------------------------------------------
void __fastcall TAboutDialog::FormCreate(TObject *Sender)
{
	TranslateComponent(this);
}
//---------------------------------------------------------------------------

