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

// �ϊ��p�����[�^���v�Z����
void Transform::CalcParameters(ControlPoint *cp)
{
	int dx, dy;

	// �܂��s�N�Z��������̊p�x���v�Z����
	dx = cp[1].v.x - cp[0].v.x;
	dy = cp[1].v.y - cp[0].v.y;

	res.x =  (cp[1].p.lon.deg - cp[0].p.lon.deg) / dx;
	res.y = -(cp[1].p.lat.deg - cp[0].p.lat.deg) / dy;

	// �摜����̍��W���v�Z����
	base.lon.deg = cp[0].p.lon.deg - cp[0].v.x * res.x;
	base.lat.deg = cp[0].p.lat.deg - (-cp[0].v.y) * res.y;

	CalcResolution();
}

//------------------------------------------------------------------
void Transform::CalcResolution(void)
{
	// �摜�𑜓x
	//	�e�N�X�`���� LOD13�Bcover �͈͓͂�k 90/8192 �x�A���� 120/8192�x�B
	//	�e�N�X�`���T�C�Y�� 256x256�B
	//	���ꂩ��Apixel ������̊p�x���v�Z�ł���B
	optres.y = (90.0 / 8192.0 / 256.0);
	optres.x = 120.0 / 8192.0 / 256.0;

	// �g�嗦���v�Z����
	mag.x = res.x / optres.x;
	mag.y = res.y / optres.y;
}

//------------------------------------------------------------------
// �摜���W����ܓx�o�x�ւ̕ϊ�
LatLon Transform::CalcLatLon(int x, int y)
{
	LatLon ll;

	ll.lon.deg = base.lon.deg + res.x * x;
	ll.lat.deg = base.lat.deg + res.y * (-y);

	return ll;
}
// �t�ϊ�
int Transform::CalcX(double lon)
{
	return (lon - base.lon.deg) / res.x;
}

int Transform::CalcY(double lat)
{
	return - (lat - base.lat.deg) / res.y;
}

//------------------------------------------------------------------
// �K���T�C�Y�v�Z
void Transform::CalcOptSize(int width, int height, int *correct_w, int *correct_h)
{
	// �␳��̃T�C�Y���v�Z
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
// �摜�␳���s
TPicture *Transform::TransImage(TPicture *pict)
{
#if 0
	int x, y, xx, yy;


	int owidth  = pict->Bitmap->Width;
	int oheight = pict->Bitmap->Height;
	TCanvas *oc = pict->Bitmap->Canvas;

	// ���t�@�����X�p�f�[�^�̍쐬
	unsigned long *cref;
	cref = new unsigned long [owidth * oheight];

	for (y = 0; y < oheight; y++) {
		for (x = 0; x < owidth; x++) {
			cref[y * owidth + x] = oc->Pixels[x][y];
		}
	}

	// �V�K�摜�̃T�C�Y���v�Z
	int cwidth, cheight;
	CalcOptSize(owidth, oheight, &cwidth, &cheight);

	// �V�K�摜����
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
			// �t���W�ϊ�
			ox = x / Xmag;
			oy = y / Ymag;

			// ���`��Ԃɂ��s�N�Z���l���v�Z����
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

	// �𑜓x�����ւ�
	Xres = OptXres;
	Yres = OptYres;
	
	return newpict;
#endif
}

