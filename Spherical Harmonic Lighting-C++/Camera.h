#ifndef __CAMERA_H_
#define __CAMERA_H_

#include <windows.h>
#include <GL/gl.h>
#include <math.h>

#pragma warning(disable:4305)
#pragma warning(disable:4244)

class Camera
{
public:
	Camera();
	Camera(float fSpeed,float fAngle);
	void OperationProc();
public:
	float camera_x,camera_y,camera_z;
	float lookat_x,lookat_y,lookat_z;
private:
	void YawCamera(float fAngle);
	void PitchCamera(float fAngle);
	void WalkStraight(float fSpeed);
	void WalkTransverse(float fSpeed);
private:
	float angle;
	float speed;
	float sight;
	float rad_xy,rad_xz;
	float rotate_xy,rotate_xz;

	float PI;
};

#endif