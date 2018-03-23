#pragma once
#include "basecorebasic.h"
#include "FCUtility.h"
#include "ISysCalib.h"
enum CalibResultType
{
	CR_UnitStepRunning = 0,
	CR_UnitCompleted = 1,
};
class CalibResult
{
public:
	CalibResultType m_crType;
	int m_curUnit;
	int m_unitStep;
	bool m_success;
};
typedef void(*ONCOMPLETEDCalibStep)(void* sender, CalibResult* cr);
typedef void(*ONCOMPLETEDCalibUnit)(void* sender, CalibResult* cr);
class CalibUnitState
{
public:
	int m_nextStep;
	bool m_startCapture;
public:
	CalibUnitState(){
		Reset();
	}
	void Reset()
	{
		m_nextStep = 0;
		m_startCapture = false;
	}

};
class CalibManager
{
private:
	std::vector<CalibUnitState*> m_states;
public:
	char* m_boardFile;
	char* m_sysFile;
	static const int MAXUNITSTEP = 9;
	ONCOMPLETEDCalibStep m_stepFunc;
	ONCOMPLETEDCalibUnit m_unitFunc;
	void* m_sender;

	CalibManager()
	{
		int size = BaseCore_CameraConfigInstance()->m_seqParam.size();
		for (int i = 0; i < size; i++)
			m_states.push_back(new CalibUnitState());
	}
	~CalibManager()
	{
		for (int i = 0; i < m_states.size(); i++)
			delete m_states[i];
		m_states.clear();
	}
	void Init(char* boardFile, char* sysFile, void* sender, ONCOMPLETEDCalibStep stepFunc, ONCOMPLETEDCalibUnit unitFunc)
	{
		m_boardFile = boardFile;
		m_sysFile = sysFile;
		m_sender = sender;
		m_stepFunc = stepFunc;
		m_unitFunc = unitFunc;
		ResetAllUnit();
	}
public:
	void ResetAllUnit()
	{
		for (int i = 0; i < m_states.size(); i++)
		{
			m_states[i]->Reset();
		}
	}
	void ResetUnit(int unit)
	{
		m_states[unit]->Reset();
	}
	bool StartCalibStep(int unit)
	{
		CalibUnitState* state = m_states[unit];
		state->m_startCapture = true;
		return true;
	}
	bool IsCaptureForStep(int& unit)
	{
		//check all unit, if there is one unit which state is startcapture, set startcapture to false and output unit number
		for (int i = 0; i < m_states.size(); i++)
		{
			CalibUnitState* state = m_states[i];
			if (!state->m_startCapture) continue;
			state->m_startCapture = false;
			unit = i;
			return true;
		}
		return false;
	}
	//capture image, return next step, if next step is max than MAXSTEP, the caller should know start to calib
	//this function can be overrided, which used for other different devices. e.g 3 cameras
	virtual int CaptureForStep(IFCMemory* memory, int unit)
	{
		CalibUnitState* state = m_states[unit];
		int i = unit * 2;

		if (state->m_nextStep == 0)
		{
			memory->ClearCalib();
		}

		char* p;
		memory->m_cameraBuf[i]->GetFrm(&p);
		memory->m_cameraBuf[i]->AppendCalib(p);

		memory->m_cameraBuf[i + 1]->GetFrm(&p);
		memory->m_cameraBuf[i + 1]->AppendCalib(p);

		state->m_nextStep++;
		return state->m_nextStep;

	}
	virtual bool CalibUnit(IFCMemory* memory, int unit)
	{
		char path[300];
		sprintf(path, "%s\\log\\calib", BaseCore_AppPath().c_str());
		FCUtility::CreateDir(path);
		memory->SaveCalib(unit, path, true);

		ISysCalib* calib = NULL;
		if (!CreateSysCalib(&calib))
			return false;
		calib->Init(m_boardFile, m_sysFile);

		bool rtn = true;
		if (calib->RunUnitCalib(unit, memory) != SC_SUCCESS)
			rtn = false;

		DestorySysCalib(&calib);
		return rtn;
	}





};