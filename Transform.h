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
	LatLon	base;			// 画像左上の緯度/経度
	XYparam	res;			// 解像度 (degree/pixel)
	XYparam optres;		       	// 理想解像度 (degree/pixel)	
	XYparam	mag;			// 画像補正拡大率

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
