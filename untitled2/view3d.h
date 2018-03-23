#ifndef VIEW3D_H
#define VIEW3D_H

#include <QObject>
#include <windows.h>
#include "LogicLayer/WS3DShow.h"

#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QQuickItem>
#include <QQuickWindow>

#define DECLRARE_Q_PROPERTY( aType, aProperty ) protected:\
	aType m_ ## aProperty; public:\
	aType aProperty(void) { return m_ ## aProperty; } \
	void set ## aProperty(aType _ ## aProperty) \
	{\
	m_ ## aProperty = _ ## aProperty; \
if (window() != Q_NULLPTR)\
		{\
		window()->update(); \
		}\
	}



class View3D : public QQuickItem, public CWS3DShow
{
	Q_OBJECT
	Q_PROPERTY(qreal rotateAngle READ RotateAngle WRITE setRotateAngle NOTIFY RotateAngleChanged)
	Q_PROPERTY(QVector3D axis READ Axis WRITE setAxis NOTIFY AxisChanged)
	Q_PROPERTY(CWS3DShow* OpenGL READ readOpenGL WRITE InitOpenGL)

public:
	explicit View3D(void);
	 
signals:
	void RotateAngleChanged(void);
	void AxisChanged(void);

protected slots:
	void Render(void);
	void OnWindowChanged(QQuickWindow* pWindow);
	void Release(void);
protected:
	int RunOnce();
	QMatrix4x4                  m_ModelViewMatrix;
	QMatrix4x4                  m_ProjectionMatrix;
	QOpenGLBuffer               m_VertexBuffer, m_IndexBuffer;
	QOpenGLBuffer               m_ColorBuffer;
	QOpenGLShaderProgram        m_ShaderProgram;

	CWSFModel *m_pointCloud;
	CWS3DShow *m_pOpenGL;
	HWND hwnd;

public:
	CWS3DShow* readOpenGL();
	void InitOpenGL(CWS3DShow* opengl);
	Q_INVOKABLE HWND get_windows_hwnd_from_title(const QString &title);


	DECLRARE_Q_PROPERTY(qreal, RotateAngle)
	DECLRARE_Q_PROPERTY(QVector3D, Axis)

	
};

#endif // VIEW3D_H
