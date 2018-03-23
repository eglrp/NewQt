#pragma once
#include "WSFModel.h"
#include "MergePC.h"
class ModelManager
{
private:
	IHWCtrl* m_hwCtrl;
	IFCMemory* m_memory;
	IMergePC* m_mergePC;
public:
	ModelManager(IFCMemory* memory, IHWCtrl* hwCtrl)
	{
		m_memory = memory;
		m_hwCtrl = hwCtrl;
		CreateMergePC(&m_mergePC);
	}
	~ModelManager()
	{
		DestoryMergePC(&m_mergePC);
	}
	bool Init(char* sysFile, char* g3dFile)
	{
		bool r = m_mergePC->Init(sysFile, g3dFile);
		return true;
	}
public:
	bool GetSeq()
	{
		if (HW_SUCCESS != m_hwCtrl->GetSeq(m_memory))
		{
			return false;
		}
		//Delete previous cap images
		char delseqfile[200];
		sprintf_s(delseqfile, "del %s\\Log\\cap\\* /f/q", BaseCore_AppPath().c_str());
		system(delseqfile);
		//Save current cap images
		char path[300];
		sprintf(path, "%s\\log\\cap", BaseCore_AppPath().c_str());
		FCUtility::CreateDir(path);
		m_memory->SaveCapture(path, true);
		return true;
	}
	bool Capture3D(CWSFModel* model)
	{
		bool rtn = m_mergePC->RunAll(m_memory, model);	
		return rtn;
	}

	
};