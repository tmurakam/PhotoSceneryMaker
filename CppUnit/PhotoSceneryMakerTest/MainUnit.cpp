//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUnit.h"

#include "ITestRunner.h"
#include "TestSuite.h"
#include "LatLonTestCase.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	ITestRunner runner;

	runner.addTest(new TestSuite(__classid(LatLonTestCase)));
	runner.run();

	Application->Terminate();
}
//---------------------------------------------------------------------------
