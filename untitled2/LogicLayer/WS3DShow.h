/********************************************************************
	created:	2015/05/029
	filename: 	WSFModel.h
	file path:	
	file base:	
	file ext:	h
	author:		red
	
	purpose:	COpenGL仅进行底层绘制，与业务逻辑无关
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
#pragma comment(lib,"gdi32.lib")	//添加该库以替代 
//#include<gl/glut.h>


enum ProType{PERSPECTIVE, ORTHO} ;// 投影类型
enum DSPMode{DSP_WRL,DSP_GENERAL};
//负责OpenGL初始化、环境设置等工作
class CWSOpenGL
{

public:
	CWSOpenGL();
	CWSOpenGL(HDC dc, GLsizei iWidth, GLsizei iHeight);
	~CWSOpenGL(void);

	//OpenGL初始化
	int InitOpenGL(HDC dc, int iWidth, int iHeight);
	//OpenGL参数设置
	int MySetPixelFormat(HDC hdc,int ColorBits,int DepthBits);
	//设置RC
	int SetRc();
	int SetRc(HDC hDC, HGLRC m_hRC);
	//清除RC
	int ClearRc();
	//其它效果
	int SpecialEffect();
	//投影及视口设置
	int SetProjection(int iWidth, int iHeight, ProType pers);


	double NearPlane() const { return m_dNearPlane; }
	void   NearPlane(double val) { m_dNearPlane = val; }
	double FarPlane() const { return m_dFarPlane; }
	void   FarPlane(double val) { m_dFarPlane = val; }

	//设置平截头体
	int SetFrustums(double dWidth, double dHeight);
	int SetFrustums(double dWidth, double dHeight, double dOrigin_x, double dOrigin_y);


// 	//重置OpenGL窗口大小
// 	void ReSizeGLScene(GLsizei width, GLsizei height); 
	//双缓存
	void SwapOpenGLBuffers(void);

private:
	HGLRC	m_hRC;  //窗口着色描述表句柄
	HDC		m_hDC;  //OpenGL渲染描述句柄
	GLfloat m_rtri; // 用于三角形的角度

	double	m_dNearPlane; 
	double	m_dFarPlane; 
	int     m_iCloudCount;//点云计数器

public:
	int	 m_iHeight;//窗口的高度（单位像素）
	int  m_iWidth;//窗口的宽度（单位像素）

	int m_iProjectionType;//投影方式，0代表透视投影（默认），1代表正交投影

	double m_dVisualSceneW;//平截头体的宽
	double m_dVisualSceneH;//平截头体的高
	double m_dOrigin_x;//平截头体的位置，相对于相机坐标系
	double m_dOrigin_y;//平截头体的位置，相对于相机坐标系

	double m_dEyePosition_z;//相机位置，相对于世界坐标系，z方向
};

//
class CWSDrawing
{
public:
	CWSDrawing();
	~CWSDrawing();

public:
	int CreateCloud(void *pWSFModel, int texture_or_cloud);//创建点云
	int CreateCloud(void *pWSFModel, int texture_or_cloud, int ICloudID);//创建多点云
	int DeleteAllCloud(void);	  //删除所有点云
	int DeleteSelCloud(int iCouldID);	  //删除选中点云
	int ShowSelCloud(int iCouldID);	  //显示选中点云
// 	int ShowAllCould();	  //显示所有点云
	int HideSelCould(int iCouldID);	  //影藏选中点云
// 	int HideAllCould();	  //影藏所有点云
	int GetCloudCount() const; //获取当前点云数量

	int LoadTex(int iTexWidth, int iTexHeight, CWSTexRGB *pWSTexRGB);//加载纹理

public:
	int DrawCloud(void *pWSFModel,int texture_or_cloud);//画点云

protected:
	int           m_iHeight;//视口高度
	int		      m_iWidth;//视口宽度
public:
	int           m_iCloudCount;//点云计数器 ，删除点云时，不减少该值，，该值主要用在显示列表中和编排外部点云id号

public:
	GLuint        listName[256*3];//显示列表，最多256个点云
	GLuint		  texture;
	DSPMode		  m_iDspMode;

};




class CWSPose
{
public:
	CWSPose();
	~CWSPose();
	int SetTransform();	//初始化矩阵
	int SetLastRot();	//初始化矩阵
	int SethisRot();	//初始化矩阵
	//左键按下
	void LeftButnDown(int MouseX,int MouseY);  //////
	//鼠标拖动
	int RotMouseDragging(int MouseX,int MouseY); ///////
	//记录鼠标左键弹起时点云的旋转矩阵
	void SetRotation();
	//点云变换（旋转与缩放）
	int  SetViewScale(int ifViewScale);		//////////
	int  SetViewScale(int ifViewScale, long lScreen_x, long lScreen_y);		//////////
	//累积平移增量
	int  AddXYShift(float ifxShift,float ifyShift);
	//设置点云旋转first click
	int  SetRotFirClick(float ifptx,float ifpty);
	//设置默认的pose
	int  SetDefPose();
	//点云的缩放
	float m_fViewScale;
	//点云的平移
	float m_fXShift;
	float m_fYShift;
	//点云的旋转
	Matrix4fT   m_Transform;			// NEW: Final Transform
	ArcBallT    m_ArcBall;				// NEW: ArcBall Instance
private:
  
	Matrix3fT   m_LastRot;				// NEW: Last Rotation
	Matrix3fT   m_ThisRot;				// NEW: This Rotation

	Point2fT    m_MousePt;				// NEW: Current Mouse Point

};


//用于点云显示（基于OpenGL）
class CWS3DShow
{
public:
	CWS3DShow();
	~CWS3DShow();
	int Initialization(HDC hDC, int width, int height);
	int AddCWS3DFModel(CWSFModel *pWSFModel,int texture_or_cloud);//传入点云指针
	int AddCWS3DFModel(CWSFModel *pWSFModel,int texture_or_cloud, int iCloudID);//传入点云指针
	void SetDspMode(DSPMode iMode);
	int RenderScene(int type); //RenderSceneList
	int RenderScene(); //使用成员变量指定渲染模式

	//设置显示点云时缩放系数，
	int  SetRCViewScale(int ifRCViewScale);
	//设置显示点云时缩放系数，以点云坐标为中心。x，y为距离点云坐标的距离。
	int SetRCViewScale(int ifRCViewScale, long lScreen_x, long lScreen_y);
	
	//设置默认姿态
	int  SetDefaultPose();
	//设置当前旋转为默认状态
	int  SetCurPose2Default();
	//设置点云显示颜色
	int  SetDisplayColor(int red,int green,int blue);

	//设置显示点云时平移系数，
	int  AddRCXYShift(float ifxShift,float ifyShift);

	//设置点云旋转first click
	int  SetRCRotFirClick(float ifptx,float ifpty);

	//设置点云旋转时鼠标拖动量
	int  RCRotMouseDragging(float ifptx,float ifpty);
	
	//重置OpenGL窗口大小
	void ReSizeGLScene(GLsizei width, GLsizei height); 

	//朱：将屏幕坐标转换为OPengl三维坐标
	int Screen2World(int x, int y, double &outx, double &outy, double &outz);  

	//朱：将屏幕坐标转换为OPengl三维坐标
	int World2Screen(double objx, double objy, double objz, 
								double &winx, double &winy, double &winz);  

	//朱浩然：设置(获得)旋转中心 20150925
	int SetCOR(double x, double y, double z);
	int GetCOR(double &x, double &y, double &z) const;

	//将模型重心移动到屏幕中心
	int Trans2ScreenCenter(float dCenx, float dCeny);

	//多角度投影
	int SynthesizeImages(bool bAxisRotate, float fDisplayScale,
		char *lpINIFilePath, char *lpBMPFilePath, char *lpDATFilePath,
		int iCallList = 1, double dEyeDistance = 1000.0);//bAxisRotate表示坐标系是否跟着转动

	//更新多边形曲线列表，列表不为空则会调用多边形显示，1点不显示，2点显示线，多点显示多边形，首位相接
	int SetPolygonVertices(vector<CPoint>*pVertices);
	int ClearPolygonVertices();
	int SetDepthCurveDC(HDC hDC, int width, int height);
	int ZoomIn();//放大
	int ZoomOut();//缩小
	int AddCurveViewXYshift(int xShift,int yShift);//单位为像素，用于鼠标拖动时对显示曲线进行平移
	int SelectCurvePt(int ptx,int pty);//在曲线显示窗体上点击，在点击位置显示3D坐标

public:
	CWSOpenGL     m_OpenGL;  		 //负责OpenGL初始化、环境设置等工作
	CWSDrawing 	  m_WSDrawing;		 //负责画图（点云、三角片、纹理）等工作
	CWSPose		  m_WSPose;			 //负责3d模型的变换工作（如：缩放、平移、旋转等）
	HDC           m_hDC;
	HDC			  m_hDC_DepthCurve;
	int			  m_hDC_DCWidth;
	int			  m_hDC_DCHeight;
	RECT		  m_RectCurve;
	int           m_r,m_g,m_b;
// 	GLsizei m_iWidth;//存放gl窗体宽
// 	GLsizei m_iHeight;//存放gl窗体高
	GLfloat m_lScreen_x;//缩放操作,存放鼠标位置与模型坐标原点在Opengl视口中的距离增量
	GLfloat m_lScreen_y;//缩放操作,存放鼠标位置与模型坐标原点在Opengl视口中的距离增量


	GLfloat m_lCOR_x;//旋转中心
	GLfloat m_lCOR_y;//旋转中心
	GLfloat m_lCOR_z;//旋转中心

	GLfloat m_lCOG_x;//重心中心（所有的点）（即为重置旋转中心），点云坐标
	GLfloat m_lCOG_y;//重心中心（所有的点）（即为重置旋转中心），点云坐标
	GLfloat m_lCOG_z;//重心中心（所有的点）（即为重置旋转中心），点云坐标

	CWSFModel     *m_pWSFModel;		 //负责接收外部点云指针

	int m_iShowType;//点云显示方式，0表示纹理显示（默认），1表示点显示
// 	bool            m_bAxisRotate;//多角度旋转投影时，坐标系是否随着转动
	vector<CPoint> m_PolyGonLine;//多边形连线
	vector<Tuple4fT>m_SelectPtsCord;
	vector<cv::Point2f> m_SelectPtsCordXY;
	//以下参数在数据更新时初始化，在鼠标操作时调整
	float m_RangeX0;//画板区域x0点，
	float m_RangeX1;//像素坐标x1点
	float m_RangeY0;//画板区域y0点
	float m_RangeY1;//像素坐标y1点
	//将曲线坐标转换为像素坐标
	int ConvertXY2Pixel(float x,float y,CPoint&pixelCor);
	vector<CPoint> m_PolyGonCurve;//多边形连线
	vector<CPoint> m_GridStart;
	vector<CPoint> m_GridEnd;
	vector<cv::Point2f>m_GridCord;
};
