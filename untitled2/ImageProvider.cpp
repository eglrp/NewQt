#include "ImageProvider.h"

ImageProvider::ImageProvider()
: QQuickImageProvider(QQuickImageProvider::Image)
{

}

ImageProvider::~ImageProvider()
{

}


QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
	if (id == "0")
	{
		return picQPixmap[0];
	}
	else
		return picQPixmap[1];

	
}