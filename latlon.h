//---------------------------------------------------------------------------
#ifndef latlonH
#define latlonH
//---------------------------------------------------------------------------
class Angle {
    public:
	double	deg;

    public:
	Angle(void) { deg = 0.0; }
	void SetStr(AnsiString s);
	AnsiString GetStr(void);
};

class LatLon {
public:
	Angle lat;
        Angle lon;

public:
	void CalcDistance(const LatLon &tgt, double *dx, double *dy);
	void CalcLatLon(const LatLon &base, double dx, double dy);
};


#endif
