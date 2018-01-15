//////////////////////////////////////////////////////
// File: "CUnitObject.h"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/22/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef _CUnitObject_H_
#define _CUnitObject_H_
#include "..\Base\CGameObject.h"
#include "..\Base\IAttackable.h"
#include "..\GUI Objects\CursorInterfaces.h"
#include "../../../Managers/Bad Chef/Networking/INetworkable.h"

#include "IAbility.h"

#include <list>

enum EForceType { NO_FORCE, SPEED_FORCE, KNOCKBACK_FORCE };

class IUnitAICommand;
class CPathNode;
class CObjectManager;
class CStatusEffect;
class CParticleEmitter;

#define SKILL_AMOUNT 3

class CSound;

class CUnitObject : public CGameObject, public ISelectable, public IHoverable, public IAttackable
{
private:
	unsigned short	m_usDefense;
	unsigned short	m_usAttack;
	float			m_fAttackSpeed;
	float			m_fEvade;
	float			m_fRange;
	float			m_fPerceptionRange;
	float			m_fPerceptionTimer;
	float			m_fAccumulator;
	unsigned short	m_MovementSpeed;
	CPathNode*		m_pNextNode;
	bool			m_bBalanceBonus;
	IUnitAICommand* m_UnitAICommand;
	std::string		m_CreationString;
	int				m_nBaseType;
	std::string		m_szBullet;

	//Stun Varables
	unsigned short m_SavedMovementSpeed;
	float m_SavedAttackSpeed;
	
	//Forces like knockback and speed (barrel roll!)
	D3DXVECTOR2		m_VelocityForce;
	EForceType		m_ForceType;

	CSound* m_AttackSound;
	CSound* m_pVoice;

	float m_AttackTimer;
	float m_fBuffTimer;
	//Modifications to stats
	//Multiplying
	float m_fAttackMultiply;
	float m_fDefenseMultiply;
	float m_fMovementMultiply;
	float m_fEvadeMultiply;
	//Adding
	float m_fAttackAdd;
	float m_fDefenseAdd;
	float m_fMovementAdd;
	float m_fEvadeAdd;
	bool  m_bNearbyUnits[4];
	bool  m_bIsDead;
	float m_fDeadTimer;

	IAbility* m_Skills[SKILL_AMOUNT];

	CGameObject* m_pArmRight;
	CGameObject* m_pArmLeft;
	CGameObject* m_pLegs;
	CGameObject* m_pFace;

	std::list<CParticleEmitter*> m_UnderParticleEmitters;
	std::list<CParticleEmitter*> m_OverParticleEmitters;
	CParticleEmitter* m_pCombo;

	std::list<CStatusEffect*> m_ActiveStatusEffects;

public:

	//A public enum for the base type of unit
	enum EUnitBaseResource	{ BASE_MEAT, BASE_EGG, BASE_FRUIT, BASE_WHEAT };

	CUnitObject(void); // Default Constructor
	CUnitObject(const CUnitObject& Source); // Copy Constructor
	CUnitObject& operator=(const CUnitObject& Source); // Assignment Operator
	~CUnitObject(void); // Destructor

	void SetFace(CGameObject* _face);
	void SetLegs(CGameObject* _legs);
	void SetArmLeft(CGameObject* _arm);
	void SetArmRight(CGameObject* _arm);

	void OnCollision(IObject* Object, D3DXVECTOR2 Overlap);	float GetZDepth() const;	IAbility* GetSkill(int _index)				{ return m_Skills[_index];		}
	void SetSkill(int _index, IAbility* _skill)	{ m_Skills[_index] = _skill;	}

	float GetAttackTimer()					{ return m_AttackTimer;		}
	void  SetAttackTimer(float _attack)		{ m_AttackTimer = _attack;	}

	int Attack(IAttackable* _pTarget);

	void SetForce(EForceType _type, D3DXVECTOR2 _force) { m_ForceType = _type; m_VelocityForce = _force; }
	EForceType GetForceType() { return m_ForceType; }
	D3DXVECTOR2 GetForceVelocity() { return m_VelocityForce; }

	CSound* GetAttackSound() const;
	void SetAttackSound(CSound* AttackSound);

	CSound* GetVoice(void) const	{ return m_pVoice; }
	void SetVoice(CSound* pVoice)	{ m_pVoice = pVoice; }

	void PlayVoice(void);

	//Functions to stun and recover a unit
	void Stun();
	void Recover();

	void AddParticleEmitterOver(CParticleEmitter* _emitter);
	void AddParticleEmitterUnder(CParticleEmitter* _emitter);
	void RemoveParticleEmitterOver(CParticleEmitter* _emitter);
	void RemoveParticleEmitterUnder(CParticleEmitter* _emitter);

	///////////////////////////////////////////////////////////////
	// Function: Update
	//
	// In: A float that is the time that has elapsed since the last update.
	//
	// Out: void
	//
	// Purpose: To Update the unit based on time.
	///////////////////////////////////////////////////////////////
	void Update(float Delta);

	///////////////////////////////////////////////////////////////
	// Function: UpdateNetworked
	//
	// In: A float that is the time that has elapsed since the last update.
	//
	// Out: void
	//
	// Purpose: To Update the unit, as a guest unit, based on time
	///////////////////////////////////////////////////////////////
	void UpdateNetworked(float dt);

	///////////////////////////////////////////////////////////////
	// Function: SendUpdatePacket
	//
	// In: void
	//
	// Out: void
	//
	// Purpose: To send a packet with current information about this unit
	///////////////////////////////////////////////////////////////
	void SendUpdatePacket();

	///////////////////////////////////////////////////////////////
	// Function: Draw
	//
	// In: void
	//
	// Out: void
	//
	// Purpose: Renders the unit based on its position and animation.
	///////////////////////////////////////////////////////////////
	void Draw(void);

	///////////////////////////////////////////////////////////////
	// Function: AddStatusEffect
	//
	// In: The status effect that will be added
	//
	// Out: bool
	//
	// Purpose: Begins a status effect and adds it to the list of status effects
	///////////////////////////////////////////////////////////////
	bool AddStatusEffect(CStatusEffect* _effect);


	///////////////////////////////////////////////////////////////
	// Function: ContextSensitiveClick
	//
	// In: void
	//
	// Out: void
	//
	// Purpose: Handles any special actions that the unit needs
	//			to do while it is being hovered over.
	///////////////////////////////////////////////////////////////
	virtual void ContextSensitiveClick(void);

	///////////////////////////////////////////////////////////////
	// Function: CheckPerceptionRange
	//
	// In: An IObject pointer to check to see if it is in range,
	//
	// Out: A boolean to tell whether or not we found one
	//
	// Purpose: Searches the area around the unit, using the 
	//			PerceptionRange as the radius of that search.
	///////////////////////////////////////////////////////////////
	static bool CheckPerceptionRange(IObject* pObject, CUnitObject* pCaller);

	///////////////////////////////////////////////////////////////
	// Function: CheckNearbyFriendlyUnits
	//
	// In: An IObject pointer to check to see if it is in range,
	//
	// Out: A boolean to tell whether or not we found one
	//
	// Purpose: Searches the area around the unit, using the 
	//			PerceptionRange as the radius of that search.
	///////////////////////////////////////////////////////////////
	static bool CheckNearbyFriendlyUnits(IObject* pObject, CUnitObject* pCaller);

	///////////////////////////////////////////////////////////////
	// Function: ScaleStats
	//
	// In: A float representing the percentage the stats are modified by
	//
	// Out: void
	//
	// Purpose: Used to modify stats for buffs or during creation 
	//			after the cooking-minigame.
	//			NOTE: The unit will not hold data to revert these stats
	///////////////////////////////////////////////////////////////
	void ScaleStats(float _percentage);


	///////////////////////////////////////////////////////////////
	// Function: ChangeAvailableSkills
	//
	// In: The number of skills a unit has available
	//
	// Out: void
	//
	// Purpose: Changes what skills a unit can perform, it is called
	//			when a unit is created
	///////////////////////////////////////////////////////////////
	void ChangeAvailableSkills(int _numSkills);

	/////////////////////Accessors and Mutators////////////////////
	unsigned short GetBaseMovement(void) const		{ return m_MovementSpeed; }
	void SetBaseMovement(unsigned short _movement)	{ m_MovementSpeed = _movement; }
	unsigned short GetMovementSpeed(void) const;
	void SetMovementSpeed(unsigned short MovementSpeed);

	IUnitAICommand* GetUnitAICommand(void) const;
	void SetUnitAICommand(IUnitAICommand* UnitAICommand);

	std::list<CStatusEffect*>* GetStatusEffects() { return &m_ActiveStatusEffects; }

	unsigned short GetBaseDefense(void) const		{ return m_usDefense; }
	void SetBaseDefense(unsigned short _def)		{ m_usDefense = _def; }
	unsigned short GetDefense(void) const			{ return (unsigned short)(m_usDefense * m_fDefenseMultiply + m_fDefenseAdd); }
	void SetDefense(unsigned short usDefense)		{ m_usDefense = usDefense; }

	unsigned short GetBaseAttack(void) const		{ return m_usAttack; }
	void SetBaseAttack(unsigned short _atk)			{ m_usAttack = _atk; }
	unsigned short GetAttack(void) const			{ return (unsigned short)(m_usAttack * m_fAttackMultiply + m_fAttackAdd); }
	void SetAttack(unsigned short usAttack)			{ m_usAttack = usAttack; }

	float GetBaseAttackSpeed(void) const			{ return m_fAttackSpeed; }
	void SetBaseAttackSpeed(float _atkspeed)		{ m_fAttackSpeed = _atkspeed; }
	float GetAttackSpeed(void) const				{ return m_fAttackSpeed; }
	void SetAttackSpeed(float fAttackSpeed)			{ m_fAttackSpeed = fAttackSpeed; }

	float GetBaseEvade(void) const					{ return m_fEvade; }
	void SetBaseEvade(float _evade)					{ m_fEvade = _evade; }
	float GetEvade(void) const						{ return m_fEvade * m_fEvadeMultiply + m_fEvadeAdd; }
	void SetEvade(float fEvade)						{ m_fEvade = fEvade; }

	float GetRange(void) const						{ return m_fRange; }
	void SetRange(float fRange)						{ m_fRange = fRange; }

	float GetPerceptionRange(void) const			{ return m_fPerceptionRange; }
	void SetPerceptionRange(float fPerceptionRange)	{ m_fPerceptionRange = fPerceptionRange; }

	float GetPerceptionTimer(void) const			{ return m_fPerceptionTimer; }
	void SetPerceptionTimer(float fPerceptionTimer)	{ m_fPerceptionTimer = fPerceptionTimer; m_fAccumulator = fPerceptionTimer; }

	bool IsBalanced(void) const						{ return m_bBalanceBonus; }
	void SetBalancedBool(bool bBalanceBonus)		{ m_bBalanceBonus = bBalanceBonus; }

	void AddAttackMultiply(float _mod)				{ m_fAttackMultiply += _mod; }
	void AddDefenseMultiply(float _mod)				{ m_fDefenseMultiply += _mod; }
	void AddEvadeMultiply(float _mod)				{ m_fEvadeMultiply += _mod; }
	void AddMovementMultiply(float _mod)			{ m_fMovementMultiply += _mod; }

	void AddAttackMod(float _mod)					{ m_fAttackAdd += _mod; }
	void AddDefenseMod(float _mod)					{ m_fDefenseAdd += _mod; }
	void AddEvadeMod(float _mod)					{ m_fEvadeAdd += _mod; }
	void AddMovementMod(float _mod)					{ m_fMovementAdd += _mod; }

	void SetAnimationType(EAnimation _type);
	void SetState(EUnitState _state);
	void SetDirection(EUnitDirection _direction);

	int GetBaseType(void)							{ return m_nBaseType; }
	void SetBaseType(int nBaseType)					{ m_nBaseType = nBaseType; }

	void SetCreationString(std::string _str)		{ m_CreationString = _str; }
	std::string	GetCreationString()					{ return m_CreationString; }

	void ApplyDamage(short Damage);

	std::string GetBullet(void)						{ return m_szBullet; }
	void SetBullet(const char* szBullet)			{ m_szBullet = szBullet; }

	bool CheckIsDead(void)							{ return m_bIsDead; }
	void SetIsDead(bool bIsDead)					{ m_bIsDead = bIsDead; }
};
#endif