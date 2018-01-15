//////////////////////////////////////////////////////
// File: "CProfileManager.h"
// Creator: AB
// Creation Date: 9/15/09
// Last Modified: 9/16/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef CPROFILEMANAGER_H_
#define CPROFILEMANAGER_H_

#include <map>
#include <string>
using namespace std;

#define PROFILE_MANAGER  CProfileManager::GetInstance()

//An Enum for the index of our different keys
enum eKeyBindings{ UP, DOWN, LEFT, RIGHT, MOVE, ATTACK, STOP, STOVE, RALLY_POINT, COOKBOOK};
enum ELanguage { EL_English, EL_Spanish, EL_MAX };

struct tProfile
{
	char szName[32];
	bool bFullscreen;
	bool bAchievements[10];
	unsigned char MusicVolume, EffectVolume, VoiceVolume, UserInterfaceVolume;
	unsigned int unUnitsKilled;
	unsigned int unUnitsBuilt[12];
	unsigned char unKeyBindings[10];
	unsigned int unCurrentLevel, unSlot;
	ELanguage Language;

	tProfile& operator=(const tProfile& Source);
};

class CProfileManager
{
	//the array for the user profiles
	tProfile m_arrUserProfiles[3];

	//the index for the current profile
	int m_nCurrentProfile;

	map<unsigned char, string> m_DIKToString;

	///////////////////////////////////////////////////////////////
	// Function: LoadProfile
	//
	// In: void
	//
	// Out: bool for whether it succeeded
	//
	// Purpose: To load the user profiles, if the profile file
	//			has been deleted it will create it with defaults.
	///////////////////////////////////////////////////////////////
	static bool LoadProfiles(void);

	///////////////////////////////////////////////////////////////
	// Function: SaveProfiles
	//
	// In: void
	//
	// Out: bool for whether it succeeded
	//
	// Purpose: To save the user profiles.
	///////////////////////////////////////////////////////////////
	static bool SaveProfiles(void);

	//Disabling the constructors, assignment operator, and destructor to turn the class into a singleton.
	CProfileManager(void);
	CProfileManager(const CProfileManager& obj) {}
	CProfileManager& operator = (const CProfileManager& obj) {}
	~CProfileManager(void);

public:

	///////////////////////////////////////////////////////////////
	// Function: GetInstance
	//
	// In: void
	//
	// Out: The singleton of the CProfilerManager
	//
	// Purpose: A static function that returns the singleton of the
	//			CProfileManager for different uses.
	///////////////////////////////////////////////////////////////
	static CProfileManager* GetInstance(void);

	///////////////////////////////////////////////////////////////
	// Function: GetCurrentProfile
	//
	// In: void
	//
	// Out: tProfile* to the current profile
	//
	// Purpose: Returns a pointer to the current profile.
	////////////////////////////////////////////////////////////////
	tProfile* GetCurrentProfile(void);

	///////////////////////////////////////////////////////////////
	// Function: InitProfileManager
	//
	// In: void
	//
	// Out: void
	//
	// Purpose: Sets up the Profile Manager including loading the
	//			profiles.
	///////////////////////////////////////////////////////////////
	void InitProfileManager(void);

	///////////////////////////////////////////////////////////////
	// Function: ShutdownProfileManager
	//
	// In: void
	//
	// Out: void
	//
	// Purpose: Shuts down the Profile Manager including saving 
	//			any changes made to the profiles.
	////////////////////////////////////////////////////////////////
	void ShutdownProfileManager(void);

	///////////////////////////////////////////////////////////////
	// Function: SetCurrentProfile
	//
	// In: int for the index of the profile you want to be the 
	//		current profile
	//
	// Out: void
	//
	// Purpose: Changes all of the setting for the game based 
	//			to the new current's settings.
	////////////////////////////////////////////////////////////////
	void SetCurrentProfile(int nCurrentProfile);

	///////////////////////////////////////////////////////////////
	// Function: SetToDefault
	//
	// In: int for the index of the profile to change
	//
	// Out: void
	//
	// Purpose: Sets the passed in profile to all of the default 
	//			settings.
	///////////////////////////////////////////////////////////////
	void SetToDefault(int nProfileIndex);

	const char* GetStringFromDIK(unsigned char ucDIK);

	tProfile* GetProfile(int nIndex);
};

#endif