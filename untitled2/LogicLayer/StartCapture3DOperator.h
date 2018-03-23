#pragma once
#include "stdafx.h"
#include "AbstractOperator.h"
#include "AbstractProxy.h"
#include "Global.h"
#include "CallbackCommand.h"
class StartCapture3DOperator :public AbstractOperator
{
private:
	FCUser* m_user;
public:
	StartCapture3DOperator(const char* key) :AbstractOperator(OP_StartCapture3D, key)
	{
	}
	~StartCapture3DOperator()
	{
		delete m_user;
	}
public:
	//receive request from center server
	virtual bool DoReceiveRequest(JsonMsg* json)
	{
		if (json->m_jsonItems[2]->m_type != JsonItemType::OBJECT) return false;
		JsonSerFCUser* ser = dynamic_cast<JsonSerFCUser*>(json->m_jsonItems[2]->m_objval);
		if (ser == NULL) return false;
		m_user = ser->get_user();
		CallbackCommand* command = new CallbackCommand(OnCallback,this);
		GlobalInstance()->commandManager->PushCommand(command);
	}
public:
	static void OnCallback(void* param)
	{
		StartCapture3DOperator* op = (StartCapture3DOperator*)param;
		op->_OnCallback();
	}
private:
	void _OnCallback()
	{
		AppStatus appStatus = GlobalInstance()->getAppStatus();
		if (appStatus == AppEnd || appStatus == AppNotInit
			|| appStatus == AppDspFailed || appStatus == AppCaptureRun)
		{
			BaseCore_LogInstance()->LogString("Error: can not start capture, appStatus is %d", appStatus);
			Response(false);
			return;
		}
		//start to capture
		GlobalInstance()->SetAppStatus(AppCaptureRun);
		BaseCore_LogInstance()->LogString("Start capture...");
		IFCMemory* memory = GlobalInstance()->fcMemory;
		//get seq
		if (!GetSeq(memory))
		{
			GlobalInstance()->SetAppStatus(AppGetSeqFailed);
			BaseCore_LogInstance()->LogString("Error: get seq failed");
			Response(false);
			return;
		}
		
		if (!RequestCapture3D(memory,GlobalInstance()->calibFile.c_str(),GlobalInstance()->g3dFile.c_str()))
		{
			GlobalInstance()->SetAppStatus(AppCaptureFailed);
			Response(false);
			return;
		}
		return;
	}
	bool GetSeq(IFCMemory* memory)
	{
		IHWCtrl* hw = GlobalInstance()->hwCtrl;
		if (HW_SUCCESS == hw->GetSeq(memory))
			return true;
		return false;
	}

	bool RequestCapture3D(IFCMemory* memory,const char* calibFile,const char* g3dFile)
	{
		AbstractProxy* modelProxy = NULL;
		if (!GlobalInstance()->modelManager->get_Proxy(&modelProxy))
			return false;

		JsonMsg json;
		this->AppendRequestHeader(&json);
		JsonSerFCUser serUser(m_user);
		json.AppendItemNameObj("user", &serUser);
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
		GlobalInstance()->SetAppStatus(r ? AppCaptureSuccess : AppCaptureFailed);
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
		if (this->m_inProxy!=NULL)
			this->m_inProxy->SendJson(&json);
		this->SetOver();
	}
};