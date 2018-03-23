#include"stdafx.h"
#include <windows.h>
#include "ArcBall.h"
#include "DataStruct.h"
#include "WSFModel.h"
#include "WS3DShow.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

#pragma warning(disable:4996)
//点云数据

const float PI2 = 2.0*3.1415926535f;	
const float PI = 3.1415926535f;	

CWSOpenGL::CWSOpenGL()
{
	m_iProjectionType = 0;
	m_dVisualSceneH = 1;
	m_dVisualSceneW = 1;
	m_dOrigin_x = 0;
	m_dOrigin_y = 0;
	m_dEyePosition_z = 1000.0f;
}


CWSOpenGL::CWSOpenGL(HDC dc, GLsizei iWidth, GLsizei iHeight)
{
	m_dNearPlane = 1.0f; 
	m_dFarPlane	 = 1.0f;
	assert(iWidth>0 && iHeight>0);
	m_hRC = NULL;
	if(dc)
	{
		m_hDC = dc;
		InitOpenGL(dc,iWidth,iHeight);
	}
	m_iProjectionType = 0;
	m_dVisualSceneH = iHeight;
	m_dVisualSceneW = iWidth;
	m_dOrigin_x = 0;
	m_dOrigin_y = 0;
	m_dEyePosition_z = 1000.0f;
}

CWSOpenGL::~CWSOpenGL()
{

}


int CWSOpenGL::InitOpenGL(HDC dc, int iWidth, int iHeight) //此处开始对OpenGL进行所有设置
{
	if (dc == NULL){
		return -1;
	}
	m_hDC = dc;
	m_dNearPlane   = 5; 
	m_dFarPlane    = 10000.0; 
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	// 1 设置像素格式  [6/6/2012 Administrator]
	MySetPixelFormat(dc,32,32);

	// 2 创建 RC
	if (!(m_hRC=wglCreateContext(dc))) // 
	{
		return -1;
	}
	// 3 全局显示设置
	SetRc();
	SpecialEffect();

	// 4 设置投影及视口
	if (ORTHO == m_iProjectionType)
	{
		SetProjection(iWidth,iHeight,ORTHO);	// 默认是perspective
	}
	else
	{
		SetProjection(iWidth,iHeight,PERSPECTIVE);	// 默认是perspective
	}

	ClearRc();

	return 0;
}


//
int CWSOpenGL::MySetPixelFormat(HDC hdc,int ColorBits,int DepthBits)
{   
	GLuint PixelFormat;//保存查找匹配的像素格式结果

	PIXELFORMATDESCRIPTOR pfd= // /pfd 告诉窗口我们所希望的东东，即窗口使用的像素格式
	{
		sizeof(PIXELFORMATDESCRIPTOR), // 上述格式描述符的大小,
		1, // 版本号
		PFD_DRAW_TO_WINDOW | // 格式支持窗口
		PFD_SUPPORT_OPENGL | // 格式必须支持OpenGL
		PFD_DOUBLEBUFFER, // 必须支持双缓冲
		PFD_TYPE_RGBA, // 申请 RGBA 格式
		ColorBits, // 选定色彩深度
		0, 0, 0, 0, 0, 0, // 忽略的色彩位
		0, // 无Alpha缓存
		0, // 忽略Shift Bit
		8, // 无累加缓存
		0, 0, 0, 0, // 忽略聚集位
		DepthBits, // 16位 Z-缓存 (深度缓存)
		0, // 无蒙板缓存
		0, // 无辅助缓存
		PFD_MAIN_PLANE, // 主绘图层
		0, // Reserved
		0, 0, 0 // 忽略层遮罩
	};

	if (!(PixelFormat=ChoosePixelFormat(hdc,&pfd))) // Windows 找到相应的象素格式了吗?
	{
		return -1; // 返回 FALSE
	}
	if(!SetPixelFormat(hdc,PixelFormat,&pfd)) // 能够设置象素格式么?
	{
		return -1; // 返回 FALSE
	}

	return 0;
}

//设置RC
int CWSOpenGL::SetRc()
{
	if (!wglMakeCurrent(m_hDC,m_hRC))
	{
		return -1;
	}
	return 0;
}

//设置RC
int CWSOpenGL::SetRc(HDC hDC, HGLRC hRC)
{
	if (!wglMakeCurrent(hDC,hRC))
	{
		return -1;
	}
	return 0;

}



//清除RC
int CWSOpenGL::ClearRc()
{
	wglMakeCurrent(NULL,NULL);
	return 0;
}


int CWSOpenGL::SpecialEffect()
{
	glShadeModel(GL_SMOOTH);//启用阴影平滑
	glClearDepth(1.0f);//设置深度缓存

	glEnable(GL_DEPTH_TEST);//启用深度测试
	glDepthFunc(GL_LEQUAL);//所作深度测试的类型

	glEnable(GL_POINT_SMOOTH);//启用平滑	

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);//告诉系统对透视进行修正

	return 0;
}

/***********************************************************

投影及视口设置 [6/6/2012 Administrator]

***********************************************************/
int CWSOpenGL::SetProjection(int iWidth, int iHeight, ProType pers)
{
	m_dVisualSceneW = iWidth;
	m_dVisualSceneH = iHeight;
	// 1 投影
	if (pers == PERSPECTIVE) 
	{
// 		GLfloat fAspect;
// 		if (iHeight > 0)
// 			fAspect = (GLfloat)m_dVisualSceneW / m_dVisualSceneH;
// 		else
// 			fAspect = 1.0f;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//设置平截头体大小，和位置
//  		double fovy = 0.0;//视野角度
//  		fovy = (180/PI) * 2 * atan((GLfloat)iHeight/2000);
//  		gluPerspective(30.0f, fAspect, 0.1f, 100000.0f);
		glFrustum((-(GLfloat)m_dVisualSceneW / 2 / m_dEyePosition_z) * 1.0f, ((GLfloat)m_dVisualSceneW / 2 / m_dEyePosition_z) * 1.0f,
			(-(GLfloat)m_dVisualSceneH / 2 / m_dEyePosition_z) * 1.0f, ((GLfloat)m_dVisualSceneH /2 / m_dEyePosition_z) * 1.0f, 
			1.0f, 100000.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0, 0.0, m_dEyePosition_z, 
					0.0, 0.0, 0.0,
					0.0, 1.0, 0.0);//设置照相机位置
	}else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
// 		glOrtho(-750.0, 750.0, -750.0*(GLfloat)iHeight/(GLfloat)iWidth, 750.0*(GLfloat)iHeight/(GLfloat)iWidth, 0.0, 100000.0);
		glOrtho(-0.5 * (GLfloat)m_dVisualSceneW, 0.5 * (GLfloat)m_dVisualSceneW, 
				-0.5 * (GLfloat)m_dVisualSceneH, 0.5 * (GLfloat)m_dVisualSceneH, 
				0.1, 100000.0);

		glMatrixMode(GL_MODELVIEW);		
	}
	// 2 视口
	glViewport(0, 0, iWidth, iHeight);

	return 0;
}

/*
(1)函数功能 设置平截头体,同时调整透视投影和正交投影的平截头体，用缩放功能
(2)作者 朱浩然
(3)日期 2015-11-10
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWSOpenGL::SetFrustums(double dWidth, //in,平截头体的宽度
						   double dHeight)//in,平截头体的高度
{
	SetRc();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (ORTHO == m_iProjectionType)
	{
		glOrtho(-0.5 * (GLfloat)dWidth, 0.5 * (GLfloat)dWidth, 
			-0.5 * (GLfloat)dHeight, 0.5 * (GLfloat)dHeight, 
			0.1, 100000.0);
	}
	else
	{
		GLfloat fovy;//视野角度
		fovy = (180/PI) * 2 * atan(static_cast<GLfloat>(dHeight)/2000);
		gluPerspective(fovy, (GLfloat)dWidth / dHeight, 0.1f, 100000.0f);
	}

	glMatrixMode(GL_MODELVIEW);		
	ClearRc();
	return 1;
}

/*
(1)函数功能 （重载）设置平截头体,同时调整透视投影和正交投影的平截头体，用缩放功能。增加改变平截头体相对相机位置的功能
(2)作者 朱浩然
(3)日期 2015-11-10
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWSOpenGL::SetFrustums(double dWidth, //in,平截头体的宽度(在z=0的平面)
						   double dHeight,//in,平截头体的高度(在z=0的平面)
						   double dOrigin_x, //in,平截头体的中心位置——x
						   double dOrigin_y)//in,平截头体的中心位置——y
{
	SetRc();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (ORTHO == m_iProjectionType)
	{
		glOrtho(-0.5 * (GLfloat)dWidth + (GLfloat)dOrigin_x, 0.5 * (GLfloat)dWidth + (GLfloat)dOrigin_x, 
			-0.5 * (GLfloat)dHeight + (GLfloat)dOrigin_y, 0.5 * (GLfloat)dHeight + (GLfloat)dOrigin_y, 
			0.1, 100000.0);
	}
	else
	{
// 		GLfloat fovy = 0.0;//视野角度
// 		fovy = (180/PI) * 2 * atan((GLfloat)dHeight/2000);
// 		gluPerspective(fovy, (GLfloat)dWidth / (GLfloat)dHeight, 0.1f, 100000.0f);
		glFrustum(((-(GLfloat)dWidth/2 + (GLfloat)dOrigin_x) / m_dEyePosition_z) * 1.0f, (((GLfloat)dWidth/2 + (GLfloat)dOrigin_x) / m_dEyePosition_z) * 1.0f,
			((-(GLfloat)dHeight/2 + (GLfloat)dOrigin_y) / m_dEyePosition_z) * 1.0f, (((GLfloat)dHeight/2 + (GLfloat)dOrigin_y) / m_dEyePosition_z) * 1.0f, 
			1.0f, 100000.0f);
	}

	glMatrixMode(GL_MODELVIEW);		
	ClearRc();
	return 1;
}

/*
(1)函数功能 重置OpenGL窗口大小
(2)作者 段洪亮
(3)日期 2014-7-1
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
void CWS3DShow::ReSizeGLScene(GLsizei iwidth,GLsizei iheight) //重置OpenGL窗口大小
{
	//
	cout<<"ReSizeGLScene called"<<endl;
	m_OpenGL.m_iWidth = iwidth;
	m_OpenGL.m_iHeight = iheight;
	if(m_OpenGL.m_iHeight==0) //防止被清零,将Height设为1
	{
		m_OpenGL.m_iHeight =1;
	}	

	//设置边界
	m_WSPose.m_ArcBall.setBounds(iwidth, iheight);
	// 设置投影及视口
	m_OpenGL.SetRc();
	if (PERSPECTIVE == m_OpenGL.m_iProjectionType)
	{
		m_OpenGL.SetProjection(m_OpenGL.m_iWidth, m_OpenGL.m_iHeight,PERSPECTIVE);	// 默认是perspective
	}
	else
	{
		m_OpenGL.SetProjection(m_OpenGL.m_iWidth, m_OpenGL.m_iHeight,ORTHO);	// 默认是perspective
	}

	m_OpenGL.ClearRc();

// 	glMatrixMode(GL_PROJECTION); //选择投影矩阵
// 	glLoadIdentity();//重置投影矩阵
	//设置正投影空间
// 	glOrtho(-750.0,750.0, -750.0*(GLfloat)m_OpenGL.m_iHeight/(GLfloat)m_OpenGL.m_iWidth, 750.0*(GLfloat)m_OpenGL.m_iHeight/(GLfloat)m_OpenGL.m_iWidth,-100000000.0,100000000.0);
	// 	glOrtho(-(GLfloat)m_OpenGL.m_iWidth/2, (GLfloat)m_OpenGL.m_iWidth/2, -(GLfloat)m_OpenGL.m_iHeight/2, (GLfloat)m_OpenGL.m_iHeight/2, -10000.0, 10000.0);//视景体和窗口一样大

// 	glMatrixMode(GL_MODELVIEW);//选择模型观察矩阵
// 	glLoadIdentity();

}


/*
(1)函数功能 交换HDC（前后的）
(2)作者 段洪亮
(3)日期 2014-7-1
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
void CWSOpenGL::SwapOpenGLBuffers()
{
	SwapBuffers(m_hDC);
}


//一下是drawing类的方法CDrawing::CDrawing(void)
CWSDrawing::CWSDrawing()
{ 
	m_iDspMode = DSP_GENERAL;
	for(int i=0;i<256*3;i++)
	{
		listName[i] =0;
	}
	listName[1] = 1;
	listName[2] = 2;
	m_iCloudCount = 0;
}


CWSDrawing::~CWSDrawing()
{

}


/*
(1)函数功能 创建点云
(2)作者 段洪亮
(3)日期 2014-7-1
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWSDrawing::GetCloudCount() const
{
	return m_iCloudCount;
}

/*
(1)函数功能 创建点云
(2)作者 段洪亮
(3)日期 2014-7-1
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWSDrawing::CreateCloud(void *pWSFModel,int texture_or_cloud)
{
	m_iCloudCount = 1;
	if(listName[1]>0)
	{		
		glDeleteLists(listName[1],1);
	}
	if(listName[2]>0)
	{		
		glDeleteLists(listName[2],1);
	}
	if(texture_or_cloud==0)
	{
		glNewList(listName[1],GL_COMPILE);
		DrawCloud(pWSFModel,0);
		glEndList();
		glNewList(listName[2],GL_COMPILE);
		DrawCloud(pWSFModel,1);
		glEndList();
	}
	else if(texture_or_cloud==1)
	{
		glNewList(listName[1],GL_COMPILE);
		DrawCloud(pWSFModel,1);
		glEndList();
		glNewList(listName[2],GL_COMPILE);
		DrawCloud(pWSFModel,1);
		glEndList();
	}
	else if(texture_or_cloud==2)
	{
		glNewList(listName[1],GL_COMPILE);
		DrawCloud(pWSFModel,2);
		glEndList();
		glNewList(listName[2],GL_COMPILE);
		DrawCloud(pWSFModel,1);
		glEndList();
	}

	return 0;
}

/*
(1)函数功能 创建多点云
(2)作者 段洪亮
(3)日期 2014-7-1
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWSDrawing::CreateCloud(void *pWSFModel,int texture_or_cloud, int ICloudID)
{

	m_iCloudCount++;

	// 	listName[2 * m_iCloudCount - 1] = 2 * m_iCloudCount - 1;
	// 	listName[2 * m_iCloudCount ] = 2 * m_iCloudCount;

	// 	if(listName[1]>0)
	// 	{		
	// 		glDeleteLists(listName[1],1);
	// 	}
	// 	if(listName[2]>0)
	// 	{		
	// 		glDeleteLists(listName[2],1);
	// 	}

	GLuint base;
	base = glGenLists(2);
	glListBase(base);

	listName[base] = 2 * ICloudID - 1;
	listName[base+1] = 2 * ICloudID;

	if (texture_or_cloud != 1)
	{
		glNewList(listName[base],GL_COMPILE);
		DrawCloud(pWSFModel,texture_or_cloud);
		glEndList();

		glNewList(listName[base + 1],GL_COMPILE);
		DrawCloud(pWSFModel,1);
		glEndList();
	}

	if (texture_or_cloud == 1)
	{
		glNewList(listName[base],GL_COMPILE);
		DrawCloud(pWSFModel,1);
		glEndList();
		glNewList(listName[base + 1],GL_COMPILE);
		DrawCloud(pWSFModel,1);
		glEndList();
	}



	return 0;
}



/*
(1)函数功能 删除选中点云
(2)作者 朱浩然
(3)日期 2015-9-5
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWSDrawing::DeleteSelCloud(int iCouldID)
{
	for (int i = 1; i <=m_iCloudCount; i++)
	{	
		if (listName[2*i -1] == 2 * iCouldID - 1)
		{
			glDeleteLists( listName[2*i -1], 1 );
			glDeleteLists( listName[2*i ], 1 );
			listName[2*i -1] = 0;
			listName[2*i] = 0;
		}
	}
	return 1;
}

/*
(1)函数功能 影藏选中点云
(2)作者 朱浩然
(3)日期 2015-9-5
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWSDrawing::HideSelCould(int iCouldID)
{
	for (int i = 1; i <=m_iCloudCount; i++)
	{	
		if (listName[2*i -1] == 2 * iCouldID - 1)
		{
			listName[2*i -1] = 0;
			listName[2*i] = 0;
		}
	}
	return 1;
}








/*
(1)函数功能 显示选中点云
(2)作者 朱浩然
(3)日期 2015-9-5
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWSDrawing::ShowSelCloud(int iCouldID)
{
	for (int i = 1; i <=m_iCloudCount; i++)
	{	
		if (2*i -1 == 2 * iCouldID - 1)
		{
			listName[2*i -1] = 2 * iCouldID - 1;
			listName[2*i] = 2 * iCouldID;
		}
	}
	return 1;
}



/*
(1)函数功能 删除所有点云
(2)作者 朱浩然
(3)日期 2015-9-5
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWSDrawing::DeleteAllCloud()
{
	for (int i = 1; i <=m_iCloudCount; i++)
	{	
		glDeleteLists( listName[2*i -1], 1 );
		glDeleteLists( listName[2*i ], 1 );
		listName[2*i -1] = 0;
		listName[2*i] = 0;
	}
	return 1;
}



/*
(1)函数功能 画点云
(2)作者 段洪亮
(3)日期 2014-7-1
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/													
int CWSDrawing::DrawCloud(void *pWSFModel,int texture_or_cloud)
{
	CWSFModel *p2currentPC = (CWSFModel *)pWSFModel;
	int size = (p2currentPC->GetHeader()->m_iTexWidth) * (p2currentPC->GetHeader()->m_iTexHeight) ;

	float iscale = 1;//p2currentPC->GetScale();
	if(texture_or_cloud==0 && p2currentPC->GetPCT()==PC_PLY)
	{
		texture_or_cloud = 2;
	}

	if(texture_or_cloud==0 )//wrl
	{
		if(DSP_GENERAL==m_iDspMode)
		{
			glColor3f(1,1,1);
			glDisable(GL_TEXTURE_2D);
			
			CWSTexCord	 *pTexCord = p2currentPC->GetTexCord();
			CWSTriangle	 *pPtTriangles = p2currentPC->GetTriangles();
			CWSTriangle	 *pTextTriangles = p2currentPC->GetTexTriangles();
			CWSPoint     *pPoint = p2currentPC->GetPoint();
			CWSTexRGB    *PTexRGB = p2currentPC->GetTexRGB();
			int ImgHeight = p2currentPC->GetHeader()->m_iTexHeight;
			int ImgWidth = p2currentPC->GetHeader()->m_iTexWidth;

			for(int i=0;i<p2currentPC->GetHeader()->m_iTriCount;++i)
			{
				glBegin(GL_TRIANGLES);

				int iv1 = pPtTriangles[i].m_iv1;
				int iv3 = pPtTriangles[i].m_iv2;
				int iv2 = pPtTriangles[i].m_iv3;
				int	itv1 = pTextTriangles[i].m_iv1;
				int	itv3 = pTextTriangles[i].m_iv2;
				int	itv2 = pTextTriangles[i].m_iv3;
				int itexIDV1 = int(pTexCord[itv1].m_fu*ImgWidth)+int((1-pTexCord[itv1].m_fv)*ImgHeight)*ImgWidth;
				int itexIDV2 = int(pTexCord[itv2].m_fu*ImgWidth)+int((1-pTexCord[itv2].m_fv)*ImgHeight)*ImgWidth;
				int itexIDV3 = int(pTexCord[itv3].m_fu*ImgWidth)+int((1-pTexCord[itv2].m_fv)*ImgHeight)*ImgWidth;

				glColor3f(PTexRGB[itexIDV1].m_ur/255.0, PTexRGB[itexIDV1].m_ug/255.0,PTexRGB[itexIDV1].m_ub/255.0); 
				glVertex3f(pPoint[iv1].m_fx, -pPoint[iv1].m_fy, -pPoint[iv1].m_fz);

				glColor3f(PTexRGB[itexIDV2].m_ur/255.0, PTexRGB[itexIDV2].m_ug/255.0,PTexRGB[itexIDV2].m_ub/255.0); 
				glVertex3f(pPoint[iv2].m_fx, -pPoint[iv2].m_fy, -pPoint[iv2].m_fz);

				glColor3f(PTexRGB[itexIDV3].m_ur/255.0, PTexRGB[itexIDV3].m_ug/255.0,PTexRGB[itexIDV3].m_ub/255.0); 
				glVertex3f(pPoint[iv3].m_fx, -pPoint[iv3].m_fy, -pPoint[iv3].m_fz);


				glEnd();
			}
			glColor3f(1,1,1);

		}
		else if(DSP_WRL == m_iDspMode)
		{
			glEnable(GL_TEXTURE_2D);
			//LoadGLTextures();
			LoadTex(p2currentPC->GetHeader()->m_iTexWidth, p2currentPC->GetHeader()->m_iTexHeight,p2currentPC->GetTexRGB());
			glBindTexture(GL_TEXTURE_2D,texture);

			CWSTexCord	 *pTexCord = p2currentPC->GetTexCord();
			CWSTriangle	 *pPtTriangles = p2currentPC->GetTriangles();
			CWSTriangle	 *pTextTriangles = p2currentPC->GetTexTriangles();
			CWSPoint     *pPoint = p2currentPC->GetPoint();

			int iv1  = -1;
			int iv2  = -1; 
			int iv3  = -1; 

			int itv1 = -1;
			int itv2 = -1;
			int itv3 = -1;

			int iCount = p2currentPC->GetHeader()->m_iTriCount;
			for(int i = 0; i < iCount; ++i)
			{
				glBegin(GL_TRIANGLES);

				iv1 = pPtTriangles[i].m_iv2;
				iv3 = pPtTriangles[i].m_iv1;
				iv2 = pPtTriangles[i].m_iv3;

				itv1 = pTextTriangles[i].m_iv2;
				itv3 = pTextTriangles[i].m_iv1;
				itv2 = pTextTriangles[i].m_iv3;


				glTexCoord2f(pTexCord[itv1].m_fu,1.0-pTexCord[itv1].m_fv ); 
				glVertex3f(pPoint[iv1].m_fx, -pPoint[iv1].m_fy, -pPoint[iv1].m_fz);

				glTexCoord2f(pTexCord[itv3].m_fu,1.0-pTexCord[itv3].m_fv ); 
				glVertex3f(pPoint[iv3].m_fx, -pPoint[iv3].m_fy, -pPoint[iv3].m_fz);

				glTexCoord2f(pTexCord[itv2].m_fu,1.0-pTexCord[itv2].m_fv ); 
				glVertex3f(pPoint[iv2].m_fx, -pPoint[iv2].m_fy, -pPoint[iv2].m_fz);

				glEnd();

				if (0 == i % 10000)
				{
					glFlush();
				}

			}
			glFlush();
			glFinish();

			glDisable(GL_TEXTURE_2D);
			glColor3f(1,1,1);
		}
	}
	else if(texture_or_cloud==1) // 点云
	{
		glColor3f(1,1,1);
		glDisable(GL_TEXTURE_2D);
		CWSPoint      *pPoint = p2currentPC->GetPoint();
		glPointSize(1.5);//朱：设置点的大小，可以是一个点映射到屏幕时保持4个像素。
		glBegin(GL_POINTS);
		for(int i=0;i<p2currentPC->GetHeader()->m_iVerCount;++i)
		{
			glVertex3f(pPoint[i].m_fx, -pPoint[i].m_fy, -pPoint[i].m_fz);
		}
		glEnd(); // 三角形绘制结束
		
	}

	else if(texture_or_cloud==2)//ply,3dm,每个点一个颜色，点数与颜色数相同
	{
		glDisable(GL_TEXTURE_2D);
		CWSTexRGB    *PTexRGB = p2currentPC->GetTexRGB();
		CWSTriangle	 *pPtTriangles = p2currentPC->GetTriangles();
		CWSPoint      *pPoint = p2currentPC->GetPoint();

		for(int i=0;i<p2currentPC->GetHeader()->m_iTriCount;++i)
		{
			glBegin(GL_TRIANGLES);

			int iv1 = pPtTriangles[i].m_iv2;
			int iv3 = pPtTriangles[i].m_iv1;
			int iv2 = pPtTriangles[i].m_iv3;

			glColor3f(PTexRGB[iv1].m_ur/255.0, PTexRGB[iv1].m_ug/255.0,PTexRGB[iv1].m_ub/255.0); 
			glVertex3f(pPoint[iv1].m_fx, -pPoint[iv1].m_fy, -pPoint[iv1].m_fz);

			glColor3f(PTexRGB[iv2].m_ur/255.0, PTexRGB[iv2].m_ug/255.0,PTexRGB[iv2].m_ub/255.0); 
			glVertex3f(pPoint[iv2].m_fx, -pPoint[iv2].m_fy, -pPoint[iv2].m_fz);

			glColor3f(PTexRGB[iv3].m_ur/255.0, PTexRGB[iv3].m_ug/255.0,PTexRGB[iv3].m_ub/255.0); 
			glVertex3f(pPoint[iv3].m_fx, -pPoint[iv3].m_fy, -pPoint[iv3].m_fz);


			glEnd();
		}
		glColor3f(1,1,1);

	}
	return 0;
}


int CWSDrawing::LoadTex(int iTexWidth, int iTexHeight, CWSTexRGB *pWSTexRGB)
{
	glEnable(GL_TEXTURE_2D);
	//载入视图
	glTexImage2D(GL_TEXTURE_2D, 0, 3, iTexWidth, iTexHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pWSTexRGB);
	//设置参数
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//绑定视图
	glBindTexture(GL_TEXTURE_2D,texture);
	return 0;
}




//CWSPose类

CWSPose::CWSPose()
{
	SetTransform();
	SetLastRot();
	SethisRot();
	ArcBallT    ArcBall(640.0f, 480.0f);
	m_ArcBall = ArcBall;         // NEW: ArcBall Instance

	//缩放
	m_fViewScale = 1.0f;
	//平移
	m_fXShift = 0.0f;
	m_fYShift = 0.0f;

}

CWSPose::~CWSPose()
{

}

//初始化SetMatrix4fT矩阵
int CWSPose::SetTransform()
{
	Matrix4fT Transform   = {  1.0f,  0.0f,  0.0f,  0.0f,				// NEW: Final Transform
		0.0f,  1.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  1.0f,  0.0f,
		0.0f,  0.0f,  0.0f,  1.0f };
	m_Transform = Transform;
	return 0;
}

//初始化SetMatrix3fT矩阵
int CWSPose::SetLastRot()
{
	Matrix3fT LastRot     = {  1.0f,  0.0f,  0.0f,					// NEW: Last Rotation
		0.0f,  1.0f,  0.0f,
		0.0f,  0.0f,  1.0f };
	m_LastRot = LastRot;
	return 0;
}

//初始化SetMatrix3fT矩阵
int CWSPose::SethisRot()
{

	Matrix3fT ThisRot = {  1.0f,  0.0f,  0.0f,					// NEW: This Rotation
		0.0f,  1.0f,  0.0f,
		0.0f,  0.0f,  1.0f };
	m_ThisRot = ThisRot;
	return 0;
}


/*
(1)函数功能 左键按下
(2)作者 段洪亮
(3)日期 2014-7-1
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
void CWSPose::LeftButnDown(int MouseX,int MouseY)
{
	m_LastRot = m_ThisRot;										// Set Last Static Rotation To Last Dynamic One
	m_MousePt.s.X = MouseX;
	m_MousePt.s.Y = MouseY;
	m_ArcBall.click(&m_MousePt);								// Update Start Vector And Prepare For Dragging
}

/*
(1)函数功能 鼠标拖动
(2)作者 段洪亮
(3)日期 2014-7-1
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWSPose::RotMouseDragging(int MouseX,int MouseY)
{
	m_MousePt.s.X = MouseX;
	m_MousePt.s.Y = MouseY;
	Quat4fT     ThisQuat;
	Matrix3fT curRot;

	m_ArcBall.drag(&m_MousePt, &ThisQuat);						// Update End Vector And Get Rotation As Quaternion
	Matrix3fSetRotationFromQuat4f(&m_ThisRot, &ThisQuat);		// Convert Quaternion Into Matrix3fT  计算旋转量

	curRot = m_ThisRot;
	Matrix3fMulMatrix3f(&curRot, &m_LastRot); // 累计旋转结果
	Matrix4fSetRotationFromMatrix3f(&m_Transform,&curRot);//得到我们的最终结果
	m_ThisRot = curRot;
	//pointerPC[0].Matrix4fMul(&curRot);
	//pointerPC[0].SetThisRot(&curRot); /*= curRot;*/
	Matrix4fSetRotationFromMatrix3f(&m_Transform, &m_ThisRot);	// Set Our Final Transform's Rotation From This One
	return 1;
}


//设置显示点云时缩放系数，
int CWSPose::SetViewScale(int ifViewScale)
{
	if(ifViewScale>0)
		m_fViewScale = m_fViewScale*1.15f;
	else if(ifViewScale<0)
		m_fViewScale = m_fViewScale/1.15f;
	return 1;
}


//设置显示点云时缩放系数，
int CWSPose::SetViewScale(int ifViewScale, long lScreen_x, long lScreen_y)
{
	if(ifViewScale>0)
		m_fViewScale = m_fViewScale*1.15f;
	else if(ifViewScale<0)
		m_fViewScale = m_fViewScale/1.15f;
	return 1;
}


//累积平移增量
int CWSPose::AddXYShift(float ifxShift,float ifyShift)
{
	m_fXShift = m_fXShift + ifxShift;
	m_fYShift = m_fYShift + ifyShift;
	return 1;
}
//设置点云旋转first click
int  CWSPose::SetRotFirClick(float ifptx,float ifpty)
{
	m_LastRot = m_ThisRot;										// Set Last Static Rotation To Last Dynamic One
	m_MousePt.s.X = ifptx;
	m_MousePt.s.Y = ifpty;
	m_ArcBall.click(&m_MousePt);								// Update Start Vector And Prepare For Dragging
	return 1;
}

//设置默认的pose
int  CWSPose::SetDefPose()
{
	m_fXShift = 0;
	m_fYShift = 0;
	m_fViewScale = 1.0f;
	//ysm cs
	SetTransform();
	SetLastRot();
	SethisRot();
	//ysm cs
	Matrix4fSetIdentity(&m_Transform);
	return 1;
}

///////////////////////////////////////////////////////////////////////////
//CWS3DShow类

//传入点云指针

CWS3DShow::CWS3DShow()
{
	m_hDC = NULL; 
	//初始化默认点云颜色
	m_r =  200;
	m_g = 200;
	m_b = 200;

	m_lScreen_x = 0.0f;
	m_lScreen_y = 0.0f;

	m_lCOR_x = 0.0f;
	m_lCOR_y = 0.0f;
	m_lCOR_z = 0.0f;

	m_lCOG_x = 0.0f;
	m_lCOG_y = 0.0f;
	m_lCOG_z = 0.0f;

	m_iShowType = 0;

	m_PolyGonLine.clear();
	m_SelectPtsCord.clear();

	m_hDC_DepthCurve = NULL;

	m_WSDrawing.listName[1]=1;
	m_WSDrawing.listName[2]=2;

	m_pWSFModel = NULL;
}

CWS3DShow::~CWS3DShow()
{
	m_PolyGonLine.clear();
	m_SelectPtsCord.clear();
}

//初始化 传入DC
int CWS3DShow::Initialization(HDC hDC, int width, int height)
{
	cout<<"Initialization called"<<endl;
	m_hDC = hDC;
	m_OpenGL.InitOpenGL(hDC, width, height);
	m_WSPose.m_ArcBall.setBounds(width,height);
	return 0;
}



int CWS3DShow::AddCWS3DFModel(CWSFModel *pWSFModel,int texture_or_cloud)
{
	m_pWSFModel = pWSFModel;
	m_OpenGL.SetRc();
	m_WSDrawing.CreateCloud(m_pWSFModel,texture_or_cloud);
	m_OpenGL.ClearRc();
	return 0;
}


int CWS3DShow::AddCWS3DFModel(CWSFModel *pWSFModel, int texture_or_cloud, int iCloudID)
{
	m_pWSFModel = pWSFModel;
	m_OpenGL.SetRc();
	m_WSDrawing.CreateCloud(m_pWSFModel,texture_or_cloud, iCloudID);
	m_OpenGL.ClearRc();
	return 0;
}

void CWS3DShow::SetDspMode(DSPMode iMode)
{
	m_OpenGL.SetRc();
	m_WSDrawing.m_iDspMode = iMode;
	m_OpenGL.ClearRc();
}

/*
(1)函数功能 指定渲染模式进行渲染 0代表纹理渲染，1代表点显示渲染,
(2)作者 朱浩然 17092845574
(3)日期 2015-10-17
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWS3DShow::RenderScene(int type ) //RenderSceneList
{
	m_iShowType = type;
	return RenderScene();
	 
	////设置当前窗口
	//m_OpenGL.SetRc();
 //	glClearColor(0.0, 0.0, 0.0, 0.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer

	//glLoadIdentity();									// Reset the current modelview matrix
	//glTranslatef(m_WSPose.m_fXShift, m_WSPose.m_fYShift, 0.0);//平移
	//glScalef(m_WSPose.m_fViewScale, m_WSPose.m_fViewScale, m_WSPose.m_fViewScale);					// Move left 1.5 units and into the screen 6.0

	//									// NEW: Apply Dynamic Transform
	////glPushMatrix();	
	////glTranslatef(m_lCOR_x, m_lCOR_y, m_lCOR_z);//旋转中心，先移动
	////glMultMatrixf(m_WSPose.m_Transform.M);
	////glTranslatef(-m_lCOR_x, -m_lCOR_y, -m_lCOR_z);//旋转中心，后还原

	//if(type==0 )
	//{
	//	glEnable(GL_TEXTURE_2D);
	//	for (int i = 1; i <= m_WSDrawing.m_iCloudCount; i++)
	//	{
	//		//glCallList(m_WSDrawing.listName[2 * i - 1]);
	//		m_WSDrawing.DrawCloud(m_pWSFModel,1);
	//	}

	//}
	//else
	//{
	//	glColor3f(1,1,1);
	//	glDisable(GL_TEXTURE_2D);
	//	glColor3f(m_r/255.0,m_g/255.0,m_b/255.0);
	//	for (int i = 1; i <= m_WSDrawing.m_iCloudCount; i++)
	//	{
	//		//glCallList(m_WSDrawing.listName[ 2 * i ]);
	//		m_WSDrawing.DrawCloud(m_pWSFModel,1);
	//	}

	//} 	
	//												// NEW: Unapply Dynamic Transform
	////glFlush ();	
	////glFinish();
	//m_OpenGL.SwapOpenGLBuffers() ;
	////glPopMatrix();	
	//// 保证程序运行
	//m_OpenGL.ClearRc();
}


/*
(1)函数功能 重载，用成员变量指定渲染模式
(2)作者 朱浩然 17092845574
(3)日期 2015-10-17
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 返回当前显示类型值m_iShowType
*/
int CWS3DShow::RenderScene() //RenderSceneList
{
	m_OpenGL.SetRc();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer
	if(NULL==m_pWSFModel)
	{
		m_OpenGL.SwapOpenGLBuffers() ;
		m_OpenGL.ClearRc();
		return -1;
	}
	//设置当前窗口
	//m_OpenGL.SetRc();
	// 	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer
	glLoadIdentity();									// Reset the current modelview matrix

	gluLookAt(0.0, 0.0, m_OpenGL.m_dEyePosition_z, 
				0.0, 0.0, 0.0,
				0.0, 1.0, 0.0);//设置照相机位置

 	glTranslatef(m_WSPose.m_fXShift, m_WSPose.m_fYShift, 0.0);//平移
	glScalef(m_WSPose.m_fViewScale, m_WSPose.m_fViewScale, m_WSPose.m_fViewScale);					// Move left 1.5 units and into the screen 6.0

	// NEW: Apply Dynamic Transform
	glPushMatrix();	
	//glTranslatef(m_lCOR_x, m_lCOR_y, m_lCOR_z);//旋转中心，先移动
	glMultMatrixf(m_WSPose.m_Transform.M);
	//glTranslatef(-m_lCOR_x, -m_lCOR_y, -m_lCOR_z);//旋转中心，后还原

	if(m_iShowType==0 )
	{
		glEnable(GL_TEXTURE_2D);
		for (int i = 1; i <= m_WSDrawing.m_iCloudCount; i++)
		{
			glCallList(m_WSDrawing.listName[2 * i - 1]);
		}
		//m_WSDrawing.DrawCloud(m_pWSFModel,m_iShowType);
	}
	else
	{
		//glColor3f(1,1,1);
		glDisable(GL_TEXTURE_2D);
		glColor3f(m_r/255.0,m_g/255.0,m_b/255.0);
		for (int i = 1; i <= m_WSDrawing.m_iCloudCount; i++)
		{
			glCallList(m_WSDrawing.listName[ 2 * i ]);
		}
		//m_WSDrawing.DrawCloud(m_pWSFModel,m_iShowType);

	} 	
	glPopMatrix();													// NEW: Unapply Dynamic Transform
	//glFlush ();	
	glFinish();
	m_OpenGL.SwapOpenGLBuffers() ;
	// 保证程序运行
	m_OpenGL.ClearRc();

if(NULL!=m_hDC_DepthCurve)
{
	//绘制曲线
	if(m_PolyGonLine.size()==2)
	{
		cout<<"在点云显示空间绘制所选剖线"<<endl;
		HPEN hpen = CreatePen(PS_SOLID,3,RGB(255,0,0));
		HPEN hOldpen = (HPEN)SelectObject(m_hDC,hpen);
		MoveToEx(m_hDC,m_PolyGonLine[0].x,m_PolyGonLine[0].y,NULL);
		LineTo(m_hDC,m_PolyGonLine[1].x,m_PolyGonLine[1].y);
		SelectObject(m_hDC,hOldpen);
		
	}
	else if(m_PolyGonLine.size()>2)
	{
		HPEN hpen = CreatePen(PS_SOLID,3,RGB(255,0,0));
		HPEN hOldpen = (HPEN)SelectObject(m_hDC,hpen);

		int i= 0;
		for(i=0;i<m_PolyGonLine.size()-1;i++)
		{
			MoveToEx(m_hDC,m_PolyGonLine[i].x,m_PolyGonLine[i].y,NULL);
			LineTo(m_hDC,m_PolyGonLine[i+1].x,m_PolyGonLine[i+1].y);
		}
		MoveToEx(m_hDC,m_PolyGonLine[0].x,m_PolyGonLine[0].y,NULL);
		LineTo(m_hDC,m_PolyGonLine[i].x,m_PolyGonLine[i].y);
		SelectObject(m_hDC,hOldpen);

	}

			//涂画布
		cout<<"刷新画布"<<endl;
		RECT mrect;
		HBRUSH mBr;
		mBr = CreateSolidBrush(RGB(128,128,128));
		mrect.right = m_hDC_DCWidth;mrect.bottom = m_hDC_DCHeight;
		FillRect(m_hDC_DepthCurve,&mrect,mBr);
		int LeftBottomMargin = 50;
		int RightUpMargin = 25;
		m_RectCurve.left = LeftBottomMargin;
		m_RectCurve.top = RightUpMargin;
		m_RectCurve.right = m_hDC_DCWidth-RightUpMargin;
		m_RectCurve.bottom = m_hDC_DCHeight-LeftBottomMargin;
		//画曲线显示区域
		Rectangle(m_hDC_DepthCurve,m_RectCurve.left-1,m_RectCurve.top-1,m_RectCurve.right+1,m_RectCurve.bottom+1);
		//填充曲线绘制区域为单色
		mBr = CreateSolidBrush(RGB(192,237,237));
		FillRect(m_hDC_DepthCurve,&m_RectCurve,mBr);

	if(m_SelectPtsCord.size()>0)
	{
		//在mrect内画网格，最少2个，最多
		cout<<"绘制网格"<<endl;
		HPEN hpenG = CreatePen(PS_DOT,1,RGB(0,0,0));
		HPEN hOldpen = (HPEN)SelectObject(m_hDC_DepthCurve,hpenG);
		for(int i=0;i<m_GridStart.size();i++)
		{
			MoveToEx(m_hDC_DepthCurve,m_GridStart[i].x,m_GridStart[i].y,NULL);
			LineTo(m_hDC_DepthCurve,m_GridEnd[i].x,m_GridEnd[i].y);
			char tex[50];
			sprintf(tex,"%.2f",m_GridCord[i].y);
			if(m_GridCord[i].x>0)
			{
				int len = strlen(tex);
				//TextOut(m_hDC_DepthCurve,m_GridStart[i].x,m_GridStart[i].y+3,tex,len);
				TextOutA(m_hDC_DepthCurve,m_GridStart[i].x,m_GridStart[i].y+3,tex,len);

			}
			else
			{
				int len = strlen(tex);
				TextOutA(m_hDC_DepthCurve,m_GridStart[i].x-LeftBottomMargin+1,m_GridStart[i].y-5,tex,len);
			}
		}
		
		//Graphics graphs(m_hDC_DepthCurve);
		cout<<"绘制曲线"<<endl;
		HPEN hpenC = CreatePen(PS_SOLID,1,RGB(255,0,0));
		SelectObject(m_hDC_DepthCurve,hpenC);
		for(int i=0;i<m_SelectPtsCordXY.size()-1;i++)
		{
			CPoint startX;
			CPoint endX;

			if(ConvertXY2Pixel(m_SelectPtsCordXY[i].x,m_SelectPtsCordXY[i].y,startX)>0 && ConvertXY2Pixel(m_SelectPtsCordXY[i+1].x,m_SelectPtsCordXY[i+1].y,endX)>0)
			{
				MoveToEx(m_hDC_DepthCurve,startX.x,startX.y,NULL);
				LineTo(m_hDC_DepthCurve,endX.x,endX.y);
			}

		}
		SelectObject(m_hDC_DepthCurve,hOldpen);
	}
}
	
	return m_iShowType;
}

/*
(1)函数功能 朱：将屏幕坐标转换为OPengl三维坐标
(2)作者 朱浩然
(3)日期 2015-9-20
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWS3DShow::Screen2World(int x, int y, double &outx, double &outy, double &outz)  
{
	int RetVal = -1;
	m_OpenGL.SetRc();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(m_WSPose.m_fXShift,m_WSPose.m_fYShift,0);//平移
	glScalef(m_WSPose.m_fViewScale, m_WSPose.m_fViewScale, m_WSPose.m_fViewScale);					// Move left 1.5 units and into the screen 6.0

	//glPushMatrix();
	//glTranslatef(m_lCOR_x, m_lCOR_y, m_lCOR_z);//旋转中心，先移动
	//glMultMatrixf(m_WSPose.m_Transform.M);
	//glTranslatef(-m_lCOR_x, -m_lCOR_y, -m_lCOR_z);//旋转中心，后还原

	GLint viewport[4];  
	GLdouble modelview[16];  
	GLdouble projection[16];
	GLfloat winX, winY, winZ;  
// 	GLdouble posX, posY, posZ;  
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);  
	glGetDoublev(GL_PROJECTION_MATRIX, projection);  
	glGetIntegerv(GL_VIEWPORT, viewport);  
	winX = (float)x;  
	winY = (float)viewport[3] - (float)y;  
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);  
	if(winZ==1)
	{
		outx = 0;outy = 0;outz = 0;RetVal = -1;
	}
	else
	{
		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &outx, &outy, &outz);
		RetVal = 1;
	}
	m_OpenGL.ClearRc();
	return RetVal;  
} 

/*
(1)函数功能 朱：将OPengl三维坐标换为屏幕坐标转
(2)作者 朱浩然
(3)日期 2015-9-24
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWS3DShow::World2Screen(double objx, //in 三维坐标
							double objy, //in 三维坐标
							double objz, //in 三维坐标
							double &winx, //out 屏幕坐标
							double &winy, //out 屏幕坐标
							double &winz)//out 屏幕坐标
{
	m_OpenGL.SetRc();
	//设置平截头体大小，和位置
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, m_OpenGL.m_dEyePosition_z, 
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);//设置照相机位置

	glScalef(m_WSPose.m_fViewScale, m_WSPose.m_fViewScale, m_WSPose.m_fViewScale);// Move left 1.5 units and into the screen 6.0
	glPushMatrix();
	//glTranslatef(m_lCOR_x, m_lCOR_y, m_lCOR_z);//旋转中心，先移动
	glMultMatrixf(m_WSPose.m_Transform.M);
	//glTranslatef(-m_lCOR_x, -m_lCOR_y, -m_lCOR_z);//旋转中心，后还原


	GLint viewport[4];  
	GLdouble modelview[16];  
	GLdouble projection[16];
  
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);  
	glGetDoublev(GL_PROJECTION_MATRIX, projection);  
	glGetIntegerv(GL_VIEWPORT, viewport);  
	gluProject(objx, objy, objz, modelview, projection, viewport, &winx, &winy, &winz);  	

// 	winx = (int)winx;  
	winy = viewport[3] - winy;  

	m_OpenGL.ClearRc();
	return 1;  
} 


//设置显示点云时缩放系数，以点云坐标为中心
int  CWS3DShow::SetRCViewScale(int ifRCViewScale)
{
	//cout<<"SetRCViewScale called"<<endl;
	return m_WSPose.SetViewScale(ifRCViewScale);
}

//设置显示点云时缩放系数，以点云坐标为中心。x，y为距离点云坐标的屏幕距离。
int  CWS3DShow::SetRCViewScale(int ifRCViewScale, long lScreen_x, long lScreen_y)
{
	m_WSPose.SetViewScale(ifRCViewScale);
	return 1;
}


//设置默认姿态
int  CWS3DShow::SetDefaultPose()
{
	m_lScreen_x = 0.0f;
	m_lScreen_y = 0.0f;
	//m_OpenGL.m_dVisualSceneW = m_OpenGL.m_iWidth * 0.4;
	//m_OpenGL.m_dVisualSceneH = m_OpenGL.m_iHeight * 0.4;
	m_OpenGL.m_dOrigin_x = 0.0f;
	m_OpenGL.m_dOrigin_y = 0.0f;
	m_OpenGL.m_dEyePosition_z = 1000.0f;
	m_WSPose.SetDefPose();
	return 1;
}


//设置当前旋转为默认状态
int  CWS3DShow::SetCurPose2Default()
{
	return 1;
}

//设置点云显示颜色
int  CWS3DShow::SetDisplayColor(int red,int green,int blue)
{
	m_r=red;
	m_g=green;
	m_b=blue;
	return 1;
}
//设置显示点云时平移系数，
int  CWS3DShow::AddRCXYShift(float ifxShift,float ifyShift)
{
	//屏幕移动和视景体比例不一致，
	m_OpenGL.m_dOrigin_x += ifxShift * m_OpenGL.m_dVisualSceneW/(GLfloat)m_OpenGL.m_iWidth;
	m_OpenGL.m_dOrigin_y += ifyShift * m_OpenGL.m_dVisualSceneW/(GLfloat)m_OpenGL.m_iWidth;
    m_WSPose.AddXYShift(ifxShift,ifyShift);
	//m_OpenGL.SetFrustums(m_OpenGL.m_dVisualSceneW, m_OpenGL.m_dVisualSceneH, -m_OpenGL.m_dOrigin_x, -m_OpenGL.m_dOrigin_y);
	RenderScene();
	return 1; 
}

//设置点云旋转first click
int  CWS3DShow::SetRCRotFirClick(float ifptx,float ifpty)
{
	return m_WSPose.SetRotFirClick(ifptx,ifpty);
}

//设置点云旋转时鼠标拖动量
int  CWS3DShow::RCRotMouseDragging(float ifptx,float ifpty)
{
	m_WSPose.RotMouseDragging(ifptx,ifpty);
	RenderScene();
	return 1;
}

/*
(1)函数功能 设置旋转中心，并且解决设置选中中心后跳动的问题
(2)作者 朱浩然
(3)日期 2015-9-24
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int  CWS3DShow::SetCOR(double x, double y, double z)
{
	//1,获得选中点变换旋转中心之前的屏幕坐标
	double dOldWinx =0.0f;
	double dOldWiny =0.0f;
	double dOldWinz =0.0f;
	World2Screen(x, y, z, dOldWinx, dOldWiny, dOldWinz);
	
	//2,获得选中点在改变旋转中心后的屏幕坐标
	double dNewWinx =0.0f;
	double dNewWiny =0.0f;
	double dNewWinz =0.0f;
			//更新旋转中心
	m_lCOR_x = x;
	m_lCOR_y = y;
	m_lCOR_z = z;
	World2Screen(x, y, z, dNewWinx, dNewWiny, dNewWinz);

	//3,求取两次旋转差值（转换到世界坐标系）
	double dShiftx =0.0f;
	double dShifty =0.0f;
	double dShiftz =0.0f;

	dShiftx = (dNewWinx - dOldWinx) * m_OpenGL.m_dVisualSceneW / (double)m_OpenGL.m_iWidth;
	dShifty = (dNewWiny - dOldWiny) * m_OpenGL.m_dVisualSceneW / (double)m_OpenGL.m_iWidth;
	dShiftz = (dNewWinz - dOldWinz) * m_OpenGL.m_dVisualSceneW / (double)m_OpenGL.m_iWidth;

	//4,修正累计偏移量，因为是正交变换 z值可以不用考虑
// 	m_WSPose.m_fXShift = m_WSPose.m_fXShift - dShiftx;
// 	m_WSPose.m_fYShift = m_WSPose.m_fYShift + dShifty;
// 
// 	m_OpenGL.m_dOrigin_x += dShiftx;
// 	m_OpenGL.m_dOrigin_y += dShifty;
// 
// 	m_OpenGL.SetFrustums(m_OpenGL.m_dVisualSceneW, 
// 						m_OpenGL.m_dVisualSceneH,
// 						-m_OpenGL.m_dOrigin_x,
// 						-m_OpenGL.m_dOrigin_y);	// 默认是perspective
	AddRCXYShift(-(dNewWinx - dOldWinx), (dNewWiny - dOldWiny));


	return 1;
}


/*
(1)函数功能 获得旋转中心
(2)作者 朱浩然
(3)日期 2015-9-24
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int  CWS3DShow::GetCOR(double &x, double &y, double &z) const
{
	x = m_lCOR_x ;
	y = m_lCOR_y ;
	z = m_lCOR_z ;
	return 1;
}

/*
(1)函数功能 将模型重心移动到屏幕中心
(2)作者 朱浩然
(3)日期 2015-9-24
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWS3DShow::Trans2ScreenCenter(
	float dCenx, //in
	float dCeny) //in
{
// 	m_WSPose.m_fXShift = -(dCenx * m_OpenGL.m_iWidth / m_OpenGL.m_dVisualSceneW);
// 	m_WSPose.m_fYShift = -(dCeny * m_OpenGL.m_iWidth / m_OpenGL.m_dVisualSceneW);

	m_OpenGL.m_dOrigin_x += (dCenx * m_OpenGL.m_iWidth / m_OpenGL.m_dVisualSceneW);
	m_OpenGL.m_dOrigin_y += (dCeny * m_OpenGL.m_iWidth / m_OpenGL.m_dVisualSceneW);
	return 1;
}


//
/*
(1)函数功能 更新多边形曲线列表，列表不为空则会调用多边形显示，1点不显示，2点显示线，多点显示多边形，首位相接
(2)作者 jhl
(3)日期 20160831
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWS3DShow::SetPolygonVertices(vector<CPoint>*pVertices)
{
	if(NULL==pVertices)
		return -1;
	if(pVertices->size()<2)
		return -1;
	m_PolyGonLine.resize(pVertices->size());
	for(int i=0;i<m_PolyGonLine.size();i++)
	{
		m_PolyGonLine[i] = (*pVertices)[i];
	}

	if(m_PolyGonLine.size()==2)
	{
		float shiftX = m_PolyGonLine[1].x - m_PolyGonLine[0].x;
		float shiftY = m_PolyGonLine[1].y - m_PolyGonLine[0].y;
		int pixOnLine = sqrt(shiftX*shiftX+shiftY*shiftY);
		if(pixOnLine>5)
		{
			cout<<"提取所有三维点"<<endl;
			//得到所有待画线三维点
			shiftX = shiftX/float(pixOnLine);
			shiftY = shiftY/float(pixOnLine);
			m_SelectPtsCord.clear();
			m_SelectPtsCord.resize(pixOnLine);
			m_SelectPtsCordXY.resize(pixOnLine);
			vector<Point2f>points;
			for(int i=0;i<pixOnLine;i++)
			{
				double ptx,pty,ptz;
				Point2f pt;
				int corx = m_PolyGonLine[0].x + shiftX*i;
				int cory = m_PolyGonLine[0].y + shiftY*i;
				int retVal = Screen2World(corx,cory,ptx,pty,ptz);
				m_SelectPtsCord[i].s.X = ptx;
				m_SelectPtsCord[i].s.Y = pty;
				m_SelectPtsCord[i].s.Z = ptz;
				m_SelectPtsCord[i].s.W = retVal;
				if(retVal>0)
				{
					pt.x = m_SelectPtsCord[i].s.X;
					pt.y = m_SelectPtsCord[i].s.Y;
					points.push_back(pt);
				}
			}

			//转化到平面xy坐标系，求取直线方向向量，然后计算与x轴夹角，接着对x、y平面坐标系进行旋转，得到旋转后的x坐标，考虑平行投影，直接将x坐标作为变换后x坐标，y轴为0
			Vec4f line;
			//cv::fitLine(Mat(points),line,CV_DIST_L1,0,0.01,0.01);
			float sintheta = line[1]/(line[0]*line[0]+line[1]*line[1]);
			float costheta = line[0]/(line[0]*line[0]+line[1]*line[1]);
			for(int i=0;i<m_SelectPtsCord.size();i++)
			{
				if(m_SelectPtsCord[i].s.W>0)
				{
					m_SelectPtsCordXY[i].x = costheta*m_SelectPtsCord[i].s.X+sintheta*m_SelectPtsCord[i].s.Y;
					m_SelectPtsCordXY[i].y = m_SelectPtsCord[i].s.Z;
				}
				else
				{
					m_SelectPtsCordXY[i].x = 0;
					m_SelectPtsCordXY[i].y = 0;
				}
			}

			//获取xy范围，初始化画板,确定xy坐标范围
			m_RangeX0 = 999999999;
			m_RangeX1 = -99999999;
			m_RangeY0 = -99999999;
			m_RangeY1 = 999999999;
			for(int i=0;i<m_SelectPtsCordXY.size();i++)
			{
				if(m_SelectPtsCord[i].s.W>0)
				{
					if(m_SelectPtsCordXY[i].y>m_RangeY0)
						m_RangeY0 = m_SelectPtsCordXY[i].y;
					if(m_SelectPtsCordXY[i].y<m_RangeY1)
						m_RangeY1 = m_SelectPtsCordXY[i].y;

					if(m_SelectPtsCordXY[i].x<m_RangeX0)
						m_RangeX0 = m_SelectPtsCordXY[i].x;
					if(m_SelectPtsCordXY[i].x>m_RangeX1)
						m_RangeX1 = m_SelectPtsCordXY[i].x;
				}
			}
			//初始化网格
			cout<<"初始化网格"<<endl;
			int YGrids = 5;
			int XGrids = YGrids*(m_hDC_DCWidth/m_hDC_DCHeight);
			m_GridStart.resize(YGrids+XGrids);
			m_GridEnd.resize(YGrids+XGrids);
			float RangeY = m_RangeY0-m_RangeY1;
			float RangeX = m_RangeX1-m_RangeX0;

			float YGridGap = RangeY/YGrids;
			float XGridGap = RangeX/XGrids;
			int SignificantDigitsYNum = 0;
			int SignificantDigitsY = 0;
			if(YGridGap>=1)
			{
				int Grid = fabs(YGridGap);
				while(Grid>=1)
				{
					SignificantDigitsY = Grid;
					SignificantDigitsYNum++;
					Grid = Grid/10;
				}
			}
			else
			{
				float Grid = fabs(YGridGap);
				while(Grid<1)
				{
					SignificantDigitsY = Grid;
					SignificantDigitsYNum--;
					Grid = Grid*10;
				}
			}

			int SignificantDigitsX = 0;
			int SignificantDigitsXNum = 0;
			if(XGridGap>=1)
			{
				int Grid = fabs(XGridGap);
				while(Grid>=1)
				{
					SignificantDigitsX = Grid;
					SignificantDigitsXNum++;
					Grid = Grid/10;
				}
			}
			else
			{
				float Grid = fabs(XGridGap);
				while(Grid<1)
				{
					SignificantDigitsX = Grid;
					SignificantDigitsXNum--;
					Grid = Grid*10;
				}
			}

			
			vector<float>xG;
			vector<float>yG;
			if(SignificantDigitsYNum>0)
			{
				YGridGap = SignificantDigitsY*pow(10,SignificantDigitsYNum-1);
			}
			if(SignificantDigitsXNum)
			{
				XGridGap = SignificantDigitsX*pow(10,SignificantDigitsXNum-1);
			}

			int temp = m_RangeY1/YGridGap-1;
			float ystart = temp*YGridGap;
			while(ystart<m_RangeY0)
			{
				if(ystart>m_RangeY1)
					yG.push_back(ystart);
				ystart = ystart + YGridGap;
			}

			temp = m_RangeX0/XGridGap-1;
			float xstart = temp*XGridGap;
			while(xstart<m_RangeX1)
			{
				if(xstart>m_RangeX0)
					xG.push_back(xstart);
				xstart = xstart + XGridGap;
			}
			cout<<"计算网格点的绘制坐标"<<endl;
			m_GridStart.clear();
			m_GridEnd.clear();
			m_GridCord.clear();
			for(int i=0;i<yG.size();i++)
			{
				CPoint temp;
				ConvertXY2Pixel(m_RangeX0,yG[i],temp);
				m_GridStart.push_back(temp);
				ConvertXY2Pixel(m_RangeX1,yG[i],temp);
				m_GridEnd.push_back(temp);
				cv::Point2f tempf;
				tempf.x = 0;
				tempf.y = yG[i];
				m_GridCord.push_back(tempf);

			}

			for(int i=0;i<xG.size();i++)
			{
				CPoint temp;
				ConvertXY2Pixel(xG[i],m_RangeY1,temp);
				m_GridStart.push_back(temp);
				ConvertXY2Pixel(xG[i],m_RangeY0,temp);
				m_GridEnd.push_back(temp);
				cv::Point2f tempf;
				tempf.x = 1;
				tempf.y = xG[i];
				m_GridCord.push_back(tempf);
			}
		}
	}

}

//将曲线坐标转换为像素坐标
int CWS3DShow::ConvertXY2Pixel(float x,float y,CPoint&pixelCor)
{
	pixelCor.y = (m_RangeY0-y)/(m_RangeY0-m_RangeY1)*(m_RectCurve.bottom-m_RectCurve.top)+m_RectCurve.top;
	if(pixelCor.y>m_RectCurve.bottom || pixelCor.y<m_RectCurve.top)
		return -1;

	pixelCor.x = (x - m_RangeX0)/(m_RangeX1-m_RangeX0)*(m_RectCurve.right-m_RectCurve.left)+m_RectCurve.left;
	if(pixelCor.x<m_RectCurve.left || pixelCor.x>m_RectCurve.right)
		return -1;

	return 1;
}

/*
(1)函数功能 清空多边形曲线顶点列表
(2)作者 jhl
(3)日期 20160831
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWS3DShow::ClearPolygonVertices()
{
	m_PolyGonLine.clear();
	return 0;
}

/*
(1)函数功能 设置深度曲线绘制句柄信息
(2)作者 jhl
(3)日期 20160831
(4)修改人
(5)修改日期
(6)修改内容
(7)特殊说明 无
*/
int CWS3DShow::SetDepthCurveDC(HDC hDC, int width, int height)
{
	m_hDC_DepthCurve = hDC;
	m_hDC_DCHeight = height;
	m_hDC_DCWidth = width;
	return 0;
}
//放大
int CWS3DShow::ZoomIn()
{

	return 0;
}

int CWS3DShow::ZoomOut()
{

	return 0;
}

int CWS3DShow::AddCurveViewXYshift(int xShift,int yShift)
{

	return 0;
}
