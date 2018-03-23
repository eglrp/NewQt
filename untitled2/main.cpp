#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <interface.h>
//#include <imageprovider.h>
#include <view3d.h>
 


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

	qmlRegisterType<InterFace>("an.qt.interFace", 1, 0, "InterFace");
	qmlRegisterType<View3D>("OpenGL3DShow", 1, 0, "View3D");
	
	InterFace interFace;// = new InterFace;
	interFace.Init();
	interFace.StartVideo();
	interFace.EnterRecognizeUi();
	
    QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("interFace", &interFace);

	//View3D view3D;
	//engine.rootContext()->setContextProperty("View3D", &view3D);
	
	engine.addImageProvider("imageProvider", (interFace.provider));
	//engine.addImageProvider("cloudProvider", (interFace->cloudProvider));

    engine.load(QUrl(QStringLiteral("main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}

