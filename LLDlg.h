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
__published:	// IDE �Ǘ��̃R���|�[�l���g
	TLabel *Label1;
	TLabel *Label2;
	TEdit *LatEdit;
	TEdit *LonEdit;
	TButton *Button1;
	void __fastcall FormCreate(TObject *Sender);
private:	// ���[�U�[�錾
public:		// ���[�U�[�錾
	__fastcall TLatLonDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLatLonDlg *LatLonDlg;
//---------------------------------------------------------------------------
#endif
