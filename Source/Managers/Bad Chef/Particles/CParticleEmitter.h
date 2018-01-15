//////////////////////////////////////////////////////
// File: "CParticleEmitter.h"
// Creator: AB
// Creation Date: 10/2/09
// Last Modified: 10/3/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef CPARTICLEEMITTER_H_
#define CPARTICLEEMITTER_H_

#include "../../../Game/Objects/Base/CGameObject.h"
#include <list>
using namespace std;
#include "../../SGD/CSGD_Direct3D.h"

class CParticleEmitter : public IObject
{
private:
	struct CParticle
	{
		D3DXVECTOR2 m_vAcceleration;
		D3DXVECTOR2 m_vVelocity;
		D3DXVECTOR2 m_vPosition;
		float		m_fMaxLife;
		float		m_fCurrentLife;

		CParticle(void)
		{
			m_vAcceleration	= D3DXVECTOR2(0.0f, 0.0f);
			m_vVelocity		= D3DXVECTOR2(0.0f, 0.0f);
			m_vPosition		= D3DXVECTOR2(0.0f, 0.0f);
			m_fMaxLife		= 0.0f;
			m_fCurrentLife	= 0.0f;
		}
	};

	struct CFrame
	{
		char*	m_szTrigger;
		float	m_fDuration;
		float	m_fAccumulator;
		bool	m_bIsEmitting;
		bool	m_bResetParticles;

		CFrame(void)
		{
			m_szTrigger = NULL;
			m_fDuration = m_fAccumulator = 0.0f;
			m_bIsEmitting = true;
			m_bResetParticles = false;
		}

		CFrame(const char* szTrigger, float fDuration, bool bIsEmitting, bool bResetParticles)
		{
			m_szTrigger = NULL;
			int nSize = strlen(szTrigger);
			m_szTrigger = new char[nSize +1];
			strcpy_s(m_szTrigger, nSize + 1, szTrigger);

			m_fDuration = m_fAccumulator = fDuration;
			m_bIsEmitting = bIsEmitting;
			m_bResetParticles = bResetParticles;
		}

		CFrame(const CFrame& obj)
		{
			m_szTrigger = NULL;
			int nSize = strlen(obj.m_szTrigger);
			m_szTrigger = new char[nSize +1];
			strcpy_s(m_szTrigger, nSize + 1, obj.m_szTrigger);

			m_fDuration = m_fAccumulator = obj.m_fDuration;
			m_bIsEmitting = obj.m_bIsEmitting;
			m_bResetParticles = obj.m_bResetParticles;
		}

		~CFrame(void)
		{
			if(m_szTrigger)
			{
				delete m_szTrigger;
				m_szTrigger = NULL;
			}
		}

		bool Update(float fDelta)
		{
			m_fAccumulator += fDelta;

			if(m_fAccumulator >= m_fDuration)
				return true;

			return false;
		}

	};

	char*							m_szName;
	IDirect3DTexture9*				m_pParticleTex;
	D3DXMATRIX						m_mOldViewMatrix;
	D3DXMATRIX						m_mViewMatrix;
	D3DXCOLOR						m_cColor;
	int								m_nNumParticles;
	CParticle*						m_arrParticles;
	vector<CParticle*>				m_pDeadList;
	vector<CParticle*>				m_pAliveList;
	vector<CParticle*>::iterator	m_DeadIter;
	vector<CParticle*>::iterator	m_AliveIter;
	int								m_nNumParticlesPerSecond;
	bool							m_bIsLooping;
	int								m_nNumFrames;
	int								m_nCurrentFrame;
	vector<CFrame>					m_vecFrames;
	D3DXVECTOR2						m_vEmitterVelocity;
	D3DXVECTOR2						m_vEmitterPosition;
	D3DXVECTOR2						m_vParticleAcceleration;
	D3DXVECTOR2						m_vParticleVelocity;
	D3DXVECTOR2						m_vForce;
	D3DXVECTOR2						m_vForcePoint;
	unsigned char					m_ucStartA, m_ucStartR, m_ucStartG, m_ucStartB;
	int								m_ucDiffA, m_ucDiffR, m_ucDiffG, m_ucDiffB;
	float							m_fParticleSpread;
	float							m_fStartScale, m_fDiffScale;
	float							m_fMinLife, m_fMaxLife;
	int								m_nRectWidth, m_nRectHeight;
	DWORD							m_nSourceBlend, m_nDestBlend;
	DWORD							m_nDefaultSourceBlend, m_nDefaultDestBlend;
	bool							m_bIsUsingForcePoint;
	bool							m_bRefreshParticles;
	bool							m_bIsPlaying;
	int								m_nParticlesMade;
	//bool							m_bIsDeletable;
	bool							m_bIsPersistent;
	CGameObject*					m_pObjectToTrack;
	D3DXVECTOR2						m_vPositionOffset;

public:
	//CParticleEmitter(void);
	CParticleEmitter(void)
	{
		m_pObjectToTrack			= NULL;
		m_vPositionOffset			= D3DXVECTOR2(0.0f, 0.0f);
		SetCollideable(false);
		m_bIsPersistent				= true;
		m_szName					= NULL;
		m_pParticleTex				= NULL;
		m_nNumParticles				= 0;
		m_arrParticles				= NULL;
		m_nNumParticlesPerSecond	= 0;
		m_vEmitterVelocity			= D3DXVECTOR2(0.0f, 0.0f);
		m_vEmitterPosition			= D3DXVECTOR2(0.0f, 0.0f);
		m_vParticleAcceleration		= D3DXVECTOR2(0.0f, 0.0f);
		m_vParticleVelocity			= D3DXVECTOR2(0.0f, 0.0f);
		m_vForce					= D3DXVECTOR2(0.0f, 0.0f);
		m_vForcePoint				= D3DXVECTOR2(0.0f, 0.0f);
		m_ucStartA = m_ucStartR = m_ucStartG = m_ucStartB = 255;
		m_ucDiffA = m_ucDiffR = m_ucDiffG = m_ucDiffB = 0;
		m_fParticleSpread			= D3DX_PI * 2.0f;
		m_fStartScale				= 1.0f;
		m_fDiffScale				= 0.0f;
		m_fMinLife = m_fMaxLife		= 0.0f;
		m_nRectWidth = m_nRectHeight= 1;
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND, &m_nDefaultSourceBlend);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND, &m_nDefaultDestBlend);
		m_nSourceBlend				= m_nDefaultSourceBlend;
		m_nDestBlend				= m_nDefaultDestBlend;
		m_bIsUsingForcePoint		= false;
		m_bRefreshParticles			= true;
		m_nNumFrames				= 0;
		m_bIsLooping				= true;
		m_bIsPlaying				= false;
		m_nParticlesMade			= 0;
		m_nCurrentFrame				= 0;

		D3DXMatrixIdentity(&m_mOldViewMatrix);
		D3DXMatrixIdentity(&m_mViewMatrix);
	}

	//CParticleEmitter(const CParticleEmitter& obj);
	CParticleEmitter::CParticleEmitter(const CParticleEmitter& obj)
	{
		SetCollideable(false);

		int nSize = strlen(obj.m_szName);
		m_szName = new char[nSize + 1];
		strcpy_s(m_szName, nSize + 1, obj.m_szName);

		m_arrParticles				= NULL;

		m_pObjectToTrack			= NULL;
		m_vPositionOffset			= D3DXVECTOR2(0.0f, 0.0f);
		m_bIsPersistent				= true;
		m_pParticleTex				= obj.m_pParticleTex;
		m_nNumParticles				= obj.m_nNumParticles;
		m_nNumParticlesPerSecond	= obj.m_nNumParticlesPerSecond;
		m_vEmitterVelocity			= obj.m_vEmitterVelocity;
		m_vEmitterPosition			= obj.m_vEmitterPosition;
		m_vParticleAcceleration		= obj.m_vParticleAcceleration;
		m_vParticleVelocity			= obj.m_vParticleVelocity;
		m_vForce					= obj.m_vForce;
		m_vForcePoint				= obj.m_vForcePoint;
		m_ucStartA					= obj.m_ucStartA;
		m_ucStartR					= obj.m_ucStartB;
		m_ucStartG					= obj.m_ucStartG;
		m_ucStartB					= obj.m_ucStartR;
		m_ucDiffA					= obj.m_ucDiffA; 
		m_ucDiffR					= obj.m_ucDiffB;
		m_ucDiffG					= obj.m_ucDiffG;
		m_ucDiffB					= obj.m_ucDiffR;
		m_fParticleSpread			= obj.m_fParticleSpread;
		m_fStartScale				= obj.m_fStartScale;
		m_fDiffScale				= obj.m_fDiffScale;
		m_fMinLife					= obj.m_fMinLife;
		m_fMaxLife					= obj.m_fMaxLife;
		m_nRectWidth				= obj.m_nRectWidth;
		m_nRectHeight				= obj.m_nRectHeight;

		m_nSourceBlend				= obj.m_nSourceBlend;
		m_nDestBlend				= obj.m_nDestBlend;
		m_bIsUsingForcePoint		= obj.m_bIsUsingForcePoint;
		m_bRefreshParticles			= obj.m_bRefreshParticles;
		m_bIsLooping				= obj.m_bIsLooping;
		m_nNumFrames				= obj.m_nNumFrames;

		for(vector<CFrame>::const_iterator iter = obj.m_vecFrames.begin(); iter != obj.m_vecFrames.end(); ++iter)
			m_vecFrames.push_back(*iter);

		m_bIsPlaying				= false;
		m_nParticlesMade			= 0;
		m_nCurrentFrame				= 0;

		D3DXMatrixIdentity(&m_mOldViewMatrix);
		D3DXMatrixIdentity(&m_mViewMatrix);
	}

	//~CParticleEmitter(void)
	~CParticleEmitter(void)
	{
		if(m_pObjectToTrack)
		{
			m_pObjectToTrack->ReleaseReference();
			m_pObjectToTrack = NULL;
		}

		m_vecFrames.clear();
		m_pDeadList.clear();
		m_pAliveList.clear();

		if(m_arrParticles)
		{
			delete[] m_arrParticles;
			m_arrParticles = NULL;
		}

		if(m_szName)
		{
			delete[] m_szName;
			m_szName = NULL;
		}
	}

	//void Update(float fDelta);
	void Update(float fDelta)
	{
		if(m_pObjectToTrack)
		{
			if(m_pObjectToTrack->CheckRemove())
			{
				m_pObjectToTrack->ReleaseReference();
				m_pObjectToTrack = NULL;

				m_bIsPlaying = false;

				if(!m_bIsPersistent)
					this->DoRemove();

				return;
			}
			else
				m_vEmitterPosition = m_pObjectToTrack->GetPosition() + m_vPositionOffset;
		}

		if(m_bIsPlaying && m_vecFrames.size() != 0)
		{
			if(m_vecFrames[m_nCurrentFrame].m_bIsEmitting)
			{
				if(!m_pAliveList.empty())
				{
					for(m_AliveIter = m_pAliveList.begin(); m_AliveIter != m_pAliveList.end(); )
					{
						if((*m_AliveIter)->m_fCurrentLife <= 0.0f)
						{
							m_pDeadList.push_back((*m_AliveIter));
							m_AliveIter = m_pAliveList.erase(m_AliveIter);
						}
						else
						{
							(*m_AliveIter)->m_vAcceleration += m_vForce * fDelta;

							if (m_bIsUsingForcePoint)
								(*m_AliveIter)->m_vAcceleration += (m_vForcePoint - (*m_AliveIter)->m_vPosition) * fDelta;

							(*m_AliveIter)->m_vVelocity += (*m_AliveIter)->m_vAcceleration * fDelta;

							(*m_AliveIter)->m_vPosition += (*m_AliveIter)->m_vVelocity * fDelta;

							(*m_AliveIter)->m_fCurrentLife -= fDelta;

							++m_AliveIter;
						}
					}
				}

				if (!m_bRefreshParticles && m_nParticlesMade >= m_nNumParticles)
				{
				}
				else
				{
					int nNumParticlesToMake = (int)(m_nNumParticlesPerSecond * fDelta);

					if(nNumParticlesToMake <= 0)
						nNumParticlesToMake = 1;

					if(!m_pDeadList.empty())
					{
						for(int i = 0; i < nNumParticlesToMake; ++i)
						{
							if(!m_pDeadList.empty())
							{
								m_DeadIter = m_pDeadList.begin();

								(*m_DeadIter)->m_vAcceleration = m_vParticleAcceleration;

								(*m_DeadIter)->m_vVelocity = m_vParticleVelocity;

								(*m_DeadIter)->m_vPosition.x = m_vEmitterPosition.x + rand()%m_nRectWidth;
								(*m_DeadIter)->m_vPosition.y = m_vEmitterPosition.y + rand()%m_nRectHeight;

								(*m_DeadIter)->m_fMaxLife = (*m_DeadIter)->m_fCurrentLife = (float)(m_fMinLife + ((float)((rand()%1000)/1000.0f) * (m_fMaxLife - m_fMinLife)));

								float fAngleToAdd = (float)((float)((rand()%1000)/1000.0f) * (m_fParticleSpread / 2));

								if (rand()%2 == 0)
									fAngleToAdd = -fAngleToAdd;

								(*m_DeadIter)->m_vVelocity.y *= -1.0f;

								D3DXVECTOR2 vRotated;
								vRotated.x = (cos(fAngleToAdd) * (*m_DeadIter)->m_vVelocity.x) + (sin(fAngleToAdd) * (*m_DeadIter)->m_vVelocity.y);
								vRotated.y = (-sin(fAngleToAdd) * (*m_DeadIter)->m_vVelocity.x) + (cos(fAngleToAdd) * (*m_DeadIter)->m_vVelocity.y);

								vRotated.y *= -1.0f;

								(*m_DeadIter)->m_vVelocity = vRotated;

								m_pAliveList.push_back((*m_DeadIter));
								m_DeadIter = m_pDeadList.erase(m_DeadIter);

								m_nParticlesMade++;
							}
						}
					}
				}
			}

			if(m_vecFrames[m_nCurrentFrame].Update(fDelta))
			{
				++m_nCurrentFrame;

				if(m_nCurrentFrame >= m_nNumFrames)
				{
					if(m_bIsLooping)
						ResetFrames();
					else
					{
						m_bIsPlaying = false;
						if(!m_bIsPersistent)
							DoRemove();
					}
				}
				else if(m_vecFrames[m_nCurrentFrame].m_bResetParticles)
					ResetParticles();
			}
		}
	}

	//void Draw(void);
	void Draw()
	{
		if(m_bIsPlaying && m_vecFrames[m_nCurrentFrame].m_bIsEmitting)
		{
			CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

			pD3D->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND, &m_nDefaultSourceBlend);
			pD3D->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND, &m_nDefaultDestBlend);
			pD3D->GetSprite()->Flush();
			pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, m_nSourceBlend);
			pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, m_nDestBlend);

			D3DXMatrixIdentity(&m_mOldViewMatrix);
			pD3D->GetSprite()->GetTransform(&m_mOldViewMatrix);

			for (m_AliveIter = m_pAliveList.begin(); m_AliveIter != m_pAliveList.end(); ++m_AliveIter)
			{
				float fPercentage = (*m_AliveIter)->m_fCurrentLife / (*m_AliveIter)->m_fMaxLife;
				fPercentage = 1.0f - fPercentage;
				D3DXMatrixIdentity(&m_mViewMatrix);
				D3DXMatrixScaling(&m_mViewMatrix, m_fStartScale + (m_fDiffScale * fPercentage), m_fStartScale + (m_fDiffScale * fPercentage), 1.0f);
				m_mViewMatrix = m_mOldViewMatrix * m_mViewMatrix;
				pD3D->GetSprite()->SetTransform(&m_mViewMatrix);
				m_cColor = D3DCOLOR_ARGB((unsigned char)(m_ucStartA + (m_ucDiffA * fPercentage)), (unsigned char)(m_ucStartB + (m_ucDiffB * fPercentage)), (unsigned char)(m_ucStartG + (m_ucDiffG * fPercentage)), (unsigned char)(m_ucStartR + (m_ucDiffR * fPercentage)));
				pD3D->GetSprite()->Draw(m_pParticleTex, NULL, NULL, &D3DXVECTOR3((*m_AliveIter)->m_vPosition.x, (*m_AliveIter)->m_vPosition.y, 0.0f), m_cColor);
			}

			pD3D->GetSprite()->SetTransform(&m_mOldViewMatrix);

			pD3D->GetSprite()->Flush();
			pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, m_nDefaultSourceBlend);
			pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, m_nDefaultDestBlend);
		}

	}

	//void Play(void);
	void Play(bool bIsPersistent = true)
	{
		ResetFrames();
		ResetParticles();
		m_bIsPlaying = true;
		m_bIsPersistent = bIsPersistent;
	}

	//void Stop(void);
	void Stop(void)
	{
		ResetFrames();
		ResetParticles();
		m_bIsPlaying = false;
	}

	//void AddFrame(const char* szTrigger, float fDuration, bool bIsEmitting, bool bResetParticles);
	void AddFrame(const char* szTrigger, float fDuration, bool bIsEmitting, bool bResetParticles)
	{
		CFrame newFrame(szTrigger, fDuration, bIsEmitting, bResetParticles);
		m_vecFrames.push_back(newFrame);
	}

	//void InitParticles(void);
	void InitParticles(void)
	{
		if(!m_arrParticles)
		{
			m_arrParticles = new CParticle[m_nNumParticles];

			m_pDeadList.clear();
			m_pAliveList.clear();

			for(int i = 0; i < m_nNumParticles; ++i)
				m_pDeadList.push_back(&m_arrParticles[i]);
		}
	}

	//void ResetParticles(void);
	void ResetParticles(void)
	{
		if(m_arrParticles)
		{
			for(int i = 0; i < m_nNumParticles; ++i)
				m_arrParticles[i].m_fCurrentLife = 0;
		}
	}

	//void ResetFrames(void);
	void CParticleEmitter::ResetFrames(void)
	{
		for(unsigned int i = 0; i < m_vecFrames.size(); ++i)
			m_vecFrames[i].m_fAccumulator = 0.0f;

		m_nCurrentFrame = 0;
	}

	float GetZDepth() const { return m_vEmitterPosition.y; }

	////////////////Accessors and Mutators////////////////
	const char* GetName(void)								{ return m_szName; }
	//void SetName(const char* szName);
	void SetName(const char* szName)
	{
		if(m_szName)
		{
			delete m_szName;
			m_szName = NULL;
		}

		int nSize = strlen(szName);
		m_szName = new char[nSize + 1];
		strcpy_s(m_szName, nSize + 1, szName);
	}

	IDirect3DTexture9* GetTexture(void)						{ return m_pParticleTex; }
	void SetTexture(IDirect3DTexture9* pTex)				{ m_pParticleTex = pTex; }

	int GetNumParticles(void)								{ return m_nNumParticles; }
	void SetNumParticles(int nNumParticles)					{ m_nNumParticles = nNumParticles; }

	int GetParticlesPerSecond(void)							{ return m_nNumParticlesPerSecond; }
	void SetParticlesPerSecond(int nNumPartPerSecond)		{ m_nNumParticlesPerSecond = nNumPartPerSecond; }

	bool GetIsLooping(void)									{ return m_bIsLooping; }
	void SetIsLooping(bool bIsLooping)						{ m_bIsLooping = bIsLooping; }

	int GetNumFrames(void)									{ return m_nNumFrames; }
	void SetNumFrames(int nNumFrames)						{ m_nNumFrames = nNumFrames; }

	D3DXVECTOR2 GetPostion(void)							{ return m_vEmitterPosition; }
	void SetPosition(D3DXVECTOR2 vEmitterPosition)			{ m_vEmitterPosition = vEmitterPosition; }

	D3DXVECTOR2 GetVelocity(void)							{ return m_vEmitterVelocity; }
	void SetVelocity(D3DXVECTOR2 vEmitterVelocity)			{ m_vEmitterVelocity = vEmitterVelocity; }

	D3DXVECTOR2 GetParticleAcceleration(void)				{ return m_vParticleAcceleration; }
	void SetParticleAcceleration(D3DXVECTOR2 vParticleAccel){ m_vParticleAcceleration = vParticleAccel; }

	D3DXVECTOR2 GetParticleVelocity(void)					{ return m_vParticleVelocity; }
	void SetParticleVelocity(D3DXVECTOR2 vParticleVel)		{ m_vParticleVelocity = vParticleVel; }

	D3DXVECTOR2 GetForce(void)								{ return m_vForce; }
	void SetForce(D3DXVECTOR2 vForce)						{ m_vForce = vForce; }

	D3DXVECTOR2 GetForcePoint(void)							{ return m_vForcePoint; }
	void SetForcePoint(D3DXVECTOR2 vForcePoint)				{ m_vForcePoint = vForcePoint; }

	//D3DXCOLOR GetStartColor(void);
	D3DXCOLOR GetStartColor(void)
	{
		return D3DCOLOR_ARGB(m_ucStartA, m_ucStartR, m_ucStartG, m_ucStartB);
	}

	//void SetStartColor(unsigned char ucA, unsigned char ucR, unsigned char ucG, unsigned char ucB);
	void SetStartColor(unsigned char ucA, unsigned char ucR, unsigned char ucG, unsigned char ucB)
	{
		m_ucStartA = ucA;
		m_ucStartR = ucR;
		m_ucStartG = ucG;
		m_ucStartB = ucB;
	}

	//D3DXCOLOR GetDiffColor(void);
	D3DXCOLOR GetDiffColor(void)
	{
		return D3DCOLOR_ARGB(m_ucDiffA, m_ucDiffR, m_ucDiffG, m_ucDiffB);
	}

	//void SetDiffColor(unsigned char ucA, unsigned char ucR, unsigned char ucG, unsigned char ucB);
	void SetDiffColor(int ucA, int ucR, int ucG, int ucB)
	{
		m_ucDiffA = ucA;
		m_ucDiffR = ucR;
		m_ucDiffG = ucG;
		m_ucDiffB = ucB;
	}

	float GetSpread(void)									{ return m_fParticleSpread; }
	void SetSpread(float fRadianSpread)						{ m_fParticleSpread = fRadianSpread; }

	float GetStartScale(void)								{ return m_fStartScale; }
	void SetStartScale(float fStartScale)					{ m_fStartScale = fStartScale; }

	float GetDiffScale(void)								{ return m_fDiffScale; }
	void SetDiffScale(float fDiffScale)						{ m_fDiffScale = fDiffScale; }

	float GetMinLife(void)									{ return m_fMinLife; }
	void SetMinLife(float fMinLife)							{ m_fMinLife = fMinLife; }

	float GetMaxLife(void)									{ return m_fMaxLife; }
	void SetMaxLife(float fMaxLife)							{ m_fMaxLife = fMaxLife; }

	int GetRectWidth(void)									{ return m_nRectWidth; }
	void SetRectWidth(int nRectWidth)						{ m_nRectWidth = nRectWidth; }

	int GetRectHeight(void)									{ return m_nRectHeight; }
	void SetRectHeight(int nRectHeight)						{ m_nRectHeight = nRectHeight; }

	DWORD GetSourceBlend(void)								{ return m_nSourceBlend; }
	void SetSourceBlend(DWORD nSourceBlend)					{ m_nSourceBlend = nSourceBlend; }

	DWORD GetDestBlend(void)								{ return m_nDestBlend; }
	void SetDestBlend(DWORD nDestBlend)						{ m_nDestBlend = nDestBlend; }

	bool GetUsingForcePoint(void)							{ return m_bIsUsingForcePoint; }
	void SetUsingForcePoint(bool bIsUsingForcePoint)		{ m_bIsUsingForcePoint = bIsUsingForcePoint; }

	bool GetRefreshParticles(void)							{ return m_bRefreshParticles;}
	void SetRefreshParticles(bool bRefresh)					{ m_bRefreshParticles = bRefresh; }

	bool GetIsPlaying(void)									{ return m_bIsPlaying; }

	CGameObject* GetObjectToTrack(void)						{ return m_pObjectToTrack; }
	void SetObjectToTrack(CGameObject* pToTrack)			
	{ 
		if(m_pObjectToTrack)
		{
			m_pObjectToTrack->ReleaseReference();
			m_pObjectToTrack = NULL;
		}

		m_pObjectToTrack = pToTrack;
		m_pObjectToTrack->AcquireReference();
	}

	D3DXVECTOR2 GetOffset(void)								{ return m_vPositionOffset; }
	void SetOffset(D3DXVECTOR2 vOffset)						{ m_vPositionOffset = vOffset; }

	RECT GetDrawRectangle(void) const
	{
		RECT drawRect;
		drawRect.top = (int)m_vEmitterPosition.y;
		drawRect.left = (int)m_vEmitterPosition.x;
		drawRect.bottom = (int)m_vEmitterPosition.y + m_nRectHeight;
		drawRect.right = (int)m_vEmitterPosition.x + m_nRectWidth;

		return drawRect;
	}
};

#endif