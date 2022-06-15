#include "stdafx.h"
#include "UtilityMgr.h"
#include "KeyMgr.h"


_USING(Client)
_IMPLEMENT_SINGLETON(UtilityMgr);

UtilityMgr::UtilityMgr()
{
	m_pKeyMgr = Engine::KeyMgr::GetInstance();
	m_pKeyMgr->AddRef();
}


void UtilityMgr::KeyCheck()
{
	if (m_pKeyMgr->KeyDown(DIK_1))
	{
		g_bColliderDraw = !g_bColliderDraw;
	}
}

bool UtilityMgr::FindString(const char * pAnimTag, const char * pFindTag)
{
	if (pAnimTag == nullptr)
	{
		_MSG_BOX("FindString pAnimTag Failed!");
		return false;
	}

	if (pFindTag == nullptr)
	{
		_MSG_BOX("FindString pFindTag Failed!");
		return false;
	}

	size_t length = strlen(pAnimTag);
	for (size_t i = 0; i < length; i++)
	{
		if (strcmp(pAnimTag + i, pFindTag) == 0)
		{
			return true;
		}
	}

	return false;
}

const D3DXVECTOR3& UtilityMgr::RandomDir(const D3DXVECTOR3 & vPos, const float timeDelta, float distance)
{
	mt19937 mtRand((size_t)(rand()));
	uniform_int_distribution<size_t> range(0, 7);
	mMoveState = range(mtRand);

	switch (mMoveState)
	{
	case OBJ_MOTION::DOWN:
		m_vRandomDir.x = 0.f;
		m_vRandomDir.z = -1.f;
		break;
	case OBJ_MOTION::LD:
		m_vRandomDir.x = 1.f;
		m_vRandomDir.z = -1.f;
		break;
	case OBJ_MOTION::LEFT:
		m_vRandomDir.x = -1.f;
		m_vRandomDir.z = 0.f;
		break;
	case OBJ_MOTION::LU:
		m_vRandomDir.x = -1.f;
		m_vRandomDir.z = 1.f;
		break;
	case OBJ_MOTION::RD:
		m_vRandomDir.x = 1.f;
		m_vRandomDir.z = -1.f;
		break;
	case OBJ_MOTION::RIGHT:
		m_vRandomDir.x = 1.f;
		m_vRandomDir.z = 0.f;
		break;
	case OBJ_MOTION::RU:
		m_vRandomDir.x = 1.f;
		m_vRandomDir.z = 1.f;
		break;
	case OBJ_MOTION::UP:
		m_vRandomDir.x = 0.f;
		m_vRandomDir.z = 1.f;
		break;
	default:
		assert(false && "UtilityMgr RandomDir!!");
		break;
	}

	m_vRandomDir.y = 0.f;
	return m_vRandomDir;
}

float UtilityMgr::Cal_TargetDistance(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vTarget, bool bYPos)
{
	D3DXVECTOR3 targetDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	
	if (bYPos == true)
	{
		D3DXVECTOR3 vTempPos = vPos;
		vTempPos.y = 0.f;
		D3DXVECTOR3 vTargetTempPos = vTarget;
		vTargetTempPos.y = 0.f;

		targetDir = vTargetTempPos - vTempPos;
	}
	else
	{
		targetDir = vTarget - vPos;
	}
	
	float distance = D3DXVec3Length(&targetDir);

	return distance;
}

const D3DXVECTOR3& UtilityMgr::CalTargetDir(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vTarget, bool bYPos)
{
	m_vTargetDir = vTarget - vPos;
	D3DXVec3Normalize(&m_vTargetDir, &m_vTargetDir);

	if (bYPos == false)
	{
		m_vTargetDir.y = 0.f;
	}


	return m_vTargetDir;
}

const float UtilityMgr::CalDirDegree(const D3DXVECTOR3 & vSrcPos, const D3DXVECTOR3 & vDestPos)
{
	D3DXVec3Normalize(&m_vSrcDir, &vSrcPos);
	D3DXVec3Normalize(&m_vDestDir, &vDestPos);

	m_vSrcDir.y = 0.f;
	m_vDestDir.y = 0.f;

	float radian = acosf(D3DXVec3Dot(&m_vSrcDir, &m_vDestDir));

	return D3DXToDegree(radian);
}

const bool UtilityMgr::Cal_DirCross(const D3DXVECTOR3 & vSrcDir, const D3DXVECTOR3 & vDestDir)
{
	m_vSrcDir.x = vSrcDir.x;
	m_vSrcDir.z = vSrcDir.z;

	m_vDestDir.x = vDestDir.x;
	m_vDestDir.z = vDestDir.z;

	D3DXVec3Cross(&m_vCross, &m_vSrcDir, &m_vDestDir);

	if (m_vCross.y > 0)
		return true;

	return false;
}

void UtilityMgr::Cal_SlidingVector(const D3DXVECTOR3 & vDir, D3DXVECTOR3 & vLineNormal, D3DXVECTOR3 * pSlide, float* pSlideSpeed)
{
	vLineNormal.y = vDir.y;
	D3DXVec3Normalize(&vLineNormal, &vLineNormal);

	*pSlide = vLineNormal * D3DXVec3Dot(&vDir, &vLineNormal);
	*pSlide = vDir - *pSlide;
	*pSlideSpeed = D3DXVec3Length(pSlide);

	D3DXVec3Normalize(pSlide, pSlide);
}

void UtilityMgr::StringReplace(string & animTag, const char * pSrcKey, const char * pReplaceKey)
{
	string::size_type pos = 0;
	string result;

	pos = animTag.find(pSrcKey);

	if (pos != string::npos)
	{
		result = animTag.replace(animTag.begin() + pos, animTag.begin() + pos + strlen(pSrcKey), pReplaceKey);
	}
}

DWORD UtilityMgr::Free()
{
	Engine::Safe_Release(m_pKeyMgr);
	return 0;
}
