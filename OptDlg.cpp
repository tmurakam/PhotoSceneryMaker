//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <Registry.hpp>

#include "gnugettext.hpp"

#include "OptDlg.h"
#include "Util.h"

#define	REG_KEY		"\\Software\\Takuya Murakami\\Photo Scenery Maker"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptionDlg *OptionDlg;
//---------------------------------------------------------------------------
__fastcall TOptionDlg::TOptionDlg(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
// オプション値をレジストリから読み込む
void TOptionDlg::LoadReg(void)
{
	TRegistry *reg = new TRegistry;
	reg->RootKey = HKEY_CURRENT_USER;
	reg->OpenKey(REG_KEY, true);

	AnsiString sdkpath;
	try {
		EditSDKPath->Text = reg->ReadString("TerrainSDKPath");
		EditImagetoolPath->Text = reg->ReadString("ImagetoolPath");
	}
	catch (const Exception &e) {
		// do nothing
	}

	reg->CloseKey();
	delete reg;
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FormCreate(TObject *Sender)
{
	TranslateComponent(this);
	LoadReg();
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FormShow(TObject *Sender)
{
	LoadReg();
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ButtonOKClick(TObject *Sender)
{
	TRegistry *reg = new TRegistry;
	reg->RootKey = HKEY_CURRENT_USER;
	reg->OpenKey(REG_KEY, true);

	reg->WriteString("TerrainSDKPath", EditSDKPath->Text);
	reg->WriteString("ImagetoolPath", EditImagetoolPath->Text);

	reg->CloseKey();
	delete reg;
}
//---------------------------------------------------------------------------

void __fastcall TOptionDlg::ButtonBrowseSDKClick(TObject *Sender)
{
	AnsiString f = SelectFolder("", Handle);
	if (!f.IsEmpty()) {
		EditSDKPath->Text = f;
	}
}
//---------------------------------------------------------------------------


void __fastcall TOptionDlg::ButtonBrowserImagetoolClick(TObject *Sender)
{
	AnsiString f = SelectFolder("", Handle);
	if (!f.IsEmpty()) {
		EditImagetoolPath->Text = f;
	}
}
//---------------------------------------------------------------------------

