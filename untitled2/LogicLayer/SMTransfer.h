#pragma once
#include <stdio.h>
#include <Windows.h>

class SMTransfer
{
private:
	char m_mapName[50];
	long m_mapSize;
	char m_requestEventName[50];
	char m_responseEventName[50];
	HANDLE hMap;
	LPVOID buf;
	HANDLE hrequest;
	HANDLE hresponse;
public:
	SMTransfer()
	{
		sprintf(m_mapName, "fpcollect_sm1");
		m_mapSize = 1024 * 1024 * 300;//300M
		sprintf(m_requestEventName, "fpcollect_event_request1");
		sprintf(m_responseEventName, "fpcollect_event_response1");
		hMap = GetMap();
		buf = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

		hrequest = GetRequestEventIn();
		hresponse = GetResponseEventIn();
	}
	~SMTransfer()
	{
		UnMap(GetMapBuf());
		CloseHandle(GetRequestEvent());
		CloseHandle(GetResponseEvent());
	}
	char* GetMapBuf()
	{
		return (char*)buf;
	}
	HANDLE GetRequestEvent()
	{
		return hrequest;
	}
	HANDLE GetResponseEvent()
	{
		return hresponse;
	}
	HANDLE GetRequestEventIn()
	{
		HANDLE e = OpenEventA(EVENT_ALL_ACCESS, FALSE, m_requestEventName);
		if (e == NULL)
		{
			e = CreateEventA(NULL, FALSE, FALSE, m_requestEventName);
		}
		return e;
	}
	HANDLE GetResponseEventIn()
	{
		HANDLE e = OpenEventA(EVENT_ALL_ACCESS, FALSE, m_responseEventName);
		if (e == NULL)
		{
			e = CreateEventA(NULL, FALSE, FALSE, m_responseEventName);
		}
		return e;
	}
private:
	HANDLE GetMap()
	{
		HANDLE hMap = ::OpenFileMappingA(FILE_MAP_ALL_ACCESS, 0, m_mapName);
		if (NULL == hMap)
		{
			hMap = ::CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, m_mapSize, m_mapName);
		}
		return hMap;
	}
	void UnMap(char* buf)
	{
		::UnmapViewOfFile(buf);
	}

};