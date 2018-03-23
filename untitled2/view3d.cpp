#include "view3d.h"

View3D::View3D(void)
{
	
	m_pOpenGL = new CWS3DShow[1];
	m_pointCloud = new CWSFModel;
	m_pointCloud->ReadWRL("01.wrl");

	// 初始化
	connect(this, SIGNAL(windowChanged(QQuickWindow*)),
		this, SLOT(OnWindowChanged(QQuickWindow*)));


} 

void View3D::OnWindowChanged(QQuickWindow* pWindow)
{
	if (pWindow == Q_NULLPTR) return;
	connect(pWindow, SIGNAL(beforeRendering()),
		this, SLOT(Render()), Qt::DirectConnection);
	pWindow->setClearBeforeRendering(false);
}

void View3D::Render()
{
	static int runOnce = RunOnce();
	Q_UNUSED(runOnce);

	// 运动
	m_ModelViewMatrix.setToIdentity();
	m_ModelViewMatrix.translate(0.0f, 0.0f, -60.0f);
	m_ModelViewMatrix.rotate(m_RotateAngle, m_Axis.x(),
		m_Axis.y(), m_Axis.z());
	// 渲染
	glViewport(0, 0, window()->width(), window()->height());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	m_ShaderProgram.bind();
	m_VertexBuffer.bind();
	int posLoc = m_ShaderProgram.attributeLocation("position");
	m_ShaderProgram.enableAttributeArray(posLoc);
	m_ShaderProgram.setAttributeBuffer(posLoc,                 // 位置
		GL_FLOAT,               // 类型
		0,                      // 偏移
		3,                      // 元大小
		0);                    // 迈

	m_ColorBuffer.bind();
	int colorLoc = m_ShaderProgram.attributeLocation("color");
	m_ShaderProgram.enableAttributeArray(colorLoc);
	m_ShaderProgram.setAttributeBuffer(colorLoc,               // 位置
		GL_FLOAT,               // 类型
		0,                      // 偏移
		4,                      // 元大小
		0);                    // 迈
	m_IndexBuffer.bind();
	m_ShaderProgram.setUniformValue("modelViewMatrix", m_ModelViewMatrix);
	m_ShaderProgram.setUniformValue("projectionMatrix", m_ProjectionMatrix);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, Q_NULLPTR);

	m_ShaderProgram.disableAttributeArray(posLoc);
	m_ShaderProgram.disableAttributeArray(colorLoc);
	m_IndexBuffer.release();
	m_VertexBuffer.release();
	m_ShaderProgram.release();
}

int View3D::RunOnce()
{
	int rt=m_pOpenGL->RenderScene(0);
	connect(window()->openglContext(),
		SIGNAL(aboutToBeDestroyed()),
		this, SLOT(Release()),
		Qt::DirectConnection);

	return rt;
}

void View3D::Release(void)
{
	qDebug("Vertex buffer and index buffer are to be destoryed.");
	m_VertexBuffer.destroy();
	m_IndexBuffer.destroy();
	m_ColorBuffer.destroy();
	delete m_pOpenGL;
	delete m_pointCloud;
}

CWS3DShow* View3D::readOpenGL()
{
	return m_pOpenGL;
}

void View3D::InitOpenGL(CWS3DShow* opengl)
{
	//HWND hWnd = ::GetFocus();
	HDC hDC = GetDC(hwnd);
	m_pOpenGL->Initialization(hDC, window()->width(), window()->height());
	m_pOpenGL->AddCWS3DFModel(m_pointCloud, 0);
}

HWND View3D::get_windows_hwnd_from_title(const QString &title)
{
	wchar_t *wtitle;
	
	int count;

	//首先获取窗口句柄
	wtitle = new wchar_t(title.count() + 10);
	//
	count = title.toWCharArray(wtitle);
	hwnd = FindWindow(NULL, (LPCWSTR)wtitle);
	//
	delete wtitle;
	//
	if (NULL == hwnd)
		return 0;


	//
	return hwnd;

}