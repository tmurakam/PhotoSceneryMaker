//---------------------------------------------------------------------------

#ifndef LLDlgH
#define LLDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TLatLonDlg : public TForm
{
__published:	// IDE 管理のコンポーネント
	TLabel *Label1;
	TLabel *Label2;
	TEdit *LatEdit;
	TEdit *LonEdit;
	TButton *Button1;
	void __fastcall FormCreate(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
	__fastcall TLatLonDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLatLonDlg *LatLonDlg;
//---------------------------------------------------------------------------
#endif
