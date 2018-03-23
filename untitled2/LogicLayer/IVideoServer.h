#pragma once
#include "stdafx.h"

//Video server, provide the video.
//The other modules will get video from this server
class IVideoServer
{
public:
	virtual bool IsStart() = 0;
	virtual void Start() = 0;
	virtual void Close() = 0;
	virtual void PushFrm(IFCMemory* memory,uint mask) = 0;
	virtual void PushDspFailed() = 0;
};
IVideoServer* VideoServerInstance();
void DeleteVideoServerInstance();