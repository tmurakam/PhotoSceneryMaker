//---------------------------------------------------------------------------

#ifndef SCGenFrmH
#define SCGenFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "Project.h"
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class TSCGenForm : public TForm
{
__published:	// IDE 管理のコンポーネント
	TButton *ButtonDoAll;
	TButton *ButtonClose;
	TGroupBox *GroupBox1;
	TButton *ButtonMakeInf;
	TButton *ButtonResample;
	TButton *ButtonConvertTexture;
	TButton *ButtonGenBGL;
	TButton *ButtonMergeAlpha;
	TButton *ButtonStop;
	TStatusBar *StatusBar;
	void __fastcall ButtonMakeInfClick(TObject *Sender);
	void __fastcall ButtonResampleClick(TObject *Sender);
	void __fastcall ButtonGenBGLClick(TObject *Sender);
	void __fastcall ButtonConvertTextureClick(TObject *Sender);
	void __fastcall ButtonDoAllClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonMergeAlphaClick(TObject *Sender);
	void __fastcall ButtonStopClick(TObject *Sender);
private:	// ユーザー宣言
	PSMProject *proj;

	int Width, Height;

	bool Processing;

	int ExecCmd(AnsiString cmdline);
	AnsiString InfFileName(int season);
	AnsiString BmpPath(int season);
	void MakeInf(int season);

	void ProcessState(bool isStart);

public:		// ユーザー宣言
	__fastcall TSCGenForm(TComponent* Owner);
	void SetParam(PSMProject *proj, int width, int height);
	void WriteIni(TIniFile * ini);
	void ReadIni(TIniFile * ini);
};
//---------------------------------------------------------------------------
extern PACKAGE TSCGenForm *SCGenForm;
//---------------------------------------------------------------------------
#endif

