//---------------------------------------------------------------------------

#ifndef PJFormH
#define PJFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#include "Project.h"
#include <Dialogs.hpp>

//---------------------------------------------------------------------------
class TPrjForm : public TForm
{
__published:	// IDE 管理のコンポーネント
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
	TPanel *Panel1;
	TButton *Button1;
	TButton *Button2;
	TEdit *EditBmpSummer;
	TEdit *EditBmpSpring;
	TCheckBox *CheckSeason;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TEdit *EditBmpFall;
	TEdit *EditBmpWinter;
	TEdit *EditBmpHSWinter;
	TEdit *EditBmpLightmap;
	TEdit *EditBmpAlpha;
	TButton *ButtonRefBmpSummer;
	TButton *ButtonRefBmpSpring;
	TButton *ButtonRefBmpFall;
	TButton *ButtonRefBmpWinter;
	TButton *ButtonRefBmpHSWinter;
	TButton *ButtonRefBmpLightmap;
	TButton *ButtonRefBmpAlpha;
	TLabel *Label8;
	TLabel *Label9;
	TEdit *EditOutDir;
	TEdit *EditBaseFile;
	TButton *ButtonRefOutDir;
	TOpenDialog *OpenDialog;
	TLabel *Label10;
	void __fastcall ButtonRefOutDirClick(TObject *Sender);
	void __fastcall ButtonRefBmpSummerClick(TObject *Sender);
	void __fastcall ButtonRefBmpSpringClick(TObject *Sender);
	void __fastcall ButtonRefBmpFallClick(TObject *Sender);
	void __fastcall ButtonRefBmpWinterClick(TObject *Sender);
	void __fastcall ButtonRefBmpHSWinterClick(TObject *Sender);
	void __fastcall ButtonRefBmpLightmapClick(TObject *Sender);
	void __fastcall ButtonRefBmpAlphaClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
	__fastcall TPrjForm(TComponent* Owner);

	void LoadData(PSMProject *proj);
	void UpdateData(PSMProject *proj);

	void RefFolder(TEdit *edit);
	void RefBmpFile(TEdit *edit);
};
//---------------------------------------------------------------------------
extern PACKAGE TPrjForm *PrjForm;
//---------------------------------------------------------------------------
#endif
