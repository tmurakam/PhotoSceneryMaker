//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <Registry.hpp>

#include "gnugettext.hpp"

#include "MForm.h"
#include "OptDlg.h"
#include "Util.h"

#define	REG_KEY		"\\Software\\Takuya Murakami\\Photo Scenery Maker"

struct _lang {
	char *code;
	char *locale;
	char *desc;
} Lang[] = {
	{ "en", "en", "English" },
	{ "ja", "ja_JP", "Japanese" },
	{ NULL, NULL }
};

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptionDlg *OptionDlg;
//---------------------------------------------------------------------------
__fastcall TOptionDlg::TOptionDlg(TComponent* Owner)
	: TForm(Owner)
{
	curLangIdx = 0;
}

//---------------------------------------------------------------------------
// Change Languages
void TOptionDlg::ChangeLang(void)
{
	UseLanguage(Lang[curLangIdx].locale);
}

AnsiString TOptionDlg::GetLangCode(void)
{
	return Lang[curLangIdx].code;
}

//---------------------------------------------------------------------------
// オプション値をレジストリから読み込む
void TOptionDlg::LoadReg(void)
{
	TRegistry *reg = new TRegistry;
	reg->RootKey = HKEY_CURRENT_USER;
	reg->OpenKey(REG_KEY, true);

	AnsiString sdkpath, lng;
	try {
		EditSDKPath->Text = reg->ReadString("TerrainSDKPath");
		EditImagetoolPath->Text = reg->ReadString("ImagetoolPath");
		lng = reg->ReadString("Language");
	}
	catch (const Exception &e) {
		// do nothing
	}

	// set language index
	for (int i = 0; ; i++) {
		if (Lang[i].code == NULL) break;
		if (lng == Lang[i].code) {
			curLangIdx = i;
			break;
		}
	}
	ListLang->ItemIndex = curLangIdx;
	ChangeLang();
	
	reg->CloseKey();
	delete reg;
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FormCreate(TObject *Sender)
{
	// Setup Language Selection
	for (int i = 0; ; i++) {
		if (Lang[i].code == NULL) break;
		ListLang->Items->Add(Lang[i].desc);
	}

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

	// Change Language
	curLangIdx = ListLang->ItemIndex;
	ChangeLang();

	reg->WriteString("Language", Lang[curLangIdx].code);

	reg->CloseKey();
	delete reg;
}

//---------------------------------------------------------------------------

void __fastcall TOptionDlg::ButtonBrowseSDKClick(TObject *Sender)
{
#if 0
	AnsiString f = SelectFolder("", Handle);
	if (!f.IsEmpty()) {
		EditSDKPath->Text = f;
	}
#endif
	OpenDialog->InitialDir = EditSDKPath->Text;
	OpenDialog->FileName = "resample.exe";
	OpenDialog->Filter = "Terrain SDK Resample tool|resample.exe";
	if (!OpenDialog->Execute()) return;

	EditSDKPath->Text = ExtractFileDir(OpenDialog->FileName);

}
//---------------------------------------------------------------------------


void __fastcall TOptionDlg::ButtonBrowserImagetoolClick(TObject *Sender)
{
#if 0
	AnsiString f = SelectFolder("", Handle);
	if (!f.IsEmpty()) {
		EditImagetoolPath->Text = f;
	}
#endif
	OpenDialog->InitialDir = EditImagetoolPath->Text;
	OpenDialog->FileName = "imagetool.exe";
	OpenDialog->Filter = "Imagetool|imagetool.exe";
	if (!OpenDialog->Execute()) return;

	EditImagetoolPath->Text = ExtractFileDir(OpenDialog->FileName);
}
//---------------------------------------------------------------------------

