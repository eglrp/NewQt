#pragma once
#include "RecResult2D.h"
#include "FaceDetect.h"
#include "SMTransfer.h"
class RecognizeManager
{
private:
	float m_standardCols;
	SMTransfer m_sm;
	void Image2Standard(cv::Mat& image, cv::Mat& dst)
	{
		float scalar = m_standardCols / (float)image.cols;
		if (scalar < 1.0)
		{
			int width = ((float)image.rows * scalar);
			cv::resize(image, dst, cv::Size(m_standardCols, width));
			return;
		}
		dst = image;
	}
	swRect& Standard2Rect(swRect& rect, int org_width)
	{
		float scalar = m_standardCols / (float)org_width;
		if (scalar < 1.0)
		{
			swRect r;
			r.x = ((float)rect.x / scalar);
			r.y = ((float)rect.y / scalar);
			r.width = ((float)rect.width / scalar);
			r.height = ((float)rect.height / scalar);
			return r;
		}
		return rect;
	}
public:
	RecognizeManager()
	{
		m_standardCols = 640;
	}
public:
	swPersonInfo m_person;
public:
	//model:register for 3D;     image2D:register for 2D
	//return 0: success; 4 error: register 3D; 8 error:register 2D; 12 error:2D and 3D; 99 error:other
	int Register(CWSFModel* pc3D, cv::Mat& image2D, swPersonInfo& person)
	{
		if (pc3D == NULL) 
			return 99;

		cv::Mat dst;
		Image2Standard(image2D, dst);
		int rtn = 0;
		swRegistMulModelInfo  registinfo;
		if (!ConvertPar2D(dst, registinfo.info2d))
			rtn |= 8;
		if (!FaceDetect3DRegister(pc3D, registinfo.info3d))
			rtn |= 4;
		if (rtn != 0)
			return rtn;

		swCommand cmd;
		cmd.cmd = CMD_REGIST;
		cmd.cmdCount = 2;
		cmd.data1 = (char*)&registinfo;
		cmd.data2 = (char*)&person;
		
		cmd.Pack(m_sm.GetMapBuf());
		SetEvent(m_sm.GetRequestEvent());
		DWORD dw = WaitForSingleObject(m_sm.GetResponseEvent(), 20000);//max 20 sec
		if (dw != WAIT_OBJECT_0)
			return 99;
		swCommand cmdRes;
		cmdRes.UnPack(m_sm.GetMapBuf());
		if (cmdRes.cmd != CMD_REGISTRESULT)
			return 99;
		swRegistResult* result = (swRegistResult*)cmdRes.data1;
		return result->recode;
		
	}
	//return 0: success; 1:no face
	int Detect2D(cv::Mat& image2D, swRect& rect)
	{
		cv::Mat dst;
		Image2Standard(image2D, dst);
		swImageData imageData = swImageData(dst.data, dst.cols, dst.rows, dst.channels());

		swCommand cmd;
		cmd.cmd = CMD_FACEDET;
		cmd.cmdCount = 1;
		cmd.data1 = (char*)&imageData;

		cmd.Pack(m_sm.GetMapBuf());
		SetEvent(m_sm.GetRequestEvent());
		DWORD dw = WaitForSingleObject(m_sm.GetResponseEvent(), 1000);//max 1 sec
		if (dw != WAIT_OBJECT_0)
			return 99;

		swCommand cmdRes;
		cmdRes.UnPack(m_sm.GetMapBuf());
		if (cmdRes.cmd != CMD_FACEDETRESULT)
			return 99;
		swRect* r = (swRect*)cmdRes.data1;
		if (r->height > 1 && r->width > 1)
		{
			rect = Standard2Rect(*r, image2D.cols);
			return 0;
		}
		else 
			return 1;
	}
	//return 1: 静止; 0:非静止; 9:没有人脸
	int MotionLess(cv::Mat& image2D, swRect& rect)
	{
		cv::Mat dst;
		Image2Standard(image2D, dst);
		swImageData imageData = swImageData(dst.data, dst.cols, dst.rows, dst.channels());

		swCommand cmd;
		cmd.cmd = CMD_MOTIONDET;
		cmd.cmdCount = 1;
		cmd.data1 = (char*)&imageData;

		cmd.Pack(m_sm.GetMapBuf());
		SetEvent(m_sm.GetRequestEvent());
		DWORD dw = WaitForSingleObject(m_sm.GetResponseEvent(), 1000);//max 1 sec
		if (dw != WAIT_OBJECT_0)
			return 99;

		swCommand cmdRes;
		cmdRes.UnPack(m_sm.GetMapBuf());
		if (cmdRes.cmd != CMD_MOTIONDETRESULT)
			return 99;
		swMotionResult* r = (swMotionResult*)cmdRes.data1;
		rect = Standard2Rect(r->rect, image2D.cols);
		return r->recode;
	}
	//return 0: success; 8: error
	int Recognize2D(cv::Mat& image2D, RecResult2D** r2d)
	{
		cv::Mat dst;
		Image2Standard(image2D, dst);
		swRecongMulModelInfo recinfo;
		if (!ConvertPar2D(dst, recinfo.info2d))
			return 99;

		swCommand cmd;
		cmd.cmd = CMD_RECONG;
		cmd.cmdCount = 1;
		cmd.data1 = (char*)&recinfo;

		cmd.Pack(m_sm.GetMapBuf());
		SetEvent(m_sm.GetRequestEvent());
		DWORD dw = WaitForSingleObject(m_sm.GetResponseEvent(), 1000);//max 1 sec
		if (dw != WAIT_OBJECT_0)
			return 99;

		swCommand cmdRes;
		cmdRes.UnPack(m_sm.GetMapBuf());
		if (cmdRes.cmd != CMD_RECONGRESULT)
			return 99;
		swMatchResult* result = (swMatchResult*)cmdRes.data1;
		if (result->nValid <= 0)
			return 8;
		swRect rect = Standard2Rect(result->MatchInfo->rect, image2D.cols);
		cv::Mat regImage; //add image in further
		*r2d = new RecResult2D(rect, result->MatchInfo->personinfo, regImage, image2D);

		return 0;

	}
	//return 0: success; 4: error
	int Recognize3D(CWSFModel* model, swPersonInfo& person)
	{
		swRecongMulModelInfo recinfo;
		if (!FaceDetect3DRecognize(model, recinfo.info3d))
			return 99;

		swCommand cmd;
		cmd.cmd = CMD_RECONG;
		cmd.cmdCount = 1;
		cmd.data1 = (char*)&recinfo;

		cmd.Pack(m_sm.GetMapBuf());
		SetEvent(m_sm.GetRequestEvent());
		DWORD dw = WaitForSingleObject(m_sm.GetResponseEvent(), 10000);//max 10 sec
		if (dw != WAIT_OBJECT_0)
			return 99;

		swCommand cmdRes;
		cmdRes.UnPack(m_sm.GetMapBuf());
		if (cmdRes.cmd != CMD_RECONGRESULT)
			return 99;
		swMatchResult* result = (swMatchResult*)cmdRes.data1;
		if (result->nValid <= 0)
			return 4;
		person = result->MatchInfo->personinfo;
		return 0;
	}

private:
	bool ConvertPar2D(cv::Mat& image2D, sw2DModelInfo& info2D)
	{
		info2D.imagedata = swImageData(image2D.data, image2D.cols, image2D.rows, image2D.channels());
		return true;
	}
	bool FaceDetect3DRegister(CWSFModel* pc3D, swRegist3DModelInfo& info)
	{
		CWSHeader3DM *pCWSHeader3DM = pc3D->GetHeader();
		CWSPoint *pGetPoint = pc3D->GetPoint();//获得点云指针
		CWSTexCord *pGetTexCord = pc3D->GetTexCord();//获得点云对应的纹理坐标指针;  w,  h 
		CWSTexRGB *pGetTexRGB = pc3D->GetTexRGB();//获得纹理RGB坐标指针

		int nKeysPoints3D[5];//人脸特征点的三维坐标
		memset(nKeysPoints3D, 0, sizeof(int) * 5);
		int *Key = pc3D->GetFaceKeyPoints3D();
		memcpy(nKeysPoints3D, Key, 5 * sizeof(int));
		// 点云
		info.pts.data = new float[pCWSHeader3DM->m_iVerCount * 3];
		int i = 0;
		for (int len_3d = 0; len_3d < pCWSHeader3DM->m_iVerCount; len_3d++)
		{
			info.pts.data[i++] = pGetPoint->m_fx;
			info.pts.data[i++] = pGetPoint->m_fy;
			info.pts.data[i++] = pGetPoint->m_fz;
			pGetPoint++;
		}
		info.pts.dim = 3;
		info.pts.nCount = pCWSHeader3DM->m_iVerCount;

		// 三维特征点
		for (int i = 0; i < 5; i++)
		{
			info.landmarkIndex3D.Index3D[i] = nKeysPoints3D[i];
		}
		// 三角面片
		CWSTriangle* ptriang = pc3D->GetTriangles();
		info.tris.data = new int[pCWSHeader3DM->m_iTriCount * 3];
		i = 0;
		for (int len_3d = 0; len_3d < pCWSHeader3DM->m_iTriCount; len_3d++)
		{
			info.tris.data[i++] = ptriang->m_iv1;
			info.tris.data[i++] = ptriang->m_iv2;
			info.tris.data[i++] = ptriang->m_iv3;
			ptriang++;
		}
		info.tris.dim = 3;
		info.tris.nCount = pCWSHeader3DM->m_iTriCount;
		// 纹理坐标
		info.texCord.data = new float[pCWSHeader3DM->m_iUVCount * 2];
		i = 0;
		for (int len_3d = 0; len_3d < pCWSHeader3DM->m_iUVCount; len_3d++)
		{
			info.texCord.data[i++] = pGetTexCord->m_fu;
			info.texCord.data[i++] = pGetTexCord->m_fv;
			pGetTexCord++;
		}
		info.texCord.dim = 2;
		info.texCord.nCount = pCWSHeader3DM->m_iUVCount;
		// 纹理三角面片
		CWSTriangle* ptextri = pc3D->GetTexTriangles();
		info.texTri.data = new int[pCWSHeader3DM->m_iTriCount * 3];
		i = 0;
		for (int len_3d = 0; len_3d < pCWSHeader3DM->m_iTriCount; len_3d++)
		{
			info.texTri.data[i++] = ptextri->m_iv1;
			info.texTri.data[i++] = ptextri->m_iv2;
			info.texTri.data[i++] = ptextri->m_iv3;
			ptextri++;
		}
		info.texTri.dim = 3;
		info.texTri.nCount = pCWSHeader3DM->m_iTriCount;
		// 纹理颜色
		CWSTexRGB* ptexrgb = pc3D->GetTexRGB();
		info.texRgb.rgb = new unsigned char[pCWSHeader3DM->m_iTexWidth * pCWSHeader3DM->m_iTexHeight * 3];
		i = 0;
		for (int len_3d = 0; len_3d < pCWSHeader3DM->m_iTexWidth * pCWSHeader3DM->m_iTexHeight; len_3d++)
		{
			info.texRgb.rgb[i++] = ptexrgb->m_ur;
			info.texRgb.rgb[i++] = ptexrgb->m_ug;
			info.texRgb.rgb[i++] = ptexrgb->m_ub;
			ptexrgb++;
		}
		info.texRgb.width = pCWSHeader3DM->m_iTexWidth;
		info.texRgb.height = pCWSHeader3DM->m_iTexHeight;
		return true;
	}
	bool FaceDetect3DRecognize(CWSFModel* pc3D, swRecong3DModelInfo& info)
	{
		CWSHeader3DM *pCWSHeader3DM = pc3D->GetHeader();
		CWSPoint *pGetPoint = pc3D->GetPoint();//获得点云指针
		CWSTexCord *pGetTexCord = pc3D->GetTexCord();//获得点云对应的纹理坐标指针;  w,  h 
		CWSTexRGB *pGetTexRGB = pc3D->GetTexRGB();//获得纹理RGB坐标指针

		int nKeysPoints3D[5];//人脸特征点的三维坐标
		memset(nKeysPoints3D, 0, sizeof(int) * 5);
		int *Key = pc3D->GetFaceKeyPoints3D();
		memcpy(nKeysPoints3D, Key, 5 * sizeof(int));

		// 点云
		info.pts.data = new float[pCWSHeader3DM->m_iVerCount * 3];
		int i = 0;
		for (int len_3d = 0; len_3d < pCWSHeader3DM->m_iVerCount; len_3d++)
		{
			info.pts.data[i++] = pGetPoint->m_fx;
			info.pts.data[i++] = pGetPoint->m_fy;
			info.pts.data[i++] = pGetPoint->m_fz;
			pGetPoint++;
		}
		info.pts.dim = 3;
		info.pts.nCount = pCWSHeader3DM->m_iVerCount;

		// 三维特征点
		for (int i = 0; i < 5; i++)
		{
			info.landmarkIndex3D.Index3D[i] = nKeysPoints3D[i];
		}
		// 三角面片
		CWSTriangle* ptriang = pc3D->GetTriangles();
		info.tris.data = new int[pCWSHeader3DM->m_iTriCount * 3];
		i = 0;
		for (int len_3d = 0; len_3d < pCWSHeader3DM->m_iTriCount; len_3d++)
		{
			info.tris.data[i++] = ptriang->m_iv1;
			info.tris.data[i++] = ptriang->m_iv2;
			info.tris.data[i++] = ptriang->m_iv3;
			ptriang++;
		}
		info.tris.dim = 3;
		info.tris.nCount = pCWSHeader3DM->m_iTriCount;
		return true;
	}
};