//////////////////////////////////////////////////////
// File: "CStatusEffect.cpp"
// Creator: MG
// Creation Date: 9/27/09
// Last Modified: 9/27/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#include "CStatusEffect.h"
#include <windows.h>
#include "IAbility.h"

CStatusEffect::FunctionPointer CStatusEffect::LookupList[32];
CStatusEffect::SenderFunctionPointer CStatusEffect::LookupSenderList[12];


void CStatusEffect::LoadStatusEffectTable()
{
	LookupList[0] = NULL;
	LookupList[1] = StartEffect_Sunny_OverEasy;
	LookupList[2] = EndEffect_Sunny_OverEasy;
	LookupList[3] = BeginBlueAndSweet;
	LookupList[4] = EndBlueAndSweet;
	LookupList[5] = BeginForbidden;
	LookupList[6] = BeginCitrus;
	LookupList[7] = EndCitrus;
	LookupList[8] = StartEffect_Waffle_Sap;
	LookupList[9] = EndEffect_Waffle_Sap;

	LookupSenderList[0] = NULL;
	LookupSenderList[1] = BeginPotassium;
	LookupSenderList[2] = EndPotassium;
}

CStatusEffect::CStatusEffect()
{
	m_TimeUp = 0;
	Begin = NULL;
	Update = NULL;
	End = NULL;
	BeginSender = NULL;
	UpdateSender = NULL;
	EndSender = NULL;
	m_Sprite = NULL;
	m_Emitter = NULL;
}

void CStatusEffect::SetTimer(int _seconds)
{
	m_TimeUp = GetTickCount() + (long)(_seconds*1000);
}

bool CStatusEffect::Process(CUnitObject* _unit)
{
	if(m_TimeUp != 0 && (long)GetTickCount() > m_TimeUp)
	{
		if(End)
			End(_unit);
		else if(EndSender)
			EndSender(_unit, this);
		return false;
	}

	if(Update)
	{
		if(Update(_unit))
		{
			return true;
		}
		else
		{
			if(End)
				End(_unit);
			else if(EndSender)
				EndSender(_unit, this);
			return false;
		}
	}
	else if(UpdateSender)
	{
		if(UpdateSender(_unit, this))
		{
			return true;
		}
		else
		{
			if(End)
				End(_unit);
			else if(EndSender)
				EndSender(_unit, this);
			return false;
		}
	}
	return true;
}