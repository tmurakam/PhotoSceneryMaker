/*
 * Photo Scenery Maker
 *
 * Copyright (C) 2003 Takuya Murakami
 *
 * Transform.h : Transform coordinates
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */
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
