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
//��������

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


int CWSOpenGL::InitOpenGL(HDC dc, int iWidth, int iHeight) //�˴���ʼ��OpenGL������������
{
	if (dc == NULL){
		return -1;
	}
	m_hDC = dc;
	m_dNearPlane   = 5; 
	m_dFarPlane    = 10000.0; 
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	// 1 �������ظ�ʽ  [6/6/2012 Administrator]
	MySetPixelFormat(dc,32,32);

	// 2 ���� RC
	if (!(m_hRC=wglCreateContext(dc))) // 
	{
		return -1;
	}
	// 3 ȫ����ʾ����
	SetRc();
	SpecialEffect();

	// 4 ����ͶӰ���ӿ�
	if (ORTHO == m_iProjectionType)
	{
		SetProjection(iWidth,iHeight,ORTHO);	// Ĭ����perspective
	}
	else
	{
		SetProjection(iWidth,iHeight,PERSPECTIVE);	// Ĭ����perspective
	}

	ClearRc();

	return 0;
}


//
int CWSOpenGL::MySetPixelFormat(HDC hdc,int ColorBits,int DepthBits)
{   
	GLuint PixelFormat;//�������ƥ������ظ�ʽ���

	PIXELFORMATDESCRIPTOR pfd= // /pfd ���ߴ���������ϣ���Ķ�����������ʹ�õ����ظ�ʽ
	{
		sizeof(PIXELFORMATDESCRIPTOR), // ������ʽ�������Ĵ�С,
		1, // �汾��
		PFD_DRAW_TO_WINDOW | // ��ʽ֧�ִ���
		PFD_SUPPORT_OPENGL | // ��ʽ����֧��OpenGL
		PFD_DOUBLEBUFFER, // ����֧��˫����
		PFD_TYPE_RGBA, // ���� RGBA ��ʽ
		ColorBits, // ѡ��ɫ�����
		0, 0, 0, 0, 0, 0, // ���Ե�ɫ��λ
		0, // ��Alpha����
		0, // ����Shift Bit
		8, // ���ۼӻ���
		0, 0, 0, 0, // ���Ծۼ�λ
		DepthBits, // 16λ Z-���� (��Ȼ���)
		0, // ���ɰ建��
		0, // �޸�������
		PFD_MAIN_PLANE, // ����ͼ��
		0, // Reserved
		0, 0, 0 // ���Բ�����
	};

	if (!(PixelFormat=ChoosePixelFormat(hdc,&pfd))) // Windows �ҵ���Ӧ�����ظ�ʽ����?
	{
		return -1; // ���� FALSE
	}
	if(!SetPixelFormat(hdc,PixelFormat,&pfd)) // �ܹ��������ظ�ʽô?
	{
		return -1; // ���� FALSE
	}

	return 0;
}

//����RC
int CWSOpenGL::SetRc()
{
	if (!wglMakeCurrent(m_hDC,m_hRC))
	{
		return -1;
	}
	return 0;
}

//����RC
int CWSOpenGL::SetRc(HDC hDC, HGLRC hRC)
{
	if (!wglMakeCurrent(hDC,hRC))
	{
		return -1;
	}
	return 0;

}



//���RC
int CWSOpenGL::ClearRc()
{
	wglMakeCurrent(NULL,NULL);
	return 0;
}


int CWSOpenGL::SpecialEffect()
{
	glShadeModel(GL_SMOOTH);//������Ӱƽ��
	glClearDepth(1.0f);//������Ȼ���

	glEnable(GL_DEPTH_TEST);//������Ȳ���
	glDepthFunc(GL_LEQUAL);//������Ȳ��Ե�����

	glEnable(GL_POINT_SMOOTH);//����ƽ��	

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);//����ϵͳ��͸�ӽ�������

	return 0;
}

/***********************************************************

ͶӰ���ӿ����� [6/6/2012 Administrator]

***********************************************************/
int CWSOpenGL::SetProjection(int iWidth, int iHeight, ProType pers)
{
	m_dVisualSceneW = iWidth;
	m_dVisualSceneH = iHeight;
	// 1 ͶӰ
	if (pers == PERSPECTIVE) 
	{
// 		GLfloat fAspect;
// 		if (iHeight > 0)
// 			fAspect = (GLfloat)m_dVisualSceneW / m_dVisualSceneH;
// 		else
// 			fAspect = 1.0f;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//����ƽ��ͷ���С����λ��
//  		double fovy = 0.0;//��Ұ�Ƕ�
//  		fovy = (180/PI) * 2 * atan((GLfloat)iHeight/2000);
//  		gluPerspective(30.0f, fAspect, 0.1f, 100000.0f);
		glFrustum((-(GLfloat)m_dVisualSceneW / 2 / m_dEyePosition_z) * 1.0f, ((GLfloat)m_dVisualSceneW / 2 / m_dEyePosition_z) * 1.0f,
			(-(GLfloat)m_dVisualSceneH / 2 / m_dEyePosition_z) * 1.0f, ((GLfloat)m_dVisualSceneH /2 / m_dEyePosition_z) * 1.0f, 
			1.0f, 100000.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0, 0.0, m_dEyePosition_z, 
					0.0, 0.0, 0.0,
					0.0, 1.0, 0.0);//���������λ��
	}else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
// 		glOrtho(-750.0, 750.0, -750.0*(GLfloat)iHeight/(GLfloat)iWidth, 750.0*(GLfloat)iHeight/(GLfloat)iWidth, 0.0, 100000.0);
		glOrtho(-0.5 * (GLfloat)m_dVisualSceneW, 0.5 * (GLfloat)m_dVisualSceneW, 
				-0.5 * (GLfloat)m_dVisualSceneH, 0.5 * (GLfloat)m_dVisualSceneH, 
				0.1, 100000.0);

		glMatrixMode(GL_MODELVIEW);		
	}
	// 2 �ӿ�
	glViewport(0, 0, iWidth, iHeight);

	return 0;
}

/*
(1)�������� ����ƽ��ͷ��,ͬʱ����͸��ͶӰ������ͶӰ��ƽ��ͷ�壬�����Ź���
(2)���� ���Ȼ
(3)���� 2015-11-10
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
int CWSOpenGL::SetFrustums(double dWidth, //in,ƽ��ͷ��Ŀ��
						   double dHeight)//in,ƽ��ͷ��ĸ߶�
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
		GLfloat fovy;//��Ұ�Ƕ�
		fovy = (180/PI) * 2 * atan(static_cast<GLfloat>(dHeight)/2000);
		gluPerspective(fovy, (GLfloat)dWidth / dHeight, 0.1f, 100000.0f);
	}

	glMatrixMode(GL_MODELVIEW);		
	ClearRc();
	return 1;
}

/*
(1)�������� �����أ�����ƽ��ͷ��,ͬʱ����͸��ͶӰ������ͶӰ��ƽ��ͷ�壬�����Ź��ܡ����Ӹı�ƽ��ͷ��������λ�õĹ���
(2)���� ���Ȼ
(3)���� 2015-11-10
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
int CWSOpenGL::SetFrustums(double dWidth, //in,ƽ��ͷ��Ŀ��(��z=0��ƽ��)
						   double dHeight,//in,ƽ��ͷ��ĸ߶�(��z=0��ƽ��)
						   double dOrigin_x, //in,ƽ��ͷ�������λ�á���x
						   double dOrigin_y)//in,ƽ��ͷ�������λ�á���y
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
// 		GLfloat fovy = 0.0;//��Ұ�Ƕ�
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
(1)�������� ����OpenGL���ڴ�С
(2)���� �κ���
(3)���� 2014-7-1
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
void CWS3DShow::ReSizeGLScene(GLsizei iwidth,GLsizei iheight) //����OpenGL���ڴ�С
{
	//
	cout<<"ReSizeGLScene called"<<endl;
	m_OpenGL.m_iWidth = iwidth;
	m_OpenGL.m_iHeight = iheight;
	if(m_OpenGL.m_iHeight==0) //��ֹ������,��Height��Ϊ1
	{
		m_OpenGL.m_iHeight =1;
	}	

	//���ñ߽�
	m_WSPose.m_ArcBall.setBounds(iwidth, iheight);
	// ����ͶӰ���ӿ�
	m_OpenGL.SetRc();
	if (PERSPECTIVE == m_OpenGL.m_iProjectionType)
	{
		m_OpenGL.SetProjection(m_OpenGL.m_iWidth, m_OpenGL.m_iHeight,PERSPECTIVE);	// Ĭ����perspective
	}
	else
	{
		m_OpenGL.SetProjection(m_OpenGL.m_iWidth, m_OpenGL.m_iHeight,ORTHO);	// Ĭ����perspective
	}

	m_OpenGL.ClearRc();

// 	glMatrixMode(GL_PROJECTION); //ѡ��ͶӰ����
// 	glLoadIdentity();//����ͶӰ����
	//������ͶӰ�ռ�
// 	glOrtho(-750.0,750.0, -750.0*(GLfloat)m_OpenGL.m_iHeight/(GLfloat)m_OpenGL.m_iWidth, 750.0*(GLfloat)m_OpenGL.m_iHeight/(GLfloat)m_OpenGL.m_iWidth,-100000000.0,100000000.0);
	// 	glOrtho(-(GLfloat)m_OpenGL.m_iWidth/2, (GLfloat)m_OpenGL.m_iWidth/2, -(GLfloat)m_OpenGL.m_iHeight/2, (GLfloat)m_OpenGL.m_iHeight/2, -10000.0, 10000.0);//�Ӿ���ʹ���һ����

// 	glMatrixMode(GL_MODELVIEW);//ѡ��ģ�͹۲����
// 	glLoadIdentity();

}


/*
(1)�������� ����HDC��ǰ��ģ�
(2)���� �κ���
(3)���� 2014-7-1
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
void CWSOpenGL::SwapOpenGLBuffers()
{
	SwapBuffers(m_hDC);
}


//һ����drawing��ķ���CDrawing::CDrawing(void)
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
(1)�������� ��������
(2)���� �κ���
(3)���� 2014-7-1
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
int CWSDrawing::GetCloudCount() const
{
	return m_iCloudCount;
}

/*
(1)�������� ��������
(2)���� �κ���
(3)���� 2014-7-1
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
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
(1)�������� ���������
(2)���� �κ���
(3)���� 2014-7-1
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
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
(1)�������� ɾ��ѡ�е���
(2)���� ���Ȼ
(3)���� 2015-9-5
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
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
(1)�������� Ӱ��ѡ�е���
(2)���� ���Ȼ
(3)���� 2015-9-5
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
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
(1)�������� ��ʾѡ�е���
(2)���� ���Ȼ
(3)���� 2015-9-5
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
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
(1)�������� ɾ�����е���
(2)���� ���Ȼ
(3)���� 2015-9-5
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
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
(1)�������� ������
(2)���� �κ���
(3)���� 2014-7-1
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
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
	else if(texture_or_cloud==1) // ����
	{
		glColor3f(1,1,1);
		glDisable(GL_TEXTURE_2D);
		CWSPoint      *pPoint = p2currentPC->GetPoint();
		glPointSize(1.5);//�죺���õ�Ĵ�С��������һ����ӳ�䵽��Ļʱ����4�����ء�
		glBegin(GL_POINTS);
		for(int i=0;i<p2currentPC->GetHeader()->m_iVerCount;++i)
		{
			glVertex3f(pPoint[i].m_fx, -pPoint[i].m_fy, -pPoint[i].m_fz);
		}
		glEnd(); // �����λ��ƽ���
		
	}

	else if(texture_or_cloud==2)//ply,3dm,ÿ����һ����ɫ����������ɫ����ͬ
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
	//������ͼ
	glTexImage2D(GL_TEXTURE_2D, 0, 3, iTexWidth, iTexHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pWSTexRGB);
	//���ò���
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//����ͼ
	glBindTexture(GL_TEXTURE_2D,texture);
	return 0;
}




//CWSPose��

CWSPose::CWSPose()
{
	SetTransform();
	SetLastRot();
	SethisRot();
	ArcBallT    ArcBall(640.0f, 480.0f);
	m_ArcBall = ArcBall;         // NEW: ArcBall Instance

	//����
	m_fViewScale = 1.0f;
	//ƽ��
	m_fXShift = 0.0f;
	m_fYShift = 0.0f;

}

CWSPose::~CWSPose()
{

}

//��ʼ��SetMatrix4fT����
int CWSPose::SetTransform()
{
	Matrix4fT Transform   = {  1.0f,  0.0f,  0.0f,  0.0f,				// NEW: Final Transform
		0.0f,  1.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  1.0f,  0.0f,
		0.0f,  0.0f,  0.0f,  1.0f };
	m_Transform = Transform;
	return 0;
}

//��ʼ��SetMatrix3fT����
int CWSPose::SetLastRot()
{
	Matrix3fT LastRot     = {  1.0f,  0.0f,  0.0f,					// NEW: Last Rotation
		0.0f,  1.0f,  0.0f,
		0.0f,  0.0f,  1.0f };
	m_LastRot = LastRot;
	return 0;
}

//��ʼ��SetMatrix3fT����
int CWSPose::SethisRot()
{

	Matrix3fT ThisRot = {  1.0f,  0.0f,  0.0f,					// NEW: This Rotation
		0.0f,  1.0f,  0.0f,
		0.0f,  0.0f,  1.0f };
	m_ThisRot = ThisRot;
	return 0;
}


/*
(1)�������� �������
(2)���� �κ���
(3)���� 2014-7-1
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
void CWSPose::LeftButnDown(int MouseX,int MouseY)
{
	m_LastRot = m_ThisRot;										// Set Last Static Rotation To Last Dynamic One
	m_MousePt.s.X = MouseX;
	m_MousePt.s.Y = MouseY;
	m_ArcBall.click(&m_MousePt);								// Update Start Vector And Prepare For Dragging
}

/*
(1)�������� ����϶�
(2)���� �κ���
(3)���� 2014-7-1
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
int CWSPose::RotMouseDragging(int MouseX,int MouseY)
{
	m_MousePt.s.X = MouseX;
	m_MousePt.s.Y = MouseY;
	Quat4fT     ThisQuat;
	Matrix3fT curRot;

	m_ArcBall.drag(&m_MousePt, &ThisQuat);						// Update End Vector And Get Rotation As Quaternion
	Matrix3fSetRotationFromQuat4f(&m_ThisRot, &ThisQuat);		// Convert Quaternion Into Matrix3fT  ������ת��

	curRot = m_ThisRot;
	Matrix3fMulMatrix3f(&curRot, &m_LastRot); // �ۼ���ת���
	Matrix4fSetRotationFromMatrix3f(&m_Transform,&curRot);//�õ����ǵ����ս��
	m_ThisRot = curRot;
	//pointerPC[0].Matrix4fMul(&curRot);
	//pointerPC[0].SetThisRot(&curRot); /*= curRot;*/
	Matrix4fSetRotationFromMatrix3f(&m_Transform, &m_ThisRot);	// Set Our Final Transform's Rotation From This One
	return 1;
}


//������ʾ����ʱ����ϵ����
int CWSPose::SetViewScale(int ifViewScale)
{
	if(ifViewScale>0)
		m_fViewScale = m_fViewScale*1.15f;
	else if(ifViewScale<0)
		m_fViewScale = m_fViewScale/1.15f;
	return 1;
}


//������ʾ����ʱ����ϵ����
int CWSPose::SetViewScale(int ifViewScale, long lScreen_x, long lScreen_y)
{
	if(ifViewScale>0)
		m_fViewScale = m_fViewScale*1.15f;
	else if(ifViewScale<0)
		m_fViewScale = m_fViewScale/1.15f;
	return 1;
}


//�ۻ�ƽ������
int CWSPose::AddXYShift(float ifxShift,float ifyShift)
{
	m_fXShift = m_fXShift + ifxShift;
	m_fYShift = m_fYShift + ifyShift;
	return 1;
}
//���õ�����תfirst click
int  CWSPose::SetRotFirClick(float ifptx,float ifpty)
{
	m_LastRot = m_ThisRot;										// Set Last Static Rotation To Last Dynamic One
	m_MousePt.s.X = ifptx;
	m_MousePt.s.Y = ifpty;
	m_ArcBall.click(&m_MousePt);								// Update Start Vector And Prepare For Dragging
	return 1;
}

//����Ĭ�ϵ�pose
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
//CWS3DShow��

//�������ָ��

CWS3DShow::CWS3DShow()
{
	m_hDC = NULL; 
	//��ʼ��Ĭ�ϵ�����ɫ
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

//��ʼ�� ����DC
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
(1)�������� ָ����Ⱦģʽ������Ⱦ 0����������Ⱦ��1�������ʾ��Ⱦ,
(2)���� ���Ȼ 17092845574
(3)���� 2015-10-17
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
int CWS3DShow::RenderScene(int type ) //RenderSceneList
{
	m_iShowType = type;
	return RenderScene();
	 
	////���õ�ǰ����
	//m_OpenGL.SetRc();
 //	glClearColor(0.0, 0.0, 0.0, 0.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer

	//glLoadIdentity();									// Reset the current modelview matrix
	//glTranslatef(m_WSPose.m_fXShift, m_WSPose.m_fYShift, 0.0);//ƽ��
	//glScalef(m_WSPose.m_fViewScale, m_WSPose.m_fViewScale, m_WSPose.m_fViewScale);					// Move left 1.5 units and into the screen 6.0

	//									// NEW: Apply Dynamic Transform
	////glPushMatrix();	
	////glTranslatef(m_lCOR_x, m_lCOR_y, m_lCOR_z);//��ת���ģ����ƶ�
	////glMultMatrixf(m_WSPose.m_Transform.M);
	////glTranslatef(-m_lCOR_x, -m_lCOR_y, -m_lCOR_z);//��ת���ģ���ԭ

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
	//// ��֤��������
	//m_OpenGL.ClearRc();
}


/*
(1)�������� ���أ��ó�Ա����ָ����Ⱦģʽ
(2)���� ���Ȼ 17092845574
(3)���� 2015-10-17
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ���ص�ǰ��ʾ����ֵm_iShowType
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
	//���õ�ǰ����
	//m_OpenGL.SetRc();
	// 	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer
	glLoadIdentity();									// Reset the current modelview matrix

	gluLookAt(0.0, 0.0, m_OpenGL.m_dEyePosition_z, 
				0.0, 0.0, 0.0,
				0.0, 1.0, 0.0);//���������λ��

 	glTranslatef(m_WSPose.m_fXShift, m_WSPose.m_fYShift, 0.0);//ƽ��
	glScalef(m_WSPose.m_fViewScale, m_WSPose.m_fViewScale, m_WSPose.m_fViewScale);					// Move left 1.5 units and into the screen 6.0

	// NEW: Apply Dynamic Transform
	glPushMatrix();	
	//glTranslatef(m_lCOR_x, m_lCOR_y, m_lCOR_z);//��ת���ģ����ƶ�
	glMultMatrixf(m_WSPose.m_Transform.M);
	//glTranslatef(-m_lCOR_x, -m_lCOR_y, -m_lCOR_z);//��ת���ģ���ԭ

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
	// ��֤��������
	m_OpenGL.ClearRc();

if(NULL!=m_hDC_DepthCurve)
{
	//��������
	if(m_PolyGonLine.size()==2)
	{
		cout<<"�ڵ�����ʾ�ռ������ѡ����"<<endl;
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

			//Ϳ����
		cout<<"ˢ�»���"<<endl;
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
		//��������ʾ����
		Rectangle(m_hDC_DepthCurve,m_RectCurve.left-1,m_RectCurve.top-1,m_RectCurve.right+1,m_RectCurve.bottom+1);
		//������߻�������Ϊ��ɫ
		mBr = CreateSolidBrush(RGB(192,237,237));
		FillRect(m_hDC_DepthCurve,&m_RectCurve,mBr);

	if(m_SelectPtsCord.size()>0)
	{
		//��mrect�ڻ���������2�������
		cout<<"��������"<<endl;
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
		cout<<"��������"<<endl;
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
(1)�������� �죺����Ļ����ת��ΪOPengl��ά����
(2)���� ���Ȼ
(3)���� 2015-9-20
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
int CWS3DShow::Screen2World(int x, int y, double &outx, double &outy, double &outz)  
{
	int RetVal = -1;
	m_OpenGL.SetRc();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(m_WSPose.m_fXShift,m_WSPose.m_fYShift,0);//ƽ��
	glScalef(m_WSPose.m_fViewScale, m_WSPose.m_fViewScale, m_WSPose.m_fViewScale);					// Move left 1.5 units and into the screen 6.0

	//glPushMatrix();
	//glTranslatef(m_lCOR_x, m_lCOR_y, m_lCOR_z);//��ת���ģ����ƶ�
	//glMultMatrixf(m_WSPose.m_Transform.M);
	//glTranslatef(-m_lCOR_x, -m_lCOR_y, -m_lCOR_z);//��ת���ģ���ԭ

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
(1)�������� �죺��OPengl��ά���껻Ϊ��Ļ����ת
(2)���� ���Ȼ
(3)���� 2015-9-24
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
int CWS3DShow::World2Screen(double objx, //in ��ά����
							double objy, //in ��ά����
							double objz, //in ��ά����
							double &winx, //out ��Ļ����
							double &winy, //out ��Ļ����
							double &winz)//out ��Ļ����
{
	m_OpenGL.SetRc();
	//����ƽ��ͷ���С����λ��
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, m_OpenGL.m_dEyePosition_z, 
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);//���������λ��

	glScalef(m_WSPose.m_fViewScale, m_WSPose.m_fViewScale, m_WSPose.m_fViewScale);// Move left 1.5 units and into the screen 6.0
	glPushMatrix();
	//glTranslatef(m_lCOR_x, m_lCOR_y, m_lCOR_z);//��ת���ģ����ƶ�
	glMultMatrixf(m_WSPose.m_Transform.M);
	//glTranslatef(-m_lCOR_x, -m_lCOR_y, -m_lCOR_z);//��ת���ģ���ԭ


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


//������ʾ����ʱ����ϵ�����Ե�������Ϊ����
int  CWS3DShow::SetRCViewScale(int ifRCViewScale)
{
	//cout<<"SetRCViewScale called"<<endl;
	return m_WSPose.SetViewScale(ifRCViewScale);
}

//������ʾ����ʱ����ϵ�����Ե�������Ϊ���ġ�x��yΪ��������������Ļ���롣
int  CWS3DShow::SetRCViewScale(int ifRCViewScale, long lScreen_x, long lScreen_y)
{
	m_WSPose.SetViewScale(ifRCViewScale);
	return 1;
}


//����Ĭ����̬
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


//���õ�ǰ��תΪĬ��״̬
int  CWS3DShow::SetCurPose2Default()
{
	return 1;
}

//���õ�����ʾ��ɫ
int  CWS3DShow::SetDisplayColor(int red,int green,int blue)
{
	m_r=red;
	m_g=green;
	m_b=blue;
	return 1;
}
//������ʾ����ʱƽ��ϵ����
int  CWS3DShow::AddRCXYShift(float ifxShift,float ifyShift)
{
	//��Ļ�ƶ����Ӿ��������һ�£�
	m_OpenGL.m_dOrigin_x += ifxShift * m_OpenGL.m_dVisualSceneW/(GLfloat)m_OpenGL.m_iWidth;
	m_OpenGL.m_dOrigin_y += ifyShift * m_OpenGL.m_dVisualSceneW/(GLfloat)m_OpenGL.m_iWidth;
    m_WSPose.AddXYShift(ifxShift,ifyShift);
	//m_OpenGL.SetFrustums(m_OpenGL.m_dVisualSceneW, m_OpenGL.m_dVisualSceneH, -m_OpenGL.m_dOrigin_x, -m_OpenGL.m_dOrigin_y);
	RenderScene();
	return 1; 
}

//���õ�����תfirst click
int  CWS3DShow::SetRCRotFirClick(float ifptx,float ifpty)
{
	return m_WSPose.SetRotFirClick(ifptx,ifpty);
}

//���õ�����תʱ����϶���
int  CWS3DShow::RCRotMouseDragging(float ifptx,float ifpty)
{
	m_WSPose.RotMouseDragging(ifptx,ifpty);
	RenderScene();
	return 1;
}

/*
(1)�������� ������ת���ģ����ҽ������ѡ�����ĺ�����������
(2)���� ���Ȼ
(3)���� 2015-9-24
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
int  CWS3DShow::SetCOR(double x, double y, double z)
{
	//1,���ѡ�е�任��ת����֮ǰ����Ļ����
	double dOldWinx =0.0f;
	double dOldWiny =0.0f;
	double dOldWinz =0.0f;
	World2Screen(x, y, z, dOldWinx, dOldWiny, dOldWinz);
	
	//2,���ѡ�е��ڸı���ת���ĺ����Ļ����
	double dNewWinx =0.0f;
	double dNewWiny =0.0f;
	double dNewWinz =0.0f;
			//������ת����
	m_lCOR_x = x;
	m_lCOR_y = y;
	m_lCOR_z = z;
	World2Screen(x, y, z, dNewWinx, dNewWiny, dNewWinz);

	//3,��ȡ������ת��ֵ��ת������������ϵ��
	double dShiftx =0.0f;
	double dShifty =0.0f;
	double dShiftz =0.0f;

	dShiftx = (dNewWinx - dOldWinx) * m_OpenGL.m_dVisualSceneW / (double)m_OpenGL.m_iWidth;
	dShifty = (dNewWiny - dOldWiny) * m_OpenGL.m_dVisualSceneW / (double)m_OpenGL.m_iWidth;
	dShiftz = (dNewWinz - dOldWinz) * m_OpenGL.m_dVisualSceneW / (double)m_OpenGL.m_iWidth;

	//4,�����ۼ�ƫ��������Ϊ�������任 zֵ���Բ��ÿ���
// 	m_WSPose.m_fXShift = m_WSPose.m_fXShift - dShiftx;
// 	m_WSPose.m_fYShift = m_WSPose.m_fYShift + dShifty;
// 
// 	m_OpenGL.m_dOrigin_x += dShiftx;
// 	m_OpenGL.m_dOrigin_y += dShifty;
// 
// 	m_OpenGL.SetFrustums(m_OpenGL.m_dVisualSceneW, 
// 						m_OpenGL.m_dVisualSceneH,
// 						-m_OpenGL.m_dOrigin_x,
// 						-m_OpenGL.m_dOrigin_y);	// Ĭ����perspective
	AddRCXYShift(-(dNewWinx - dOldWinx), (dNewWiny - dOldWiny));


	return 1;
}


/*
(1)�������� �����ת����
(2)���� ���Ȼ
(3)���� 2015-9-24
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
int  CWS3DShow::GetCOR(double &x, double &y, double &z) const
{
	x = m_lCOR_x ;
	y = m_lCOR_y ;
	z = m_lCOR_z ;
	return 1;
}

/*
(1)�������� ��ģ�������ƶ�����Ļ����
(2)���� ���Ȼ
(3)���� 2015-9-24
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
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
(1)�������� ���¶���������б��б�Ϊ�������ö������ʾ��1�㲻��ʾ��2����ʾ�ߣ������ʾ����Σ���λ���
(2)���� jhl
(3)���� 20160831
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
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
			cout<<"��ȡ������ά��"<<endl;
			//�õ����д�������ά��
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

			//ת����ƽ��xy����ϵ����ȡֱ�߷���������Ȼ�������x��нǣ����Ŷ�x��yƽ������ϵ������ת���õ���ת���x���꣬����ƽ��ͶӰ��ֱ�ӽ�x������Ϊ�任��x���꣬y��Ϊ0
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

			//��ȡxy��Χ����ʼ������,ȷ��xy���귶Χ
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
			//��ʼ������
			cout<<"��ʼ������"<<endl;
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
			cout<<"���������Ļ�������"<<endl;
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

//����������ת��Ϊ��������
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
(1)�������� ��ն�������߶����б�
(2)���� jhl
(3)���� 20160831
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
int CWS3DShow::ClearPolygonVertices()
{
	m_PolyGonLine.clear();
	return 0;
}

/*
(1)�������� ����������߻��ƾ����Ϣ
(2)���� jhl
(3)���� 20160831
(4)�޸���
(5)�޸�����
(6)�޸�����
(7)����˵�� ��
*/
int CWS3DShow::SetDepthCurveDC(HDC hDC, int width, int height)
{
	m_hDC_DepthCurve = hDC;
	m_hDC_DCHeight = height;
	m_hDC_DCWidth = width;
	return 0;
}
//�Ŵ�
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
