/*
 * Photo Scenery Maker
 *
 * Copyright (C) 2003 Takuya Murakami
 *
 * Transform.cpp : Transform coordinates.
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
#include <vcl.h>
#pragma hdrstop
#include <IniFiles.hpp>

#include "Transform.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

// 変換パラメータを計算する
void Transform::CalcParameters(ControlPoint *cp)
{
	int dx, dy;

	// まずピクセルあたりの角度を計算する
	dx = cp[1].v.x - cp[0].v.x;
	dy = cp[1].v.y - cp[0].v.y;

	res.x =  (cp[1].p.lon.deg - cp[0].p.lon.deg) / dx;
	res.y = -(cp[1].p.lat.deg - cp[0].p.lat.deg) / dy;

	// 画像左上の座標を計算する
	base.lon.deg = cp[0].p.lon.deg - cp[0].v.x * res.x;
	base.lat.deg = cp[0].p.lat.deg - (-cp[0].v.y) * res.y;

	CalcResolution();
}

//------------------------------------------------------------------
void Transform::CalcResolution(void)
{
	// 画像解像度
	//	テクスチャは LOD13。cover 範囲は南北 90/8192 度、東西 120/8192度。
	//	テクスチャサイズは 256x256。
	//	これから、pixel あたりの角度が計算できる。
	optres.y = (90.0 / 8192.0 / 256.0);
	optres.x = 120.0 / 8192.0 / 256.0;

	// 拡大率を計算する
	mag.x = res.x / optres.x;
	mag.y = res.y / optres.y;
}

//------------------------------------------------------------------
// 画像座標から緯度経度への変換
LatLon Transform::CalcLatLon(int x, int y)
{
	LatLon ll;

	ll.lon.deg = base.lon.deg + res.x * x;
	ll.lat.deg = base.lat.deg + res.y * (-y);

	return ll;
}
// 逆変換
int Transform::CalcX(double lon)
{
	return (lon - base.lon.deg) / res.x;
}

int Transform::CalcY(double lat)
{
	return - (lat - base.lat.deg) / res.y;
}

//------------------------------------------------------------------
// 適正サイズ計算
void Transform::CalcOptSize(int width, int height, int *correct_w, int *correct_h)
{
	// 補正後のサイズを計算
	*correct_w = width * mag.x;
	*correct_h = height * mag.y;
}

inline void CMultiAdd(int &r, int &g, int &b, const TColor &cref, double m)
{
	int rc, gc, bc;

	rc = (cref >> 16) & 0xff;
	gc = (cref >> 8) & 0xff;
	bc = cref & 0xff;

	r += rc * m;
	g += gc * m;
	b += bc * m;
}	

//--------------------------------------------------------------------------
// 画像補正実行
TPicture *Transform::TransImage(TPicture *pict)
{
#if 0
	int x, y, xx, yy;


	int owidth  = pict->Bitmap->Width;
	int oheight = pict->Bitmap->Height;
	TCanvas *oc = pict->Bitmap->Canvas;

	// リファレンス用データの作成
	unsigned long *cref;
	cref = new unsigned long [owidth * oheight];

	for (y = 0; y < oheight; y++) {
		for (x = 0; x < owidth; x++) {
			cref[y * owidth + x] = oc->Pixels[x][y];
		}
	}

	// 新規画像のサイズを計算
	int cwidth, cheight;
	CalcOptSize(owidth, oheight, &cwidth, &cheight);

	// 新規画像生成
	Graphics::TBitmap *newbmp = new Graphics::TBitmap;
	newbmp->Width = cwidth;
	newbmp->Height = cheight;
	TCanvas *c = newbmp->Canvas;

	double ox, oy, wx, wy;
	unsigned long color, c00, c01, c10, c11;
	int r, g, b;
	for (y = 0; y < cheight; y++) {

		MainForm->SetProgress((double)y / cheight * 100.0);
		Application->ProcessMessages();

		for (x = 0; x < cwidth; x++) {
			// 逆座標変換
			ox = x / Xmag;
			oy = y / Ymag;

			// 線形補間によりピクセル値を計算する
			xx = (int)ox;
			yy = (int)oy;

			wx = ox - xx;
			wy = oy - yy;

			c00 = cref[yy * owidth + xx];
			c01 = cref[(yy+1) * owidth + xx];
			c10 = cref[yy * owidth + (xx+1)];
			c11 = cref[(yy+1) * owidth + (xx+1)];

			r = g = b = 0;
			CMultiAdd(r, g, b, c00, (1.0 - wx) * (1.0 - wy));
			CMultiAdd(r, g, b, c01, (1.0 - wx) * wy);
			CMultiAdd(r, g, b, c10, wx * (1.0 - wy));
			CMultiAdd(r, g, b, c11, wx * wy);
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;

			color = (r << 16) | (g << 8) | b;
			c->Pixels[x][y] = color;
		}
	}

	TPicture *newpict = new TPicture;
	newpict->Bitmap = newbmp;

	delete cref;

	// 解像度を入れ替え
	Xres = OptXres;
	Yres = OptYres;
	
	return newpict;
#endif
}

