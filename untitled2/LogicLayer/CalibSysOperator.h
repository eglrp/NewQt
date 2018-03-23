#pragma once
#include "stdafx.h"
#include "AbstractOperator.h"
#include "AbstractProxy.h"
#include "Global.h"
#include "CallbackCommand.h"
class CalibSysOperator :public AbstractOperator
{
public:
	CalibSysOperator(const char* key) :AbstractOperator(OP_CalibSys, key)
	{
	}
public:
	//receive request from center server
	virtual bool DoReceiveRequest(JsonMsg* json)
	{
		if (json->m_jsonItems[2]->m_type != JsonItemType::STRING) return false;
		std::string fcId = json->m_jsonItems[2]->m_strval;

		GlobalInstance()->SetAppStatus(AppCalibSysRun);
		CallbackCommand* command = new CallbackCommand(OnCallback, this);
		GlobalInstance()->commandManager->PushCommand(command);
	}

public:
	static void OnCallback(void* param)
	{
		CalibSysOperator* op = (CalibSysOperator*)param;
		op->_OnCallback();
	}
private:
	void _OnCallback()
	{
		AppStatus appStatus = GlobalInstance()->getAppStatus();
		if (appStatus != AppCalibSysRun)
		{
			BaseCore_LogInstance()->LogString("Error: can not start calibsys, appStatus is %d", appStatus);
			Response(false);
			return;
		}
		//start to capture
		BaseCore_LogInstance()->LogString("Start calibsys...");
		IFCMemory* memory = GlobalInstance()->fcMemory;
		//get seq
		if (!GetSeq(memory))
		{
			BaseCore_LogInstance()->LogString("Error: get seq failed");
			Response(false);
			return;
		}
		if (!RequestCalibSys(memory, GlobalInstance()->calibFile.c_str(), GlobalInstance()->g3dFile.c_str()))
		{
			BaseCore_LogInstance()->LogString("Error: calib sys failed");
			Response(false);
			return;
		}
	}
	bool GetSeq(IFCMemory* memory)
	{
		IHWCtrl* hw = GlobalInstance()->hwCtrl;
		if (HW_SUCCESS == hw->GetSeq(memory))
			return true;
		return false;
	}

	bool RequestCalibSys(IFCMemory* memory, const char* calibFile, const char* g3dFile)
	{
		
		AbstractProxy* modelProxy = NULL;
		if (!GlobalInstance()->modelManager->get_Proxy(&modelProxy))
			return false;
		JsonMsg json;
		this->AppendRequestHeader(&json);
		JsonSerFile serCalibFile(calibFile);
		json.AppendItemNameObj("calibfile", &serCalibFile);
		JsonSerFile serG3dFile(g3dFile);
		json.AppendItemNameObj("g3dfile", &serG3dFile);
		JsonSerFCMemoryCapture serCapture(memory);
		json.AppendItemNameObj("memory", &serCapture);
		modelProxy->AppendOperator(this);
		return modelProxy->SendJson(&json);
	}
public:
	//receive response from model server
	virtual bool DoReceiveResponse(JsonMsg* json)
	{
		if (json->m_jsonItems.size() != 3) return false;

		bool r = JsonConvert::Str2Bool(json->m_jsonItems[2]->m_strval);
		GlobalInstance()->SetAppStatus(r ? AppCalibSysSuccess : AppCaptureFailed);
		Response(r);
		return true;
	}
private:
	void Response(bool rtn)
	{
		//response result to center server
		JsonMsg json;
		this->AppendResponseHeader(&json);
		json.AppendItemNameVal("rtn", rtn ? "1" : "0");
		this->m_inProxy->SendJson(&json);
		this->SetOver();
	}
};