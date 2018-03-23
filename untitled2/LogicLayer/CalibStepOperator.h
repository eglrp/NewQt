#pragma once
#include "FPProtocol.h"
#include "Global.h"
#include "CallbackCommand.h"
class CalibStepOperator :public AbstractOperator
{
public:
	CalibStepOperator(const char* key) :AbstractOperator(OP_CalibStep, key)
	{
	}
public:
	//receive request from center server
	virtual bool DoReceiveRequest(JsonMsg* json)
	{
		if (json->m_jsonItems[2]->m_type != JsonItemType::STRING) return false;
		int unit = JsonConvert::Str2Int(json->m_jsonItems[2]->m_strval);

		CalibManager* cm = GlobalInstance()->calibManager;
		cm->m_stepCallback = stepCallback;
		cm->m_callbackParam = this;
		cm->StartCalibStep(unit);
		GlobalInstance()->SetAppStatus(AppStatus::AppCalibStepRun);//then cmd thread will run calib step and callback
	}
public:
	static void stepCallback(void* param, CalibResult* cr)
	{
		CalibStepOperator* op = (CalibStepOperator*)param;
		op->Response(cr);
	}
private:
	void Response(CalibResult* cr)
	{
		//response result to center server
		JsonMsg json;
		this->AppendResponseHeader(&json);
		char buf[2];
		json.AppendItemNameVal("curunit", itoa(cr->m_curUnit,buf,10));
		json.AppendItemNameVal("unitstep", itoa(cr->m_unitStep,buf,10));
		json.AppendItemNameVal("crtype", itoa(cr->m_crType, buf, 10));
		json.AppendItemNameVal("rtn", cr->m_success ? "1" : "0");
		this->m_inProxy->SendJson(&json);
		this->SetOver();
		//
		GlobalInstance()->SetAppStatus(AppStatus::AppCalibDspRun);
	}

};