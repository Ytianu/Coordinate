#include"stdafx.h"
#include"math.h"
#include"rover_const.h"
#include"revor_struct.h"


/*********************************************************
function:BLH to ECEF
input:blh
output:xyz
test?:yes
*********************************************************/
void BLH2XYZ(BLH blh, XYZ *xyz)
{
	double RE, b, l, h;
	b = blh.Latitude*rad;
	l = blh.Longitude*rad;
	h = blh.Height;
	RE = WGS84_a / sqrt(1.0 - WGS84_ee*sin(b)*sin(b));
	xyz->x = (RE + h)*cos(b)*cos(l);
	xyz->y = (RE + h)*cos(b)*sin(l);
	xyz->z = ((1.0 - WGS84_ee)*RE + h)*sin(b);
}

/***************************************************
function:�ռ�����ת��Ϊ������꣨ECEF to BLH)
input:X Y Z
output:B L H
test?;yes
****************************************************/
void XYZ2BLH(XYZ xyz, BLH *blh)
{
	double x = xyz.x, y = xyz.y, z = xyz.z;
	double p = sqrt(x*x + y*y);
	double RE = WGS84_a;
	double sinb;
	double h = 0;
	double b0 = 1;
	blh->Latitude = atan2(z,p);    //ʹ�õ���γ�������Ƴ�ʼ�����γ�ȣ�����γ��С�ڴ��γ��
	double det = 1.0e-9;
	while (abs(blh->Latitude - b0) > det)
	{
		b0 = blh->Latitude;
		sinb = z / ((1.0 - WGS84_ee)*RE + h);
		blh->Latitude = atan2(z + WGS84_ee*RE*sinb, p);
		RE = WGS84_a / sqrt(1 - WGS84_ee*sin(blh->Latitude)*sin(blh->Latitude));
		h = p / cos(blh->Latitude) - RE;
	}
	blh->Latitude *= deg;
	blh->Longitude = atan2(y, x)*deg;
	blh->Height = h;
}



/*******************************************************************
function:�Խ��ջ�λ��Ϊվ��ԭ�㣬������ECEFת��ΪENU(վ������ϵ��վ������ϵΪ����������ϵ��
�ڼ������Ǹ߶ȽǺͷ�λ��ʱ���á�
input:recpositin satposition
output:enu
test?:yes
***********************************************************************/
void XYZ2ENU(XYZ recxyz, XYZ satxyz, ENU *enu)
{
	double x = satxyz.x, y = satxyz.y, z = satxyz.z;
	double dx, dy, dz;
	dx = x - recxyz.x;
	dy = y - recxyz.y;
	dz = z - recxyz.z;
	BLH blh;
	XYZ2BLH(recxyz, &blh);
	enu->e = -sin(blh.Longitude)*dx + cos(blh.Longitude)*dy;
	enu->n = -cos(blh.Longitude)*sin(blh.Latitude)*dx - sin(blh.Longitude)*sin(blh.Latitude)*dy + cos(blh.Latitude)*dz;
	enu->u = cos(blh.Longitude)*cos(blh.Latitude)*dx + sin(blh.Longitude)*cos(blh.Latitude)*dy + sin(blh.Latitude)*dz;
}

/**************************************************************************
function:ENU to ECEF
input:ENU satenu XYZ recxyz
output:XYZ satxyz
test?:yes
***************************************************************************/
void ENU2XYZ(ENU satenu, XYZ recxyz, XYZ *satxyz)
{
	BLH recblh;
	XYZ2BLH(recxyz, &recblh);
	double dx, dy, dz;
	double e = satenu.e, n = satenu.n, u = satenu.u;
	dx = -sin(recblh.Longitude)*e - cos(recblh.Longitude)*sin(recblh.Latitude)*n + cos(recblh.Longitude)*cos(recblh.Latitude)*u;
	dy = cos(recblh.Longitude)*e - sin(recblh.Longitude)*sin(recblh.Latitude)*n + sin(recblh.Longitude)*cos(recblh.Latitude)*u;
	dz = cos(recblh.Latitude)*n + sin(recblh.Latitude)*u;
	satxyz->x = recxyz.x+dx;
	satxyz->y = recxyz.y+dy;
	satxyz->z = recxyz.z+dz;
}


void BDT2GPST(BDTime bdtime, GPSTime *gpstime)
{
	gpstime->week = bdtime.week - 1356;
	gpstime->second = bdtime.second - 14;
}




/***********************************************************************
function:��̬�任ģ��,ʵ��ŷ���Ǻ���̬ת�ƾ���Ļ���
***********************************************************************/


/******************************************
function:Euler to CTM����̬ת�ƾ���
input��Euler 
ouput:CTM
test?:yes
*******************************************/
void Euler2CTM(Euler angle,double CTM[][3])
{
	double roll, pitch, yaw;
	roll = angle.roll*rad;
	pitch = angle.pitch*rad;
	yaw = angle.yaw*rad;
	CTM[0][0] = cos(pitch)*cos(yaw);
	CTM[0][1] = cos(pitch)*sin(yaw);
	CTM[0][2] = -sin(pitch);
	CTM[1][0] = -cos(roll)*sin(yaw) + sin(roll)*sin(pitch)*cos(yaw);
	CTM[1][1] = cos(roll)*cos(yaw) + sin(roll)*sin(pitch)*sin(yaw);
	CTM[1][2] = sin(roll)*cos(pitch);
	CTM[2][0] = sin(roll)*sin(yaw) + cos(roll)*sin(pitch)*cos(yaw);
	CTM[2][1] = -sin(roll)*cos(yaw) + cos(roll)*sin(pitch)*sin(yaw);
	CTM[2][2] = cos(roll)*cos(pitch);
}



/******************************************
function: CTM����̬ת�ƾ���to Euler
input��CTM
ouput:Euler
test?:yes
*******************************************/
void CTM2Euler(double CTM[][3], Euler *angle)
{
	angle->roll = atan2(CTM[1][2], CTM[2][2])*deg;
	angle->pitch = -asin(CTM[0][2])*deg;
	angle->yaw = atan2(CTM[0][1], CTM[0][0])*deg;
}