//---------------------------------------------------------------------------
#include "TestCase.h"
#include <string>
//---------------------------------------------------------------------------
class LatLonTestCase : public TestCase
{
public:
	LatLonTestCase(std::string name) : TestCase (name) {}

private:
	void angleStrSub(double d);
		
__published:
	// test methods
	void testAngleZero();
	void testAnglePlusDMS();
	void testAngleMinusDMS();
	void testAngleFloat();
	void testAngleStr();
};
//---------------------------------------------------------------------------

 
