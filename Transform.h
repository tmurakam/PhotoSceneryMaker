//---------------------------------------------------------------------------

#ifndef TransformH
#define TransformH

#include <IniFiles.hpp>

#include "latlon.h"

//---------------------------------------------------------------------------
class Vector {
public:
	int x, y;
};

class ControlPoint {
public:
	LatLon	p;
	Vector	v;
};

class XYparam {
public:
	double	x;
	double	y;
};

class Transform {
private:
	LatLon	base;			// �摜����̈ܓx/�o�x
	XYparam	res;			// �𑜓x (degree/pixel)
	XYparam optres;		       	// ���z�𑜓x (degree/pixel)	
	XYparam	mag;			// �摜�␳�g�嗦

private:
	void CalcResolution(void);
	void writeResolution(XYparam r)	{ res = r; CalcResolution(); }


public:
	void CalcParameters(ControlPoint *cp);
	LatLon CalcLatLon(int x, int y);
	int CalcX(double lon);
	int CalcY(double lat);
	void CalcOptSize(int width, int height, int *correct_w, int *correct_h);

        TPicture *TransImage(TPicture *pict);

	__property LatLon Base = { read=base, write=base};
	__property XYparam Resolution = { read=res, write=writeResolution};
};

#endif
