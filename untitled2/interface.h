#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
//#include "LogicLayer/LogicLayerDef.h"
//#include "HWCtrl.h"
//#include "LogicLayer/CalibManager.h"
//#include "LogicLayer/ModelManager.h"
//#include "LogicLayer/RecognizeManager.h"
//#include "LogicLayer/VideoManager.h"
//#include "LogicLayer/Queue2D.h" 

#include <opencv2/opencv.hpp>
#include "LogicLayer/LogicLayer.h" 

#include "ImageProvider.h"
#include "PointCloudProvider.h"

using namespace cv;
 
class InterFace : public QObject
{
	Q_OBJECT
	//Q_PROPERTY(swPersonInfo person READ readPerson WRITE setPerson NOTIFY personChanged)

public:
	explicit InterFace(QObject *parent = 0);
	~InterFace();


	Q_INVOKABLE bool Init();
	//Q_INVOKABLE bool StopAndBack();
	Q_INVOKABLE bool StartVideo();

	Q_INVOKABLE bool EnterRegisterUi();
	Q_INVOKABLE bool TriggerRegister();

	Q_INVOKABLE bool EnterRecognizeUi();
	Q_INVOKABLE bool TriggerRecognize();

	Q_INVOKABLE bool EnterCalibUi();
	Q_INVOKABLE bool TriggerCalibStep(int unit);
	
	Q_INVOKABLE void SetPerson(/*string name, */const QString &id);


public:
	LogicLayer* layer;
	CConfigDir configDir;
	std::vector<DspBuf*> *m_Pimage;
	ImageProvider *provider;
	PointCloudProvider *cloudProvider;
	
	CWSFModel* m_pc3D;
	DspBuf* m_image2D;
	swPersonInfo m_person;

public:
	static void OnChangeAppStatus(void* sender, AppStatus status);

	static void OnCompletedVideo(void* sender, std::vector<DspBuf*> *dspBufs, bool ret);
	void _OnCompletedVideo(std::vector<DspBuf*> *dspBufs, bool ret);

	static void OnCompletedRegister(void* sender, CWSFModel* pc3D, DspBuf* image2D, swPersonInfo* person, int ret);
	void _OnCompletedRegister(CWSFModel* pc3D, DspBuf* image2D, swPersonInfo* person, int ret);

	static void OnCompletedRecognize(void* sender, CWSFModel* pc3D, DspBuf** images2D, swPersonInfo* person, int ret);
	void _OnCompletedRecognize(CWSFModel* pc3D, DspBuf** images2D, swPersonInfo* person, int ret);

	static void OnCompletedCalibStep(void* sender, CalibResult* cr);
	void OnCompletedCalibStep(CalibResult* cr);

	static void OnCompletedCalibUnit(void* sender, CalibResult* cr);
	void _OnCompletedCalibUnit(CalibResult* cr);

signals:
	void imageRefresh();
	void registerSuccess();
	void registerFailed();

	void personChanged(const swPersonInfo &person);

	void recognizeSuccess(swPersonInfo &person);
	void recognizeFailed();

	void calibStepSuccess();
	void calibStepFailed();

	void calibUnitSuccess();
	void calibUnitFailed();

public:
	//swPersonInfo readPerson();
	//void  setPerson(const swPersonInfo &person);
	//void  setPerson(const string &str);
};

#endif // INTERFACE_H
