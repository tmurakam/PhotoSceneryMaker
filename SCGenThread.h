//---------------------------------------------------------------------------

#ifndef SCGenThreadH
#define SCGenThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>

#include "Project.h"

#define	EX_MAKEINF	0x1
#define	EX_RESAMPLE	0x2
#define	EX_MRGALPHA	0x4
#define	EX_CONVTEX	0x8
#define	EX_GENBGL	0x10

//---------------------------------------------------------------------------
class SCGenThread : public TThread
{            
private:
protected:
	void __fastcall Execute();

	PSMProject *Proj;
	int Sw;

	void MakeInf(void);
	AnsiString InfFileName(int season);
	AnsiString BmpPath(int season);
	void MakeInf(int season);

	int ExecCmd(AnsiString cmdline);

	void Resample(void);
	void MergeAlpha(void);
	void ConvTex(void);
	void GenBgl(void);

	void SetStatusMsg(AnsiString msg);

public:
	__fastcall SCGenThread(PSMProject *proj, int sw);
};
//---------------------------------------------------------------------------
#endif
