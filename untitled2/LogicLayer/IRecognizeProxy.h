#pragma once
#include "BaseCoreBasic.h"

class IRecognizeProxy
{
public:
	virtual bool Open() = 0;
	virtual bool Recognize() = 0;
};