#ifndef _CAudioManager_H_
#define _CAudioManager_H_
#include <fmod.hpp>
#include <list>
#include <string>
#include <d3dx9.h>
#pragma comment(lib, "fmodex_vc.lib")

class CSound
{
	std::string m_Name;
	FMOD::Sound* m_Sound;
	FMOD::Channel* m_Channel;
	float m_Volume;
public:
	CSound();
	CSound(const CSound& Source);
	CSound& operator=(const CSound& Source);
	~CSound();

	const char* GetName() const;
	void SetName(const char* Name);

	FMOD::Sound* GetSoundPointer() const;
	void SetSoundPointer(FMOD::Sound* Sound);

	FMOD::Channel* GetChannel() const;
	void SetChannel(FMOD::Channel* Channel);

	float GetVolume() const;
	void SetVolume(float Volume);

	void Play(int LoopsToPlay = 0);
	void Play(D3DXVECTOR2 Position, D3DXVECTOR2 Velocity = D3DXVECTOR2(0, 0), int LoopsToPlay = 0);

	bool IsPlaying() const;

	void StopPlaying();
};

class CSoundCategory
{
	std::string m_Name;
	std::list<CSound*> m_Sounds;
	float m_Volume;
public:
	CSoundCategory();
	CSoundCategory(const CSoundCategory& Source);
	CSoundCategory& operator=(const CSoundCategory& Source);
	~CSoundCategory();

	const char* GetName() const;
	void SetName(const char* Name);

	void AddSound(CSound* Sound);

	CSound* GetSound(const char* Name) const;

	bool RemoveSound(CSound* Sound);
	bool RemoveSound(const char* Name);

	void SetVolume(float Volume);
};

class CAudioManager
{
	static CAudioManager* m_Instance;
	FMOD::System* m_FMODAPI;
	std::list<CSoundCategory*> m_Categories;

	CAudioManager();
	CAudioManager(const CAudioManager& Source);
	CAudioManager& operator=(const CAudioManager& Source);
	~CAudioManager();
public:
	static CAudioManager* GetInstance();
	static void DeleteInstance();

	bool Initialize();
	FMOD::System* GetFMODAPI() const;

	void Update();

	void AddCategory(const char* CategoryName);
	CSoundCategory* GetCategory(const char * CategoryName);
	bool RemoveCategory(const char* CategoryName);

	bool LoadSound(const char* FileName, const char* CategoryName, bool ThreeDimensional = false, bool Streaming = false);
	CSound* GetSound(const char* Name, const char* CategoryName);
	bool UnloadSound(const char* Name, const char* CategoryName);
};
#endif