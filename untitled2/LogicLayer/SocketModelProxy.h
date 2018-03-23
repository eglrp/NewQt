#pragma once
#include "FPProtocol.h"
#include "HeartOperator.h"
class SocketModelProxy :public AbstractProxy
{
private:	
	std::string m_serverName;
	std::string m_serverIP;
	int m_serverPort;
	int m_heartinterval;
	IFPMemory* m_memory;
	JsonMsgHandler* m_msgHandler;
	SOCKET m_client;

	bool m_bOpen;
public:
	SocketModelProxy(OperatorManager* opManager, TransferConfig* tc, const char* ip, int port,int heartinterval):AbstractProxy(opManager)
	{
		
		m_serverName = "model server";
		m_serverIP = ip;
		m_serverPort = port;
		m_heartinterval = heartinterval * 1000;
		m_bOpen = false;
		m_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		m_msgHandler = new JsonMsgHandler(this);
		m_memory = new FPMemory(tc);

	}
	virtual ~SocketModelProxy()
	{
		closesocket(m_client);
		delete m_msgHandler;
		delete m_memory;	
		
	}
public:
	virtual bool Open()
	{
		if (m_bOpen)
			return true;
		if (!_Open())
			return false;
		//start Threads
		CreateThread(NULL, 0, _ThreadReceive, this, 0, NULL);
		CreateThread(NULL, 0, _HeartThreadLoop, this, 0, NULL);

		return true;
	}
	virtual void Close()
	{
		closesocket(m_client);
	}
	virtual bool SendJson(JsonMsg* json)
	{
		if (!m_bOpen)
			return false;

		FPSocketSender fs(m_client, m_memory);
		int n = fs.SendJson(json);
		if (n < 0)
		{
			BaseCore_LogInstance()->LogString("%s connection Error: request failed, error code =%d", m_serverName, n);
			return false;
		}
		else if (n == 0)
		{
			BaseCore_LogInstance()->LogString("%s connection Error: connection lost", m_serverName);
			closesocket(m_client);
			return false;
		}
		return true;
	}
private:
	bool _Open()
	{
		// 指明远程服务器的地址信息(端口号、IP地址等)    
		SOCKADDR_IN server;
		memset(&server, 0, sizeof(SOCKADDR_IN)); //先将保存地址的server置为全0      
		server.sin_family = PF_INET; //声明地址格式是TCP/IP地址格式      
		server.sin_port = htons(m_serverPort); //指明连接服务器的端口号，htons()用于 converts values between the host and network byte order      
		server.sin_addr.s_addr = inet_addr(m_serverIP.c_str()); //指明连接服务器的IP地址    
		if (0 != connect(m_client, (struct sockaddr *) &server, sizeof(SOCKADDR_IN)))
		{
			m_bOpen = false;
			return false;
		}
		m_bOpen = true;
		return true;
	}
private:
	static DWORD WINAPI _ThreadReceive(void* pParam)
	{
		SocketModelProxy* proxy = (SocketModelProxy*)pParam;
		return proxy->ThreadReceive();
	}
	DWORD ThreadReceive()
	{
		while (m_bOpen)
		{
			//To do,wait for receive
			FPSocketReceiver fr(m_client, m_memory);
			int n = fr.ReadAndHandle(m_msgHandler);
			if (n < -1)
			{
				if (n != -10001)// msg parse error 
					BaseCore_LogInstance()->LogString("%s connection Error: received msg is invalid", m_serverName);
				else
					BaseCore_LogInstance()->LogString("%s connection Error: receive error, error code = %d", m_serverName, n);
			}
			else if (n == 0 || n == -1)
			{
				BaseCore_LogInstance()->LogString("%s connection Error: connection lost", m_serverName);
				closesocket(m_client);
				break;//if connect lost, exit thread 
			}
		}
		m_bOpen = false;
		return 0;
	}

	static DWORD WINAPI _HeartThreadLoop(void* pParam)
	{
		SocketModelProxy* proxy = (SocketModelProxy*)pParam;
		while (true)
		{
			if (!proxy->m_bOpen) break;
			Sleep(proxy->m_heartinterval);
			if (!proxy->m_bOpen) break;
			bool r = false;
			FPEvent e(&r);
			HeartOperator* op = new HeartOperator(&e);
			if (op->Request(proxy))
			{
				WaitForSingleObject(e.get_event(), INFINITE);
				if (r) continue;  // if heart ok, continue
			}
			//if no heart, close proxy 
			proxy->Close();
			break;
		}
		return 0;
	}
};