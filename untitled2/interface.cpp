#include "interface.h"

#pragma comment(lib,"opencv_world300.lib")
#pragma comment(lib,"DataStructX64.lib")
#pragma comment(lib,"BaseCoreBasic.lib")
#pragma comment(lib,"BaseCore.lib")
#pragma comment(lib,"HWCtrlX64.lib")
#pragma comment(lib,"MergePC.lib")
#pragma comment(lib,"SysCalibX64.lib")

QImage cvMat2QImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1  
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)  
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat  
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3  
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		int width = image.width();
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		return QImage();
	}
}

void InterFace::OnChangeAppStatus(void* sender, AppStatus status)
{
	//std::cout << "app status changed to " << status << std::endl;
}

void InterFace::OnCompletedVideo(void* sender, std::vector<DspBuf*> *dspBufs, bool ret)
{
	InterFace *p = (InterFace*)sender;
	p->_OnCompletedVideo(dspBufs, ret);
}

void InterFace::_OnCompletedVideo(std::vector<DspBuf*> *dspBufs, bool ret)
{
	int num = dspBufs->size();
	char id[2][10];
	for (int i = 0; i <num; i++)
	{
		DspBuf* buf = dspBufs->at(i);
		cv::Mat image = cv::Mat(buf->h, buf->w, CV_8UC3);
		image.data = (uchar*)buf->buf;
		cv::resize(image, image, cv::Size(buf->w / 2, buf->h / 2));
		

		QImage picQImage;

		cvtColor(image, image, cv::COLOR_RGBA2RGB);//三通道图片需bgr翻转成rgb CV_BGR2RGB

		picQImage = cvMat2QImage(image);
		provider->picQPixmap[i] = QPixmap::fromImage(picQImage);
	}
	emit imageRefresh();
}

void InterFace::OnCompletedRegister(void* sender, CWSFModel* pc3D, DspBuf* image2D, swPersonInfo* person, int ret)
{
	InterFace *p = (InterFace*)sender;
	p->_OnCompletedRegister(pc3D, image2D, person, ret);
}

void InterFace::_OnCompletedRegister(CWSFModel* pc3D, DspBuf* image2D, swPersonInfo* person, int ret)
{
	m_pc3D = pc3D;
	m_image2D = image2D;
	if (0 == ret)//注册成功
	{
		emit registerSuccess();
	}
	else
		emit registerFailed();
}

bool InterFace::TriggerRegister()
{
	//获取person 信息 ysm 

	emit registerSuccess();
	return layer->TriggerRegister(&m_person);
}

InterFace::InterFace(QObject *parent) : QObject(parent)
{
	std::string appPath = ::BaseCore_AppPath();
	sprintf(configDir.m_cameraConfig, "%s\\ini\\%s", appPath.c_str(), "camconfig.ini");
	sprintf(configDir.m_guiAppConfig, "%s\\ini\\%s", appPath.c_str(), "FPCollect.config.ini");
	sprintf(configDir.m_logConfig, "%s\\ini\\%s", appPath.c_str(), "log.config.ini");
	BaseCore_Init(&configDir);

	sprintf(configDir.m_memoryConfig, "%s\\ini\\%s", appPath.c_str(), "memory.ini");
	sprintf(configDir.m_sycFixedConfig, "%s\\ini\\%s", appPath.c_str(), "cbinfoBB.ini");
	sprintf(configDir.m_sycCalConfig, "%s\\ini\\%s", appPath.c_str(), "CamPara.sys");
	sprintf(configDir.m_G3DGenerateConfig, "%s\\ini\\%s", appPath.c_str(), "G3dconfig.ini");

	layer = new LogicLayer(configDir.m_memoryConfig);
	provider = new ImageProvider();

	sprintf(m_person.sPersonName, "000");
	sprintf(m_person.sPersonID, "000");
	
}

InterFace::~InterFace()
{
	delete layer; layer = NULL;
	delete provider; provider = NULL;
}

bool InterFace::Init()
{
	return layer->Init(configDir.m_sycFixedConfig,
		configDir.m_sycCalConfig,
		configDir.m_G3DGenerateConfig,
		OnChangeAppStatus);
}

bool InterFace::StartVideo()
{
	 return layer->StartVideo(this, OnCompletedVideo);
}

void InterFace::SetPerson(/*string name, */const QString &id)
{
	string _id =id.toStdString();
	strcpy(m_person.sPersonID, _id.c_str());
}
//swPersonInfo InterFace::readPerson()
//{
//	return m_person;
//}
//
//void InterFace::setPerson(const swPersonInfo &person)
//{
//	m_person = person;
//	emit personChanged(m_person);
//}

void InterFace::OnCompletedRecognize(void* sender, CWSFModel* pc3D, DspBuf** images2D, swPersonInfo* person, int ret)
{
	InterFace *p = (InterFace*)sender;
	p->_OnCompletedRecognize(pc3D, images2D, person, ret);
}

void InterFace::_OnCompletedRecognize(CWSFModel* pc3D, DspBuf** images2D, swPersonInfo* person, int ret)
{
	if (ret == 0)//识别成功
		emit recognizeSuccess(*person);
	else
		emit recognizeFailed();
}

bool InterFace::EnterRegisterUi()
{
	return layer->StartRegister(this, OnCompletedRegister);
}

bool InterFace::EnterRecognizeUi()
{
	return layer->StartRecognize(this, OnCompletedRecognize);
}

bool InterFace::TriggerRecognize()
{
	emit recognizeSuccess(m_person);

	return layer->TriggerRecognize();
}

bool InterFace::EnterCalibUi()
{
	emit calibUnitFailed();//ysm
	return true;

	return layer->StartCalib(NULL, OnCompletedCalibStep, OnCompletedCalibUnit);
}

void InterFace::OnCompletedCalibUnit(void* sender, CalibResult* cr)
{
	InterFace *p = (InterFace*)sender;
	p->_OnCompletedCalibUnit(cr);
}

void InterFace::_OnCompletedCalibUnit(CalibResult* cr)
{
	if (cr->m_success)//Calib unit successfully
		emit calibUnitSuccess();
	else 
		emit calibUnitFailed();
}

void InterFace::OnCompletedCalibStep(void* sender, CalibResult* cr)
{
	InterFace *p = (InterFace*)sender;
	p->OnCompletedCalibStep(cr);
}

void InterFace::OnCompletedCalibStep(CalibResult* cr)
{
	if (cr->m_success)//单步标定成功
		emit calibStepSuccess();
	else
		emit calibStepFailed();
}

bool InterFace::TriggerCalibStep(int unit)
{
	emit calibStepFailed();

	return layer->TriggerCalibStep(0);
}