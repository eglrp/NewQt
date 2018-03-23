#ifndef POINTCLOUDPROVIDER_H
#define POINTCLOUDPROVIDER_H

#pragma once
#include <QQuickImageProvider>
#include <QObject>



class PointCloudProvider :public QObject, public QQuickImageProvider
{
	Q_OBJECT

public:
	PointCloudProvider();
	~PointCloudProvider();

	virtual QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

public:
	QPixmap picQPixmap[10];
};

#endif

