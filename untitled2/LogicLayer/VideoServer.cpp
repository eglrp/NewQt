#include "Global.h"
#include "IVideoServer.h"
#include "SocketVideoServer.h"

static IVideoServer* g_videoServer = NULL;

IVideoServer* VideoServerInstance()
{
	if (g_videoServer == NULL)
	{
		AppConfig* appConfig = GlobalInstance()->appConfig;
		g_videoServer = new SocketVideoServer(appConfig->videoserver_ip.c_str(), appConfig->videoserver_port,
			appConfig->videoserver_heartinterval);
	}
	return g_videoServer;
}

void DeleteVideoServerInstance()
{
	if (g_videoServer != NULL)
		delete g_videoServer;
}