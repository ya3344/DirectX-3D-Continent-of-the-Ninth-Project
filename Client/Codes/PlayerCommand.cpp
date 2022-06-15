#include "stdafx.h"
#include "PlayerCommand.h"
#include "ComponentMgr.h"
#include "DecalBox.h"
#include "ObjectMgr.h"

_USING(Client)


PlayerCommand::PlayerCommand(LPDIRECT3DDEVICE9 pDevice)
	: Component(pDevice)
	, m_pKeyMgr(Engine::KeyMgr::GetInstance())
	,m_pUtilityMgr(UtilityMgr::GetInstance())
	,m_pEnemyMgr(EnemyMgr::GetInstance())
	,m_pBulletMgr(BulletMgr::GetInstance())
	, m_pSoundMgr(SoundMgr::GetInstance())
{
	m_pKeyMgr->AddRef();
	m_pUtilityMgr->AddRef();
	//m_pEnemyMgr->AddRef();
	//m_pBulletMgr->AddRef();
}

PlayerCommand::PlayerCommand(const PlayerCommand & rhs)
	: Component(rhs)
	,m_pKeyMgr(rhs.m_pKeyMgr)
	, m_pUtilityMgr(rhs.m_pUtilityMgr)
	, m_pEnemyMgr(rhs.m_pEnemyMgr)
	, m_pBulletMgr(rhs.m_pBulletMgr)
	, m_pSoundMgr(rhs.m_pSoundMgr)
{
	m_pKeyMgr->AddRef();
	m_pUtilityMgr->AddRef();
	//m_pEnemyMgr->AddRef();
	//m_pBulletMgr->AddRef();
}

HRESULT PlayerCommand::SetInit(Engine::DynamicMesh * pMesh, Engine::Transform * pTransform, Engine::Navigation* pNavigation, Player* pPlayer, Trail* pTrail)
{
	{
		m_pMesh = pMesh;
		if (m_pMesh == nullptr)
		{
			_MSG_BOX("PlayerCommand m_pMesh Setting Failed!");
			return E_FAIL;
		}
		m_pMesh->AddRef();
	}

	{
		m_pTransform = pTransform;
		if (m_pTransform == nullptr)
		{
			_MSG_BOX("PlayerCommand Transform Setting Failed!");
			return E_FAIL;
		}
		m_pTransform->AddRef();

		m_pDir = m_pTransform->GetInfo(Engine::Transform::INFO_LOOK);
		m_pRightDir = m_pTransform->GetInfo(Engine::Transform::INFO_RIGHT);
		m_pPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);
	}

	{
		m_pNavigation = pNavigation;
		if (m_pNavigation == nullptr)
		{
			_MSG_BOX("PlayerCommand Navigation Setting Failed!");
			return E_FAIL;
		}
		m_pNavigation->AddRef();

		m_pPlaneY = m_pNavigation->GetPlaneY();
		*m_pPlaneY = *m_pPos;
		m_pTransform->SetPlaneY(m_pPlaneY); // 주소전달
	}

	{
		mAnimState = "BattleIdle";
		m_pAnimCtrl_Info = m_pMesh->Set_AnimaByName(mAnimState.c_str());
		m_pPlayer = pPlayer;
	}

	{
		m_pTrail = pTrail;
		m_pDecal = (DecalBox*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_EFFECT, L"Layer_Decal", L"Prototype_GameObject_PlayerDecal");
		m_pEffectMgr = EffectMgr::GetInstance();
	}


	//mForwardSpeed = 20.f;
	//mBackwardSpeed = -17.f;
	return NOERROR;
}

HRESULT PlayerCommand::Set_WeaponCollider(Engine::Collider* pWeaponCollider, Engine::Collider* pWeapon_SphereCollider)
{
	{
		m_pWeaponCollider = pWeaponCollider;
		if (m_pWeaponCollider == nullptr)
		{
			_MSG_BOX("PlayerCommand m_pWeaponCollider Setting Failed!");
			return E_FAIL;
		}
		m_pWeaponCollider->AddRef();
	}

	{
		m_pWeapon_SphereCollider = pWeapon_SphereCollider;
		if (m_pWeapon_SphereCollider == nullptr)
		{
			_MSG_BOX("PlayerCommand m_pWeapon_SphereCollider Setting Failed!");
			return E_FAIL;
		}
		m_pWeapon_SphereCollider->AddRef();
	}

	return NOERROR;
}
void PlayerCommand::LinkAnimation(int attackCount)
{
	const char* pFind = nullptr;
	string::size_type pos = 0;

	mt19937 mtRand((size_t)(rand()));
	uniform_int_distribution<size_t> range(1, 10);
	mLinkAnim_Percent = range(mtRand);

	switch (attackCount)
	{
	case 0:
		pFind = "Ready";
		break;
	case 1:
		pFind = "Shot";
	default:
		break;
	}

	if (attackCount == 1)
	{
		if (mAnimState == "LockOnTargetShot")
			return;
	}

	if (mAnimState == "GailShot")
	{
		++mGailCount;
		if (mGailCount <= 1)
		{
			//cout << "GailCount" << endl;
			mAnimTime = 0.0;
			return;
		}
		
	}

	switch (attackCount)
	{
	case 0:
		if (mAnimState.find("LockOnTarget") != string::npos)
		{
			m_bTargetOnShot = false;
			mAnimInfo = ANIM_INFO(1, 1.f);
		}
		else if (mAnimState.find("ShootJump") != string::npos)
		{
			mAnimInfo = ANIM_INFO(1, 1.f);
		}
		else if (mAnimState.find("Gail") != string::npos)
		{
			mAnimInfo = ANIM_INFO(1, 1.4f);
			mGailCount = 0;
		}
		else if (mAnimState.find("Fire") != string::npos)
		{
			mAnimInfo = ANIM_INFO(1, 1.f);
		}

		m_pUtilityMgr->StringReplace(mAnimState, pFind, "Shot");

		break;
	case 1:
		if (mAnimState.find("ShootJump") != string::npos)
		{
			mAnimInfo = ANIM_INFO(-1, 1.f);
		}
		else if (mAnimState.find("Gail") != string::npos)
		{
			mAnimInfo = ANIM_INFO(-1, 1.6f);
		}
		else if (mAnimState.find("Fire") != string::npos)
		{
			mAnimInfo = ANIM_INFO(-1, 1.f);
		}

		m_pUtilityMgr->StringReplace(mAnimState, pFind, "End");
		break;
	case 2:
		if (m_pPos->y <= m_pPlaneY->y)
		{
			mAnimState = "DownC";
			mAnimInfo = ANIM_INFO(3, 1.f);
		}
		m_bDamaged = true;
		break;
	case 3:
		mAnimState = "RisingAttack";
		mAnimInfo = ANIM_INFO(-1, 1.2f);

		/*if (mLinkAnim_Percent <= 5)
		{
			mAnimState = "QuickStandB";
			mAnimInfo = ANIM_INFO(-1, 1.5f);
		}
		else
		{
			mAnimState = "RisingAttack";
			mAnimInfo = ANIM_INFO(-1, 1.5f);
		}*/
		m_bDamaged = true;
		break;
	default:
		break;
	}
}

void PlayerCommand::Cal_SlidngVector()
{
	m_vLineNormal.y = m_pDir->y;
	D3DXVec3Normalize(&m_vLineNormal, &m_vLineNormal);

	m_vSlide = m_vLineNormal * D3DXVec3Dot(m_pDir, &m_vLineNormal);
	m_vSlide = *m_pDir - m_vSlide;
	mSlideSpeed = D3DXVec3Length(&m_vSlide);

	D3DXVec3Normalize(&m_vSlide, &m_vSlide);
}

void PlayerCommand::CalMove(const float dirSpeed, const float animSpeed, const float timeDelta, const bool bRightStep)
{
	if (bRightStep)
	{
		m_vDir = *m_pRightDir;
	}
	else
	{
		m_vDir = *m_pDir;
	}

	m_vDir = *D3DXVec3Normalize(&m_vDir, &m_vDir) * dirSpeed * timeDelta;

	if (m_pNavigation->is_Move(*m_pPos, m_vDir, &m_vLineNormal))
	{
		
		if (bRightStep == false)
		{
			m_pTransform->GoStraight(dirSpeed * timeDelta);
		}
		else
		{
			m_pTransform->RightStep(dirSpeed * timeDelta);
		}
	}
	else
	{

		m_pUtilityMgr->Cal_SlidingVector(*m_pDir, m_vLineNormal, &m_vSlide, &mSlideSpeed);

		m_vDir = m_vSlide * dirSpeed * mSlideSpeed * timeDelta;

		if (m_pNavigation->is_Move(*m_pPos, m_vDir, &m_vLineNormal) == true)
		{
			*m_pDir = m_vSlide;
			if (bRightStep == false)
			{
				m_pTransform->GoStraight(dirSpeed * mSlideSpeed * timeDelta);
			}
			else
			{
				m_pTransform->RightStep(dirSpeed *mSlideSpeed * timeDelta);
			}
		}
	}


}

void PlayerCommand::Gravity(const float timeDelta)
{
	// 중력 적용.
	if (m_pPos->y > m_pPlaneY->y)
	{
		m_pTransform->Gravity(timeDelta * 10.f);
	}
}

void PlayerCommand::Jump(const float timeDelta, const size_t jumpState)
{
	mJumpTime += timeDelta * mJumpSpeed;
	
	m_pTransform->CalJump(&m_vJumpDir, jumpState, mJumpVelocity, mJumpTime, 90.f, 60.f);

	if (m_pNavigation->is_Move(*m_pPos, m_vJumpDir, &m_vLineNormal)
		|| m_pNavigation->Get_CellState() == CELL_JUMP)
	{
		m_pTransform->DirStep(m_vJumpDir);
		if (m_bJump)
		{
			if (m_pNavigation->is_Move(*m_pPos, m_vJumpDir, &m_vLineNormal) == false)
			{
				m_pNavigation->Cal_JumpIndex();
				m_bJump = false;
			}
		}
	}
	else
	{
		Gravity(timeDelta);
	}
}

void PlayerCommand::DamagedJump(const float timeDelta, const size_t jumpState, const float jumpVelocity, const float sinDegree, const float cosDegree, const D3DXVECTOR3* pEnemyDir)
{
	mJumpTime += timeDelta;

	m_pTransform->CalJump(&m_vJumpDir, jumpState, jumpVelocity, mJumpTime, sinDegree, cosDegree, pEnemyDir);

	if (m_pNavigation->is_Move(*m_pPos, m_vJumpDir, &m_vLineNormal))
	{
		m_pTransform->DirStep(m_vJumpDir);
	}
	else
	{
		Gravity(timeDelta);
	}

	//if (m_pBossPos != nullptr)
	//{
	//	D3DXVECTOR3 vDir = m_pUtilityMgr->CalTargetDir(*m_pBossPos, *m_pPos);
	//	float dirDegree = m_pUtilityMgr->CalDirDegree(vDir, *m_pDir);
	//	m_pTransform->Set_FixRotation(D3DXVECTOR3(0.f, dirDegree, 0.f));
	//}
}

bool PlayerCommand::LockOnTarget_Operation(const float timeDelta)
{
	if (mAnimState != "LockOnTargetShot")
		return false;

	if (m_pKeyMgr->KeyDown(DIK_NUMPAD1))
	{
		//m_bLockOnTarget_End = !m_bLockOnTarget_End;
		mAnimInfo = ANIM_INFO(-1, 1.1f);
		mAnimState = "LockOnTargetEnd";
		g_bMainBase = false;
	}
	if (m_pKeyMgr->KeyDown(DIK_NUMPAD2))
	{
		if (mAnimTime >= (m_pAnimCtrl_Info->animPeriod))
		{
			m_bTargetOnShot = true;
			m_bTargetOnShot_Bulelt = true;
			mAnimTime = 0.0;
		}
	}

	if (m_pKeyMgr->KeyPressing(DIK_LEFT))
	{
		m_pTransform->SetRotation(D3DXVECTOR3(0.f, -30.f * timeDelta, 0.f));
	}

	if (m_pKeyMgr->KeyPressing(DIK_RIGHT))
	{
		m_pTransform->SetRotation(D3DXVECTOR3(0.f, 30.f * timeDelta, 0.f));
	}

	return true;

}

DWORD PlayerCommand::Free()
{
	Engine::Safe_Release(m_pKeyMgr);
	Engine::Safe_Release(m_pUtilityMgr);
	//Engine::Safe_Release(m_pEnemyMgr);
	//Engine::Safe_Release(m_pBulletMgr);

	if (m_bClone)
	{
		Engine::Safe_Release(m_pMesh);
		Engine::Safe_Release(m_pTransform);
		Engine::Safe_Release(m_pNavigation);
		Engine::Safe_Release(m_pWeaponCollider);
		Engine::Safe_Release(m_pWeapon_SphereCollider);
	}

	Component::Free();
	return 0;
}
