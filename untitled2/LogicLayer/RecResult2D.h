#pragma once

#include "common.h"

class RecResult2D
{
public:
	RecResult2D(swRect& r,swPersonInfo& p, cv::Mat& regImage, cv::Mat& recImage)
	{
		rect = r;
		person = p;
		image[0] = regImage;
		image[1] = recImage;
	}
	~RecResult2D()
	{
	}
	bool IsSamePerson(RecResult2D* r)
	{
		if (strcmp(person.sPersonID, r->person.sPersonID) == 0)
			return true;
		return false;
	}
public:
	swRect rect;
	swPersonInfo person;
	cv::Mat image[2];
};