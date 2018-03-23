#include "stdafx.h"
#include "AbstractOperator.h"
#include "Global.h"
//接收心跳，返回
class VideoHeartOperator :public AbstractOperator
{
public:
	VideoHeartOperator(const char* key) :AbstractOperator(OP_VideoHeart, key)
	{
	}
public:
	virtual bool DoReceiveRequest(JsonMsg* json)
	{
		//don't response, the video frame is enough
		this->SetOver();
		return true;
	}
};