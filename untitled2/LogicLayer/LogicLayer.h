#pragma once
#include "LogicLayerDef.h"
#include "HWCtrl.h"
#include "CalibManager.h"
#include "ModelManager.h"
#include "RecognizeManager.h"
#include "VideoManager.h"
#include "Queue2D.h"

class LogicLayer
{

private:
	VideoStatus m_videoStatus;
	AppStatus m_appStatus;
	IFCMemory* m_fcMemory;
	IHWCtrl* m_hwCtrl;
	CWSFModel* m_3DModel;
	ModelManager* m_modelManager;
	CalibManager* m_calibManager;
	RecognizeManager* m_recManager;
	VideoManager* m_videoManager;	
	
	char* m_boardFile;
	char* m_sysFile;

	Queue2D m_queue2D;

	HANDLE m_evtRegister;
	HANDLE m_evtRecognize;

	void* m_owner;
	ONCHANGEAPPSTATUS m_pFuncAppStatus;
	ONCOMPLETEDRegister m_pFuncRegister;
	ONCOMPLETEDRecognize m_pFuncRecognize;

public:
	LogicLayer(const char* memConfigFile);
	~LogicLayer();
	bool Init(char* boardFile, char* sysFile, char* g3dFile, ONCHANGEAPPSTATUS pFuncChangeAppStatus);
	bool StopAndBack();
public:
	//video
	bool StartVideo(void* owner, ONCOMPLETEDCameraVideo pFunc);
private:
	static DWORD WINAPI VideoLoop(LPVOID param);
	DWORD _VideoLoop();

public:
	bool StartCalib(void* owner, ONCOMPLETEDCalibStep stepFunc, ONCOMPLETEDCalibUnit unitFunc);
	bool TriggerCalibStep(int unit);
public:
	//register
	bool StartRegister(void* owner, ONCOMPLETEDRegister pFunc);
	bool TriggerRegister(swPersonInfo* person);
private:
	static DWORD WINAPI DoRegister(LPVOID param);
	DWORD _DoRegister();


public:
	//recognize
	bool StartRecognize(void* owner, ONCOMPLETEDRecognize pFunc);
	bool TriggerRecognize();
private:
	static DWORD WINAPI DoRecognize(LPVOID param);
	//0:success, 1:continue, 2:no motionless and continue
	int WorkflowWithTrigger();
	int WorkFlowNoTrigger();
	DWORD _DoRecognize();
};


