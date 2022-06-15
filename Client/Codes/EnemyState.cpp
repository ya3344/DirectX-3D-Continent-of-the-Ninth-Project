#include "stdafx.h"
#include "EnemyState.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "PlayerAnim.h"
#include "Player.h"
#include "Trail.h"
#include "CamearFPS.h"
#include "LightManager.h"

_USING(Client)

EnemyState::EnemyState()
{
	m_pUtilityMgr = UtilityMgr::GetInstance();
	m_pUtilityMgr->AddRef();

	m_pEnemyMgr = EnemyMgr::GetInstance();
	//m_pEnemyMgr->AddRef();

	m_pBulletMgr = BulletMgr::GetInstance();
	//m_pBulletMgr->AddRef();

	m_pLightMgr = Engine::LightManager::GetInstance();

	m_pUIMgr = UIMgr::GetInstance();

	m_pEffectMgr = EffectMgr::GetInstance();

	m_pSoundMgr = SoundMgr::GetInstance();
}


HRESULT EnemyState::SetInit(const ENEMY_AI_INFO enemy_AIInfo, Engine::DynamicMesh * pMesh, Engine::Transform * pTransform, Engine::Transform * pPlayerTransform, Engine::Navigation * pNavigation)
{
	{
		m_pMesh = pMesh;
		if (m_pMesh == nullptr)
		{
			_MSG_BOX("EnemyAnim Mesh Setting Failed!");
			return E_FAIL;
		}
		m_pMesh->AddRef();
	}

	{
		m_pTransform = pTransform;
		if (m_pTransform == nullptr)
		{
			_MSG_BOX("EnemyAnim Transform Setting Failed!");
			return E_FAIL;
		}
		m_pTransform->AddRef();

		m_pEnemyDir = m_pTransform->GetInfo(Engine::Transform::INFO_LOOK);
		m_pEnemyRightDir = m_pTransform->GetInfo(Engine::Transform::INFO_RIGHT);
		m_pEnemyPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);
	}


	{
		m_pPlayerTransform = pPlayerTransform;
		if (m_pPlayerTransform == nullptr)
		{
			_MSG_BOX("EnemyAnim PlayerTransform Setting Failed!");
			return E_FAIL;
		}
		m_pPlayerTransform->AddRef();

		m_pPlayerDir = m_pPlayerTransform->GetInfo(Engine::Transform::INFO_LOOK);
		m_pPlayerPos = m_pPlayerTransform->GetInfo(Engine::Transform::INFO_POSITION);
	}

	{
		m_pNavigation = pNavigation;
		if (m_pNavigation == nullptr)
		{
			_MSG_BOX("EnemyrAnim Navigation Setting Failed!");
			return E_FAIL;
		}
		m_pNavigation->AddRef();
		m_pEnemyIndex = m_pNavigation->Get_CurrentIndex();

		m_pPlaneY = m_pNavigation->GetPlaneY();
		*m_pPlaneY = *m_pEnemyPos;
		m_pTransform->SetPlaneY(m_pPlaneY); // 주소전달
	}

	{
		mEnemyAI_Info = enemy_AIInfo;
		m_pAnimCtrl_Info = m_pMesh->Set_AnimaByName("BattleIdle");
		mAnimState = "BattleIdle";
		Set_HPInit();
	}

	{
		//Component_Player_Anim
		m_pPlayerAnim = (PlayerAnim*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Anim_Player", L"Prototype_GameObject_Player");
		m_pPlayerAnim->AddRef();

		m_pPlayerAnimState = m_pPlayerAnim->GetAnimState();

		m_pPlayer = (Player *)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_PLAYER, L"Layer_Player", L"Prototype_GameObject_Player");
		m_pPlayerInfo = m_pPlayer->Get_PlayerInfo();

		if (m_pPlayerInfo == nullptr)
			return E_FAIL;

		
	}

	{
		if (enemy_AIInfo.enemyType == HUMAN_BOSS)
		{
			m_pLeftTrail = (Trail*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_TRAIL, L"Layer_Trail", L"Prototype_GameObject_Boss_LeftTrail");
			m_pRightTrail = (Trail*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_TRAIL, L"Layer_Trail", L"Prototype_GameObject_Boss_RightTrail");
			m_pDecal = (DecalBox*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_EFFECT, L"Layer_Decal", L"Prototype_GameObject_BossDecal");
			m_pCamerFPS = (CameraFPS*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_STAGE, L"Layer_Camera", L"Prototype_GameObject_CameraFPS");
			m_pCamerFPS->SetBossPos(m_pEnemyPos);
			
		}
	}



	return NOERROR;
}

HRESULT EnemyState::SetCollider(Engine::Collider * pCollider, Engine::Collider * pPlayerCollider, Engine::Collider* pWeaponCollider)
{
	m_pCollider = pCollider;
	if (m_pCollider == nullptr)
	{
		_MSG_BOX("EnemyState m_pCollider Failed!");
		return E_FAIL;
	}
	m_pCollider->AddRef();

	m_pPlayerCollider = pPlayerCollider;
	if (m_pPlayerCollider == nullptr)
	{
		_MSG_BOX("EnemyState m_pPlayerCollider Failed!");
		return E_FAIL;
	}
	m_pPlayerCollider->AddRef();

	m_pWeaponCollider = pWeaponCollider;
	if (m_pWeaponCollider == nullptr)
	{
		_MSG_BOX("EnemyState m_pWeaponCollider Failed!");
		return E_FAIL;
	}
	m_pWeaponCollider->AddRef();

	return NOERROR;
}

void EnemyState::PreAISet(const int AIMode)
{
	mAnimState = "";
	mPreAnimState = "";
	mPre_AIMode = -1;
	m_pAnimCtrl_Info->animPeriod = 0.0;
	mCorrectionTime = 0.0;
	mAnimTime = 0.0;

	if (AIMode != -1)
	{
		mAIMode = AIMode;
	}
}



void EnemyState::ChangeInit()
{
	m_pAnimCtrl_Info = m_pMesh->Set_AnimaByName(mAnimState.c_str());
	mPreAnimState = mAnimState;
	mAnimTime = 0.0;

	switch (mAIMode)
	{
	case AI_DAMAGE_SWO:
		mCorrectionTime = (mAnimSpeed * m_pAnimCtrl_Info->animPeriod * 0.02);
		*m_pAnimCtrl_Info->pDuration = 0.01;
		break;
	default:
		mCorrectionTime = (mAnimSpeed * m_pAnimCtrl_Info->animPeriod * 0.02);
		*m_pAnimCtrl_Info->pDuration = 0.25;
		break;
	}

}

void EnemyState::CalDir(const float timeDelta)
{
	m_vDir = m_pUtilityMgr->CalTargetDir(*m_pEnemyPos, *m_pPlayerPos);

	if (m_vDir != m_vPreDir)
	{
		m_bCross = m_pUtilityMgr->Cal_DirCross(*m_pEnemyDir, m_vDir);
		m_vPreDir = m_vDir;
	}

	if (m_bCross)
	{
		mRotSpeed = 140.f * timeDelta;
	}
	else
	{
		mRotSpeed = -140.f * timeDelta;
	}

	mTargetDegree = m_pUtilityMgr->CalDirDegree(m_vDir, *m_pEnemyDir);

	if (mTargetDegree >= 3.f)
	{
		m_pTransform->SetRotation(D3DXVECTOR3(0.f, mRotSpeed, 0.f));
	}
	else
	{
		if (mAIMode == AI_BOW_ATTACK_READY)
		{
			mAIMode = AI_BOW_ATTACK;
			mAnimTime = 0.0;
		}
	}
}

void EnemyState::Gravity(const float timeDelta)
{
	// 중력 적용.
	if (m_pEnemyPos->y > m_pPlaneY->y)
	{
		m_pTransform->Gravity(timeDelta * 20.f);
	}
}

void EnemyState::Jump(const float timeDelta, const size_t jumpState, const float jumpVelocity, const float sinDegree, const float cosDegree)
{
	mJumpTime += timeDelta;

	m_pTransform->CalJump(&m_vJumpDir, jumpState, jumpVelocity, mJumpTime, sinDegree, cosDegree);

	if (m_pNavigation->is_Move(*m_pEnemyPos, m_vJumpDir, &m_vLineNormal))
	{
		m_pTransform->DirStep(m_vJumpDir);
	}
	else
	{
		Gravity(timeDelta);
	}
}

void EnemyState::GoStraight(const float timeDelta, const float speed)
{
	D3DXVec3Normalize(m_pEnemyDir, m_pEnemyDir);
	D3DXVECTOR3 vDir = *m_pEnemyDir * speed * timeDelta;

	if (m_pNavigation->is_Move(*m_pEnemyPos, vDir, &m_vLineNormal))
	{
		m_pTransform->GoStraight(speed * timeDelta);
	}
	else
	{

	}
}

void EnemyState::SetDamged(const float damaged)
{
	if (*mEnemyAI_Info.pHP <= 0.f)
		return;

	mDamagedValue = damaged + (m_pPlayerInfo->level * 2.f);

	//cout << mDamagedValue << endl;

	if (*mEnemyAI_Info.pHP >= 0.f)
	{
		*mEnemyAI_Info.pHP -= mDamagedValue;
		m_pUIMgr->NumberingUI_Create(*m_pTransform->Get_WorldMatrix(), mDamagedValue * 1000.f);
		m_pUIMgr->AddComboCout();
		DamagedEffect();

		switch (mEnemyAI_Info.enemyType)
		{
		case BABEGAZI_AXE:
			m_pSoundMgr->PlaySound(L"Babe_Axe_Dam_1.wav", CHANNEL_BABE_AXE);
			break;
		case BABEGAZI_BOW:
			m_pSoundMgr->PlaySound(L"Babe_Bow_Dam_1.wav", CHANNEL_BABE_AXE);
			break;
		case BABEGAZI_WARRIOR:
			m_pSoundMgr->PlaySound(L"Babe_Warrior_Dam_1.wav", CHANNEL_BABE_AXE);
			break;
		case KNOLE_COMMANDER:
			m_pSoundMgr->PlaySound(L"Knole_Commander_Dam_1.wav", CHANNEL_KNOLE_COMMANDER);
			break;
		case KNOLE_WARRIOR:
			m_pSoundMgr->PlaySound(L"Knole_Commander_Dam_2.wav", CHANNEL_KNOLE_WARRIOR);
			break;
		case HUMAN_BOSS:
			m_pSoundMgr->PlaySound(L"Hanuman_Breath_Voice.wav", CHANNEL_BOSS);
			break;
		}
	}

	if (*mEnemyAI_Info.pHP <= 0.f)
	{
		*mEnemyAI_Info.pHP = 0.f;
		switch (mEnemyAI_Info.enemyType)
		{
		case BABEGAZI_AXE:
			m_pPlayer->SetEXP(40);
			m_pSoundMgr->PlaySound(L"Babe_Axe_Death.wav", CHANNEL_BABE_AXE);
			break;
		case BABEGAZI_BOW:
			m_pPlayer->SetEXP(40);
			m_pSoundMgr->PlaySound(L"Babe_Bow_Death.wav", CHANNEL_BABE_AXE);
			break;
		case BABEGAZI_WARRIOR:
			m_pPlayer->SetEXP(40);
			m_pSoundMgr->PlaySound(L"Babe_Warrior_Death.wav", CHANNEL_BABE_AXE);
			break;
		case KNOLE_COMMANDER:
			m_pPlayer->SetEXP(80);
			m_pSoundMgr->PlaySound(L"Knole_Commander_Death.wav", CHANNEL_KNOLE_COMMANDER);
			break;
		case KNOLE_WARRIOR:
			m_pPlayer->SetEXP(80);
			m_pSoundMgr->PlaySound(L"Knole_Commander_Dam_3.wav", CHANNEL_KNOLE_WARRIOR);
			break;
		case HUMAN_BOSS:
			m_pPlayer->SetEXP(300);

			if (*mEnemyAI_Info.pBoss_HPCount > 1)
			{
				*mEnemyAI_Info.pHP = BOSS_ORIGINAL_HP;
				--*mEnemyAI_Info.pBoss_HPCount;

				m_pLightMgr->ChangeLightColor(LIGHT_BOSS_STATE, *mEnemyAI_Info.pBoss_HPCount);
				m_pSoundMgr->PlaySound(L"M_Diadeath.wav", CHANNEL_BOSS);
			}
			else
			{
				m_pSoundMgr->PlaySound(L"M_Diadeath.wav", CHANNEL_BOSS);
			}
			break;
		}
		//cout << "사망빠염" << endl;
	}
}

void EnemyState::DamagedEffect()
{
	int effectRand = 0;

	mt19937 mtRand((size_t)(rand()));
	uniform_real_distribution<float> XRange(m_pCollider->Get_MeshInfo().vMin.x, m_pCollider->Get_MeshInfo().vMax.x);
	uniform_real_distribution<float> YRange(m_pCollider->Get_MeshInfo().vMin.y, m_pCollider->Get_MeshInfo().vMax.y);
	uniform_real_distribution<float> ZRange(m_pCollider->Get_MeshInfo().vMin.z, m_pCollider->Get_MeshInfo().vMax.z);
	
	uniform_int_distribution<int> effectRange(1, 3);
	mEffectRand = effectRange(mtRand);

	m_vEffect_DamgedPos.x = XRange(mtRand);
	m_vEffect_DamgedPos.y = YRange(mtRand);
	m_vEffect_DamgedPos.z = ZRange(mtRand);

	D3DXVec3TransformCoord(&m_vEffect_DamgedPos, &m_vEffect_DamgedPos, m_pTransform->Get_WorldMatrix());

	switch (mEnemyAI_Info.enemyType)
	{
	case BABEGAZI_AXE:
	case BABEGAZI_BOW:
	case BABEGAZI_WARRIOR:
		m_vEffectScale = D3DXVECTOR3(5.f, 5.f, 5.f);
		break;
	case KNOLE_COMMANDER:
	case KNOLE_WARRIOR:
		m_vEffectScale = D3DXVECTOR3(8.f, 8.f, 8.f);
		break;
	case HUMAN_BOSS:
		m_vEffectScale = D3DXVECTOR3(13.f, 13.f, 13.f);
		break;
	default:
		break;
	}

	switch (mEffectRand)
	{
	case EFFECT_RED:
		m_pEffectMgr->CreateEffect(L"Prototype_Effect_Blood", EFFECT_INFO(&m_vEffect_DamgedPos, m_vEffectScale, EFFECT_BLOOD));
		break;
	case EFFECT_BLUE:
		m_pEffectMgr->CreateEffect(L"Prototype_Effect_BlueBlood", EFFECT_INFO(&m_vEffect_DamgedPos, m_vEffectScale, EFFECT_BLUE_BLOOD));
		break;
	case EFFECT_PURPLE:
		m_pEffectMgr->CreateEffect(L"Prototype_Effect_PurpleBlood", EFFECT_INFO(&m_vEffect_DamgedPos, m_vEffectScale, EFFECT_PURPLE_BLOOD));
		break;
	default:
		break;
	}
	
}

void EnemyState::CalDiagonalDir(const D3DXVECTOR3& vRightDir)
{
	D3DXVECTOR3 vEnemyDir = *m_pEnemyDir;
	D3DXVec3Normalize(&vEnemyDir, &vEnemyDir);
	vEnemyDir *= -1.f;
	vEnemyDir *= 0.5f;
	m_vEnemy_DiagonalDir = vEnemyDir + vRightDir;
	//m_vEnemy_DiagonalDir *= -1.f;
	D3DXVec3Normalize(&m_vEnemy_DiagonalDir, &m_vEnemy_DiagonalDir);
}

void EnemyState::Set_HPInit()
{
	switch (mEnemyAI_Info.enemyType)
	{
	case BABEGAZI_WARRIOR:
		*mEnemyAI_Info.pHP = 75.f;
		break;
	case BABEGAZI_AXE:
		*mEnemyAI_Info.pHP = 75.f;
		break;
	case BABEGAZI_BOW:
		*mEnemyAI_Info.pHP = 30.f;
		break;
	case KNOLE_COMMANDER:
		*mEnemyAI_Info.pHP = 105.f;
		break;
	case KNOLE_WARRIOR:
		*mEnemyAI_Info.pHP = 105.f;
		break;
	case HUMAN_BOSS:
		*mEnemyAI_Info.pHP = BOSS_ORIGINAL_HP;
		break;
	default:
		assert(false && "EnemyState SetHP!!");
		break;
	}
}

DWORD EnemyState::Free()
{
	Engine::Safe_Release(m_pUtilityMgr);
	//Engine::Safe_Release(m_pEnemyMgr);
	//Engine::Safe_Release(m_pBulletMgr);

	Engine::Safe_Release(m_pMesh);
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pPlayerTransform);
	Engine::Safe_Release(m_pNavigation);
	Engine::Safe_Release(m_pPlayerAnim);

	Engine::Safe_Release(m_pCollider);
	Engine::Safe_Release(m_pPlayerCollider);
	Engine::Safe_Release(m_pWeaponCollider);

	return 0;
}
