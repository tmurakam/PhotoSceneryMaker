//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "gnugettext.hpp"

#include "LLDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLatLonDlg *LatLonDlg;
//---------------------------------------------------------------------------
__fastcall TLatLonDlg::TLatLonDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TLatLonDlg::FormCreate(TObject *Sender)
{
	TranslateComponent(this);
}
//---------------------------------------------------------------------------

