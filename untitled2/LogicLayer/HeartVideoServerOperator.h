#include "stdafx.h"
#include "AbstractOperator.h"
#include "Global.h"
//ÐÄÌø
class HeartVideoServerOperator :public AbstractOperator
{
public:
	HeartVideoServerOperator(const char* key) :AbstractOperator(OP_Heart, key)
	{
	}
public:
	virtual bool DoReceiveRequest(JsonMsg* json)
	{
		//don't care the data, response it directly
		Response();
		return true;
	}
	void Response()
	{
		JsonMsg json;
		this->AppendResponseHeader(&json);
		this->m_inProxy->SendJson(&json);
		this->SetOver();
	}
};