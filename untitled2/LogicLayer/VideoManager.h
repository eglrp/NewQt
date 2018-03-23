#pragma once
#include "LogicLayerDef.h"

class VideoManager
{
public:
	VideoManager()
	{}
	~VideoManager()
	{}
	void Assign(void* owner, ONCOMPLETEDCameraVideo pFunc)
	{
		m_owner = owner;
		m_pFunc = pFunc;
	}
public:
	void* m_owner;
	ONCOMPLETEDCameraVideo m_pFunc;
};