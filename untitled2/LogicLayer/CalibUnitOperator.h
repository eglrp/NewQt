#pragma once
#include "stdafx.h"
#include "AbstractOperator.h"
#include "AbstractProxy.h"
#include "Global.h"
#include "CallbackCommand.h"
class CalibUnitOperator :public AbstractOperator
{
public:
	CalibUnitOperator(const char* key) :AbstractOperator(OP_CalibUnit, key)
	{
	}
public:
	//receive request from center server
	virtual bool DoReceiveRequest(JsonMsg* json)
	{
		if (json->m_jsonItems[2]->m_type != JsonItemType::STRING) return false;
		int unit = JsonConvert::Str2Int(json->m_jsonItems[2]->m_strval);

		CalibManager* cm = GlobalInstance()->calibManager;
		cm->m_unitCallback = unitCallback;
		cm->m_callbackParam = this;
		GlobalInstance()->SetAppStatus(AppStatus::AppCalibUnitRun);//then cmd thread will run calib step and callback
	}
	
public:
	static void unitCallback(void* param, int unit)
	{
		CalibUnitOperator* op = (CalibUnitOperator*)param;
		op->_unitCallback(unit);
	}
private:
	void _unitCallback(int unit)
	{
		AppStatus appStatus = GlobalInstance()->getAppStatus();
		if (appStatus != AppCalibUnitRun)
		{
			BaseCore_LogInstance()->LogString("Error: can not start calibunit, appStatus is %d", appStatus);
			Response(false);
			return;
		}
		//start to capture
		BaseCore_LogInstance()->LogString("Start calibunit...");
		IFCMemory* memory = GlobalInstance()->fcMemory;
		if (!RequestCalibUnit(unit, memory, GlobalInstance()->calibFile.c_str(), GlobalInstance()->calibBoardFile.c_str()))
		{
			BaseCore_LogInstance()->LogString("Error: calib unit failed");
			Response(false);
			return;
		}
	}
	bool RequestCalibUnit(int unit,IFCMemory* memory, const char* calibFile, const char* cbFile)
	{
		AbstractProxy* modelProxy = NULL;
		if (!GlobalInstance()->modelManager->get_Proxy(&modelProxy))
			return false;

		char si[2];
		JsonMsg json;
		this->AppendRequestHeader(&json);
		JsonSerFile serCalibFile(calibFile);
		json.AppendItemNameObj("calibfile", &serCalibFile);
		JsonSerFile sercbFile(cbFile);
		json.AppendItemNameObj("cbfile", &sercbFile);
		json.AppendItemNameVal("unit", itoa(unit, si, 10));
		JsonSerFCMemoryCalib serCalib(memory);
		json.AppendItemNameObj("memory", &serCalib);
		modelProxy->AppendOperator(this);
		return modelProxy->SendJson(&json);
	}
public:
	virtual bool DoReceiveResponse(JsonMsg* json)
	{
		if (json->m_jsonItems.size() != 3) return false;

		bool r = JsonConvert::Str2Bool(json->m_jsonItems[2]->m_strval);
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
		//
		GlobalInstance()->SetAppStatus(AppCalibDspRun);
	}

};