//////////////////////////////////////////////////////
// File: "CParticleManager.cpp"
// Creator: AB
// Creation Date: 10/4/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "CParticleManager.h"
#include "../../SGD/CSGD_Direct3D.h"
#include <stdio.h>

CParticleManager::CParticleManager(void)
{
}

CParticleManager::~CParticleManager(void)
{
	//for(unsigned int i = 0; i < m_vecParticleEmitters.size(); ++i)
	//{
	//	delete m_vecParticleEmitters[i];
	//	m_vecParticleEmitters[i] = NULL;
	//}
	//m_vecParticleEmitters.clear();

	for(map<string, IDirect3DTexture9*>::iterator iter = m_LoadedTextures.begin(); iter != m_LoadedTextures.end(); ++iter)
	{
		if(iter->second)
			iter->second->Release();
	}

	m_LoadedTextures.clear();

	for(map<string, CParticleEmitter*>::iterator iter = m_ParticleEffects.begin(); iter != m_ParticleEffects.end(); ++iter)
	{
		if(iter->second)
		{
			delete iter->second;
			iter->second = NULL;
		}
	}

	m_ParticleEffects.clear();

	m_vecLoadedEffects.clear();
}

bool CParticleManager::Load(const char* szFileName)
{
	if(szFileName)
	{
		FILE* pFile = NULL;

		fopen_s(&pFile, szFileName, "rb");

		if(pFile)
		{
			CParticleEmitter* newEmitter = new CParticleEmitter();

			float fTemp, fTempX, fTempY;
			int nTemp1, nTemp2, nTemp3, nTemp4;
			char* szTemp;

			fread(&nTemp1, sizeof(int), 1, pFile);
			szTemp = new char[nTemp1 +1];
			fread(szTemp, sizeof(char), nTemp1, pFile);
			szTemp[nTemp1] = '\0';

			newEmitter->SetName(szTemp);
			m_ParticleEffects[szTemp] = newEmitter;

			delete szTemp;
			szTemp = NULL;

			fread(&nTemp1, sizeof(int), 1, pFile);
			szTemp = new char[nTemp1 + 1];
			fread(szTemp, sizeof(char), nTemp1, pFile);
			szTemp[nTemp1] = '\0';

			bool bFound = false;

			for(map<string, IDirect3DTexture9*>::iterator iter = m_LoadedTextures.begin(); iter != m_LoadedTextures.end(); ++iter)
			{
				if(strcmp(iter->first.c_str(), szTemp) == 0)
				{
					newEmitter->SetTexture(iter->second);
					bFound = true;
				}
			}

			if(!bFound)
			{
				char file[128];

				sprintf_s(file, 128, "Resources/Graphics/Particles/%s", szTemp);

				IDirect3DTexture9* pTemp = NULL;

				D3DXCreateTextureFromFile(CSGD_Direct3D::GetInstance()->GetDirect3DDevice(), file, &pTemp);

				newEmitter->SetTexture(pTemp);

				m_LoadedTextures[szTemp] = pTemp;
			}

			delete szTemp;
			szTemp = NULL;

			fread(&nTemp1, sizeof(int), 1, pFile);
			newEmitter->SetNumParticles(nTemp1);

			fread(&nTemp1, sizeof(int), 1, pFile);
			newEmitter->SetParticlesPerSecond(nTemp1);

			fread(&fTempX, sizeof(float), 1, pFile);
			fread(&fTempY, sizeof(float), 1, pFile);
			newEmitter->SetPosition(D3DXVECTOR2(fTempX, fTempY));

			fread(&fTempX, sizeof(float), 1, pFile);
			fread(&fTempY, sizeof(float), 1, pFile);
			newEmitter->SetVelocity(D3DXVECTOR2(fTempX, fTempY));

			fread(&fTemp, sizeof(float), 1, pFile);
			newEmitter->SetSpread(fTemp);

			fread(&nTemp1, sizeof(int), 1, pFile);
			fread(&nTemp2, sizeof(int), 1, pFile);
			newEmitter->SetRectWidth(nTemp1);
			newEmitter->SetRectHeight(nTemp2);

			fread(&fTempX, sizeof(float), 1, pFile);
			fread(&fTempY, sizeof(float), 1, pFile);
			newEmitter->SetMinLife(fTempX);
			newEmitter->SetMaxLife(fTempY);

			fread(&fTemp, sizeof(float), 1, pFile);
			
			fread(&fTempX, sizeof(float), 1, pFile);
			fread(&fTempY, sizeof(float), 1, pFile);
			newEmitter->SetParticleAcceleration(D3DXVECTOR2(fTempX, fTempY));

			fread(&fTempX, sizeof(float), 1, pFile);
			fread(&fTempY, sizeof(float), 1, pFile);
			newEmitter->SetParticleVelocity(D3DXVECTOR2(fTempX, fTempY));

			fread(&nTemp1, sizeof(int), 1, pFile);
			fread(&nTemp2, sizeof(int), 1, pFile);
			fread(&nTemp3, sizeof(int), 1, pFile);
			fread(&nTemp4, sizeof(int), 1, pFile);
			newEmitter->SetStartColor((unsigned char)nTemp1,(unsigned char)nTemp2, (unsigned char)nTemp3, (unsigned char)nTemp4);

			fread(&nTemp1, sizeof(int), 1, pFile);
			fread(&nTemp2, sizeof(int), 1, pFile);
			fread(&nTemp3, sizeof(int), 1, pFile);
			fread(&nTemp4, sizeof(int), 1, pFile);
			newEmitter->SetDiffColor(nTemp1, nTemp2, nTemp3, nTemp4);

			fread(&nTemp1, sizeof(int), 1, pFile);
			fread(&nTemp2, sizeof(int), 1, pFile);
			newEmitter->SetSourceBlend((DWORD)nTemp1);
			newEmitter->SetDestBlend((DWORD)nTemp2);

			fread(&fTempX, sizeof(float), 1, pFile);
			fread(&fTempY, sizeof(float), 1, pFile);
			newEmitter->SetStartScale(fTempX);
			newEmitter->SetDiffScale(fTempY);

			fread(&fTempX, sizeof(float), 1, pFile);
			fread(&fTempY, sizeof(float), 1, pFile);
			newEmitter->SetForcePoint(D3DXVECTOR2(fTempX, fTempY));

			fread(&fTempX, sizeof(float), 1, pFile);
			fread(&fTempY, sizeof(float), 1, pFile);
			newEmitter->SetForce(D3DXVECTOR2(fTempX, fTempY));

			fread(&nTemp1, sizeof(int), 1, pFile);
			if(nTemp1 != 0)
				newEmitter->SetUsingForcePoint(true);
			else
				newEmitter->SetUsingForcePoint(false);

			fread(&nTemp1, sizeof(int), 1, pFile);
			if(nTemp1 != 0)
				newEmitter->SetRefreshParticles(true);
			else
				newEmitter->SetRefreshParticles(false);

			fread(&nTemp1, sizeof(int), 1, pFile);
			if(nTemp1 != 0)
				newEmitter->SetIsLooping(true);
			else
				newEmitter->SetIsLooping(false);

			fread(&nTemp1, sizeof(int), 1, pFile);
			newEmitter->SetNumFrames(nTemp1);

			for(int i = 0; i < newEmitter->GetNumFrames(); ++i)
			{
				fread(&nTemp1, sizeof(int), 1, pFile);
				szTemp = new char[nTemp1 + 1];
				fread(szTemp, sizeof(char), nTemp1, pFile);
				szTemp[nTemp1] = '\0';

				fread(&fTemp, sizeof(float), 1, pFile);
				fread(&nTemp2, sizeof(int), 1, pFile);
				fread(&nTemp3, sizeof(int), 1, pFile);

				bool bTemp1, bTemp2;

				if(nTemp2 == 0)
					bTemp1 = false;
				else
					bTemp1 = true;

				if(nTemp3 == 0)
					bTemp2 = false;
				else
					bTemp2 = true;

				newEmitter->AddFrame(szTemp, fTemp, bTemp1, bTemp2);

				delete szTemp;
			}

			fclose(pFile);

			return true;
		}
		else
			return false;
	}
	else
		return false;
}

CParticleManager* CParticleManager::GetInstance(void)
{
	static CParticleManager m_Instance;

	return &m_Instance;
}

void CParticleManager::Update(void)
{
	/*for(vector<CParticleEmitter*>::iterator iter = m_vecParticleEmitters.begin(); iter != m_vecParticleEmitters.end(); iter++)
	{
		if((*iter)->GetDeletable())
		{
			delete (*iter);
			iter = m_vecParticleEmitters.erase(iter);
		}
	}*/
}

void CParticleManager::ShutdownParticleManger(void)
{
	/*for(unsigned int i = 0; i < m_vecParticleEmitters.size(); ++i)
	{
		delete m_vecParticleEmitters[i];
		m_vecParticleEmitters[i] = NULL;
	}
	m_vecParticleEmitters.clear();*/

	for(map<string, IDirect3DTexture9*>::iterator iter = m_LoadedTextures.begin(); iter != m_LoadedTextures.end(); ++iter)
	{
		if(iter->second)
			iter->second->Release();
	}

	m_LoadedTextures.clear();

	for(map<string, CParticleEmitter*>::iterator iter = m_ParticleEffects.begin(); iter != m_ParticleEffects.end(); ++iter)
	{
		if(iter->second)
		{
			delete iter->second;
			iter->second = NULL;
		}
	}

	m_ParticleEffects.clear();

	m_vecLoadedEffects.clear();
}

bool CParticleManager::LoadParticleEffect(const char* szFileName)
{
	for(unsigned int i = 0; i < m_vecLoadedEffects.size(); ++i)
	{
		if(0 == strcmp(szFileName, m_vecLoadedEffects[i].c_str()))
			return true;
	}

	return Load(szFileName);
}

CParticleEmitter* CParticleManager::GetParticleEffect(const char* szEffectName)
{
	for(map<string, CParticleEmitter*>::iterator iter = m_ParticleEffects.begin(); iter != m_ParticleEffects.end(); ++iter)
	{
		if(0 == strcmp(szEffectName, iter->first.c_str()))
		{
			CParticleEmitter* newEmitter = new CParticleEmitter((*iter->second));

			//m_vecParticleEmitters.push_back(newEmitter);

			return (newEmitter);
		}
	}
	return NULL;
}