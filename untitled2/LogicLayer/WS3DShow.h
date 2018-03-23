/********************************************************************
	created:	2015/05/029
	filename: 	WSFModel.h
	file path:	
	file base:	
	file ext:	h
	author:		red
	
	purpose:	COpenGL�����еײ���ƣ���ҵ���߼��޹�
*********************************************************************/

#pragma once

//#include<windows.h>
// #ifdef _WIN64
// #define GLUT_USE_SGI_OPENGL
// #endif // _WIN32

#include "GL/GL.h"
#include "GL/glu.h"
#include "GL/GLAUX.H"
#include "GL/GLUT.H"

#include "ArcBall.h"
#include "WSFModel.h"
#include "atltypes.h"
#include <vector>
#include <opencv2\opencv.hpp>
using namespace std;
// #pragma  comment(lib,"opengl32.lib")
// #pragma  comment(lib,"glut32.lib")
// #pragma  comment(lib,"glaux.lib")
// #pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")	//��Ӹÿ������ 
//#include<gl/glut.h>


enum ProType{PERSPECTIVE, ORTHO} ;// ͶӰ����
enum DSPMode{DSP_WRL,DSP_GENERAL};
//����OpenGL��ʼ�����������õȹ���
class CWSOpenGL
{

public:
	CWSOpenGL();
	CWSOpenGL(HDC dc, GLsizei iWidth, GLsizei iHeight);
	~CWSOpenGL(void);

	//OpenGL��ʼ��
	int InitOpenGL(HDC dc, int iWidth, int iHeight);
	//OpenGL��������
	int MySetPixelFormat(HDC hdc,int ColorBits,int DepthBits);
	//����RC
	int SetRc();
	int SetRc(HDC hDC, HGLRC m_hRC);
	//���RC
	int ClearRc();
	//����Ч��
	int SpecialEffect();
	//ͶӰ���ӿ�����
	int SetProjection(int iWidth, int iHeight, ProType pers);


	double NearPlane() const { return m_dNearPlane; }
	void   NearPlane(double val) { m_dNearPlane = val; }
	double FarPlane() const { return m_dFarPlane; }
	void   FarPlane(double val) { m_dFarPlane = val; }

	//����ƽ��ͷ��
	int SetFrustums(double dWidth, double dHeight);
	int SetFrustums(double dWidth, double dHeight, double dOrigin_x, double dOrigin_y);


// 	//����OpenGL���ڴ�С
// 	void ReSizeGLScene(GLsizei width, GLsizei height); 
	//˫����
	void SwapOpenGLBuffers(void);

private:
	HGLRC	m_hRC;  //������ɫ��������
	HDC		m_hDC;  //OpenGL��Ⱦ�������
	GLfloat m_rtri; // ���������εĽǶ�

	double	m_dNearPlane; 
	double	m_dFarPlane; 
	int     m_iCloudCount;//���Ƽ�����

public:
	int	 m_iHeight;//���ڵĸ߶ȣ���λ���أ�
	int  m_iWidth;//���ڵĿ�ȣ���λ���أ�

	int m_iProjectionType;//ͶӰ��ʽ��0����͸��ͶӰ��Ĭ�ϣ���1��������ͶӰ

	double m_dVisualSceneW;//ƽ��ͷ��Ŀ�
	double m_dVisualSceneH;//ƽ��ͷ��ĸ�
	double m_dOrigin_x;//ƽ��ͷ���λ�ã�������������ϵ
	double m_dOrigin_y;//ƽ��ͷ���λ�ã�������������ϵ

	double m_dEyePosition_z;//���λ�ã��������������ϵ��z����
};

//
class CWSDrawing
{
public:
	CWSDrawing();
	~CWSDrawing();

public:
	int CreateCloud(void *pWSFModel, int texture_or_cloud);//��������
	int CreateCloud(void *pWSFModel, int texture_or_cloud, int ICloudID);//���������
	int DeleteAllCloud(void);	  //ɾ�����е���
	int DeleteSelCloud(int iCouldID);	  //ɾ��ѡ�е���
	int ShowSelCloud(int iCouldID);	  //��ʾѡ�е���
// 	int ShowAllCould();	  //��ʾ���е���
	int HideSelCould(int iCouldID);	  //Ӱ��ѡ�е���
// 	int HideAllCould();	  //Ӱ�����е���
	int GetCloudCount() const; //��ȡ��ǰ��������

	int LoadTex(int iTexWidth, int iTexHeight, CWSTexRGB *pWSTexRGB);//��������

public:
	int DrawCloud(void *pWSFModel,int texture_or_cloud);//������

protected:
	int           m_iHeight;//�ӿڸ߶�
	int		      m_iWidth;//�ӿڿ��
public:
	int           m_iCloudCount;//���Ƽ����� ��ɾ������ʱ�������ٸ�ֵ������ֵ��Ҫ������ʾ�б��кͱ����ⲿ����id��

public:
	GLuint        listName[256*3];//��ʾ�б����256������
	GLuint		  texture;
	DSPMode		  m_iDspMode;

};




class CWSPose
{
public:
	CWSPose();
	~CWSPose();
	int SetTransform();	//��ʼ������
	int SetLastRot();	//��ʼ������
	int SethisRot();	//��ʼ������
	//�������
	void LeftButnDown(int MouseX,int MouseY);  //////
	//����϶�
	int RotMouseDragging(int MouseX,int MouseY); ///////
	//��¼����������ʱ���Ƶ���ת����
	void SetRotation();
	//���Ʊ任����ת�����ţ�
	int  SetViewScale(int ifViewScale);		//////////
	int  SetViewScale(int ifViewScale, long lScreen_x, long lScreen_y);		//////////
	//�ۻ�ƽ������
	int  AddXYShift(float ifxShift,float ifyShift);
	//���õ�����תfirst click
	int  SetRotFirClick(float ifptx,float ifpty);
	//����Ĭ�ϵ�pose
	int  SetDefPose();
	//���Ƶ�����
	float m_fViewScale;
	//���Ƶ�ƽ��
	float m_fXShift;
	float m_fYShift;
	//���Ƶ���ת
	Matrix4fT   m_Transform;			// NEW: Final Transform
	ArcBallT    m_ArcBall;				// NEW: ArcBall Instance
private:
  
	Matrix3fT   m_LastRot;				// NEW: Last Rotation
	Matrix3fT   m_ThisRot;				// NEW: This Rotation

	Point2fT    m_MousePt;				// NEW: Current Mouse Point

};


//���ڵ�����ʾ������OpenGL��
class CWS3DShow
{
public:
	CWS3DShow();
	~CWS3DShow();
	int Initialization(HDC hDC, int width, int height);
	int AddCWS3DFModel(CWSFModel *pWSFModel,int texture_or_cloud);//�������ָ��
	int AddCWS3DFModel(CWSFModel *pWSFModel,int texture_or_cloud, int iCloudID);//�������ָ��
	void SetDspMode(DSPMode iMode);
	int RenderScene(int type); //RenderSceneList
	int RenderScene(); //ʹ�ó�Ա����ָ����Ⱦģʽ

	//������ʾ����ʱ����ϵ����
	int  SetRCViewScale(int ifRCViewScale);
	//������ʾ����ʱ����ϵ�����Ե�������Ϊ���ġ�x��yΪ�����������ľ��롣
	int SetRCViewScale(int ifRCViewScale, long lScreen_x, long lScreen_y);
	
	//����Ĭ����̬
	int  SetDefaultPose();
	//���õ�ǰ��תΪĬ��״̬
	int  SetCurPose2Default();
	//���õ�����ʾ��ɫ
	int  SetDisplayColor(int red,int green,int blue);

	//������ʾ����ʱƽ��ϵ����
	int  AddRCXYShift(float ifxShift,float ifyShift);

	//���õ�����תfirst click
	int  SetRCRotFirClick(float ifptx,float ifpty);

	//���õ�����תʱ����϶���
	int  RCRotMouseDragging(float ifptx,float ifpty);
	
	//����OpenGL���ڴ�С
	void ReSizeGLScene(GLsizei width, GLsizei height); 

	//�죺����Ļ����ת��ΪOPengl��ά����
	int Screen2World(int x, int y, double &outx, double &outy, double &outz);  

	//�죺����Ļ����ת��ΪOPengl��ά����
	int World2Screen(double objx, double objy, double objz, 
								double &winx, double &winy, double &winz);  

	//���Ȼ������(���)��ת���� 20150925
	int SetCOR(double x, double y, double z);
	int GetCOR(double &x, double &y, double &z) const;

	//��ģ�������ƶ�����Ļ����
	int Trans2ScreenCenter(float dCenx, float dCeny);

	//��Ƕ�ͶӰ
	int SynthesizeImages(bool bAxisRotate, float fDisplayScale,
		char *lpINIFilePath, char *lpBMPFilePath, char *lpDATFilePath,
		int iCallList = 1, double dEyeDistance = 1000.0);//bAxisRotate��ʾ����ϵ�Ƿ����ת��

	//���¶���������б��б�Ϊ�������ö������ʾ��1�㲻��ʾ��2����ʾ�ߣ������ʾ����Σ���λ���
	int SetPolygonVertices(vector<CPoint>*pVertices);
	int ClearPolygonVertices();
	int SetDepthCurveDC(HDC hDC, int width, int height);
	int ZoomIn();//�Ŵ�
	int ZoomOut();//��С
	int AddCurveViewXYshift(int xShift,int yShift);//��λΪ���أ���������϶�ʱ����ʾ���߽���ƽ��
	int SelectCurvePt(int ptx,int pty);//��������ʾ�����ϵ�����ڵ��λ����ʾ3D����

public:
	CWSOpenGL     m_OpenGL;  		 //����OpenGL��ʼ�����������õȹ���
	CWSDrawing 	  m_WSDrawing;		 //����ͼ�����ơ�����Ƭ�������ȹ���
	CWSPose		  m_WSPose;			 //����3dģ�͵ı任�������磺���š�ƽ�ơ���ת�ȣ�
	HDC           m_hDC;
	HDC			  m_hDC_DepthCurve;
	int			  m_hDC_DCWidth;
	int			  m_hDC_DCHeight;
	RECT		  m_RectCurve;
	int           m_r,m_g,m_b;
// 	GLsizei m_iWidth;//���gl�����
// 	GLsizei m_iHeight;//���gl�����
	GLfloat m_lScreen_x;//���Ų���,������λ����ģ������ԭ����Opengl�ӿ��еľ�������
	GLfloat m_lScreen_y;//���Ų���,������λ����ģ������ԭ����Opengl�ӿ��еľ�������


	GLfloat m_lCOR_x;//��ת����
	GLfloat m_lCOR_y;//��ת����
	GLfloat m_lCOR_z;//��ת����

	GLfloat m_lCOG_x;//�������ģ����еĵ㣩����Ϊ������ת���ģ�����������
	GLfloat m_lCOG_y;//�������ģ����еĵ㣩����Ϊ������ת���ģ�����������
	GLfloat m_lCOG_z;//�������ģ����еĵ㣩����Ϊ������ת���ģ�����������

	CWSFModel     *m_pWSFModel;		 //��������ⲿ����ָ��

	int m_iShowType;//������ʾ��ʽ��0��ʾ������ʾ��Ĭ�ϣ���1��ʾ����ʾ
// 	bool            m_bAxisRotate;//��Ƕ���תͶӰʱ������ϵ�Ƿ�����ת��
	vector<CPoint> m_PolyGonLine;//���������
	vector<Tuple4fT>m_SelectPtsCord;
	vector<cv::Point2f> m_SelectPtsCordXY;
	//���²��������ݸ���ʱ��ʼ������������ʱ����
	float m_RangeX0;//��������x0�㣬
	float m_RangeX1;//��������x1��
	float m_RangeY0;//��������y0��
	float m_RangeY1;//��������y1��
	//����������ת��Ϊ��������
	int ConvertXY2Pixel(float x,float y,CPoint&pixelCor);
	vector<CPoint> m_PolyGonCurve;//���������
	vector<CPoint> m_GridStart;
	vector<CPoint> m_GridEnd;
	vector<cv::Point2f>m_GridCord;
};
