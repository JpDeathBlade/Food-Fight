#include "CAudioManager.h"

// CSound //

CSound::CSound()
{
	m_Sound = NULL;
	m_Channel = NULL;
	m_Volume = 1.0f;
}

CSound::~CSound()
{
	m_Sound->release();
}

const char* CSound::GetName() const
{
	return m_Name.c_str();
}

void CSound::SetName(const char* Name)
{
	m_Name = Name;
}

FMOD::Sound* CSound::GetSoundPointer() const
{
	return m_Sound;
}

void CSound::SetSoundPointer(FMOD::Sound* Sound)
{
	m_Sound = Sound;
}

FMOD::Channel* CSound::GetChannel() const
{
	return m_Channel;
}

void CSound::SetChannel(FMOD::Channel* Channel)
{
	m_Channel = Channel;
}

float CSound::GetVolume() const
{
	return m_Volume;
}

void CSound::SetVolume(float Volume)
{
	m_Volume = Volume;

	if(m_Channel)
	{
		if(m_Volume != 0)
		{
			m_Channel->setMute(false);
			m_Channel->setVolume(m_Volume);
		}
		else
		{
			m_Channel->setMute(true);
		}
	}
}

void CSound::Play(int LoopsToPlay)
{
	FMOD::System* FMODAPI = CAudioManager::GetInstance()->GetFMODAPI();
	FMODAPI->playSound(FMOD_CHANNEL_FREE, GetSoundPointer(), true, &m_Channel);
	if(m_Volume != 0)
	{
		m_Channel->setMute(false);
		m_Channel->setVolume(m_Volume);
	}
	else
	{
		m_Channel->setMute(true);
	}
	m_Channel->setLoopCount(LoopsToPlay);
	m_Channel->setPaused(false);
}

void CSound::Play(D3DXVECTOR2 Position, D3DXVECTOR2 Velocity, int LoopsToPlay)
{
	FMOD::System* FMODAPI = CAudioManager::GetInstance()->GetFMODAPI();
	FMODAPI->playSound(FMOD_CHANNEL_FREE, GetSoundPointer(), true, &m_Channel);
	FMOD_VECTOR Position3D;
	Position3D.x = Position.x;
	Position3D.y = Position.y;
	Position3D.z = 0;
	FMOD_VECTOR Velocity3D;
	Velocity3D.x = Velocity.x;
	Velocity3D.y = Velocity.y;
	Velocity3D.z = 0;
	m_Channel->set3DAttributes(&Position3D, &Velocity3D);
	if(m_Volume != 0)
	{
		m_Channel->setMute(false);
		m_Channel->setVolume(m_Volume);
	}
	else
	{
		m_Channel->setMute(true);
	}
	m_Channel->setLoopCount(LoopsToPlay);
	m_Channel->setPaused(false);
}

bool CSound::IsPlaying() const
{
	bool IsPlaying = false;
	if(m_Channel)
		m_Channel->isPlaying(&IsPlaying);
	return IsPlaying;
}

void CSound::StopPlaying()
{
	if(m_Channel)
		m_Channel->stop();
}

// CSoundCategory //

CSoundCategory::CSoundCategory()
{
	m_Volume = 1.0f;
}

CSoundCategory::~CSoundCategory()
{
	for(std::list<CSound*>::iterator Iterator = m_Sounds.begin(); Iterator != m_Sounds.end();)
	{
		delete (*Iterator);
		Iterator = m_Sounds.erase(Iterator);
	}
}

const char* CSoundCategory::GetName() const
{
	return m_Name.c_str();
}

void CSoundCategory::SetName(const char* Name)
{
	m_Name = Name;
}

void CSoundCategory::AddSound(CSound* Sound)
{
	m_Sounds.push_back(Sound);
	Sound->SetVolume(m_Volume);
}

CSound* CSoundCategory::GetSound(const char* Name) const
{
	for(std::list<CSound*>::const_iterator Iterator = m_Sounds.begin(); Iterator != m_Sounds.end(); ++Iterator)
	{
		if(!strcmp((*Iterator)->GetName(), Name))
		{
			return (*Iterator);
		}
	}
	return NULL;
}

bool CSoundCategory::RemoveSound(CSound* Sound)
{
	for(std::list<CSound*>::iterator Iterator = m_Sounds.begin(); Iterator != m_Sounds.end(); ++Iterator)
	{
		if((*Iterator) == Sound)
		{
			delete (*Iterator);
			m_Sounds.erase(Iterator);
			return true;
		}
	}
	return false;
}

bool CSoundCategory::RemoveSound(const char* Name)
{
	for(std::list<CSound*>::iterator Iterator = m_Sounds.begin(); Iterator != m_Sounds.end(); ++Iterator)
	{
		if(!strcmp((*Iterator)->GetName(), Name))
		{
			delete (*Iterator);
			m_Sounds.erase(Iterator);
			return true;
		}
	}
	return false;
}

void CSoundCategory::SetVolume(float Volume)
{
	m_Volume = Volume;
	for(std::list<CSound*>::iterator Iterator = m_Sounds.begin(); Iterator != m_Sounds.end(); ++Iterator)
	{
		(*Iterator)->SetVolume(Volume);
	}
}

// CAudioManager //

CAudioManager* CAudioManager::m_Instance = NULL;

CAudioManager::CAudioManager()
{
	m_FMODAPI = NULL;
}

CAudioManager::~CAudioManager()
{
	for(std::list<CSoundCategory*>::iterator Iterator = m_Categories.begin(); Iterator != m_Categories.end();)
	{
		delete (*Iterator);
		Iterator = m_Categories.erase(Iterator);
	}
	m_FMODAPI->release();
}

CAudioManager* CAudioManager::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CAudioManager;
	return m_Instance;
}

void CAudioManager::DeleteInstance()
{
	delete m_Instance;
}

bool CAudioManager::Initialize()
{
	if(FMOD::System_Create(&m_FMODAPI) != FMOD_OK)
		return false;

	if(m_FMODAPI->init(100, FMOD_INIT_NORMAL, NULL) != FMOD_OK)
		return false; // TODO: Shutdown the system due to failure to initialize.

	m_FMODAPI->set3DSettings(1.0f, 50.0f, 1.0f);

	return true;
}

FMOD::System* CAudioManager::GetFMODAPI() const
{
	return m_FMODAPI;
}

void CAudioManager::Update()
{
	m_FMODAPI->update();
}

void CAudioManager::AddCategory(const char* CategoryName)
{
	CSoundCategory* SoundCategory = new CSoundCategory;
	SoundCategory->SetName(CategoryName);
	m_Categories.push_back(SoundCategory);
}

bool CAudioManager::RemoveCategory(const char* CategoryName)
{
	for(std::list<CSoundCategory*>::iterator Iterator = m_Categories.begin(); Iterator != m_Categories.end(); ++Iterator)
	{
		if(!strcmp((*Iterator)->GetName(), CategoryName))
		{
			m_Categories.erase(Iterator);
				return true;
		}
	}
	return false;
}

CSoundCategory* CAudioManager::GetCategory(const char * CategoryName)
{
	for(std::list<CSoundCategory*>::iterator Iterator = m_Categories.begin(); Iterator != m_Categories.end(); ++Iterator)
	{
		if(!strcmp((*Iterator)->GetName(), CategoryName))
		{
			return (*Iterator);
		}
	}
	return NULL;
}

bool CAudioManager::LoadSound(const char* FileName, const char* CategoryName, bool ThreeDimensional, bool Streaming)
{
	for(std::list<CSoundCategory*>::iterator Iterator = m_Categories.begin(); Iterator != m_Categories.end(); ++Iterator)
	{
		if(!strcmp((*Iterator)->GetName(), CategoryName))
		{
			FMOD::Sound* Sound;
			FMOD_MODE ModeFlag = FMOD_DEFAULT;
			if(ThreeDimensional)
				ModeFlag = FMOD_3D;
			if(Streaming)
				m_FMODAPI->createStream(FileName, ModeFlag, NULL, &Sound);
			else
				m_FMODAPI->createSound(FileName, ModeFlag, NULL, &Sound);
			if(!Sound)
				return false;

			CSound* NewSound = new CSound;
			NewSound->SetName(FileName);
			NewSound->SetSoundPointer(Sound);

			(*Iterator)->AddSound(NewSound);
			return true;
		}
	}
	return false;
}

CSound* CAudioManager::GetSound(const char* Name, const char* CategoryName)
{
	for(std::list<CSoundCategory*>::iterator Iterator = m_Categories.begin(); Iterator != m_Categories.end(); ++Iterator)
	{
		if(!strcmp((*Iterator)->GetName(), CategoryName))
		{
			return (*Iterator)->GetSound(Name);
		}
	}
	return NULL;
}

bool CAudioManager::UnloadSound(const char* Name, const char* CategoryName)
{
	for(std::list<CSoundCategory*>::iterator Iterator = m_Categories.begin(); Iterator != m_Categories.end(); ++Iterator)
	{
		if(!strcmp((*Iterator)->GetName(), CategoryName))
		{
			return (*Iterator)->RemoveSound(Name);
		}
	}
	return false;
}