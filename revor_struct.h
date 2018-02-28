#ifndef REVOR_STRUCT_H
#define REVOR_STRUCT_H


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


#endif


