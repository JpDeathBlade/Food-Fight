//////////////////////////////////////////////////////
// File: "CStatusEffect.h"
// Creator: MG
// Creation Date: 9/27/09
// Last Modified: 9/27/09
// Last Modified By: MG
//////////////////////////////////////////////////////

#ifndef CSTATUSEFFECT_H_
#define CSTATUSEFFECT_H_
#include "../Base/IReferenceable.h"
#include "../Base/IAnimable.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"

class CUnitObject;
class CParticleEmitter;

class CStatusEffect : public IReferenceable
{
private:
	CSprite* m_Sprite;
	long m_TimeUp;
	CParticleEmitter* m_Emitter;

public:
	typedef bool (*FunctionPointer)(CUnitObject*);
	typedef bool (*SenderFunctionPointer)(CUnitObject*, CStatusEffect*);

	CStatusEffect();
	static void LoadStatusEffectTable();
	static CStatusEffect::FunctionPointer LookupList[32];
	static CStatusEffect::SenderFunctionPointer LookupSenderList[12];
	static FunctionPointer GetStatusEffect(short index)				{ return LookupList[index]; }
	static SenderFunctionPointer GetStatusEffectSender(short index)	{ return LookupSenderList[index]; }
	FunctionPointer Begin;
	FunctionPointer Update;
	FunctionPointer End;
	SenderFunctionPointer BeginSender;
	SenderFunctionPointer UpdateSender;
	SenderFunctionPointer EndSender;
	bool Process(CUnitObject* _unit);

	void SetSprite(CSprite* _sprite) { m_Sprite = _sprite; }
	void SetTimer(int _seconds);
	void SetParticleEmitter(CParticleEmitter* _emitter) { m_Emitter = _emitter; }

	CSprite* GetSprite() { return m_Sprite; }
	CParticleEmitter* GetParticleEmitter() { return m_Emitter; }
};

class CDamageOverTimeEffect : public CStatusEffect
{
private:
	short damage;
	long interval;
	unsigned long startTime;
public:
	CDamageOverTimeEffect() { startTime = GetTickCount(); }
	void SetInterval(float _seconds) { interval = (long)(_seconds*1000); }
	void SetDamage(short _damage)	{ damage = _damage; }
	void SetStartTime(unsigned long _startTime) { startTime = _startTime; }

	unsigned long GetStartTime() { return startTime; }
	short GetDamage() { return damage; }
	long GetInterval() { return interval; }
};

class CExpTrackEffect : public CStatusEffect
{
private:
	unsigned short lastExp;
	short tracker;
public:
	void SetTracker(short _tracker) { tracker = _tracker; }
	void AddTracker(short _add) { tracker += _add; }
	short GetTracker() { return tracker; }
	unsigned short GetLastExp() { return lastExp; }
	void SetLastExp(unsigned short _lastExp) { lastExp = _lastExp; }
};


#endif