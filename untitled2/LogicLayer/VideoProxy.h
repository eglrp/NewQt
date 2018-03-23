#pragma once
#include "FPProtocol.h"
#include "VideoHeartOperator.h"
#include "OperatorManager.h"
class VideoProxy :public AbstractProxy
{
private:
	std::string m_sysId;
	SOCKET m_socket;
	IFPMemory* m_memory;
	
public:
	VideoProxy(OperatorManager* opManager, const char* sysId, SOCKET s, IFPMemory* memory) :AbstractProxy(opManager)
	{
		m_sysId = sysId;
		m_socket = s;
		m_memory = memory;
	}
	virtual ~VideoProxy(){
	}
	virtual bool Open()
	{
		BaseCore_DebugInstance()->Assert(false);
		return true;
	}
	void Close()
	{
		closesocket(m_socket);
	}
	virtual bool SendJson(JsonMsg* json)
	{
		FPSocketSender fs(m_socket, m_memory);
		int n = fs.SendJson(json);
		if (n < 0)
		{
			BaseCore_LogInstance()->LogString("%s connection Error: request failed, error code =%d", m_sysId.c_str(), n);
			return false;
		}
		else if (n == 0)
		{
			BaseCore_LogInstance()->LogString("%s connection Error: connection lost", m_sysId.c_str());
			Close();
			return false;
		}
		return true;
	}
	virtual IOperator* CreateOPInReceiveRequest(const char* opName, const char* key)
	{
		IOperator* op = NULL;
		if (strcmp(OP_VideoHeart, opName) == 0)
			BCORE_NEW(op, VideoHeartOperator(key))
		//...

		return op;
	}

	bool SendFrame(VideoFrame* frame)
	{
		FPSocketSender fs(m_socket, m_memory);
		int n = fs.SendVideo(frame);
		if (n < 0)
		{
			BaseCore_LogInstance()->LogString("%s connection Error: send video failed, n =%d", m_sysId.c_str(), n);
			return false;
		}
		else if (n == 0)
		{
			BaseCore_LogInstance()->LogString("%s connection Error: connection lost", m_sysId.c_str());
			Close();
			return false;
		}
		return true;
	}

	
};