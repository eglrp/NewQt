#pragma once
#include "FPProtocol.h"
//发送心跳，得到结果
class HeartOperator :public AbstractOperator
{
private:
	FPEvent* m_e;
public:
	HeartOperator(FPEvent* e) :AbstractOperator(OP_Heart)
	{
		m_e = e;
	}
	virtual ~HeartOperator()
	{
		SetEvent(m_e->get_event());
	}
public:
	bool Request(AbstractProxy* proxy)
	{
		m_curSec = 10; // after 10 sec, this will be deleted by op manager
		JsonMsg json;
		this->AppendRequestHeader(&json);
		proxy->AppendOperator(this);
		return proxy->SendJson(&json);

	}
	virtual bool DoReceiveResponse(JsonMsg* json)
	{
		bool* p = (bool*)m_e->get_buf();
		*p = true;
		std::cout << "received response of heart" << std::endl;
		SetEvent(m_e->get_event());

		this->SetOver();
		return true;
	}
};