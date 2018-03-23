#pragma once
#include <Windows.h>
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

	AppEnd
};

class DspBuf
{
public:
	DspBuf(int h, int w)
	{
		m_h = h; m_w = w;
	}

public:
	int m_h;
	int m_w;
	char* buf;
};

typedef void(*ONCOMPLETEDCameraVideo)(void* sender, std::vector<DspBuf*> *dspBufs, bool ret);

typedef void(*ONCHANGEAPPSTATUS)(void* sender, AppStatus status);

//callback function when register completed
//recognize both 3D and 2D;
//ret: 0 success; -1 error: get seq; -2 error:capture3D;  -3 error:register 3D; -4 error:register 2D; -5 error:3D and 2D; -9 error:unknown 
typedef void(*ONCOMPLETEDRegister)(void* sender, CWSFModel* pc3D, DspBuf* image2D, swPersonInfo* person, int ret);

////callback function when recoginize completed
//pc3D: current pc
//images2D[0]:register image; images2D[1]:rec image
//ret: -1 error: get seq; -2 error:capture3D;  -3 error:rec 3D; -4 error:rec 2D; -5 error:3D and 2D; ;-9 error:unknown 
typedef void(*ONCOMPLETEDRecognize)(void* sender, CWSFModel* pc3D, DspBuf** images2D, swPersonInfo* person, int ret);
class ILogicLayer
{
public:
	virtual ~ILogicLayer() = 0;
	virtual bool Init(char* camFile, ONCHANGEAPPSTATUS pFuncChangeAppStatus) = 0;
	virtual bool StopAndBack();
public:
	//video
	bool StartVideo(void* owner, ONCOMPLETEDCameraVideo pFunc);
public:
	//register
	bool StartRegister(void* owner, ONCOMPLETEDRegister pFunc);
	bool TriggerRegister(swPersonInfo* person);
public:
	//recognize
	bool StartRecognize(void* owner, ONCOMPLETEDRecognize pFunc);
	bool TriggerRecognize();

};


