//////////////////////////////////////////////////////
// File: "CParticleManager.h"
// Creator: AB
// Creation Date: 10/4/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#ifndef CPARTICLEMANAGER_H_
#define CPARTICLEMANAGER_H_

#include <map>
#include <string>
#include <vector>
using namespace std;

#include "CParticleEmitter.h"

class CParticleManager
{
private:
	map<string, CParticleEmitter*>	m_ParticleEffects;
	map<string, IDirect3DTexture9*> m_LoadedTextures;
	vector<string>					m_vecLoadedEffects;
	//vector<CParticleEmitter*>		m_vecParticleEmitters;

	CParticleManager(void);
	CParticleManager(const CParticleManager& obj);
	CParticleManager& operator =(const CParticleManager& obj);
	~CParticleManager(void);

	bool Load(const char* szFileName);

public:

	static CParticleManager* GetInstance(void);

	void Update(void);

	void ShutdownParticleManger(void);

	bool LoadParticleEffect(const char* szFileName);

	CParticleEmitter* GetParticleEffect(const char* szEffectName);
};

#endif