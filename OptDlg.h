//---------------------------------------------------------------------------

#ifndef OptDlgH
#define OptDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TOptionDlg : public TForm
{
__published:	// IDE 管理のコンポーネント
	TButton *ButtonOK;
	TButton *Button2;
	TLabel *Label1;
	TEdit *EditSDKPath;
	TButton *ButtonBrowseSDK;
	TLabel *Label2;
	TEdit *EditImagetoolPath;
	TButton *ButtonBrowserImagetool;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonOKClick(TObject *Sender);
	void __fastcall ButtonBrowseSDKClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonBrowserImagetoolClick(TObject *Sender);
private:	// ユーザー宣言
	void LoadReg(void);
public:		// ユーザー宣言
	__fastcall TOptionDlg(TComponent* Owner);
	inline AnsiString GetSDKPath(void) { return EditSDKPath->Text; }
	inline AnsiString GetImagetoolPath(void) { return EditImagetoolPath->Text; }
};
//---------------------------------------------------------------------------
extern PACKAGE TOptionDlg *OptionDlg;
//---------------------------------------------------------------------------
#endif
