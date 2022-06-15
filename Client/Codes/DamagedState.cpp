#include "stdafx.h"
#include "DamagedState.h"
#include "ComponentMgr.h"
#include "Trail.h"
_USING(Client)

DamagedState::DamagedState()
{
	
}

void DamagedState::AIState(const float timeDelta)
{
#ifdef _DEBUG
#ifdef ENEMY_BONE_TEST
	static int count = 0;
	string temp;
	char buffer[5] = "";

	if (Engine::KeyMgr::GetInstance()->KeyDown(DIK_9))
	{
		++count;
		if (count > 61)
		{
			count = 0;
		}

		temp += "_";
		_itoa_s(count, buffer, 10);
		temp += buffer;
		cout << "Enemy Bone Name: " << temp.c_str() << endl;
		pBonePos = m_pMesh->Get_BonePos(temp.c_str());
	}
#endif
#endif
}

void DamagedState::AISet()
{
	mDamagedTime += gTimeDelta * 10;

	mt19937 mtSamllRand((size_t)(rand()));
	uniform_real_distribution<float> smallRange(5.f, 10.f);
	mDamaged_Value = smallRange(mtSamllRand);

	if (mDamagedTime >= 3.5f)
	{
		switch (mAIMode)
		{
		case AI_DAMAGED:
			mAnimState = "Damaged";
			mAnimSpeed = 1.2f;
			mPushSpeed = 8.f;
			SetDamged(mDamaged_Value);
			break;
		case AI_DAMAGED_LEFT:
			mAnimState = "DamageLeft";
			mAnimSpeed = 2.f;
			SetDamged(mDamaged_Value);
			break;
		case AI_DAMAGED_RIGHT:
			mAnimState = "DamageRight";
			mAnimSpeed = 1.5f;
			SetDamged(mDamaged_Value);
			break;
		default:
			break;
		}

		mDamagedTime = 0.f;
	}

	
	if (mPre_AIMode == mAIMode)
		return;

	switch (mPre_AIMode)
	{
	case AI_DAMAGE_SWO:
		m_pTransform->RightStep(4.7f);
		m_pTransform->GoStraight(9.7f);
		m_pTransform->SetRotation(D3DXVECTOR3(0.f, 180.f, 0.f));
		//SetDamged(mDamaged_Value * 1.5f);
		break;
	default:
		break;
	}

	mt19937 mtRand((size_t)(rand()));
	uniform_real_distribution<float> range(10.f, 25.f);
	mDamaged_Value = range(mtRand);

	switch (mEnemyAI_Info.enemyType)
	{
	case HUMAN_BOSS:
		mStunSpeed = 3.f;
		break;
	default:
		mStunSpeed = 1.f;
		break;
	}

	switch (mAIMode)
	{
	case AI_DAMAGED:
		mAnimState = "Damaged";
		mAnimSpeed = 1.2f;
		mPushSpeed = 8.f;
		SetDamged(mDamaged_Value * 0.1f);
		break;
	case AI_DAMAGED_LEFT:
		mAnimState = "DamageLeft";
		mAnimSpeed = 1.2f;
		SetDamged(mDamaged_Value * 0.1f);
		break;
	case AI_DAMAGED_RIGHT:
		mAnimState = "DamageRight";
		mAnimSpeed = 1.2f;
		SetDamged(mDamaged_Value * 0.1f);
		break;
	case AI_STAND_DOWN:
		mAnimState = "StandDown";
		mAnimSpeed = 1.5f;
		break;
	case AI_STAND_UP:
		mAnimState = "StandUp";
		mAnimSpeed = 2.f;
		break;
	case AI_DEFENCE:
		mAnimState = "Defence";
		mAnimSpeed = 2.f;
		break;
	case AI_DESH_BACK:
		mAnimState = "DeshBack";
		mAnimSpeed = 1.2f;
		mPushSpeed = 6.f;

		switch (mEnemyAI_Info.enemyType)
		{
		case BABEGAZI_AXE:
			m_pSoundMgr->PlaySound(L"Babe_Axe_Def_Sword_1.wav", CHANNEL_BABE_AXE);
			break;
		case BABEGAZI_BOW:
			m_pSoundMgr->PlaySound(L"Babe_Axe_Def_Sword_1.wav", CHANNEL_BABE_BOW);
			break;
		case BABEGAZI_WARRIOR:
			m_pSoundMgr->PlaySound(L"Babe_Warrior_Def_Sword_1.wav", CHANNEL_BABE_WARRIOR);
			break;
		}
		break;
	case AI_DAMAGE_SWO:
		mAnimState = "DamageSwo";
		mAnimSpeed = 1.8f;
		SetDamged(mDamaged_Value * 1.8f);

		switch (mEnemyAI_Info.enemyType)
		{
		case BABEGAZI_AXE:
			m_pSoundMgr->PlaySound(L"WitchDisiram.wav", CHANNEL_BABE_AXE);
			break;
		case BABEGAZI_BOW:
			m_pSoundMgr->PlaySound(L"WitchDisiram.wav", CHANNEL_BABE_BOW);
			break;
		case BABEGAZI_WARRIOR:
			m_pSoundMgr->PlaySound(L"WitchDisiram.wav", CHANNEL_BABE_WARRIOR);
			break;
		}
		
		break;
	case AI_SPIN_A:
		mAnimState = "Spin_A";
		mAnimSpeed = 1.f;
		SetDamged(mDamaged_Value * 2.2f);
		break;
	case AI_STUN:
		mAnimState = "Stun";
		mAnimSpeed = mStunSpeed;
		SetDamged(mDamaged_Value * 1.f);
		break;
	case AI_REATTACK:
		mAnimState = "ReAttack";
		mAnimSpeed = 0.8f;
		m_pSoundMgr->PlaySound(L"WitchDisiram.wav", CHANNEL_BABE_WARRIOR);
		break;
	case AI_DAMAGED_DOWN:
		mAnimState = "DamagedDown";
		mAnimSpeed = 1.f;
		break;
	case AI_DAM_UFA:
		mAnimState = "DamUFa";
		mAnimSpeed = 1.f;
		SetDamged(mDamaged_Value * 2.3f);

		m_pSoundMgr->PlaySound(L"Floor_Hit.wav", CHANNEL_KNOLE_COMMANDER);
		break;
	case AI_MOVE_DASH_L:
		mAnimState = "MoveDashL";
		mAnimSpeed = 1.5f;
		break;
	case AI_MOVE_DASH_R:
		mAnimState = "MoveDashR";
		mAnimSpeed = 1.5f;
		break;
	case AI_ROUND_PUNCH:
		mAnimState = "RoundPunchDamage";
		mAnimSpeed = 1.f;
		SetDamged(mDamaged_Value * 2.5f);
		m_pSoundMgr->PlaySound(L"Hanuman_RoundPunch_Voice.wav", CHANNEL_BOSS);
		break;
	case AI_BOSS_STAND_DOWN:
		mAnimState = "StandDown";
		mAnimSpeed = 2.f;
		SetDamged(55.f); // 2.5f
		m_pSoundMgr->PlaySound(L"Hanuman_Roar_Voice.wav", CHANNEL_BOSS);
		break;
	case AI_CRAZY_FOOT:
		mAnimState = "CrazyFoot";
		mAnimSpeed = 1.5f;
		if (m_pLeftTrail != nullptr && m_pRightTrail != nullptr)
		{
			m_pLeftTrail->SetTextureNum(18);
			m_pRightTrail->SetTextureNum(18);
		}
		break;
	case AI_DOWN:
	case AI_DIE:
		mAnimState = "Down";
		mAnimSpeed = 1.5f;
		m_pSoundMgr->PlaySound(L"Floor_Hit.wav", CHANNEL_BABE_WARRIOR);
		break;
	default:
		assert(false && "DamagedState AISet!!");
		break;
	}

	mPre_AIMode = mAIMode;
}

void DamagedState::AI_StateChange(const float timeDelta)
{
	D3DXVECTOR3 vPos;

	if (mPreAnimState != mAnimState)
	{
		ChangeInit();
	}

	if (mAIMode == AI_SPIN_A && mAnimTime >= 0.2)
	{
		if (m_pEnemyPos->y <= m_pPlaneY->y)
		{
			mAIMode = AI_STAND_DOWN;
			mJumpTime = 0.f;
		}
	}

	if (mAIMode == AI_DAM_UFA && mAnimTime >= 0.2)
	{
		if (m_pEnemyPos->y <= m_pPlaneY->y)
		{
			mAIMode = AI_DAMAGED_DOWN;
			mJumpTime = 0.f;
		}
	}

	if ((mAIMode == AI_ROUND_PUNCH || mAIMode == AI_BOSS_STAND_DOWN)  && mAnimTime >= 0.2)
	{
		if (m_pEnemyPos->y <= m_pPlaneY->y)
		{
			mJumpTime = 0.f;
		}
	}



	if (mAnimTime >= ((m_pAnimCtrl_Info->animPeriod) - mCorrectionTime))
	{
		mt19937 mtRand((size_t)(rand()));
		uniform_int_distribution<size_t> range(1, 10);
		mForceAttack_Percent = range(mtRand);

		AIChange();
		mAnimTime = 0.0;
	
	}

	if (mPre_AIMode == AI_DIE)
	{
		mAnimSpeed = 0.f;
	}

	if (FAILED(m_pMesh->PlayAnimation(mAnimSpeed * timeDelta)))
	{
		_MSG_BOX("DamagedState PlayAnimation FAILED!");
	}


}


void DamagedState::AIChange()
{
	switch (mAIMode)
	{
	case AI_DAMAGED:
		mAIMode = AI_STAND_DOWN;

		if (*mEnemyAI_Info.pDamaged)
		{
			*mEnemyAI_Info.pDamaged = false;
		}

		break;
	case AI_DAMAGED_DOWN:
	case AI_STAND_DOWN:
		if (*mEnemyAI_Info.pHP <= 0)
		{
			mAIMode = AI_DOWN;
		}
		else
		{
			mAIMode = AI_STAND_UP;
		}
		break;
	case AI_DAMAGE_SWO:
		mAIMode = AI_STAND_UP;
		break;
	case AI_SPIN_A:
		if (mAnimTime >= 0.2)
		{
			if (m_pEnemyPos->y <= m_pPlaneY->y)
			{
				mAIMode = AI_STAND_DOWN;
				mJumpTime = 0.f;
				if (*mEnemyAI_Info.pDamaged)
				{
					*mEnemyAI_Info.pDamaged = false;
				}
			}
		}
		break;
	case AI_DAM_UFA:
		if (mAnimTime >= 0.2)
		{
			if (m_pEnemyPos->y <= m_pPlaneY->y)
			{
				mAIMode = AI_DAMAGED_DOWN;
				mJumpTime = 0.f;
			}
		}
		break;
	case AI_DOWN:
		mAIMode = AI_DIE;
		break;
	default:
		AI_Init();
		break;
	}
}

void DamagedState::AI_Init()
{
	if (*mEnemyAI_Info.pHP <= 0)
	{
		mAIMode = AI_DOWN;
		return;
	}

	*mEnemyAI_Info.pEnemy_AIMode = AI_IDLE;
	if (*mEnemyAI_Info.pDamaged)
	{
		*mEnemyAI_Info.pDamaged = false;
	}

	mAIMode = AI_IDLE;
	mPre_AIMode = AI_IDLE;

	switch (mEnemyAI_Info.enemyType)
	{
	case BABEGAZI_AXE:
	case BABEGAZI_BOW:
	case BABEGAZI_WARRIOR:
		mForceAttack_Value = 2;
		break;
	case KNOLE_COMMANDER:
	case KNOLE_WARRIOR:
		mForceAttack_Value = 3;
		break;
	case HUMAN_BOSS:
		mForceAttack_Value = 6;
		break;
	default:
		break;
	}

	if (mForceAttack_Percent <= mForceAttack_Value)
	{
		*mEnemyAI_Info.pForecAttack = true;
	}
}

void DamagedState::AIUpdate(const float timeDelta)
{
	mAnimTime += mAnimSpeed *timeDelta;

	switch (mAIMode)
	{
	case AI_DAMAGED:
		if (mEnemyAI_Info.enemyType <= BABEGAZI_BOW)
		{
			GoStraight(timeDelta, -mPushSpeed);
			//m_pTransform->GoStraight(-mPushSpeed * timeDelta);
		}
		break;
	case AI_DESH_BACK:
		if (mAnimTime >= 0.2f && mAnimTime <= 1.3f)
		{
			GoStraight(timeDelta, -mPushSpeed);
			//m_pTransform->GoStraight(-mPushSpeed *  timeDelta);
		}
		break;
	case AI_DEFENCE:
	case AI_REATTACK:
		CalDir(timeDelta);
		break;
	case AI_SPIN_A:
		Jump(timeDelta, JUMP_BACK, 3.f, 90.f, 75.f);
		break;
	case AI_DAM_UFA:
		Jump(timeDelta, JUMP_BACK, 2.5f, 90.f, 75.f);
		break;
	case AI_BOSS_STAND_DOWN:
	case AI_ROUND_PUNCH:
		Jump(timeDelta, JUMP_BACK, 3.5f, 45.f, 75.f);
		break;
	default:
		break;
	}
}

void DamagedState::Collision(const float timeDelta)
{

}


DamagedState * DamagedState::Create()
{
	DamagedState* pInstance = new DamagedState();
	return pInstance;
}

DWORD DamagedState::Free()
{
	EnemyState::Free();
	return 0;
}
