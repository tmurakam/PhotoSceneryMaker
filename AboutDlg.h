//---------------------------------------------------------------------------

#ifndef AboutDlgH
#define AboutDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TAboutDialog : public TForm
{
__published:	// IDE 管理のコンポーネント
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *LabelURL;
	TButton *Button1;
	TImage *Image1;
	TMemo *Memo1;
	void __fastcall LabelURLClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
	__fastcall TAboutDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutDialog *AboutDialog;
//---------------------------------------------------------------------------
#endif
