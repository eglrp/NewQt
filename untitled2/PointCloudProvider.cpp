#include "PointCloudProvider.h"


PointCloudProvider::PointCloudProvider()
: QQuickImageProvider(QQuickImageProvider::Image)
{

}

PointCloudProvider::~PointCloudProvider()
{

}


QPixmap PointCloudProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
	for (int i = 0; i < 10;i++)
		return picQPixmap[i];
}

