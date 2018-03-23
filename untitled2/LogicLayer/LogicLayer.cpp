#include "stdafx.h"
#include "LogicLayer.h"
#include <ctime>

LogicLayer::LogicLayer(const char* memConfigFile)
{	
	HWCtrlCreate((void**)&m_hwCtrl);
	m_fcMemory = BaseCore_CreateFCMemory(memConfigFile);
	m_modelManager = new ModelManager(m_fcMemory,m_hwCtrl);
	m_calibManager = new CalibManager();
	m_recManager = new RecognizeManager();
	m_videoManager = new VideoManager();
	m_3DModel = new CWSFModel();
	m_evtRegister = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_evtRecognize = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_owner = NULL;
	m_pFuncRegister = NULL;
	m_pFuncRecognize = NULL;
	m_videoStatus = VideoStatus::VideoClosed;
	m_appStatus = AppStatus::AppNotInit;
}
LogicLayer::~LogicLayer()
{

	HWCtrlDestory((void**)&m_hwCtrl);	
	delete m_calibManager;
	delete m_modelManager;
	
	delete m_3DModel;
}
bool LogicLayer::Init(char* boardFile, char* sysFile, char* g3dFile, ONCHANGEAPPSTATUS pFuncChangeAppStatus)
{
	m_pFuncAppStatus = pFuncChangeAppStatus;
	m_appStatus = AppStatus::AppNotInit;
	if (HW_SUCCESS!=m_hwCtrl->InitSensor())
		return false;
	if (!m_modelManager->Init(sysFile, g3dFile))
		return false;
	m_boardFile = boardFile;
	m_sysFile = sysFile;
	m_appStatus = AppStatus::AppInit;
	return true;
}
bool LogicLayer::StopAndBack()
{
	if (m_appStatus != AppStatus::AppRegisterStart 
		&& m_appStatus != AppStatus::AppRecognizeStart
		&& m_appStatus != AppStatus::AppInit)
		return false;
	m_appStatus = AppStatus::AppInit;
	m_videoStatus = VideoStatus::VideoClosed;
	return true;
}
bool LogicLayer::StartVideo(void* owner, ONCOMPLETEDCameraVideo pFunc)
{
	if (m_videoStatus != VideoStatus::VideoClosed)
		return false;
	m_videoManager->Assign(owner,pFunc);
	CreateThread(NULL, 0, VideoLoop, this, 0, NULL);
	m_videoStatus = VideoStatus::VideoRun;
	return true;
}
DWORD WINAPI LogicLayer::VideoLoop(LPVOID param)
{
	LogicLayer* layer = (LogicLayer*)param;
	return layer->_VideoLoop();
}


DWORD LogicLayer::_VideoLoop()
{
	std::vector<DspBuf*> dspBufs;
	std::vector<SEQSTU*>* sts = &(BaseCore_CameraConfigInstance()->m_seqParam);
	for (int i = 0; i < sts->size(); i++)
		dspBufs.push_back(new DspBuf(0, 0));
	while (m_videoStatus != VideoStatus::VideoClosed)
	{
		if (m_videoStatus != VideoStatus::VideoRun)
		{
			Sleep(200);
			continue;
		}
		Sleep(30);//control the video speed
		static int c = 0;
		c++;
		if (HW_SUCCESS != m_hwCtrl->GetFrm(m_fcMemory, 0xFF))
		{
			//video failed
			m_videoManager->m_pFunc(m_videoManager->m_owner, &dspBufs, false);
		}
		for (int i = 0; i < sts->size(); i++)
		{
			char* p = NULL;
			m_fcMemory->m_cameraBuf[i]->GetFrm(&p);
			dspBufs[i]->h = sts->at(i)->Tri0_h;
			dspBufs[i]->w = sts->at(i)->Tri0_w;
			dspBufs[i]->buf = p;			
			/*cv::Mat image;
			m_fcMemory->m_cameraBuf[i]->GetFrmImage(image);
			cv::rectangle(image, cv::Rect(30, 30, 300, 300), cv::Scalar(0, 255, 0),2);
			dspBufs[i]->h = image.rows;
			dspBufs[i]->w = image.cols;
			dspBufs[i]->buf = (char*)image.data;*/
		}
		//video success
		m_videoManager->m_pFunc(m_videoManager->m_owner, &dspBufs, true);
		//calib operation
		if (m_appStatus == AppStatus::AppCalibStepRun)
		{
			int unit;
			//check if a unit captured
			if (!m_calibManager->IsCaptureForStep(unit))
				continue;
			CalibResult cr;
			cr.m_curUnit = unit;
			//catpure one image
			cr.m_unitStep = m_calibManager->CaptureForStep(m_fcMemory, unit);
			//do step calib if captured less than 9 calib images
			if (cr.m_unitStep < CalibManager::MAXUNITSTEP)
			{
				cr.m_success = true;
				cr.m_crType = CalibResultType::CR_UnitStepRunning;
				m_calibManager->m_stepFunc(m_calibManager->m_sender, &cr);
				m_appStatus = AppStatus::AppInit;
				continue;
			}
			//do unit calib if captured 9 calib images
			m_calibManager->ResetUnit(unit);
			std::cout << "Start calib unit" << std::endl;
			cr.m_success = m_calibManager->CalibUnit(m_fcMemory, unit);
			cr.m_crType = CalibResultType::CR_UnitCompleted;
			m_calibManager->m_unitFunc(m_calibManager->m_sender, &cr);
			m_appStatus = AppStatus::AppInit;
		}
	}
	for (int i = 0; i < dspBufs.size(); i++)
		if (dspBufs[i] != NULL) delete dspBufs[i];
	return 0;
}
bool LogicLayer::StartCalib(void* owner, ONCOMPLETEDCalibStep stepFunc, ONCOMPLETEDCalibUnit unitFunc)
{
	if (m_appStatus != AppStatus::AppInit)
		return false;
	m_calibManager->Init(m_boardFile, m_sysFile, owner, stepFunc, unitFunc);
	return true;
}
bool LogicLayer::TriggerCalibStep(int unit)
{
	if (m_appStatus != AppStatus::AppInit)
		return false;
	m_calibManager->StartCalibStep(unit);
	m_appStatus = AppStatus::AppCalibStepRun;
	return true;
}

bool LogicLayer::StartRegister(void* owner,  ONCOMPLETEDRegister pFunc)
{
	if (m_appStatus != AppStatus::AppInit)
		return false;
	m_owner = owner;
	m_pFuncRegister = pFunc;
	CreateThread(NULL, 0, DoRegister, this, 0, NULL);
	m_appStatus = AppStatus::AppRegisterStart;
	return true;
}
bool LogicLayer::TriggerRegister(swPersonInfo* person)
{
	if (m_appStatus != AppStatus::AppRegisterStart)
		return false;
	m_recManager->m_person = *person;
	SetEvent(m_evtRegister);
	return true;
}
DWORD WINAPI LogicLayer::DoRegister(LPVOID param)
{
	LogicLayer* layer = (LogicLayer*)param;
	return layer->_DoRegister();
}
DWORD LogicLayer::_DoRegister()
{
	while (m_appStatus != AppStatus::AppInit && m_appStatus != AppStatus::AppEnd)
	{
		DWORD dw = WaitForSingleObject(m_evtRegister, 1000);
		if (dw != WAIT_OBJECT_0)
			continue;
		m_videoStatus = VideoStatus::VideoPause;
		m_appStatus = AppStatus::AppRegisterTrigger;
		long t0 = clock();
		long t1 = clock();
		if (!m_modelManager->GetSeq())
		{
			m_pFuncRegister(m_owner, NULL, NULL, NULL, LL_ERROR_GetSeq);
			m_videoStatus = VideoStatus::VideoRun;
			m_appStatus = AppStatus::AppRegisterStart;
			continue;
		}
		long t2 = clock(); std::cout << "Time GetSeq takes:" << t2 - t1 << endl; t1 = clock();

		if (!m_modelManager->Capture3D(m_3DModel))
		{
			m_pFuncRegister(m_owner, NULL, NULL, NULL, LL_ERROR_Capture3D);
			m_videoStatus = VideoStatus::VideoRun;
			m_appStatus = AppStatus::AppRegisterStart;
			continue;
		}
		t2 = clock(); std::cout << "Time Capture3D takes:" << t2 - t1 << endl; t1 = clock();

		cv::Mat image2D;
		FCUtility::FCMemory2Image2D(m_fcMemory, image2D);
		int r = m_recManager->Register(m_3DModel, image2D, m_recManager->m_person);
		if (LL_SUCCESS != r)
		{
			m_pFuncRegister(m_owner, NULL, NULL, NULL, r);
			m_videoStatus = VideoStatus::VideoRun;
			m_appStatus = AppStatus::AppRegisterStart;
			continue;
		}
		t2 = clock();std::cout << "Time Capture3D takes:" << t2 - t1 << endl;
		std::cout << "Total time of register takes:" << t2 - t0 << endl;

		DspBuf d2;
		FCUtility::Image2DspBuf(image2D, d2);
		m_pFuncRegister(m_owner, m_3DModel, &d2, &(m_recManager->m_person), LL_SUCCESS);
		m_videoStatus = VideoStatus::VideoRun;
		m_appStatus = AppStatus::AppRegisterStart;
	}
	return 0;
}



bool LogicLayer::StartRecognize(void* owner, ONCOMPLETEDRecognize pFunc)
{
	if (m_appStatus != AppStatus::AppInit)
		return false;

	m_owner = owner;
	m_pFuncRecognize = pFunc;
	CreateThread(NULL, 0, DoRecognize, this, 0, NULL);
	m_appStatus = AppStatus::AppRecognizeStart;
	return true;
}
bool LogicLayer::TriggerRecognize()
{
	if (m_appStatus != AppStatus::AppRecognizeStart)
		return false;
	SetEvent(m_evtRecognize);
	return true;
}
DWORD WINAPI LogicLayer::DoRecognize(LPVOID param)
{
	LogicLayer* layer = (LogicLayer*)param;
	return layer->_DoRecognize();
}
int LogicLayer::WorkflowWithTrigger()
{
	
	//recognize 2D
	{
		cv::Mat image2D;
		FCUtility::FCMemory2Image2D(m_fcMemory, image2D);
		RecResult2D* r2d = NULL;
		int r = m_recManager->Recognize2D(image2D, &r2d);
		if (LL_SUCCESS == r)
		{
			m_queue2D.Push(r2d);
		}
	}
	//wait trigger event
	{
		DWORD dw = WaitForSingleObject(m_evtRecognize, 50);
		if (dw != WAIT_OBJECT_0)
			return 1;
		m_appStatus = AppStatus::AppRecognizeTrigger;
	}
	// if queue2D is empty, recognize 2D 10 times
	{
		if (m_queue2D.Empty())
		{
			for (int i = 0; i < 10; i++)
			{
				cv::Mat image2D;
				FCUtility::FCMemory2Image2D(m_fcMemory, image2D);

				RecResult2D* r2d = NULL;
				int r = m_recManager->Recognize2D(image2D, &r2d);
				if (LL_SUCCESS == r)
				{
					m_queue2D.Push(r2d);
					break;
				}
				Sleep(30);
			}
		}
	}
	//start position move checking
	{
		swRect rect;
		cv::Mat image2D;
		for (int i = 0; i < 30; i++)
		{
			Sleep(10);
			FCUtility::FCMemory2Image2D(m_fcMemory, image2D);
			if (m_recManager->MotionLess(image2D, rect) == 1)
			{	//motionless

				m_videoStatus = VideoStatus::VideoPause;
				m_appStatus = AppStatus::AppRecognizeRun3D;
				return 0;
			}

		}	
	}
	return 2;//no motionless
}
//return 0: triggered; 1: no trigger
int LogicLayer::WorkFlowNoTrigger()
{
	//cv::Mat s;
	//FCUtility::FCMemory2Image2D(m_fcMemory, s);
	//cv::resize(s, s, cv::Size(s.cols / 4, s.rows / 4));
	//cv::imshow("trigger cam", s);
	//cv::waitKey(2);
	//return 1;
	swRect rect;
	long t1;
	long t2;
	//recognize 2D
	{
		t1 = clock();
		cv::Mat image2D;	
		FCUtility::FCMemory2Image2D(m_fcMemory, image2D);
		RecResult2D* r2d = NULL;
		t1 = clock();
		int r = m_recManager->Recognize2D(image2D, &r2d);
		t2 = clock(); std::cout << "Time recognize 2D takes:" << t2 - t1 << ", return code = " << r << endl;
		if (LL_SUCCESS == r)
		{
			m_queue2D.Push(r2d);
			SEQSTU* st = BaseCore_CameraConfigInstance()->m_seqParam[0];
			st->lastFaceRect = cv::Rect(r2d->rect.x, r2d->rect.y, r2d->rect.width, r2d->rect.height);
		}
		
	}
	//detect 2D
	{
		for (int i = 0; i < 3; i++)
		{
			Sleep(50);
			cv::Mat image2D;
			FCUtility::FCMemory2Image2D(m_fcMemory, image2D);
			t1 = clock();
			int r = m_recManager->Detect2D(image2D, rect);
			t2 = clock(); std::cout << "Time detect 2D takes:" << t2 - t1 << endl;
			if (LL_SUCCESS == r)
			{
				SEQSTU* st = BaseCore_CameraConfigInstance()->m_seqParam[0];
				st->lastFaceRect = cv::Rect(rect.x, rect.y, rect.width, rect.height);
			}
		}
		
	}
	//Motionless 
	{
		cv::Mat image2D;
		for (int i = 0; i < 10; i++)
		{
			Sleep(50);	
			FCUtility::FCMemory2Image2D(m_fcMemory, image2D);
			t1 = clock();
			int r = m_recManager->MotionLess(image2D, rect);
			t2 = clock(); std::cout << "Time MotionLess 2D takes:" << t2 - t1 << ", return code = " << r << endl;
			SEQSTU* st = BaseCore_CameraConfigInstance()->m_seqParam[0];
			st->lastFaceRect = cv::Rect(rect.x, rect.y, rect.width, rect.height);
			if (r==1)
			{	//motionless
				/*cv::rectangle(image2D, st->lastFaceRect, cv::Scalar(0, 255, 0), 5);
				cv::Mat s = image2D;
				cv::resize(s, s, cv::Size(s.cols / 4, s.rows / 4));
				cv::imshow("motionless detect", s);
				cv::waitKey(2);
				return 11;*/
				m_videoStatus = VideoStatus::VideoPause;
				m_appStatus = AppStatus::AppRecognizeRun3D;
				return 0;
			}
			
		}
	}
	return 1;
}
DWORD LogicLayer::_DoRecognize()
{
	while (m_appStatus != AppStatus::AppInit && m_appStatus != AppStatus::AppEnd)
	{
		//if (!WorkflowWithTrigger())
		if (0 != WorkFlowNoTrigger())
			continue;
		//start capture 3D
		{
			int r = LL_SUCCESS;
			long t0 = clock();
			long t1 = clock();
			if (!m_modelManager->GetSeq())
				r = LL_ERROR_GetSeq;
			long t2 = clock(); std::cout << "Time GetSeq takes:" << t2 - t1 << endl; t1 = clock();
			if (r == LL_SUCCESS)
			{
				if (!m_modelManager->Capture3D(m_3DModel))
					r = LL_ERROR_Capture3D;
				t2 = clock(); std::cout << "Time Capture3D takes:" << t2 - t1 << endl; t1 = clock();
			}
			//start recognize 3D
			swPersonInfo person3D;
			DspBuf* images2D = new DspBuf[2];
			if (!m_queue2D.Empty())
			{
				FCUtility::Image2DspBuf(m_queue2D.One()->image[0], images2D[0]);
				FCUtility::Image2DspBuf(m_queue2D.One()->image[1], images2D[1]);
			}
			if (r == LL_SUCCESS)
			{
				r = m_recManager->Recognize3D(m_3DModel, person3D);
				t2 = clock(); std::cout << "Time recognize3D takes:" << t2 - t1 << endl; t1 = clock();
				std::cout << "Total time of recognization takes:" << t2 - t0 << endl;
			}
			if (r == LL_SUCCESS)
			{
				//3d successfully
				if (m_queue2D.Empty())
					m_pFuncRecognize(m_owner, m_3DModel, NULL, &person3D, LL_SUCCESS);// but 2d failed
				else
				{
					//2d successfully too.
					RecResult2D* r2d = m_queue2D.One();
					if (strcmp(r2d->person.sPersonID, person3D.sPersonID) == 0)
						m_pFuncRecognize(m_owner, m_3DModel, &images2D, &person3D, LL_SUCCESS); //person is same
					else
						m_pFuncRecognize(m_owner, m_3DModel, NULL, &person3D, LL_SUCCESS);  //person is not same, use 3d result
				}
			}
			else
			{	//3d failed
				if (m_queue2D.Empty())
					m_pFuncRecognize(m_owner, m_3DModel, NULL, &person3D, LL_ERROR_Both);//2d failed too.
				else
					m_pFuncRecognize(m_owner, m_3DModel, &images2D, &(m_queue2D.One()->person), LL_SUCCESS);// but 2d successfully, user 2d result
			}
			delete[] images2D;
			m_videoStatus = VideoStatus::VideoRun;
			m_appStatus = AppStatus::AppRecognizeStart;
		}
	}
	return 0;
}