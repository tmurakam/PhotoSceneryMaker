//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "latlon.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void Angle::SetStr(AnsiString s)
{
	char buf[100];
        strncpy(buf, s.c_str(), sizeof(buf));

	char *p;
        double dd, mm, ss;

        dd = mm = ss = 0.0;

	p = strtok(buf, ":");
	if (p) {
		dd = atof(p);

		p = strtok(NULL, ":");
		if (p) {
			mm = atof(p);
			p = strtok(NULL, ":");
			if (p) {
				ss = atof(p);
			}
		}
	}

        deg = dd + mm / 60.0 + ss / 3600.0;
}

AnsiString Angle::GetStr(void)
{
	int dd, mm;
        double ss, t;

	t = deg;
        dd = (int)t;
	t = (t - dd) * 60.0;
        mm = (int)t;
        t = (t - mm) * 60.0;
        ss = t;

        AnsiString s;
        s.sprintf("%d:%02d:%2.2f", dd, mm, ss);
        return s;
}

//---------------------------------------------------------------------------
#define EQ2NPL          (40007.0 * 1000 / 4)    /* distance from equator to north pole */
#define LAT2M(x)        ((x) / 90.0 * EQ2NPL)     /* latitude[degree] to distance[meter] */
#define	DEG2RAD(d)	((d) * M_PI / 180.0)
#define M2LAT(x)	((x) * 90.0 / EQ2NPL)

void LatLon::CalcDistance(const LatLon &tgt, double *dx, double *dy)
{
	*dy = LAT2M(tgt.lat.deg - lat.deg);
	*dx = LAT2M(tgt.lon.deg - lon.deg) *
        	cos(DEG2RAD((tgt.lat.deg + lat.deg) / 2.0));
}

void LatLon::CalcLatLon(const LatLon &base, double dx, double dy)
{
	lat.deg = base.lat.deg + M2LAT(dy);
	lon.deg = base.lon.deg + M2LAT(dx) /
		cos(DEG2RAD(base.lat.deg + lat.deg) / 2.0);
}

