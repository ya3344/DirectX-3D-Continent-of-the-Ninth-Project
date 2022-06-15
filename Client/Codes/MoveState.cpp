#include "stdafx.h"
#include "MoveState.h"
#include "ComponentMgr.h"
#include "CamearFPS.h"
#include "PlayerAnim.h"

_USING(Client)

MoveState::MoveState()
{
	m_vDir = m_pUtilityMgr->RandomDir(*m_pEnemyPos, 0);
}


void MoveState::AIState(const float timeDelta)
{
	mDistance = m_pUtilityMgr->Cal_TargetDistance(*m_pEnemyPos, *m_pPlayerPos);

	mt19937 mtRand((size_t)(rand()));
	uniform_int_distribution<size_t> range(1, 3);

	switch (mEnemyAI_Info.enemyType)
	{
	case BABEGAZI_BOW:
		mRange = DETECT_BOW_RANGE;
		mShortRange = DETECT_BOW_AWAY_RANGE;
		mSelectMode = AI_CHASE;
		break;
	case HUMAN_BOSS:
		mRange = DETECT_BOSS_RANGE;
		mShortRange = DETECT_BOSS_SHORT_RANGE;

		if (*mEnemyAI_Info.pBoss_HPCount > 1)
		{
			mSelectMode = AI_CHASE;
		}
		else
		{
			mSelectMode = AI_SPECIAL_CHASE;
		}
		break;
	default:
		mRange = DETECT_RANGE;
		mShortRange = DETECT_SHORT_RANGE;
		mSelectMode = AI_CHASE;
		break;
	}

	switch (mEnemyAI_Info.enemyType)
	{
	case BABEGAZI_BOW:
		if (mDistance > mRange + mRangeAssitant  && mAIMode != AI_DISCOVERY)
		{
			if (mMoveTime + mRandomTime < mAccumulationTime)
			{
				if (m_bRotation == false)
				{
					mRandomTime = (float)range(mtRand);

					mMoveTime = mAccumulationTime;
					m_vDir = m_pUtilityMgr->RandomDir(*m_pEnemyPos, timeDelta);
					m_bCross = m_pUtilityMgr->Cal_DirCross(*m_pEnemyDir, m_vDir);
				}
			}
			mRangeAssitant = 0;
			mAIMode = AI_WALK;
		}
		else if (mDistance <= mShortRange)
		{
			m_vDir = m_pUtilityMgr->CalTargetDir(*m_pEnemyPos, *m_pPlayerPos);
			m_vDir *= -1.f;
			m_bCross = m_pUtilityMgr->Cal_DirCross(*m_pEnemyDir, m_vDir);
			mAIMode = AI_CHASE;
		}
		else
		{
			m_vDir = m_pUtilityMgr->CalTargetDir(*m_pEnemyPos, *m_pPlayerPos);
			m_bCross = m_pUtilityMgr->Cal_DirCross(*m_pEnemyDir, m_vDir);
			if (mAIMode != AI_CHASE && mAIMode != AI_KNOLE_CHASE)
			{
				mAIMode = AI_DISCOVERY;
			}
			mRangeAssitant = mShortRange;
		}
		break;
	default:
		if (mDistance > mRange + mRangeAssitant && mAIMode != AI_DISCOVERY)
		{
			if (mMoveTime + mRandomTime < mAccumulationTime)
			{
				if (m_bRotation == false)
				{
					mRandomTime = (float)range(mtRand);

					mMoveTime = mAccumulationTime;
					m_vDir = m_pUtilityMgr->RandomDir(*m_pEnemyPos, timeDelta);
					m_bCross = m_pUtilityMgr->Cal_DirCross(*m_pEnemyDir, m_vDir);
				}
			}
			mRangeAssitant = 0;
			mAIMode = AI_WALK;
		}
		else if (mDistance <= mShortRange)
		{
			m_vDir = m_pUtilityMgr->CalTargetDir(*m_pEnemyPos, *m_pPlayerPos);
			m_bCross = m_pUtilityMgr->Cal_DirCross(*m_pEnemyDir, m_vDir);
			mAIMode = mSelectMode;
		}
		else
		{
			m_vDir = m_pUtilityMgr->CalTargetDir(*m_pEnemyPos, *m_pPlayerPos);
			m_bCross = m_pUtilityMgr->Cal_DirCross(*m_pEnemyDir, m_vDir);
			if (mAIMode != AI_CHASE && mAIMode != AI_KNOLE_CHASE)
			{
				mAIMode = AI_DISCOVERY;
			}
			mRangeAssitant = mShortRange;
		}
		break;
	}

	MoveDir(timeDelta);
}

void MoveState::AISet()
{
	if (mPre_AIMode == mAIMode)
		return;

	mt19937 mtRand((size_t)(rand()));
	uniform_int_distribution<size_t> walkRange(8, 11);
	uniform_int_distribution<size_t> ChaseRange(16, 21);

	float mChase_AnimSpeed = 0.f;
	float mDiscovery_AnimSpeed = 0.f;

	switch (mEnemyAI_Info.enemyType)
	{
	case KNOLE_COMMANDER:
	case KNOLE_WARRIOR:
		mRandomMove_Speed = walkRange(mtRand);
		mRandomChase_Speed = ChaseRange(mtRand);
		mChase_AnimSpeed = 1.5f;
		mDiscovery_AnimSpeed = 2.f;
		break;
	case BABEGAZI_BOW:
		mRandomMove_Speed = walkRange(mtRand);
		mRandomChase_Speed = 14;
		mChase_AnimSpeed = 1.5f;
		mDiscovery_AnimSpeed = 2.f;
		break;
	case HUMAN_BOSS:
		mRandomMove_Speed = walkRange(mtRand);
		mRandomChase_Speed = 32;
		mChase_AnimSpeed = 2.f;

		if (mAIMode == AI_DISCOVERY)
		{
			if (m_bBossInit == false)
			{
				mDiscovery_AnimSpeed = 0.5f;
				m_pCamerFPS->SetBossPos(m_pEnemyPos);
				m_pCamerFPS->SetBossDir(m_pEnemyDir);
				m_pCamerFPS->SetCameraMode(CAMERA_BOSS_ACTION);
				m_pPlayerAnim->SetBossDir(m_pEnemyDir);
				m_bBossInit = true;
			
			}
			else
			{
				m_pCamerFPS->SetCameraMode(CAMERA_BOSS);
				mDiscovery_AnimSpeed = 1.5f;
			}
		}
		else
		{
			if (m_bBossInit == true)
			{
				m_pCamerFPS->SetCameraMode(CAMERA_BOSS);
			}
		}
		break;
	default:
		mRandomMove_Speed = walkRange(mtRand);
		mRandomChase_Speed = ChaseRange(mtRand);
		mChase_AnimSpeed = 1.4f;
		mDiscovery_AnimSpeed = 1.5f;
		break;
	}
	

	switch (mAIMode)
	{
	case AI_IDLE:
		mAnimState = "BattleIdle";
		mAnimSpeed = 1.f;
		break;
	case AI_WALK:
		mMoveSpeed = mRandomMove_Speed;
		mAnimState = "Walk";
		mAnimSpeed = 1.2f;
		break;
	case AI_CHASE:
		mMoveSpeed = mRandomChase_Speed;
		mAnimState = "Run";
		mAnimSpeed = mChase_AnimSpeed;

		if (mEnemyAI_Info.enemyType == HUMAN_BOSS)
		{
			m_pSoundMgr->PlaySound(L"HanumanWalk2.wav", CHANNEL_BOSS);
		}
		break;
	case AI_KNOLE_CHASE:
		mMoveSpeed = mRandomChase_Speed;
		mAnimState = "Chase";
		mAnimSpeed = 1.8f;
		break;
	case AI_DISCOVERY:
		mAnimState = "Discovery";
		mAnimSpeed = mDiscovery_AnimSpeed;

		switch (mEnemyAI_Info.enemyType)
		{
		case BABEGAZI_AXE:
			m_pSoundMgr->PlaySound(L"Babe_Axe_Discovery.wav", CHANNEL_BABE_AXE);
			break;
		case BABEGAZI_BOW:
			m_pSoundMgr->PlaySound(L"Babe_Bow_Discovery.wav", CHANNEL_BABE_BOW);
			break;
		case BABEGAZI_WARRIOR:
			m_pSoundMgr->PlaySound(L"Babe_Warrior_Discovery.wav", CHANNEL_BABE_WARRIOR);
			break;
		case KNOLE_COMMANDER:
			m_pSoundMgr->PlaySound(L"Knole_Commander_Discovery.wav", CHANNEL_KNOLE_COMMANDER);
			break;
		case KNOLE_WARRIOR:
			m_pSoundMgr->PlaySound(L"Knole_Commander_Discovery.wav", CHANNEL_KNOLE_WARRIOR);
			break;
		case HUMAN_BOSS:
			m_pSoundMgr->PlaySound(L"Hanuman_Breath_Voice.wav", CHANNEL_BOSS);
			break;
		}
		break;
	case AI_SPECIAL_CHASE:
		mMoveSpeed = mRandomChase_Speed;
		mAnimState = "RoundPunchB";
		mAnimSpeed = 2.f;
		m_pSoundMgr->PlaySound(L"HanumanWalk2.wav", CHANNEL_BOSS);
		break;
	default:
		assert(false && "MoveState AISet!!");
		break;
	}

	mPre_AIMode = mAIMode;
}

void MoveState::AI_StateChange(const float timeDelta)
{
	if (m_pUtilityMgr->Cal_TargetDistance(*m_pEnemyPos, *m_pPlayerPos, true) > OPERATION_RANGE)
		return;
	

	if (mPreAnimState != mAnimState)
	{
		ChangeInit();
	}

	if (mAnimTime >= ((m_pAnimCtrl_Info->animPeriod) - mCorrectionTime))
	{
		if (mAIMode == AI_DISCOVERY)
		{
			switch (mEnemyAI_Info.enemyType)
			{
			case KNOLE_COMMANDER:
			case KNOLE_WARRIOR:
				mAIMode = AI_KNOLE_CHASE;
				break;
			default:
				mAIMode = AI_CHASE;
				break;
			}

		}
		else if(mAIMode == AI_KNOLE_CHASE)
		{
			mAIMode = AI_CHASE;
		}

		if (*mEnemyAI_Info.pAttack == true)
		{
			*mEnemyAI_Info.pAttack = false;
		}

		mAnimTime = 0.0;
	}

	if (FAILED(m_pMesh->PlayAnimation(mAnimSpeed * timeDelta)))
	{
		_MSG_BOX("MoveState PlayAnimation FAILED!");
	}
}

void MoveState::AIUpdate(const float timeDelta)
{
	if (m_pUtilityMgr->Cal_TargetDistance(*m_pEnemyPos, *m_pPlayerPos, true) > OPERATION_RANGE)
		return;


	if(Engine::KeyMgr::GetInstance()->KeyPressing(DIK_P))
	{
		SetDamged(10.f);
		//m_pUIMgr->NumberingUI_Create(*m_pTransform->Get_WorldMatrix(), 50000);
	}
	

	mAccumulationTime += timeDelta;
	mAnimTime += mAnimSpeed *timeDelta;

	if (m_bCross)
	{
		mRotSpeed = 100.f * timeDelta;
	}
	else
	{
		mRotSpeed = -100.f * timeDelta;
	}
	
	switch (mAIMode)
	{
	case AI_IDLE:
		break;
	case AI_WALK:
		CalWalk(timeDelta);
		break;
	case AI_CHASE:
	case AI_SPECIAL_CHASE:
		CalChase(timeDelta);
		break;
	case AI_KNOLE_CHASE:
		if (mAnimTime >= 0.3 && mAnimTime <= 1.5)
		{
			CalChase(timeDelta);
		}
		break;
	default:
		break;
	}
}

void MoveState::Collision(const float timeDelta)
{
	if (m_pEnemyMgr->Enemy_CollisionEX(m_pCollider, &m_vPush))
	{
		mPushDistance = mMoveSpeed * 0.5f;
		m_pTransform->DirStep(m_vPush * 2.5f * timeDelta);
		//cout << mPushDistance << endl;
	}
	else
	{
		mPushDistance = 0.f;
	}

}

void MoveState::CalWalk(const float timeDelta)
{
	D3DXVec3Normalize(m_pEnemyDir, m_pEnemyDir);
	D3DXVECTOR3 vDir = *m_pEnemyDir * (float)mMoveSpeed * timeDelta;
	D3DXVECTOR3 vCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	if (m_pNavigation->is_Move(*m_pEnemyPos, vDir, &m_vLineNormal)
		&& *m_pEnemyIndex > *mEnemyAI_Info.pNaviMinIndex
		&& *m_pEnemyIndex < *mEnemyAI_Info.pNaviMaxIndex)
	{
		m_pTransform->GoStraight(mMoveSpeed * timeDelta);
	}
	else
	{
		//vCenterPos = m_pNavigation->GetCenterPos();
		//m_vDir = m_pUtilityMgr->CalTargetDir(*m_pEnemyPos, vCenterPos);

		m_vDir = *m_pEnemyDir * -1.f;
		m_bCross = m_pUtilityMgr->Cal_DirCross(*m_pEnemyDir, m_vDir);

		//m_vDir = m_pUtilityMgr->RandomDir(*m_pEnemyPos, timeDelta);
		//m_bCross = m_pUtilityMgr->Cal_DirCross(*m_pEnemyDir, m_vDir);
		mRandomTime += 0.15f;
		m_bRotation = true;
	}
}

void MoveState::CalChase(const float timeDelta)
{
	D3DXVec3Normalize(m_pEnemyDir, m_pEnemyDir);
	D3DXVECTOR3 vDir = *m_pEnemyDir * (mMoveSpeed + mPushDistance) * timeDelta;

	if (m_pNavigation->is_Move(*m_pEnemyPos, vDir, &m_vLineNormal))
	{
		m_pTransform->GoStraight((mMoveSpeed + mPushDistance) * timeDelta);
		Collision(timeDelta);
	}
	else
	{
		m_pUtilityMgr->Cal_SlidingVector(*m_pEnemyDir, m_vLineNormal, &m_vSlide, &mSlideSpeed);
		vDir = m_vSlide * (float)mMoveSpeed * timeDelta;

		if (m_pNavigation->is_Move(*m_pEnemyPos, vDir, &m_vLineNormal) == true)
		{
			*m_pEnemyDir = m_vSlide;
			m_pTransform->GoStraight(mMoveSpeed * timeDelta);
		}
	}

	if (mEnemyAI_Info.enemyType == HUMAN_BOSS)
	{
		mBossTime += timeDelta * 1.5f;
		if (mBossTime >= 1.f)
		{
			m_pSoundMgr->PlaySound(L"HanumanWalk2.wav", CHANNEL_BOSS);
			mBossTime = 0.f;
		}
		
	}

	m_pEnemyMgr->Enemy_AttackCount();
}

void MoveState::MoveDir(const float timeDelta)
{
	mTargetDegree = m_pUtilityMgr->CalDirDegree(m_vDir, *m_pEnemyDir);
	
	if (mTargetDegree >= 5.f)
	{
		m_pTransform->SetRotation(D3DXVECTOR3(0.f, mRotSpeed, 0.f));
		m_bRotation = true;
	}
	else
	{
		m_bRotation = false;
	}
}

void MoveState::Cal_SlidngVector()
{
	m_vLineNormal.y = m_pEnemyDir->y;
	D3DXVec3Normalize(&m_vLineNormal, &m_vLineNormal);

	m_vSlide = m_vLineNormal * D3DXVec3Dot(m_pEnemyDir, &m_vLineNormal);
	m_vSlide = *m_pEnemyDir - m_vSlide;

	//mSlideSpeed = D3DXVec3Length(&m_vSlide);
	*m_pEnemyDir = m_vSlide;
}

MoveState * MoveState::Create()
{
	MoveState* pInstance = new MoveState();
	return pInstance;
}

DWORD MoveState::Free()
{
	EnemyState::Free();
	return 0;
}
