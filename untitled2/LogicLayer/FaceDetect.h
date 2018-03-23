
#pragma once
#include <Windows.h>
#include <vector>
typedef struct fdRect
{
	fdRect()
	{
		x = 0; y = 0; width = 0; height = 0;
	}
	fdRect(int x_, int y_, int width_, int height_)
	{
		x = x_; y = y_; width = width_; height = height_;
	}
	int area()
	{
		return width*height;
	}
	int x;
	int y;
	int width;
	int height;
}fdRect;
typedef struct fdPoint
{
	fdPoint()
	{
		x = 0; y = 0;
	}
	fdPoint(int x_, int y_)
	{
		x = x_; y = y_;
	}
	int x;
	int y;
}fdPoint;
typedef struct fdImageData
{
	fdImageData() 
	{
		data = nullptr;
		width = 0;
		height = 0;
		num_channels = 0;
	}

	fdImageData(int img_width, int img_height,int img_num_channels,BYTE *img_data) 
	{
		data = img_data;
		width = img_width;
		height = img_height;
		num_channels = img_num_channels;
	}

	BYTE* data;
	int width;
	int height;
	int num_channels;
}fdImageData;

class CFaceDetect
{
public:
	~CFaceDetect(){}
public:
	virtual bool Init() =0;
	virtual void AdjustThreshHold(double matchThreshHold,double adjustThreshHold) = 0;	
	virtual bool GetFaceRect(fdImageData &pic, fdRect detectRect, std::vector<fdRect> &faces) = 0;
	virtual bool GetFaceKeys(fdImageData &pic_, fdRect &face, std::vector<fdPoint> &Keys) = 0;
};


CFaceDetect* FaceDetectInstance();
