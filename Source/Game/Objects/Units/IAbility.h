#ifndef FF_JS_CSKILL_H_
#define FF_JS_CSKILL_H_

#include "../Base/IFunctor.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../GUI Objects/CButton.h"

class CSprite;
class CUnitObject;
class CStatusEffect;

class IAbility : public IFunctor
{
	private:
		CSprite* m_Icon;
		CUnitObject* m_Owner;
		unsigned long m_EnableTime;

	protected:
		CButton m_Frame;

	public:
		IAbility();

		void Enable()					{ m_EnableTime = GetTickCount(); }
		void Disable()					{ m_EnableTime = ULONG_MAX; }

		bool GetEnabled()				{ return (m_EnableTime <= GetTickCount()) ? (true) : (false); }

		CSprite* GetIcon()				{ return m_Icon;	}
		CUnitObject* GetOwner()			{ return m_Owner;	}
		unsigned long GetCooldown()		{ return m_EnableTime; }
		CButton* GetFrame()				{ return &m_Frame; }

		void SetIcon(CSprite* _sprite)		{ m_Icon = _sprite; }
		void SetOwner(CUnitObject* _owner)	{ m_Owner = _owner;	}
		void SetCooldown(float _seconds) { m_EnableTime = GetTickCount() + (unsigned long)(_seconds * 1000); }

		void operator()() { }
};

class CSkill_GOD_Suicide : public IAbility
{
	public:
		CSkill_GOD_Suicide(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceEgg.spr")); }
		~CSkill_GOD_Suicide();

		void operator()();
};

class CSkill_GOD_Nuke : public IAbility
{
	public:
		CSkill_GOD_Nuke(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceFruit.spr")); }
		~CSkill_GOD_Nuke();

		void operator()();
};

class CSkill_GOD_Nuke_Override : public IAbility { void operator()(); };

////////////////////////////////////////
// REAL SPELLS
////////////////////////////////////////

class CSkill_Toast_Crust : public IAbility
{
	public:
		CSkill_Toast_Crust(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/CrustIcon.spr")); 
			m_Frame.SetString("Crust\nIncreases Defense for 10 seconds.");}
		~CSkill_Toast_Crust();

		void operator()();
};

class CSkill_Toast_Yeast : public IAbility
{
	public:
		CSkill_Toast_Yeast(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/YeastIcon.spr"));
			m_Frame.SetString("Yeast\nIncreases all stats for 20 seconds. Can only be used ONCE.");}
		~CSkill_Toast_Yeast();

		void operator()();
};

class CSkill_Toast_SlicedBread : public IAbility
{
	public:
		CSkill_Toast_SlicedBread(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/SlicedBreadIcon.spr"));
			m_Frame.SetString("Sliced Bread\nSplits this toast unit into 3 toast units. Available at level 4.");}
		~CSkill_Toast_SlicedBread();

		void operator()();
};

////////////////////////////////////////

class CSkill_Sunny_Benedict : public IAbility
{
	public:
		CSkill_Sunny_Benedict(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/FlagIcon.spr"));
			m_Frame.SetString("Benedict\nSlightly increases health.");}
		~CSkill_Sunny_Benedict();

		void operator()();
};

class CSkill_Sunny_Caviar : public IAbility
{
	public:
		CSkill_Sunny_Caviar(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/CaviarIcon.spr"));
			m_Frame.SetString("Caviar\nIncreases the movement speed of this unit for 20 seconds.");}
		~CSkill_Sunny_Caviar();

		void operator()();
};

class CSkill_Sunny_OverEasy : public IAbility
{
	public:
		CSkill_Sunny_OverEasy(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/OverEasyIcon.spr"));
			m_Frame.SetString("Over Easy\nIncreases the movement speed of all ranged units nearby.");}
		~CSkill_Sunny_OverEasy();

		void operator()();
};

////////////////////////////////////////

class CSkill_Scrambled_BeatIt : public IAbility
{
	public:
		CSkill_Scrambled_BeatIt(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/BenedictIcon.spr"));
			m_Frame.SetString("Beat It\nIncreases movement speed for 12 seconds.");}
		~CSkill_Scrambled_BeatIt();

		void operator()();
};

class CSkill_Scrambled_Raw : public IAbility
{
	public:
		CSkill_Scrambled_Raw(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/FloatingEggIcon.spr"));
			m_Frame.SetString("Raw\nUnit becomes immune to attacks in exchange for being stunned.  Recovers in 8 seconds.");}
		~CSkill_Scrambled_Raw();

		void operator()();
};

class CSkill_Scrambled_Easter : public IAbility
{
	public:
		CSkill_Scrambled_Easter(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/EasterIcon.spr"));
			m_Frame.SetString("Easter\nMakes unit invisible for 10 seconds.");}
		~CSkill_Scrambled_Easter();

		void operator()();
};

////////////////////////////////////////

class CSkill_Sausage_Kielbasa : public IAbility
{
	public:
		CSkill_Sausage_Kielbasa(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/KBasa.spr"));
			m_Frame.SetString("Kielbasa\nIncreases Attack for 12 seconds.");}
		~CSkill_Sausage_Kielbasa();

		void operator()();
};

class CSkill_Sausage_Bratwurste : public IAbility
{
	public:
		CSkill_Sausage_Bratwurste(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/BratIcon.spr"));
			m_Frame.SetString("Bratwurste\nDamages all units within melee range at once.");}
		~CSkill_Sausage_Bratwurste();

		void operator()();
};

class CSkill_Sausage_SPAM : public IAbility
{
	public:
		CSkill_Sausage_SPAM(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/SpamIcon.spr"));
			m_Frame.SetString("SPAM\nSplits this unit into 5 smaller, weaker units.");}
		~CSkill_Sausage_SPAM();

		void operator()();
};

////////////////////////////////////////

class CSkill_Ham_Butcher : public IAbility
{
	public:
		CSkill_Ham_Butcher(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/ButcherIcon.spr"));
			m_Frame.SetString("Butcher\nFor 12 seconds, each enemy unit hit adds to this unit's attack power.");}
		~CSkill_Ham_Butcher();

		void operator()();
};

class CSkill_Ham_RumpRoast : public IAbility
{
	public:
		CSkill_Ham_RumpRoast(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/RumpRoastIcon.spr"));
			m_Frame.SetString("Rump Roast\nRegain all health in exchange for no experience and all status effects gone.");}
		~CSkill_Ham_RumpRoast();

		void operator()();
};

class CSkill_Ham_HoneyRoasted : public IAbility
{
	public:
		CSkill_Ham_HoneyRoasted(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/HoneyRoastedIcon.spr"));
			m_Frame.SetString("Honey Roasted\nMovement is decreased, attack is increased.");}
		~CSkill_Ham_HoneyRoasted();

		void operator()();
};

////////////////////////////////////////

class CSkill_Pancake_Buttermilk : public IAbility
{
	public:
		CSkill_Pancake_Buttermilk(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/ButtermilkIcon.spr"));
			m_Frame.SetString("Buttermilk\nHeals a friendly unit equal to half its health.");}
		~CSkill_Pancake_Buttermilk();

		void operator()();
};
class CSkill_Pancake_Buttermilk_Override : public IAbility
{
public:
	CSkill_Pancake_Buttermilk_Override(CUnitObject* _owner)	{ SetOwner(_owner);	}
	void operator()();
};

class CSkill_Pancake_Flappjack : public IAbility
{
	public:
		CSkill_Pancake_Flappjack(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/FlapjackIcon.spr"));
			m_Frame.SetString("Flapjack\nInstantly returns this unit to its home stove.");}
		~CSkill_Pancake_Flappjack();

		void operator()();
};

class CSkill_Pancake_Topping : public IAbility
{
	public:
		CSkill_Pancake_Topping(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/ToppingIcon.spr"));
			m_Frame.SetString("Topping\nLevels up a friendly unit in exchange for some of its movement speed.");}
		~CSkill_Pancake_Topping();

		void operator()();
};

class CSkill_Pancake_Topping_Click : public IAbility
{
public:
	CSkill_Pancake_Topping_Click(CUnitObject* _owner) { SetOwner(_owner); }
	void operator()();
};

////////////////////////////////////////

class CSkill_Waffle_Sap : public IAbility
{
	public:
		CSkill_Waffle_Sap(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/SapIcon.spr"));
			m_Frame.SetString("Sap\nChoose an enemy unit to Sap.  That unit and all units around lose movement speed.");}
		~CSkill_Waffle_Sap();

		void operator()();
};
class CSkill_Waffle_Sap_Override : public IAbility
{
public:
	CSkill_Waffle_Sap_Override(CUnitObject* _owner) { SetOwner(_owner); }
	void operator()();
};

class CSkill_Waffle_BelgianSurprise : public IAbility
{
	public:
		CSkill_Waffle_BelgianSurprise(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/SurpriseIcon.spr"));
			m_Frame.SetString("Belgian Surprise\nIncreases Attack for 20 seconds.");}
		~CSkill_Waffle_BelgianSurprise();

		void operator()();
};

class CSkill_Waffle_FlavoredSyrup : public IAbility
{
	public:
		CSkill_Waffle_FlavoredSyrup(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/AmberIcon.spr"));
			m_Frame.SetString("Amber\nStuns an enemy unit for 4 seconds.");}
		~CSkill_Waffle_FlavoredSyrup();

		void operator()();
};

class CSkill_Waffle_FlavoredSyrup_Click : public IAbility
{
public:
	CSkill_Waffle_FlavoredSyrup_Click(CUnitObject* _owner) { SetOwner(_owner); }
	void operator()();
};

//////////////////////////////////////////////////////////
//						French Toast
/////////////////////////////////////////////////////////

class CSkill_FrenchToast_EggWash : public IAbility
{
	public:
		CSkill_FrenchToast_EggWash(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/EggWashIcon.spr"));
			m_Frame.SetString("Egg Wash\nClears all negative status effects from a unit.");}
		~CSkill_FrenchToast_EggWash();

		void operator()();
};

class CSkill_FrenchToast_Baguette : public IAbility
{
	public:
		CSkill_FrenchToast_Baguette(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/BagetteIcon.spr"));
			m_Frame.SetString("Baguette\nIncreases Defense for 15 seconds.");}
		~CSkill_FrenchToast_Baguette();

		void operator()();
};

class CSkill_FrenchToast_PowderedSugar : public IAbility
{
	public:
		CSkill_FrenchToast_PowderedSugar(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/PowderedSugarIcon.spr"));
			m_Frame.SetString("Powdered Sugar\nIncreases movement speed for 10 seconds.");}
		~CSkill_FrenchToast_PowderedSugar();

		void operator()();
};

//////////////////////////////////////////////////////////
//						Blueberry Muffin
/////////////////////////////////////////////////////////

class CSkill_Blueberry_Scone : public IAbility
{
	public:
		CSkill_Blueberry_Scone(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/SconeIcon.spr"));
			m_Frame.SetString("Scone\nIncreases Defense for 15 seconds.");}
		~CSkill_Blueberry_Scone();

		void operator()();
};

class CSkill_Blueberry_Shrapnel : public IAbility
{
	public:
		CSkill_Blueberry_Shrapnel(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/ShrapnelIcon.spr"));
			m_Frame.SetString("Shrapnel\nFires a projectile to damage a unit, and surrounding units.");}
		~CSkill_Blueberry_Shrapnel();

		void operator()();
};

class CSkill_Blueberry_Shrapnel_Click : public IAbility
{
public:
	CSkill_Blueberry_Shrapnel_Click(CUnitObject* _owner) { SetOwner(_owner); }
	void operator()();
};

class CSkill_Blueberry_BlueAndSweet : public IAbility
{
	public:
		CSkill_Blueberry_BlueAndSweet(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/BlueAndSweetIcon.spr"));
			m_Frame.SetString("Blue and Sweet\nFor 10 seconds, this unit gains health for damage it does.");}
		~CSkill_Blueberry_BlueAndSweet();

		void operator()();
};

//////////////////////////////////////////////////////////
//						Grapefruit
/////////////////////////////////////////////////////////

class CSkill_Grapefruit_Citrus : public IAbility
{
	public:
		CSkill_Grapefruit_Citrus(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/CitrusIcon.spr"));
			m_Frame.SetString("Citrus\nDecreases defense of an enemy unit.");}
		~CSkill_Grapefruit_Citrus();

		void operator()();
};

class CSkill_Grapefruit_Citrus_Click : public IAbility
{
public:
	CSkill_Grapefruit_Citrus_Click(CUnitObject* _owner) { SetOwner(_owner); }
	void operator()();
};

class CSkill_Grapefruit_BarrelRoll : public IAbility
{
	public:
		CSkill_Grapefruit_BarrelRoll(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/BarrelRollIcon.spr"));
			m_Frame.SetString("Barrel Roll\nQuickly rolls this unit in the direction of your next click.");}
		~CSkill_Grapefruit_BarrelRoll();

		void operator()();
};

class CSkill_Grapefruit_BarrelRoll_Click : public IAbility 
{
public:
	CSkill_Grapefruit_BarrelRoll_Click(CUnitObject* _owner)	{ SetOwner(_owner);	}
	void operator()();
};


class CSkill_Grapefruit_ForbiddenFruit : public IAbility
{
	public:
		CSkill_Grapefruit_ForbiddenFruit(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/ForbiddenFruitIcon.spr"));
			m_Frame.SetString("Forbidden Fruit\nThis unit dies in exchange for doubling the stats of another. The other unit dies in 10 seconds.");}
		~CSkill_Grapefruit_ForbiddenFruit();

		void operator()();
};

class CSkill_Grapefruit_ForbiddenFruit_Click : public IAbility 
{
public:
	CSkill_Grapefruit_ForbiddenFruit_Click(CUnitObject* _owner)	{ SetOwner(_owner);	}
	void operator()();
};

//////////////////////////////////////////////////////////
//						Banana
/////////////////////////////////////////////////////////

class CSkill_Banana_Split : public IAbility
{
	public:
		CSkill_Banana_Split(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/SplitIcon.spr"));
			m_Frame.SetString("Split\nSplits this unit into 2 banana units. Available at level 5.");}
		~CSkill_Banana_Split();

		void operator()();
};

class CSkill_Banana_Peel : public IAbility
{
	public:
		CSkill_Banana_Peel(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/PeelIcon.spr"));
			m_Frame.SetString("Peel\nPermanently increases movement speed in exchange for defense. Can only use once.");}
		~CSkill_Banana_Peel();

		void operator()();
};

class CSkill_Banana_Potassium : public IAbility
{
	public:
		CSkill_Banana_Potassium(CUnitObject* _owner)	{ SetOwner(_owner);	
			SetIcon(CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/Skills/PotassiumIcon.spr"));
			m_Frame.SetString("Potassium\nPoisions an enemy unit and does damage over time.");}
		~CSkill_Banana_Potassium();

		void operator()();
};

class CSkill_Banana_Potassium_Click : public IAbility 
{
public:
	CSkill_Banana_Potassium_Click(CUnitObject* _owner)	{ SetOwner(_owner);	}
	void operator()();
};



//Forward Declaring Status Effect Functions
bool StartEffect_Sunny_OverEasy(CUnitObject* _unit);
bool EndEffect_Sunny_OverEasy(CUnitObject* _unit);
bool BeginPotassium(CUnitObject* unit, CStatusEffect* effect);
bool EndPotassium(CUnitObject* unit, CStatusEffect* effect);
bool BeginForbidden(CUnitObject* unit);
bool BeginCitrus(CUnitObject* unit);
bool EndCitrus(CUnitObject* unit);
bool BeginBlueAndSweet(CUnitObject* unit);
bool EndBlueAndSweet(CUnitObject* unit);
bool StartEffect_Waffle_Sap(CUnitObject* _unit);
bool EndEffect_Waffle_Sap(CUnitObject* _unit);


bool StartEffect_Waffle_FlavoredSyrup(CUnitObject* _unit);
bool EndEffect_Waffle_FlavoredSyrup(CUnitObject* _unit);

#endif