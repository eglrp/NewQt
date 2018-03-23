#pragma once
#include "AbstractProxy.h"
class SMModelProxy :public AbstractProxy
{
private:
	char m_mapName[50];
	long m_mapSize;
	char m_requestSemName[50];
	char m_responseSemName[50];
	JsonMsgHandler* m_jsonMsgHandler;
	bool m_bOpen;
public:
	SMModelProxy(OperatorManager* opManager):AbstractProxy(opManager)
	{
		sprintf(m_mapName, "SMModelMap");
		m_mapSize = 300 * 1024 * 1024;//300M
		sprintf(m_requestSemName, "SMModelRequestSem");
		sprintf(m_responseSemName, "SMModelResponseSem");
		m_jsonMsgHandler = new JsonMsgHandler(this);
		m_bOpen = false;
	}
	~SMModelProxy()
	{
		delete m_jsonMsgHandler;
	}
public:
	virtual bool Open()
	{
		if (m_bOpen)
			return true;
		CreateThread(NULL, 0, _ThreadReceive, this, 0, NULL);
		return true;
	}
	virtual void Close()
	{
		if (!m_bOpen)
			return;
		m_bOpen = false;
	}
	virtual bool SendJson(JsonMsg* json)
	{
		if (!m_bOpen)
			return false;

		FPSMSender fs(m_mapName, m_mapSize, m_requestSemName);
		int n = fs.SendJson(json);
		if (n <= 0)
		{
			BaseCore_LogInstance()->LogString("%s connection Error: request failed, error code =%d", m_mapName, n);
			return false;
		}
		return true;
	}
	//there is no request from model server, so need not impl func CreateOPInReceiveRequest
private:
	static DWORD WINAPI _ThreadReceive(void* pParam)
	{
		SMModelProxy* proxy = (SMModelProxy*)pParam;
		return proxy->ThreadReceive();
	}
	DWORD ThreadReceive()
	{
		m_bOpen = true;
		while (m_bOpen)
		{
			//To do,wait for receive, 
			//Because FPCollect is client, so here wait responseSem from model server
			//If it's model server, here wait requestSem from client
			FPSMReceiver fr(m_mapName, m_mapSize, m_responseSemName);
			int n = fr.ReadAndHandle(m_jsonMsgHandler);
			if (n < 0)
			{
				if (n != -10001)// msg parse error 
					BaseCore_LogInstance()->LogString("%s connection Error: received msg is invalid", m_mapName);
				else
					BaseCore_LogInstance()->LogString("%s connection Error: receive error, error code = %d", m_mapName, n);
			}
		}
		m_bOpen = false;
		return 0;
	}
	
};