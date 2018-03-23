#pragma once
#include "IVideoServer.h"
#include "videoProxy.h"
class SocketVideoServer;
class ThreadArgVideo
{
public:
	ThreadArgVideo(SocketVideoServer* server, SOCKET socket,std::string sysId)
	{
		m_server = server;
		m_socket = socket;
	}
	SocketVideoServer* m_server;

	SOCKET m_socket;
	std::string m_sysId;
};

class SocketVideoServer :public IVideoServer
{
private:
	std::string m_ip;
	int m_port;
	int m_heartinterval;
	bool m_start;
	SOCKET m_sListen;
	std::vector<VideoProxy*> m_proxys;
	CRITICAL_SECTION m_cs;
	OperatorManager m_opManager;
public:
	SocketVideoServer(const char* ip, int port, int heartinterval)
	{
		m_ip = ip;
		m_port = port;
		m_heartinterval = heartinterval * 1000;//sec to milsec
		InitializeCriticalSection(&m_cs);
	}
	~SocketVideoServer()
	{
		Close();
		DeleteCriticalSection(&m_cs);
	}
public:	
	virtual void PushFrm(IFCMemory* fcMemory,uint mask)
	{
		//from frm to frame
		VideoFrame frame(NULL);
		for (int i = 0; i < fcMemory->m_cameraBuf.size(); i++)
		{
			int bShow = mask >> i & 1;
			if (0 == bShow) continue;
			VideoFrameItem* item = new VideoFrameItem(NULL);
			item->m_camIndex = i;
			fcMemory->m_cameraBuf[i]->GetFrmImage(item->m_image);
			frame.m_items.push_back(item);
		}
		//send frame to all clients
		CSMonitor monitor(&m_cs);
		for (int i = 0; i < m_proxys.size(); i++)
			m_proxys[i]->SendFrame(&frame);
	}
	virtual void PushDspFailed()
	{
		VideoFrame frame(NULL);
		//send frame to all clients, the items of frame is 0
		CSMonitor monitor(&m_cs);
		for (int i = 0; i < m_proxys.size(); i++)
			m_proxys[i]->SendFrame(&frame);
	}
	virtual bool IsStart()
	{
		return m_start;
	}
	virtual void Close()
	{
		{
			CSMonitor monitor(&m_cs);
			for (int i = 0; i < m_proxys.size(); i++)
				m_proxys[i]->Close();
		}
		closesocket(m_sListen);
		m_start = false;
	}

	virtual void Start()
	{
		m_start = false;
		SOCKET sClient;
		SOCKADDR_IN local;
		SOCKADDR_IN client;
		int iaddrSize = sizeof(SOCKADDR_IN);

		m_sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		local.sin_family = AF_INET;
		local.sin_port = htons(m_port);
		local.sin_addr.s_addr = htonl(INADDR_ANY);
		if (0 != bind(m_sListen, (struct sockaddr *) &local, sizeof(SOCKADDR_IN))) return;

		if (0 != listen(m_sListen, 3)) return;

		BaseCore_LogInstance()->LogString("Video Server start successfully");
		m_start = true;
		while (m_start)
		{
			SOCKET sClient = accept(m_sListen, (struct sockaddr *) &client, &iaddrSize);
			if (sClient == INVALID_SOCKET)
			{
				Sleep(100);
				break;
			}

			char* ip = inet_ntoa(client.sin_addr);
			int port = ntohs(client.sin_port);
			char sysId[50];
			sprintf(sysId, "%s:%d", ip, port);
			BaseCore_LogInstance()->LogString("A client connected: ip = %s, port = %d", ip, port);

			//the receive thread only get heart json msg
			ThreadArgVideo* arg; BCORE_NEW(arg, ThreadArgVideo(this, sClient,sysId))
			CreateThread(NULL, 0, _ThreadLoop, arg, 0, NULL);
		}
		m_start = false;
	}
	static DWORD WINAPI _ThreadLoop(void* pParam)
	{
		ThreadArgVideo* arg = (ThreadArgVideo*)pParam;
		SocketVideoServer* server = arg->m_server;
		SOCKET socket = arg->m_socket;
		std::string sysId = arg->m_sysId;
		BCORE_DELETE(arg)
		return server->threadLoop(socket, sysId.c_str());
	}
private:
	DWORD threadLoop(SOCKET socket, const char* sysId)
	{
		//if no heart during m_heartinterval sec, n return 0 then close connection
		setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&m_heartinterval, sizeof(int));
		IFPMemory* memory; BCORE_NEW(memory, FPVideoMemory(GlobalInstance()->videoTransferConfig))
		VideoProxy* proxy; BCORE_NEW(proxy, VideoProxy(&m_opManager,sysId, socket, memory))
		m_proxys.push_back(proxy);
		while (true)
		{
			FPSocketReceiver fr(socket, memory);
			JsonMsgHandler jsonHandler(proxy);
			int n = fr.ReadAndHandle(&jsonHandler);
			if (n < -1)
			{
				if (n != -10001)// msg parse error 
					BaseCore_LogInstance()->LogString("Error: received msg is invalid, id = %s", sysId);
				else
					BaseCore_LogInstance()->LogString("Error: receive error,, id = %s, error code = %d", sysId, n);
			}
			else if (n == 0 || n == -1)
			{
				BaseCore_LogInstance()->LogString("Error: connection lost, id = %s ", sysId);
				proxy->Close();
				break;//if connect lost, exit thread 
			}
		}

		//remove proxy
		{
			CSMonitor monitor(&m_cs);
			std::vector<VideoProxy*>::iterator it;
			for (it = m_proxys.begin(); it != m_proxys.end();)
			{
				if (proxy == *it)
				{
					m_proxys.erase(it);
					break;
				}
				it++;
			}
		}
		return 0;
	}

};