#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QObject>

class ImageProvider : public QObject,public QQuickImageProvider
{
	Q_OBJECT

public:
	ImageProvider();
	~ImageProvider();

	virtual QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
	

private:

public:
	QPixmap picQPixmap[2];
	
};

#endif // IMAGEPROVIDER_H
