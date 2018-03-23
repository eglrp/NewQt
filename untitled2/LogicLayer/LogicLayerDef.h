#pragma once
#include "WSFModel.h"
#include "common.h"
enum VideoStatus
{
	VideoClosed,
	VideoRun,
	VideoPause,
};
enum AppStatus
{
	AppNotInit,
	AppInit,

	AppRegisterStart,
	AppRegisterTrigger,

	AppRecognizeStart,
	AppRecognizeTrigger,
	AppRecognizeRun3D,

	AppCalibStepRun,
	AppCalibUnitRun,

	AppEnd
};

class DspBuf
{
public:
	DspBuf()
	{ 
		h = 0; w = 0;  buf = NULL;
	}
	DspBuf(int ih, int iw)
	{
		h = ih; w = iw; buf = NULL;
	}
	DspBuf& operator =(const DspBuf& other)
	{
		h = other.h; w = other.w; buf = other.buf;
		return *this;
	}
public:
	int h;
	int w;
	char* buf;
};
#define LL_SUCCESS 0
#define LL_ERROR_GetSeq 1
#define LL_ERROR_Capture3D 2
#define LL_ERROR_Register3D 4
#define LL_ERROR_Register2D 8
#define LL_ERROR_Both 12
#define LL_ERROR_Unknown 99
typedef void(*ONCOMPLETEDCameraVideo)(void* sender, std::vector<DspBuf*> *dspBufs, bool ret);

typedef void(*ONCHANGEAPPSTATUS)(void* sender, AppStatus status);

//callback function when register completed
//recognize both 3D and 2D;
//ret: 0 success; 1 error: get seq; 2 error:capture3D;  4 error:register 3D; 8 error:register 2D; 12 error:3D and 2D; 99 error:unknown 
typedef void(*ONCOMPLETEDRegister)(void* sender, CWSFModel* pc3D, DspBuf* image2D, swPersonInfo* person, int ret);

////callback function when recoginize completed
//pc3D: current pc
//images2D[0]:register image; images2D[1]:rec image
//ret: 0 success; 1 error: get seq; 2 error:capture3D;  4 error:rec 3D; 8 error:rec 2D; 12 error:3D and 2D; ;99 error:unknown 
typedef void(*ONCOMPLETEDRecognize)(void* sender, CWSFModel* pc3D, DspBuf** images2D, swPersonInfo* person, int ret);

