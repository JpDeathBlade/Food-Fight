//////////////////////////////////////////////////////
// File: "CProfileManager.cpp"
// Creator: AB
// Creation Date: 9/15/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "CProfileManager.h"
#include <stdio.h>
#include "../String Table/CStringTable.h"
#include "dinput.h"

tProfile& tProfile::operator=(const tProfile& Source)
{
	strcpy_s(szName, 32, Source.szName);
	bFullscreen = Source.bFullscreen;
	for(unsigned char i = 0; i < 10; ++i)
		bAchievements[i] = Source.bAchievements[i];
	MusicVolume = Source.MusicVolume;
	EffectVolume = Source.EffectVolume;
	VoiceVolume = Source.VoiceVolume;
	UserInterfaceVolume = Source.UserInterfaceVolume;
	unUnitsKilled = Source.unUnitsKilled;
	for(unsigned char i = 0; i < 12; ++i)
		unUnitsBuilt[i] = Source.unUnitsBuilt[i];
	for(unsigned char i = 0; i < 11; ++i)
		unKeyBindings[i] = Source.unKeyBindings[i];
	unCurrentLevel = Source.unCurrentLevel;
	unSlot = Source.unSlot;
	Language = Source.Language;
	return *this;
}

CProfileManager::CProfileManager()
{

}

CProfileManager::~CProfileManager()
{

}

CProfileManager* CProfileManager::GetInstance(void)
{
	//making the singleton and returning it
	static CProfileManager instance;
	return &instance;
}

tProfile* CProfileManager::GetCurrentProfile(void)
{
	//returning the current profile
	return &m_arrUserProfiles[m_nCurrentProfile];
}

bool CProfileManager::LoadProfiles(void)
{
	//make the file pointer
	FILE* pFile = NULL;

	//atttempt to open the profile file
	fopen_s(&pFile, "Resources/Data/ff_UserProfiles.bin", "rb");

	//if it failed
	if(!pFile)
	{
		//set all of the profiles to defaults
		for(int i = 0; i < 3; ++i)
			PROFILE_MANAGER->SetToDefault(i);

		//set the current profile to the first one
		PROFILE_MANAGER->m_nCurrentProfile = 0;

		//make the file by saving it out, if something went wrong we are finished and it is time to leave
		if(!PROFILE_MANAGER->SaveProfiles())
			return false;
	}
	else //if it opened the file
	{
		//read in the profiles
		fread(&PROFILE_MANAGER->m_nCurrentProfile, sizeof(int), 1, pFile);
		fread(PROFILE_MANAGER->m_arrUserProfiles, sizeof(tProfile), 3, pFile);

		//close the file
		fclose(pFile);
	}

	//everything is honkey dorey, return
	return true;
}

bool CProfileManager::SaveProfiles(void)
{
	//make the file pointer
	FILE* pFile = NULL;

	//atttempt to open the profile file
	fopen_s(&pFile, "Resources/Data/ff_UserProfiles.bin", "wb");

	//if it failed, something is wrong, time to leave
	if(!pFile)
		return false;

	//write out the profiles as is
	fwrite(&PROFILE_MANAGER->m_nCurrentProfile, sizeof(int), 1, pFile);
	fwrite(PROFILE_MANAGER->m_arrUserProfiles, sizeof(tProfile), 3, pFile);

	//close the file
	fclose(pFile);

	return true;
}

void CProfileManager::InitProfileManager(void)
{
	//load the user profiles
	PROFILE_MANAGER->LoadProfiles();

	m_DIKToString[0x01] = "Escape";    
	m_DIKToString[0x02] = "1";           
	m_DIKToString[0x03] = "2";            
	m_DIKToString[0x04] = "3";           
	m_DIKToString[0x05] = "4";           
	m_DIKToString[0x06] = "5";            
	m_DIKToString[0x07] = "6";            
	m_DIKToString[0x08] = "7";            
	m_DIKToString[0x09] = "8";            
	m_DIKToString[0x0A] = "9";           
	m_DIKToString[0x0B] = "0";           
	m_DIKToString[0x0C] = "Minus (-)";       
	m_DIKToString[0x0D] = "Equals (=)";      
	m_DIKToString[0x0E] = "Back";       
	m_DIKToString[0x0F] = "Tab";         
	m_DIKToString[0x10] = "Q";           
	m_DIKToString[0x11] = "W";           
	m_DIKToString[0x12] = "E";           
	m_DIKToString[0x13] = "R";            
	m_DIKToString[0x14] = "T";           
	m_DIKToString[0x15] = "Y";           
	m_DIKToString[0x16] = "U";            
	m_DIKToString[0x17] = "I";            
	m_DIKToString[0x18] = "O";            
	m_DIKToString[0x19] = "P";            
	m_DIKToString[0x1A] = "Left Bracket ({)";    
	m_DIKToString[0x1B] = "Right Bracket (})";    
	m_DIKToString[0x1C] = "Return";      
	m_DIKToString[0x1D] = "Left Control";    
	m_DIKToString[0x1E] = "A";           
	m_DIKToString[0x1F] = "S";           
	m_DIKToString[0x20] = "D";           
	m_DIKToString[0x21] = "F";           
	m_DIKToString[0x22] = "G";           
	m_DIKToString[0x23] = "H";            
	m_DIKToString[0x24] = "J";           
	m_DIKToString[0x25] = "K";           
	m_DIKToString[0x26] = "L";            
	m_DIKToString[0x27] = "Semicolon (;)";    
	m_DIKToString[0x28] = "Apostrophe (')";   
	m_DIKToString[0x29] = "Grave (~)";        
	m_DIKToString[0x2A] = "Left Shift";      
	m_DIKToString[0x2B] = "Backslash";   
	m_DIKToString[0x2C] = "Z";           
	m_DIKToString[0x2D] = "X";           
	m_DIKToString[0x2E] = "C";           
	m_DIKToString[0x2F] = "V";           
	m_DIKToString[0x30] = "B";           
	m_DIKToString[0x31] = "N";           
	m_DIKToString[0x32] = "M";           
	m_DIKToString[0x33] = "Comma (,)";        
	m_DIKToString[0x34] = "Period (.)";      
	m_DIKToString[0x35] = "Slash (/)";       
	m_DIKToString[0x36] = "Right Shift";       
	m_DIKToString[0x37] = "Mulitply";     
	m_DIKToString[0x38] = "Left Alt";        
	m_DIKToString[0x39] = "Space";        
	m_DIKToString[0x3A] = "Capital";     
	m_DIKToString[0x3B] = "F1";          
	m_DIKToString[0x3C] = "F2";          
	m_DIKToString[0x3D] = "F3";          
	m_DIKToString[0x3E] = "F4";          
	m_DIKToString[0x3F] = "F5";          
	m_DIKToString[0x40] = "F6";          
	m_DIKToString[0x41] = "F7";          
	m_DIKToString[0x42] = "F8";          
	m_DIKToString[0x43] = "F9";           
	m_DIKToString[0x44] = "F10";         
	m_DIKToString[0x45] = "NumLock";     
	m_DIKToString[0x46] = "Scroll";       
	m_DIKToString[0x47] = "7 (Numpad)";     
	m_DIKToString[0x48] = "8 (Numpad)";      
	m_DIKToString[0x49] = "9 (Numpad)";      
	m_DIKToString[0x4A] = "Subtract (-)";    
	m_DIKToString[0x4B] = "4 (Numpad)";     
	m_DIKToString[0x4C] = "5 (Numpad)";     
	m_DIKToString[0x4D] = "6 (Numpad)";     
	m_DIKToString[0x4E] = "Add (+)";         
	m_DIKToString[0x4F] = "1 (Numpad)";     
	m_DIKToString[0x50] = "2 (Numpad)";     
	m_DIKToString[0x51] = "3 (Numpad)";     
	m_DIKToString[0x52] = "0 (Numpad)";     
	m_DIKToString[0x53] = "Decimal (.)";      
	m_DIKToString[0x56] = "OEM_102";     
	m_DIKToString[0x57] = "F11";         
	m_DIKToString[0x58] = "F12";          
	m_DIKToString[0x64] = "F13";          
	m_DIKToString[0x65] = "F14";          
	m_DIKToString[0x66] = "F15";          
	m_DIKToString[0x70] = "KANA";        
	m_DIKToString[0x73] = "ABNT_C1";     
	m_DIKToString[0x79] = "CONVERT";     
	m_DIKToString[0x7B] = "NOCONVERT";   
	m_DIKToString[0x7D] = "YEN";         
	m_DIKToString[0x7E] = "ABNT_C2";     
	m_DIKToString[0x8D] = "Numpad Equals (=)";
	m_DIKToString[0x90] = "Previous Track";   
	m_DIKToString[0x91] = "At (@)";          
	m_DIKToString[0x92] = "Colon (:)";        
	m_DIKToString[0x93] = "Underscore (_)";   
	m_DIKToString[0x94] = "KANJI";       
	m_DIKToString[0x95] = "Stop";         
	m_DIKToString[0x96] = "Ax";           
	m_DIKToString[0x97] = "UNLABELED";    
	m_DIKToString[0x99] = "NextTrack";    
	m_DIKToString[0x9C] = "Numpad Enter"; 
	m_DIKToString[0x9D] = "Right Control";    
	m_DIKToString[0xA0] = "Mute";        
	m_DIKToString[0xA1] = "Calculator";  
	m_DIKToString[0xA2] = "Play Pause";   
	m_DIKToString[0xA4] = "Media Stop";   
	m_DIKToString[0xAE] = "Volume Down";  
	m_DIKToString[0xB0] = "Volume Up";    
	m_DIKToString[0xB2] = "Web Home";     
	m_DIKToString[0xB3] = "Numpad Comma";  
	m_DIKToString[0xB5] = "Divide (/)";      
	m_DIKToString[0xB7] = "SYSRQ";       
	m_DIKToString[0xB8] = "Right Alt";        
	m_DIKToString[0xC5] = "Pause";        
	m_DIKToString[0xC7] = "Home";         
	m_DIKToString[0xC8] = "Up Arrow";           
	m_DIKToString[0xC9] = "Prior";       
	m_DIKToString[0xCB] = "Left Arrow";        
	m_DIKToString[0xCD] = "Right Arrow";       
	m_DIKToString[0xCF] = "End";         
	m_DIKToString[0xD0] = "Down Arrow";        
	m_DIKToString[0xD1] = "Next";        
	m_DIKToString[0xD2] = "Insert";      
	m_DIKToString[0xD3] = "Delete";      
	m_DIKToString[0xDB] = "Left Window Key";        
	m_DIKToString[0xDC] = "Right Window Key";         
	m_DIKToString[0xDD] = "Apps";        
	m_DIKToString[0xDE] = "Power";       
	m_DIKToString[0xDF] = "Sleep";        
	m_DIKToString[0xE3] = "Wake";         
	m_DIKToString[0xE5] = "Web Search";    
	m_DIKToString[0xE6] = "Web Favorites"; 
	m_DIKToString[0xE7] = "Web Refresh";  
	m_DIKToString[0xE8] = "Web Stop";     
	m_DIKToString[0xE9] = "Web Forward";  
	m_DIKToString[0xEA] = "Web Back";     
	m_DIKToString[0xEB] = "My Computer";  
	m_DIKToString[0xEC] = "Mail";        
	m_DIKToString[0xED] = "Media Select"; 

}

void CProfileManager::ShutdownProfileManager(void)
{
	//save the user profiles
	PROFILE_MANAGER->SaveProfiles();
}

void CProfileManager::SetCurrentProfile(int nCurrentProfile)
{
	if(nCurrentProfile >= 0 && nCurrentProfile < 3)
	{
		m_nCurrentProfile = nCurrentProfile;

		switch(m_arrUserProfiles[m_nCurrentProfile].Language)
		{
		case EL_English:
			break;
		case EL_Spanish:
			CStringTable::GetInstance()->LoadDictionary("Resources/Data/Spanish.txt");
			break;
		default:
			break;
		}
	}
}

void CProfileManager::SetToDefault(int nProfileIndex)
{
	if(nProfileIndex >= 0 && nProfileIndex < 3)
	{
		for(int j = 0; j < 10; ++j)
				m_arrUserProfiles[nProfileIndex].bAchievements[j] = false;

		m_arrUserProfiles[nProfileIndex].bFullscreen = false;

		m_arrUserProfiles[nProfileIndex].szName[0] = 'D';
		m_arrUserProfiles[nProfileIndex].szName[1] = 'e';
		m_arrUserProfiles[nProfileIndex].szName[2] = 'f';
		m_arrUserProfiles[nProfileIndex].szName[3] = 'a';
		m_arrUserProfiles[nProfileIndex].szName[4] = 'u';
		m_arrUserProfiles[nProfileIndex].szName[5] = 'l';
		m_arrUserProfiles[nProfileIndex].szName[6] = 't';
		m_arrUserProfiles[nProfileIndex].szName[7] = '\0';

		m_arrUserProfiles[nProfileIndex].MusicVolume		= 50;
		m_arrUserProfiles[nProfileIndex].EffectVolume	= 50;
		m_arrUserProfiles[nProfileIndex].VoiceVolume	= 50;
		m_arrUserProfiles[nProfileIndex].UserInterfaceVolume	= 50;
		m_arrUserProfiles[nProfileIndex].unCurrentLevel	= 0;

		m_arrUserProfiles[nProfileIndex].unKeyBindings[UP]			= DIK_UP;
		m_arrUserProfiles[nProfileIndex].unKeyBindings[DOWN]		= DIK_DOWN;
		m_arrUserProfiles[nProfileIndex].unKeyBindings[LEFT]		= DIK_LEFT;
		m_arrUserProfiles[nProfileIndex].unKeyBindings[RIGHT]		= DIK_RIGHT;
		m_arrUserProfiles[nProfileIndex].unKeyBindings[MOVE]		= DIK_M;
		m_arrUserProfiles[nProfileIndex].unKeyBindings[ATTACK]		= DIK_A;
		m_arrUserProfiles[nProfileIndex].unKeyBindings[STOP]		= DIK_T;
		m_arrUserProfiles[nProfileIndex].unKeyBindings[STOVE]		= DIK_S;
		m_arrUserProfiles[nProfileIndex].unKeyBindings[RALLY_POINT]	= DIK_R;
		m_arrUserProfiles[nProfileIndex].unKeyBindings[COOKBOOK]	= DIK_TAB;

		for(int k = 0; k < 12 ; ++k)
			m_arrUserProfiles[nProfileIndex].unUnitsBuilt[k] = 0;

		m_arrUserProfiles[nProfileIndex].unUnitsKilled = 0;

		m_arrUserProfiles[nProfileIndex].unSlot = (unsigned int)nProfileIndex;

		m_arrUserProfiles[nProfileIndex].Language = EL_English;
	}
}

tProfile* CProfileManager::GetProfile(int nIndex)
{
	if(nIndex >= 0 && nIndex < 3)
	{
		return &m_arrUserProfiles[nIndex];
	}
	return NULL;
}

const char* CProfileManager::GetStringFromDIK(unsigned char ucDIK)
{
	for(map<unsigned char, string>::iterator iter = m_DIKToString.begin(); iter != m_DIKToString.end(); ++iter)
	{
		if(iter->first == ucDIK)
			return iter->second.c_str();
	}

	return NULL;
}