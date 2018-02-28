#ifndef REVOR_STRUCT_H
#define REVOR_STRUCT_H
#define Maxsatnum 32
#define maxrawlen 4096
#define polycrc32 0xEDB88320u

/*******************************
coordinate_define
********************************/
typedef struct
{
	double x;
	double y;
	double z;
} XYZ;

typedef struct
{
	double Latitude;
	double Longitude;
	double Height;
} BLH;

typedef struct
{
	double e;
	double n;
	double u;

}ENU;

typedef struct
{
	double roll;
	double pitch;
	double yaw;
}Euler;

/******************************
time_define
*******************************/

typedef struct
{
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	double second;
} CommonTime;

typedef struct
{
	unsigned int week;
	double second;
} GPSTime;


typedef struct
{
	unsigned int week;
	double second;
} BDTime;



typedef struct
{
	short prn;
	double toc;
	int iode1;
	int iode2;
	double detn;
	double m0;
	double e;
	double a;
	double toe;
	int week;
	double crs, crc, cuc, cus, cic, cis;
	double omiga0;
	double omiga;
	double omigarate;
	double i0;
	double idot;
	double iodc;
	double ura;
	short health;
	double tgd;
	double af0, af1, af2;
	double tow;
}Ephem;

typedef struct
{
	double alpha[4];
	double beta[4];
	short valid;
	int week;
	int tot;
} GPSION;

typedef struct
{
	int satnum;
	unsigned short Prn[Maxsatnum];
	double P[Maxsatnum];
	double L[Maxsatnum];
	double D[Maxsatnum];
	unsigned short Glofreq[Maxsatnum];
	double Pstd[Maxsatnum];
	double Locktime[Maxsatnum];
} Obserdata;

typedef struct
{
	double x;
	double y;
	double z;
	int valid;
} NaviResult;

typedef struct
{
	GPSTime time;
	Obserdata obserdata;
	Ephem ephem[Maxsatnum];
	NaviResult position;
	GPSION gpsion;
}OneEpochAllData;

#endif


