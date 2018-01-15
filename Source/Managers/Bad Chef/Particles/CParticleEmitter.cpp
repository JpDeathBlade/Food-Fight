//////////////////////////////////////////////////////
// File: "CParticleEmitter.cpp"
// Creator: AB
// Creation Date: 10/2/09
// Last Modified: 10/3/09
// Last Modified By: AB
//////////////////////////////////////////////////////
//#include "CParticleEmitter.h"


//CParticleEmitter::CParticle::CParticle(void)
//{
//	m_vAcceleration	= D3DXVECTOR2(0.0f, 0.0f);
//	m_vVelocity		= D3DXVECTOR2(0.0f, 0.0f);
//	m_vPosition		= D3DXVECTOR2(0.0f, 0.0f);
//	m_fMaxLife		= 0.0f;
//	m_fCurrentLife	= 0.0f;
//}

//CParticleEmitter::CFrame::CFrame(void)
//{
//	m_szTrigger = NULL;
//	m_fDuration = m_fAccumulator = 0.0f;
//	m_bIsEmitting = true;
//	m_bResetParticles = false;
//}

//CParticleEmitter::CFrame::CFrame(const char* szTrigger, float fDuration, bool bIsEmitting, bool bResetParticles)
//{
//	m_szTrigger = NULL;
//	int nSize = strlen(szTrigger);
//	m_szTrigger = new char[nD3DXVECTOR2 +1];
//	strcpy_s(m_szTrigger, nD3DXVECTOR2 + 1, szTrigger);
//
//	m_fDuration = m_fAccumulator = fDuration;
//	m_bIsEmitting = bIsEmitting;
//	m_bResetParticles = bResetParticles;
//}

//CParticleEmitter::CFrame::CFrame(const CFrame& obj)
//{
//	m_szTrigger = NULL;
//	int nSize = strlen(obj.m_szTrigger);
//	m_szTrigger = new char[nD3DXVECTOR2 +1];
//	strcpy_s(m_szTrigger, nD3DXVECTOR2 + 1, obj.m_szTrigger);
//
//	m_fDuration = m_fAccumulator = obj.m_fDuration;
//	m_bIsEmitting = obj.m_bIsEmitting;
//	m_bResetParticles = obj.m_bResetParticles;
//}

//CParticleEmitter::CFrame::~CFrame(void)
//{
//	if(m_szTrigger)
//	{
//		delete m_szTrigger;
//		m_szTrigger = NULL;
//	}
//}

//bool CParticleEmitter::CFrame::Update(float fDelta)
//{
//	m_fAccumulator += fDelta;
//
//	if(m_fAccumulator >= m_fDuration)
//		return true;
//
//	return false;
//}

//CParticleEmitter::CParticleEmitter(void)
//{
//	SetCollideable(false);
//	m_szName					= NULL;
//	m_pParticleTex				= NULL;
//	m_nNumParticles				= 0;
//	m_arrParticles				= NULL;
//	m_nNumParticlesPerSecond	= 0;
//	m_vEmitterVelocity			= D3DXVECTOR2(0.0f, 0.0f);
//	m_vEmitterPosition			= D3DXVECTOR2(0.0f, 0.0f);
//	m_vParticleAcceleration		= D3DXVECTOR2(0.0f, 0.0f);
//	m_vParticleVelocity			= D3DXVECTOR2(0.0f, 0.0f);
//	m_vForce					= D3DXVECTOR2(0.0f, 0.0f);
//	m_vForcePoint				= D3DXVECTOR2(0.0f, 0.0f);
//	m_ucStartA = m_ucStartR = m_ucStartG = m_ucStartB = 255;
//	m_ucDiffA = m_ucDiffR = m_ucDiffG = m_ucDiffB = 0;
//	m_fParticleSpread			= D3DX_PI * 2.0f;
//	m_fStartScale				= 1.0f;
//	m_fDiffScale				= 0.0f;
//	m_fMinLife = m_fMaxLife		= 0.0f;
//	m_nRectWidth = m_nRectHeight= 1;
//	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND, &m_nDefaultSourceBlend);
//	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND, &m_nDefaultDestBlend);
//	m_nSourceBlend				= m_nDefaultSourceBlend;
//	m_nDestBlend				= m_nDefaultDestBlend;
//	m_bIsUsingForcePoint		= false;
//	m_bRefreshParticles			= true;
//	m_nNumFrames				= 0;
//	m_bIsLooping				= true;
//	m_bIsPlaying				= false;
//	m_nParticlesMade			= 0;
//	m_nCurrentFrame				= 0;
//
//	D3DXMatrixIdentity(&m_mOldViewMatrix);
//	D3DXMatrixIdentity(&m_mViewMatrix);
//}

//CParticleEmitter::CParticleEmitter(const CParticleEmitter& obj)
//{
//	SetCollideable(false);
//
//	int nSize = strlen(obj.m_szName);
//	m_szName = new char[nD3DXVECTOR2 + 1];
//	strcpy_s(m_szName, nD3DXVECTOR2 + 1, obj.m_szName);
//
//	m_arrParticles				= NULL;
//
//	m_pParticleTex				= obj.m_pParticleTex;
//	m_nNumParticles				= obj.m_nNumParticles;
//	m_nNumParticlesPerSecond	= obj.m_nNumParticlesPerSecond;
//	m_vEmitterVelocity			= obj.m_vEmitterVelocity;
//	m_vEmitterPosition			= obj.m_vEmitterPosition;
//	m_vParticleAcceleration		= obj.m_vParticleAcceleration;
//	m_vParticleVelocity			= obj.m_vParticleVelocity;
//	m_vForce					= obj.m_vForce;
//	m_vForcePoint				= obj.m_vForcePoint;
//	m_ucStartA					= obj.m_ucStartA;
//	m_ucStartR					= obj.m_ucStartB;
//	m_ucStartG					= obj.m_ucStartG;
//	m_ucStartB					= obj.m_ucStartR;
//	m_ucDiffA					= obj.m_ucDiffA; 
//	m_ucDiffR					= obj.m_ucDiffB;
//	m_ucDiffG					= obj.m_ucDiffG;
//	m_ucDiffB					= obj.m_ucDiffR;
//	m_fParticleSpread			= obj.m_fParticleSpread;
//	m_fStartScale				= obj.m_fStartScale;
//	m_fDiffScale				= obj.m_fDiffScale;
//	m_fMinLife					= obj.m_fMinLife;
//	m_fMaxLife					= obj.m_fMaxLife;
//	m_nRectWidth				= obj.m_nRectWidth;
//	m_nRectHeight				= obj.m_nRectHeight;
//	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND, &m_nDefaultSourceBlend);
//	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND, &m_nDefaultDestBlend);
//	m_nSourceBlend				= obj.m_nSourceBlend;
//	m_nDestBlend				= obj.m_nDestBlend;
//	m_bIsUsingForcePoint		= obj.m_bIsUsingForcePoint;
//	m_bRefreshParticles			= obj.m_bRefreshParticles;
//	m_bIsLooping				= obj.m_bIsLooping;
//	m_nNumFrames				= obj.m_nNumFrames;
//
//	for(vector<CFrame>::const_iterator iter = obj.m_vecFrames.begin(); iter != obj.m_vecFrames.end(); ++iter)
//		m_vecFrames.push_back(*iter);
//
//	m_bIsPlaying				= false;
//	m_nParticlesMade			= 0;
//	m_nCurrentFrame				= 0;
//
//	D3DXMatrixIdentity(&m_mOldViewMatrix);
//	D3DXMatrixIdentity(&m_mViewMatrix);
//}

//CParticleEmitter::~CParticleEmitter(void)
//{
//	m_vecFrames.clear();
//	m_pDeadList.clear();
//	m_pAliveList.clear();
//
//	if(m_arrParticles)
//	{
//		delete[] m_arrParticles;
//		m_arrParticles = NULL;
//	}
//
//	if(m_szName)
//	{
//		delete[] m_szName;
//		m_szName = NULL;
//	}
//}


//void CParticleEmitter::Update(float fDelta)
//{
//	if(m_bIsPlaying && m_vecFrames.size() != 0)
//	{
//		if(m_vecFrames[m_nCurrentFrame].m_bIsEmitting)
//		{
//			if(!m_pAliveList.empty())
//			{
//				for(m_AliveIter = m_pAliveList.begin(); m_AliveIter != m_pAliveList.end(); )
//				{
//					if((*m_AliveIter)->m_fCurrentLife <= 0.0f)
//					{
//						m_pDeadList.push_back((*m_AliveIter));
//						m_AliveIter = m_pAliveList.erase(m_AliveIter);
//					}
//					else
//					{
//						(*m_AliveIter)->m_vAcceleration += m_vForce * fDelta;
//
//						if (m_bIsUsingForcePoint)
//							(*m_AliveIter)->m_vAcceleration += (m_vForcePoint - (*m_AliveIter)->m_vPosition) * fDelta;
//
//						(*m_AliveIter)->m_vVelocity += (*m_AliveIter)->m_vAcceleration * fDelta;
//
//						(*m_AliveIter)->m_vPosition += (*m_AliveIter)->m_vVelocity * fDelta;
//
//						(*m_AliveIter)->m_fCurrentLife -= fDelta;
//
//						++m_AliveIter;
//					}
//				}
//			}
//
//			if (!m_bRefreshParticles && m_nParticlesMade >= m_nNumParticles)
//            {
//            }
//			else
//			{
//				int nNumParticlesToMake = (int)(m_nNumParticlesPerSecond * fDelta);
//
//				if(!m_pDeadList.empty())
//				{
//					for(int i = 0; i < nNumParticlesToMake; ++i)
//					{
//						m_DeadIter = m_pDeadList.begin();
//
//						(*m_DeadIter)->m_vAcceleration = m_vParticleAcceleration;
//
//						(*m_DeadIter)->m_vVelocity = m_vParticleVelocity;
//
//						(*m_DeadIter)->m_vPosition.x = m_vEmitterPosition.x + rand()%m_nRectWidth;
//						(*m_DeadIter)->m_vPosition.y = m_vEmitterPosition.y + rand()%m_nRectHeight;
//
//						(*m_DeadIter)->m_fMaxLife = (*m_DeadIter)->m_fCurrentLife = (float)(m_fMinLife + ((float)((rand()%1000)/1000.0f) * (m_fMinLife - m_fMaxLife)));
//
//						float fAngleToAdd = (float)((float)((rand()%1000)/1000.0f) * (m_fParticleSpread / 2));
//
//						if (rand()%2 == 0)
//							fAngleToAdd = -fAngleToAdd;
//
//						(*m_DeadIter)->m_vVelocity.y *= -1.0f;
//
//						D3DXVECTOR2 vRotated;
//						vRotated.x = (cos(fAngleToAdd) * (*m_DeadIter)->m_vVelocity.x) + (sin(fAngleToAdd) * (*m_DeadIter)->m_vVelocity.y);
//						vRotated.y = (-sin(fAngleToAdd) * (*m_DeadIter)->m_vVelocity.x) + (cos(fAngleToAdd) * (*m_DeadIter)->m_vVelocity.y);
//
//						vRotated.y *= -1.0f;
//
//						(*m_DeadIter)->m_vVelocity = vRotated;
//
//						m_pAliveList.push_back((*m_DeadIter));
//						m_DeadIter = m_pDeadList.erase(m_DeadIter);
//
//						m_nParticlesMade++;
//					}
//				}
//			}
//		}
//
//		if(m_vecFrames[m_nCurrentFrame].Update(fDelta))
//		{
//			++m_nCurrentFrame;
//
//			if(m_nCurrentFrame >= m_nNumFrames)
//			{
//				if(m_bIsLooping)
//					ResetFrames();
//				else
//					m_bIsPlaying = false;
//			}
//			else if(m_vecFrames[m_nCurrentFrame].m_bResetParticles)
//				ResetParticles();
//		}
//	}
//}

//void CParticleEmitter::Draw(void)
//{
//	if(m_bIsPlaying && m_vecFrames[m_nCurrentFrame].m_bIsEmitting)
//	{
//		CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
//		pD3D->GetSprite()->Flush();
//		pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, m_nSourceBlend);
//		pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, m_nDestBlend);
//
//		D3DXMatrixIdentity(&m_mOldViewMatrix);
//		pD3D->GetSprite()->GetTransform(&m_mOldViewMatrix);
//
//		for (m_AliveIter = m_pAliveList.begin(); m_AliveIter != m_pAliveList.end(); ++m_AliveIter)
//        {
//            float fPercentage = (*m_AliveIter)->m_fCurrentLife / (*m_AliveIter)->m_fMaxLife;
//            fPercentage = 1.0f - fPercentage;
//			D3DXMatrixIdentity(&m_mViewMatrix);
//			D3DXMatrixScaling(&m_mViewMatrix, m_fStartScale + (m_fDiffScale * fPercentage), m_fStartScale + (m_fDiffScale * fPercentage), 1.0f);
//			m_mViewMatrix = m_mOldViewMatrix * m_mViewMatrix;
//			pD3D->GetSprite()->SetTransform(&m_mViewMatrix);
//			m_cColor = D3DCOLOR_ARGB((unsigned char)(m_ucStartA + (m_ucDiffA * fPercentage)), (unsigned char)(m_ucStartR + (m_ucDiffR * fPercentage)), (unsigned char)(m_ucStartG + (m_ucDiffG * fPercentage)), (unsigned char)(m_ucStartB + (m_ucDiffB * fPercentage)));
//			pD3D->GetSprite()->Draw(m_pParticleTex, NULL, NULL, &D3DXVECTOR3((*m_AliveIter)->m_vPosition.x, (*m_AliveIter)->m_vPosition.y, 0.0f), m_cColor);
//        }
//
//		pD3D->GetSprite()->SetTransform(&m_mOldViewMatrix);
//
//        pD3D->GetSprite()->Flush();
//		pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, m_nDefaultSourceBlend);
//		pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, m_nDefaultDestBlend);
//	}
//
//}

//void CParticleEmitter::Play(void)
//{
//	ResetFrames();
//	ResetParticles();
//	m_bIsPlaying = true;
//}

//void CParticleEmitter::Stop(void)
//{
//	ResetFrames();
//	ResetParticles();
//	m_bIsPlaying = false;
//}

//void CParticleEmitter::AddFrame(const char* szTrigger, float fDuration, bool bIsEmitting, bool bResetParticles)
//{
//	CFrame newFrame(szTrigger, fDuration, bIsEmitting, bResetParticles);
//	m_vecFrames.push_back(newFrame);
//}

//void CParticleEmitter::InitParticles(void)
//{
//	if(!m_arrParticles)
//	{
//		m_arrParticles = new CParticle[m_nNumParticles];
//
//		m_pDeadList.clear();
//		m_pAliveList.clear();
//
//		for(int i = 0; i < m_nNumParticles; ++i)
//			m_pDeadList.push_back(&m_arrParticles[i]);
//	}
//}

//void CParticleEmitter::ResetParticles(void)
//{
//	if(m_arrParticles)
//	{
//		for(int i = 0; i < m_nNumParticles; ++i)
//			m_arrParticles[i].m_fCurrentLife = 0;
//	}
//}

//void CParticleEmitter::ResetFrames(void)
//{
//	for(unsigned int i = 0; i < m_vecFrames.size(); ++i)
//		m_vecFrames[i].m_fAccumulator = 0.0f;
//
//	m_nCurrentFrame = 0;
//}

//void CParticleEmitter::SetName(const char* szName)
//{
//	if(m_szName)
//	{
//		delete m_szName;
//		m_szName = NULL;
//	}
//
//	int nSize = strlen(szName);
//	m_szName = new char[nD3DXVECTOR2 + 1];
//	strcpy_s(m_szName, nD3DXVECTOR2 + 1, szName);
//}

//D3DXCOLOR CParticleEmitter::GetStartColor(void)
//{
//	return D3DCOLOR_ARGB(m_ucStartA, m_ucStartR, m_ucStartG, m_ucStartB);
//}

//void CParticleEmitter::SetStartColor(unsigned char ucA, unsigned char ucR, unsigned char ucG, unsigned char ucB)
//{
//	m_ucStartA = ucA;
//	m_ucStartR = ucR;
//	m_ucStartG = ucG;
//	m_ucStartB = ucB;
//}

//D3DXCOLOR CParticleEmitter::GetDiffColor(void)
//{
//	return D3DCOLOR_ARGB(m_ucDiffA, m_ucDiffR, m_ucDiffG, m_ucDiffB);
//}

//void CParticleEmitter::SetDiffColor(unsigned char ucA, unsigned char ucR, unsigned char ucG, unsigned char ucB)
//{
//	m_ucDiffA = ucA;
//	m_ucDiffR = ucR;
//	m_ucDiffG = ucG;
//	m_ucDiffB = ucB;
//}