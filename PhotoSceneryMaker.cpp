//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("MForm.cpp", MainForm);
USEFORM("LLDlg.cpp", LatLonDlg);
USEFORM("OptDlg.cpp", OptionDlg);
USEFORM("AboutDlg.cpp", AboutDialog);
USEFORM("SCGenFrm.cpp", SCGenForm);
USEFORM("PJForm.cpp", PrjForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Photo Scenery Maker";
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TLatLonDlg), &LatLonDlg);
		Application->CreateForm(__classid(TOptionDlg), &OptionDlg);
		Application->CreateForm(__classid(TAboutDialog), &AboutDialog);
		Application->CreateForm(__classid(TSCGenForm), &SCGenForm);
		Application->CreateForm(__classid(TPrjForm), &PrjForm);
		Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
