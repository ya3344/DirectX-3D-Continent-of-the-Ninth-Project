#include "stdafx.h"
#include "CamearFPS.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "PlayerAnim.h"

_USING(Client)

CameraFPS::CameraFPS(LPDIRECT3DDEVICE9 pDevice)
	:Camera(pDevice)
	,m_pUtilityMgr(UtilityMgr::GetInstance())
{
	m_pUtilityMgr->AddRef();
}

CameraFPS::CameraFPS(const CameraFPS & rhs)
	: Camera(rhs)
	, m_pUtilityMgr(rhs.m_pUtilityMgr)
	, m_pBossPos(rhs.m_pBossPos)
{
	m_pUtilityMgr->AddRef();
}

HRESULT CameraFPS::Init()
{
	Engine::Component* pComponent = nullptr;

	{
		//Component_Player_Transform
		m_pPlayerTransform = (Engine::Transform*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Transform", L"Prototype_GameObject_Player");
		m_pPlayerTransform->AddRef();

		m_pPlayerDir = m_pPlayerTransform->GetInfo(Engine::Transform::INFO_LOOK);
		m_pPlayerPos = m_pPlayerTransform->GetInfo(Engine::Transform::INFO_POSITION);
		m_pPlayerRight = m_pPlayerTransform->GetInfo(Engine::Transform::INFO_RIGHT);
	}

	{
		//Component_Player_Anim
		m_pPlayerAnim = (PlayerAnim*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Anim_Player", L"Prototype_GameObject_Player");
		m_pPlayerAnim->AddRef();

		m_pAnimState = m_pPlayerAnim->GetAnimState();
		m_pAnimTime = m_pPlayerAnim->GetAnimTime();
	}

	{
		m_pPlayerMesh = ((Engine::DynamicMesh*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Mesh_Player"));
		if (m_pPlayerMesh == nullptr)
			return E_FAIL;

		m_pHandMatrix = m_pPlayerMesh->Get_BoneMatrix("_86");
		if (m_pHandMatrix == nullptr)
			return E_FAIL;
	}

	{
		pComponent = m_pNavigation = (Engine::Navigation*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Navigation");
		if (m_pNavigation == nullptr)
			return E_FAIL;
		if (FAILED(Engine::GameObject::AddComponent(L"Component_Navigation", pComponent)))
			return E_FAIL;
		m_pNavigation->Setup_CurrentIndex(0);

		m_pPlayerNavigation = ((Engine::Navigation*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Navigation"));
		if (m_pPlayerNavigation == nullptr)
			return E_FAIL;

		m_pPlayerIndex = m_pPlayerNavigation->Get_CurrentIndex();
	}

	{
		Cal_CameraFPS();
	}



	return NOERROR;
}

HRESULT CameraFPS::ReadyPrototype(const D3DXVECTOR3 & vEye, const D3DXVECTOR3 & vAt, const D3DXVECTOR3 & vAxisY, const float fovY, const float aspect, const float projNear, const float projFar)
{
	if (FAILED(Camera::ReadyPrototype(vEye, vAt, vAxisY, fovY, aspect, projNear, projFar)))
		return E_FAIL;

	return NOERROR;
}

UPDATE CameraFPS::Update(const float timeDelta)
{
	if (*m_pAnimState == "LockOnTargetShot")
	{
		LockOnTarget_Mode();
		m_bModeChange = true;
	}
	else
	{
		if (m_bModeChange == true)
		{
			Cal_CameraFPS();
			m_bModeChange = false;
		}

		switch (mCameraMode)
		{
		case CAMERA_NORMAL:
			mPlayerDistance = 27.f;
			if (*m_pPlayerIndex >= 83 && *m_pPlayerIndex <= 94)
			{
				mCameraY = 20.f;
			}
			else
			{
				mCameraY = 13.f;
			}
			
			NormalMode(m_pPlayerDir);
			break;
		case CAMERA_BOSS:
			mPlayerDistance = 22.f;
			mCameraY = 13.f;
			NormalMode(m_pBossDir, 1.f);
			break;
		case CAMERA_BOSS_ACTION:
			BossActionMode();
			break;
		case CAMERA_GATE:
			mPlayerDistance = 60.f;
			mCameraY = 45.f;
			GateMode(m_pPlayerDir);
			//NormalMode(m_pPlayerDir);
			break;
		case CAMERA_SNOW_BALL:
			mPlayerDistance = 13.f;
			mCameraY = 6.f;
			NormalMode(m_pPlayerDir);
			break;
		default:
			break;
		}
	}
	

	return UPDATE::NORMAL;
}

UPDATE CameraFPS::LastUpdate(const float timeDelta)
{
	Camera::LastUpdate(timeDelta);

	return UPDATE::NORMAL;
}

D3DXVECTOR2 CameraFPS::Cal_IntersectPoint(const D3DXVECTOR2 & lineA, const D3DXVECTOR2 & lineB, const D3DXVECTOR2 & lineC, const D3DXVECTOR2 & lineD)
{
	D3DXVECTOR2 vResult = D3DXVECTOR2(0.f, 0.f);

	float increaseA = 0.f, constantA = 0.f, sameValueA = 0.f;
	float increaseC = 0.f, constantC = 0.f, sameValueC = 0.f;

	// 첫번째 직선
	if (lineA.x == lineB.x) // y축과 평행
	{
		sameValueA = lineA.x;
	}
	else
	{
		increaseA = (float)(lineB.y - lineA.y) / (lineB.x - lineA.x);
		constantA = lineA.y - increaseA * lineA.x;
	}

	// 두번째 직선
	if (lineC.x == lineD.x) // y축과 평행
	{
		sameValueC = lineC.x;
	}
	else
	{
		increaseC = (float)(lineD.y - lineC.y) / (lineD.x - lineC.x);
		constantC = lineC.y - increaseC * lineC.x;
	}

	if (lineA.x == lineB.x && lineC.x == lineD.x)
		return vResult;

	if (lineA.x == lineB.x)
	{
		vResult = D3DXVECTOR2(sameValueA, increaseC * sameValueA + constantC);
	}
	else if (lineC.x == lineD.x)
	{
		vResult = D3DXVECTOR2(sameValueC, increaseA * sameValueC + constantA);
	}
	else
	{
		vResult.x = -(constantA - constantC) / (increaseA - increaseC);
		vResult.y = (increaseA * vResult.x) + constantA;
	}

	return vResult;
}

void CameraFPS::NormalMode(const D3DXVECTOR3* pTargetDir, const float dir)
{
	if (pTargetDir == nullptr)
		return;

	D3DXVECTOR3 vPlayerDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVec3Normalize(&vPlayerDir, pTargetDir);

	m_vCameraPos = vPlayerDir * dir * (mPlayerDistance);
	m_vNaviPos = vPlayerDir * dir * (NAVI_DISTANCE);
	m_vCameraPos += (*m_pPlayerPos);
	mCameraDistance = m_pUtilityMgr->Cal_TargetDistance(mViewInfo.vEye, m_vCameraPos);

	if (mCameraDistance >= 1.f)
	{
		m_vTargetDir = m_pUtilityMgr->CalTargetDir(mViewInfo.vEye, m_vCameraPos) * 1.f;

		if (m_pNavigation->is_Move(*m_pPlayerPos, m_vNaviPos, &m_vLIneNoraml))
		{
			mViewInfo.vEye += m_vTargetDir;
 		}
		else
		{
			m_pNavigation->Setup_CurrentIndex(*m_pPlayerIndex);
		}
		
	}

	mViewInfo.vEye.y = m_pPlayerPos->y + mCameraY;
	mViewInfo.vAt = *m_pPlayerPos;
	mViewInfo.vAt.y += 6.f;
	
	
	if (*m_pAnimState == "StunAttack" || *m_pAnimState == "SpinCrushC" || *m_pAnimState == "UpperKick1E")
	{
		//m_vActionEye = D3DXVECTOR3(0.f, m_pHandMatrix->m[3][1], 0.f);
		m_vActionAt = D3DXVECTOR3(m_pHandMatrix->m[3][0] * 0.5f, m_pHandMatrix->m[3][1] * 0.5f, 0.f);
		//mViewInfo.vEye.y += m_pHandMatrix->m[3][1];
		//mViewInfo.vAt.y += m_pHandMatrix->m[3][1];
	}
	else if (*m_pAnimState == "AttShelTechnic3")
	{
		//m_vActionEye = D3DXVECTOR3((m_pHandMatrix->m[3][0] * 0.2f), 0.f, 0.f);
		m_vActionAt = D3DXVECTOR3((m_pHandMatrix->m[3][0] * 0.5f), m_pHandMatrix->m[3][1] * 0.5f, 0.f);

		//mViewInfo.vEye.x += (m_pHandMatrix->m[3][0] * 0.2f);
		//mViewInfo.vAt.x += (m_pHandMatrix->m[3][0] * 0.2f);
	}
	else if (m_pAnimState->find("Gail") != string::npos)
	{
		//m_vActionEye = D3DXVECTOR3((m_pHandMatrix->m[3][0] * 0.1f), 0.f, 0.f);
		m_vActionAt = D3DXVECTOR3(m_pHandMatrix->m[3][0] * 0.2f , m_pHandMatrix->m[3][1] * 0.1f, 0.f);

	/*	mViewInfo.vEye.x += (m_pHandMatrix->m[3][0] * 0.1f);
		mViewInfo.vAt.x += (m_pHandMatrix->m[3][0] * 0.1f);*/
		//mViewInfo.vEye.y += m_pHandMatrix->m[3][1] * 0.3f;
		//mViewInfo.vAt.y += m_pHandMatrix->m[3][1] * 0.3f;
	}
	else if(*m_pAnimState == "Damaged" || *m_pAnimState == "Stun" || *m_pAnimState == "DownA" || *m_pAnimState == "DownC")
	{
		m_vActionAt = D3DXVECTOR3((m_pHandMatrix->m[3][0] * 0.3f), (m_pHandMatrix->m[3][1] * 0.5f), 0.f);
	}
	else
	{
		m_vActionEye = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_vActionAt = D3DXVECTOR3(0.f, 0.f, 0.f);
	}

	mViewInfo.vAt += m_vActionAt;
}

void CameraFPS::GateMode(const D3DXVECTOR3 * pTargetDir)
{
	if (pTargetDir == nullptr)
		return;

	D3DXVECTOR3 vPlayerDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVec3Normalize(&vPlayerDir, pTargetDir);

	m_vCameraPos = vPlayerDir * -1 * (mPlayerDistance);
	m_vNaviPos = vPlayerDir * -1 * (NAVI_DISTANCE);
	m_vCameraPos += (*m_pPlayerPos);
	mCameraDistance = m_pUtilityMgr->Cal_TargetDistance(mViewInfo.vEye, m_vCameraPos);

	if (mCameraDistance >= 1.f)
	{
		m_vTargetDir = m_pUtilityMgr->CalTargetDir(mViewInfo.vEye, m_vCameraPos) * 1.f;

		if (m_pNavigation->is_Move(*m_pPlayerPos, m_vNaviPos, &m_vLIneNoraml))
		{
			mViewInfo.vEye += m_vTargetDir;
		}
		else
		{
			m_pNavigation->Setup_CurrentIndex(*m_pPlayerIndex);
		}

	}

	mViewInfo.vEye.y = m_pPlayerPos->y + mCameraY;
	mViewInfo.vAt = *m_pPlayerPos;
	mViewInfo.vAt.y += mCameraY;
}

void CameraFPS::BossActionMode()
{
	if (m_pBossPos == nullptr)
		return;

	float bossDistance = 0.f;
	D3DXVECTOR3 vTargetDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	bossDistance = m_pUtilityMgr->Cal_TargetDistance(mViewInfo.vEye, *m_pBossPos);

	vTargetDir = m_pUtilityMgr->CalTargetDir(mViewInfo.vEye, *m_pBossPos, true);

	if (bossDistance >= 33.f)
	{
		mViewInfo.vEye += vTargetDir * 55.f * gTimeDelta;
		mViewInfo.vEye.y = m_pBossPos->y + 15.f;
		mViewInfo.vAt = *m_pBossPos;
		mViewInfo.vAt.y += 12.f;
	}
	else
	{
		SoundMgr::GetInstance()->PlaySound(L"Hanuman_Breath_Voice.wav", CHANNEL_BOSS);
	}
}

void CameraFPS::LockOnTarget_Mode()
{
	D3DXVECTOR3 vPlayerDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vLook = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vRight = D3DXVECTOR3(0.f, 0.f, 0.f);

	D3DXVec3Normalize(&vRight, m_pPlayerRight);

	D3DXVec3Normalize(&vPlayerDir, m_pPlayerDir);
	vLook = vPlayerDir * 7.f + *m_pPlayerPos - (vRight * 0.2f);
	vLook.y += m_pHandMatrix->m[3][1];
	m_vCameraPos = vPlayerDir * 1.5f + *m_pPlayerPos - (vRight * 0.2f);

	mViewInfo.vEye = m_vCameraPos;
	mViewInfo.vEye.y += 1.f;
	mViewInfo.vAt = vLook;
	mViewInfo.vAt.y += 2.f;
}

inline void CameraFPS::Cal_CameraFPS()
{
	D3DXVECTOR3 vPlayerDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVec3Normalize(&vPlayerDir, m_pPlayerDir);

	m_vCameraPos = vPlayerDir * -1.f * (CAMERA_FPS_DISTANCE);
	m_vCameraPos += *m_pPlayerPos;
		
	mViewInfo.vEye = m_vCameraPos;
	mViewInfo.vEye.y = m_pPlayerPos->y + CAMERA_FPS_Y;
}


CameraFPS * CameraFPS::Create(LPDIRECT3DDEVICE9 pDevce, const D3DXVECTOR3 & vEye, const D3DXVECTOR3 & vAt, const D3DXVECTOR3 & vAxisY, const float fovY, const float aspect, const float projNear, const float projFar)
{
	CameraFPS* pInstance = new CameraFPS(pDevce);

	if (FAILED(pInstance->ReadyPrototype(vEye, vAt, vAxisY, fovY, aspect, projNear, projFar)))
	{
		_MSG_BOX("Camera_FPS Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * CameraFPS::Clone()
{
	CameraFPS*	pInstance = new CameraFPS(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("CameraFPS Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD CameraFPS::Free(void)
{

	Engine::Safe_Release(m_pUtilityMgr);
	if (m_bClone)
	{
		Engine::Safe_Release(m_pPlayerTransform);
		Engine::Safe_Release(m_pNavigation);
		Engine::Safe_Release(m_pPlayerAnim);
	}
	Camera::Free();

	return DWORD();
}



