//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LatLonTestCase.h"

#include "TestSuite.h"
#include "TestCaller.h"

#include "latlon.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
// LatLonTestCase class implementation

void LatLonTestCase::testAngleZero()
{
	Angle a;
	a.SetStr("0");
	assert(a.deg == 0);
}

void LatLonTestCase::testAnglePlusDMS()
{
	Angle a;
	a.SetStr("45");
	assert(a.deg == 45);

	a.SetStr("45:30");
	assert(a.deg == 45.5);

	a.SetStr("45:30:10");
	assert(a.deg == 45 + 30.0 / 60 + 10.0 / 3600);
}

void LatLonTestCase::testAngleMinusDMS()
{
	Angle a;
	a.SetStr("45");
	assert(a.deg == 45);

	a.SetStr("-45:30");
	assert(a.deg == -45.5);

	a.SetStr("-45:30:10");
	assert(a.deg == -(45 + 30.0 / 60 + 10.0 / 3600));
}

void LatLonTestCase::testAngleFloat()
{
	Angle a;
	a.SetStr("30.12345678");
	assert(a.deg == 30.12345678);

	a.SetStr("-30.12345678");
	assert(a.deg == -30.12345678);
}

void LatLonTestCase::angleStrSub(double deg)
{
	Angle a;
	AnsiString s;
	double deg2;

	a.deg = deg;
	s = a.GetStr();

	a.deg = 0;
	a.SetStr(s);
	assert(a.deg == deg);
}

void LatLonTestCase::testAngleStr()
{
	Angle a;
	AnsiString s;

	angleStrSub(0);
	angleStrSub(45);
	angleStrSub(-45);
	angleStrSub(45.1234567);
	angleStrSub(-45.1234567);
}

//---------------------------------------------------------------------------

