#pragma once
#include "stdafx.h"
#include "FPProtocol.h"

class RegisterOperator :public AbstractOperator
{
private:
	FPEvent* m_e;
public:
	RegisterOperator(FPEvent* e) :AbstractOperator(OP_FCRegister)
	{
		m_e = e;
	}
	~RegisterOperator()
	{
		SetEvent(m_e->get_event());
	}
public:
	bool Request(AbstractProxy* proxy, const char* clientID,const char* ip,int port)
	{
		JsonMsg json;
		this->AppendRequestHeader(&json);
		json.AppendItemNameVal("clientid", clientID);
		json.AppendItemNameVal("info", FPUtility::CombinInfo(clientID, ip, port).c_str());
		proxy->AppendOperator(this);
		return proxy->SendJson(&json);

	}
	virtual bool DoReceiveResponse(JsonMsg* json)
	{
		if (json->m_jsonItems.size() != 3) return false;

		bool r = JsonConvert::Str2Bool(json->m_jsonItems[2]->m_strval);
		//notify somebody that the operation is successful
		bool* p = (bool*)m_e->get_buf();
		*p = r;
		SetEvent(m_e->get_event());

		this->SetOver();
		return true;
	}
};