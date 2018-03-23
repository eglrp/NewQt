#pragma once
#include "stdafx.h"
#include "FaceDetect.h"
class FCUtility
{
public:
	static bool CreateDir(const char* dir)
	{
		if (!CreateDirectoryA(dir, NULL))
		{
			char parentDir[MAX_PATH] = { 0 };
			strcpy(parentDir, dir);
			char * pEnd = strchr(parentDir, '\\');
			if (!pEnd)
				return false;
			*pEnd = '\0';
			CreateDir(parentDir);
			CreateDirectoryA(dir, NULL);
		}
		return true;
	}
	static void FCMemory2Image2D(IFCMemory* memory, cv::Mat& image)
	{
		char* p = NULL;
		memory->m_cameraBuf[0]->GetFrm(&p);
		SEQSTU* st = BaseCore_CameraConfigInstance()->m_seqParam[0];
		image = cv::Mat(st->Tri0_h, st->Tri0_w, CV_8UC3);
		image.data = (uchar*)p;
		
	}
	static void Image2DspBuf(cv::Mat& image, DspBuf& data)
	{

		data.h = image.rows;
		data.w = image.cols;
		data.buf = (char*)image.data;
	}
	static void Image2Data(cv::Mat& image, fdImageData& data,fdRect& rect)
	{
		data.height = image.rows;
		data.width = image.cols;
		data.num_channels = image.channels();
		data.data = image.data;

		rect.x = 0; rect.y = 0;
		rect.height = image.rows; rect.width = image.cols;
		
	}

};
