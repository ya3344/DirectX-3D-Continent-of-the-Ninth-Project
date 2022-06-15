#include "stdafx.h"
#include "AttackState.h"
#include "ComponentMgr.h"
#include "PlayerAnim.h"
#include "Player.h"
#include "Trail.h"
#include "DecalBox.h"
_USING(Client)

AttackState::AttackState()
{

	m_pSoundMgr->SetVolume(CHANNEL_BABE_AXE, 0.1f);
}

void AttackState::AIState(const float timeDelta)
{
	if (mPre_AIMode == mAIMode)
		return;

	mt19937 mtRand((size_t)(rand()));
	uniform_int_distribution<size_t> range(1, 10);
	mAttackPercent = range(mtRand);

	switch (mEnemyAI_Info.enemyType)
	{
	case BABEGAZI_BOW:
		if (mAIMode != AI_BOW_ATTACK)
		{
			mAIMode = AI_BOW_ATTACK_READY;
			m_bArrow = false;
		}
		break;
	case BABEGAZI_WARRIOR:
		mAIMode = AI_ATTACK;
		mAttackTime = 1.55;
		mAttackValue = 5;
		m_pSoundMgr->PlaySound(L"Babe_Axe_Attack_1.wav", CHANNEL_BABE_WARRIOR);
		
		/*if (mAttackPercent <= 8)
		{
			mAIMode = AI_ATTACK;
			mAttackTime = 1.55;
			mAttackValue = 5;
		}
		else
		{
			mAIMode = AI_DEFENCE_SWORD;
			mAttackTime = 1.2;
			mAttackValue = 5;
		}*/
		mAttackRange = 7;
		break;
	case BABEGAZI_AXE:
		if (mAttackPercent <= 5)
		{
			mAIMode = AI_ATTACK;
			mAttackTime = 1.7;
			mAttackValue = 5;
			m_pSoundMgr->PlaySound(L"Babe_Axe_Attack_1.wav", CHANNEL_BABE_AXE);
			
		}
		else
		{
			mAIMode = AI_ATTACK2;
			mAttackTime = 1.4;
			mAttackValue = 5;
			m_pSoundMgr->PlaySound(L"Babe_Axe_Attack_2.wav", CHANNEL_BABE_AXE);
		}
		mAttackRange = 7.5f;
		break;
	case KNOLE_COMMANDER:
	case KNOLE_WARRIOR:
		//mAIMode = AI_DOWN_ATTACK;
		//mAttackTime = 1.5;
		//mAttackValue = 10;

		if (mPre_AIMode == AI_INIT_ATTACK)
		{
			mAIMode = AI_HAND_ONE_TWO;
			mAttackTime = 0.1;
			mAttackValue = 7;
		}
		else if (mPre_AIMode == AI_HAND_ONE_TWO)
		{
			if (mAttackPercent <= 5)
			{
				mAIMode = AI_HAND_ONE_TWO;
				mAttackTime = 0.1;
				mAttackValue = 6;
			}
			else
			{
				mAIMode = AI_SPIN_ATTACK;
				mAttackTime = 0.2;
				mAttackValue = 6;
			}
		}
		else
		{
			if (mAttackPercent <= 3)
			{
				mAIMode = AI_ATTACK;
				mAttackTime = 0.8;
				mAttackValue = 6;
			}
			else if (mAttackPercent <= 6)
			{
				mAIMode = AI_SPIN_ATTACK;
				mAttackTime = 0.2;
				mAttackValue = 6;
			}
			else if (mAttackPercent <= 8)
			{
				mAIMode = AI_DOWN_ATTACK;
				mAttackTime = 1.5;
				mAttackValue = 7;
			}
			else
			{
				mAIMode = AI_INIT_ATTACK;
			}
		}
		mAttackRange = 9.f;
		break;

	case HUMAN_BOSS:
		//if (mAttackPercent <= 5)
		//{
		//	mAIMode = AI_BOSS_ATTACK;
		//	mAttackTime = 1.2; // 5.7
		//	mAttackValue =10;
		//	mAttackRange = 15.f;
		//}
		//else
		//{
		//	mAIMode = AI_BOSS_ATTACK2;
		//	mAttackTime = 3.f;
		//	mAttackValue = 10;
		//	mAttackRange = 15.f;
		//}
	/*	mAIMode = AI_BOSS_ATTACK;
		mAttackRange = 20.f;*/

		/*mAIMode = AI_BOSS_ATTACK2;
		mAttackRange = 50.f;*/

	//	mAIMode = AI_CRAZY_FOOT;

		//mAIMode = AI_BOSS_COMBO_0;
		//mAttackRange = 20.f;

		if (*mEnemyAI_Info.pBoss_HPCount == 3)
		{
			if (mPre_AIMode == AI_CRAZY_FOOT)
			{
				mAIMode = AI_BOSS_ATTACK;
				mAttackRange = 20.f;
			}
			else
			{
				if (mAttackPercent <= 5)
				{
					mAIMode = AI_BOSS_ATTACK;
					mAttackRange = 20.f;
				}
				else
				{
					mAIMode = AI_CRAZY_FOOT;
				}
			}
		}
		else if (*mEnemyAI_Info.pBoss_HPCount == 2)
		{
			if (mPre_AIMode == AI_CRAZY_FOOT)
			{
				mAIMode = AI_BOSS_COMBO_0;
				mAttackRange = 20.f;

				//mAIMode = AI_CRAZY_FOOT;
			}
			else
			{
				if (mAttackPercent <= 5)
				{
					mAIMode = AI_BOSS_COMBO_0;
					mAttackRange = 20.f;
				}
				else if (mAttackPercent <= 7)
				{
					mAIMode = AI_BOSS_ATTACK;
					mAttackRange = 20.f;
				}
				else
				{
					mAIMode = AI_CRAZY_FOOT;
				}
			}
		}
		else
		{
			if(mPre_AIMode == AI_BOSS_ATTACK2)
			{
				if (mAttackPercent <= 3)
				{
					//AI_BOSS_COMBO_0
					mAIMode = AI_BOSS_COMBO_0;
					mAttackRange = 20.f;
				}
				else
				{
					mAIMode = AI_BOSS_ATTACK;
					mAttackRange = 20.f;
				}
			}
			else
			{
				if (mAttackPercent <= 6)
				{
					mAIMode = AI_BOSS_ATTACK2;
					mAttackRange = 50.f;
				}
				else
				{
					mAIMode = AI_ROUND_PUNCH_C;
					mAttackRange = 30.f;
				}
			}
		}

	/*	if (mAIMode == AI_SPECIAL_CHASE)
		{
			
			if (mAttackPercent <= 2)
			{
				mAIMode = AI_BOSS_ATTACK;
				mAttackRange = 20.f;

			}
			else if (mAttackPercent <= 4)
			{
				mAIMode = AI_BOSS_ATTACK2;
				mAttackRange = 50.f;
			}
			else if (mAttackPercent <= 6)
			{
				mAIMode = AI_CRAZY_FOOT;
			}
			else if (mAttackPercent <= 8)
			{
				mAIMode = AI_BOSS_COMBO_0;
				mAttackRange = 20.f;
			}
			else
			{
				mAIMode = AI_ROUND_PUNCH_C;
				mAttackRange = 30.f;
			}
		}
		else
		{
			if (mAttackPercent <= 2)
			{
				mAIMode = AI_BOSS_ATTACK;
				mAttackRange = 20.f; 

			}
			else if (mAttackPercent <= 5)
			{
				mAIMode = AI_BOSS_ATTACK2;
				mAttackRange = 50.f;
			}
			else if (mAttackPercent <= 7)
			{
				mAIMode = AI_CRAZY_FOOT;
			}
			else
			{
				mAIMode = AI_BOSS_COMBO_0;
				mAttackRange = 20.f;
			}
		}*/

		break;

	default:
		assert(false && "AttackState AIState!!!");
		break;
	}


}

void AttackState::AISet()
{
	if (mPre_AIMode == mAIMode)
		return;

	switch (mAIMode)
	{
	case AI_ATTACK:
		m_bArrow = false;
		mAnimState = "Attack";
		mAnimSpeed = 1.4f;
		break;
	case AI_ATTACK2:
		mAnimState = "Attack2";
		mAnimSpeed = 1.2f;
		break;
	case AI_SPIN_ATTACK:
		m_pSoundMgr->PlaySound(L"Knole_Commander_Weapon_4.wav", CHANNEL_KNOLE_COMMANDER);
		mAnimState = "SpinAttack";
		mAnimSpeed = 0.8f;
		break;
	case AI_DOWN_ATTACK:
		m_pSoundMgr->PlaySound(L"Knole_Commander_Weapon_3.wav", CHANNEL_KNOLE_COMMANDER);
		mAnimState = "DownAttack";
		mAnimSpeed = 1.2f;
		break;
	case AI_BOW_ATTACK:
		m_pSoundMgr->PlaySound(L"Babe_Bow_Attack_1.wav", CHANNEL_BABE_BOW);
		mAnimState = "BowAttack";
		mAnimSpeed = 1.1f;
		break;
	case AI_BOW_ATTACK_READY:
		mAnimState = "BowAttack_Ready";
		mAnimSpeed = 1.3f;
		break;
	case AI_INIT_ATTACK:
		m_pSoundMgr->PlaySound(L"Knole_Commander_Weapon_1.wav", CHANNEL_KNOLE_COMMANDER);
		mAnimState = "InitAttack";
		mAnimSpeed = 1.5f;
		break;
	case AI_HAND_ONE_TWO:
		m_pSoundMgr->PlaySound(L"Knole_Commander_Weapon_2.wav", CHANNEL_KNOLE_COMMANDER);
		mAnimState = "HandOneTwo";
		mAnimSpeed = 1.f;
		break;
	case AI_BOSS_ATTACK: //"BossAttack -> TrailTexture23";
		mAnimState = "BossAttack";
		if (m_pLeftTrail != nullptr && m_pRightTrail != nullptr)
		{
			m_pLeftTrail->SetTextureNum(23); 
			m_pRightTrail->SetTextureNum(23);
		}
		mAnimSpeed = 1.4f; // 2.f
		break;
	case AI_BOSS_ATTACK2: //"BossAttack2-> TrailTexture3"; //RoundPunchB -> TrailTexture7
		mAnimState = "BossAttack2"; 
		if (m_pLeftTrail != nullptr && m_pRightTrail != nullptr)
		{
			m_pLeftTrail->SetTextureNum(3);
			m_pRightTrail->SetTextureNum(3);
		}
		mAnimSpeed = 1.6f; //2.f
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
	case AI_DEFENCE_SWORD:
		mAnimState = "DefenceSword";
		mAnimSpeed = 1.5f;
		break;
	case AI_BOSS_COMBO_0:
		mAnimState = "Combo0";
		mAnimSpeed = 1.4f;
		break;
	case AI_ROUND_PUNCH_C:
		mAnimState = "RoundPunchC";
		mAnimSpeed = 1.5f;
		if (m_pLeftTrail != nullptr && m_pRightTrail != nullptr)
		{
			m_pLeftTrail->SetTextureNum(23);
			m_pRightTrail->SetTextureNum(23);
		}
		break;
	default:
		assert(false && "AttackState AISet!!");
		break;
	}

	mPre_AIMode = mAIMode;
}

void AttackState::AI_StateChange(const float timeDelta)
{
	if (mPreAnimState != mAnimState)
	{
		ChangeInit();
	}

	if (mAnimTime >= ((m_pAnimCtrl_Info->animPeriod) - mCorrectionTime))
	{
		mAnimTime = 0.0;

		if (m_bArrow)
		{
			m_bArrow = false;
		}
		//if (m_pCollider->Collision_AABB(m_pPlayerCollider, nullptr) == false)
		//{
		//	if (*mEnemyAI_Info.pAttack == true)
		//	{
		//		*mEnemyAI_Info.pAttack = false;
		//	}
		//}	

		if (mEnemyAI_Info.enemyType == HUMAN_BOSS)
		{
			Boss_ComboSet();
			//Attack_DefaultSet();
		}
		else
		{
			Attack_DefaultSet();
		}
	}

	if (FAILED(m_pMesh->PlayAnimation(mAnimSpeed * timeDelta)))
	{
		_MSG_BOX("AttackState PlayAnimation FAILED!");
	}
}

void AttackState::AIUpdate(const float timeDelta)
{
	if (Engine::KeyMgr::GetInstance()->KeyPressing(DIK_P))
	{
		SetDamged(10.f);
		//m_pUIMgr->NumberingUI_Create(*m_pTransform->Get_WorldMatrix(), 50000);
	}

	mAnimTime += mAnimSpeed *timeDelta;

	switch (mAIMode)
	{
	case AI_BOW_ATTACK:
		//cout << "화살발사시간: " << mAnimTime << endl;
		//if (mAnimTime >= 1.02)
		//{
			//cout << "화살발사시간: " << mAnimTime << endl;
			if (m_bArrow == false)
			{
				m_pBulletMgr->AddBullet(L"Prototype_GameObject_Arrow", BULLET_ARROW, *m_pEnemyDir, *m_pEnemyPos);
				m_bArrow = true;
			}
		//}
		break;
	case AI_CRAZY_FOOT:
		//cout << "CrazyFoot Time: " << mAnimTime << endl;
		if (mAnimTime >= 1.8f && mAnimTime <= 1.84f)
		{
			m_pSoundMgr->PlaySound(L"Hanuman_CrazyFoot_Floor_1.wav", CHANNEL_BOSS);
			m_pDecal->SetDecalInfo(*m_pEnemyPos + (*m_pEnemyRightDir * -5.f), D3DXVECTOR3(10.f, 4.f, 10.f), 1, DECAL_CRAZY_FOOT, true);
			m_pBulletMgr->IceMisile(L"Prototype_GameObject_Ice_Missile1", BULLET_ICEMISSILE, 6, *m_pEnemyDir, *m_pEnemyPos);
			m_pBulletMgr->IceMisile(L"Prototype_GameObject_Ice_Missile2", BULLET_ICEMISSILE2, 3, *m_pEnemyDir, *m_pEnemyPos);
		}
		else if (mAnimTime > 2.35f && mAnimTime <= 2.39f)
		{
			m_pSoundMgr->PlaySound(L"Hanuman_CrazyFoot_Floor_1.wav", CHANNEL_BOSS);
			m_pDecal->SetDecalInfo(*m_pEnemyPos + (*m_pEnemyRightDir * 5.f), D3DXVECTOR3(10.f, 4.f, 10.f), 1, DECAL_CRAZY_FOOT, true);
			m_pBulletMgr->IceMisile(L"Prototype_GameObject_Ice_Missile1", BULLET_ICEMISSILE, 6, *m_pEnemyDir, *m_pEnemyPos);
			m_pBulletMgr->IceMisile(L"Prototype_GameObject_Ice_Missile2", BULLET_ICEMISSILE2, 3, *m_pEnemyDir, *m_pEnemyPos);
		}
		break;
	default:
		mDistance = m_pUtilityMgr->Cal_TargetDistance(*m_pEnemyPos, *m_pPlayerPos);
		break;
	}

	Collision(timeDelta);
	CollisionEX(timeDelta);
	CalDir(timeDelta);
	m_pEnemyMgr->Enemy_AttackCount();
}

void AttackState::Collision(const float timeDelta)
{
	mt19937 mtRand((size_t)(rand()));
	uniform_int_distribution<size_t> range(1, 10);
	mAttack_SucessPercent = range(mtRand);
	mAttack_CriticalPercent = range(mtRand);

	//cout << "mAttack_SucessPercent: " << mAttack_SucessPercent << endl;

	if (mAnimTime >= mAttackTime)
	{
		if (m_bAttack == false)
		{
			if (mAttack_SucessPercent <= mAttackValue)
			{
				if (m_pPlayerAnimState->find("Jump") != string::npos)
					return;

				//if (m_pCollider->Collision_AABB(m_pPlayerCollider))
				if(m_pUtilityMgr->Cal_TargetDistance(*m_pEnemyPos, *m_pPlayerPos) <= mAttackRange)
				{
					//cout << m_pUtilityMgr->Cal_TargetDistance(*m_pEnemyPos, *m_pPlayerPos) << endl;
					switch (mAIMode)
					{
					case AI_ATTACK:
					case AI_DEFENCE_SWORD:
					case AI_ATTACK2:
					case AI_SPIN_ATTACK:
					case AI_HAND_ONE_TWO:
						if (m_pPlayerAnim->Get_DamagedCount() <= 5)
						{
							if (m_pPlayerAnim->Get_DamagedState() == false)
							{
			
								*m_pPlayerAnimState = "Damaged";
							}
							else
							{
								if (mAttack_CriticalPercent <= 4)
								{
									if (m_pPlayerAnim->Get_DamagedState() == false
										|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Damaged"))
									{
										m_pPlayerAnim->Set_DamagedState(false);
										m_pPlayerAnim->SetJumpInfo(JUMP_INFO(2.5f, 90.f, 75.f, JUMP_BACK));
										*m_pPlayerAnimState = "DownA";
									}
								}
							}
							

						}
						else
						{
							if (m_pPlayerAnim->Get_DamagedState() == false
								|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Damaged"))
							{
								*m_pPlayerAnimState = "Stun";
							}
						}
						break;
					case AI_DOWN_ATTACK:
						if (m_pPlayerAnim->Get_DamagedState() == false
							/*|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Damaged")*/
							|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Stun"))
						{
							m_pPlayerAnim->Set_DamagedState(false);
							m_pPlayerAnim->SetJumpInfo(JUMP_INFO(2.5f, 90.f, 85.f, JUMP_BACK));
							*m_pPlayerAnimState = "DownA";
						}
						break;
					default:
						break;
					}

				}
			}
			m_bAttack = true;
		}
	}

	if (mEnemyAI_Info.enemyType != HUMAN_BOSS)
		return;

	if (m_pUtilityMgr->Cal_TargetDistance(*m_pEnemyPos, *m_pPlayerPos) <= mAttackRange)
	{
		switch (mAIMode)
		{
		case AI_BOSS_ATTACK:
			if (mAnimTime >= 1.16f && mAnimTime <= 1.35f)
			{
	/*			if (m_pPlayerAnim->Get_DamagedState() == false
					|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Damaged"))
				{*/
					m_pSoundMgr->PlaySound(L"Hanuman_Attack_1_1.wav", CHANNEL_BOSS);
					m_pPlayerAnim->Set_DamagedState(false);
					*m_pPlayerAnimState = "Stun";
		//		}
			}
			else if (mAnimTime >= 3.1f && mAnimTime <= 3.2f)
			{
		/*		if (m_pPlayerAnim->Get_DamagedState() == false
					|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Damaged")
					|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Stun"))
				{*/
					m_pPlayerAnim->Set_DamagedState(false);
					m_pPlayerAnim->Set_EnemyRightDir(*m_pEnemyRightDir * -1.f);
					m_pPlayerAnim->SetJumpInfo(JUMP_INFO(3.7f, 30.f, 45.f, JUMP_DIR));
					*m_pPlayerAnimState = "DownA";
					m_pSoundMgr->PlaySound(L"Hanuman_Attack_1_2.wav", CHANNEL_BOSS);
			//	}
			}
			break;
		case AI_BOSS_ATTACK2:
			if (mAnimTime >= 3.f && mAnimTime <= 3.2f)
			{
				m_pDecal->SetDecalInfo(*m_pEnemyPos + (*m_pEnemyDir * 10.f), D3DXVECTOR3(60.f, 4.f, 60.f), 3, DECAL_BOSS_ATTACK2, true);

			/*	if (m_pPlayerAnim->Get_DamagedState() == false
					|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Damaged")
					|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Stun"))
				{*/
					m_pPlayerAnim->Set_DamagedState(false);
					//m_pPlayerAnim->Set_EnemyRightDir(*m_pEnemyRightDir * -1.f);
					m_pPlayerAnim->SetJumpInfo(JUMP_INFO(3.9f, 90.f, 90.f, JUMP_BACK));
					*m_pPlayerAnimState = "DownA";
					m_pSoundMgr->PlaySound(L"Hanuman_Attack_2.wav", CHANNEL_BOSS);
			//	}
			}
			break;
		case  AI_ROUND_PUNCH_C:
			if (mAnimTime >= 0.6f && mAnimTime <= 0.65f)
			{
	/*			if (m_pPlayerAnim->Get_DamagedState() == false
					|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Damaged")
					|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Stun"))
				{*/
					m_pPlayerAnim->Set_DamagedState(false);
					m_pPlayerAnim->Set_EnemyRightDir(*m_pEnemyRightDir * -1.f);
					m_pPlayerAnim->SetJumpInfo(JUMP_INFO(3.7f, 30.f, 45.f, JUMP_DIR));
					*m_pPlayerAnimState = "DownA";
					m_pSoundMgr->PlaySound(L"Hanuman_RoundPunch_Fall_1.wav", CHANNEL_BOSS);
		//		}
			}
			break;
		default:
			if (mAnimState == "Combo1")
			{
				//cout << "ComboTime: " << mAnimTime << endl;
				if (mAnimTime >= 0.45f && mAnimTime <= 0.6f)
				{
					if (m_pPlayerAnim->Get_DamagedState() == false
						|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Damaged"))
					{
						m_pPlayerAnim->Set_DamagedState(false);
						*m_pPlayerAnimState = "Stun";
						m_pSoundMgr->PlaySound(L"Hanuman_Combo_Attack_1.wav", CHANNEL_BOSS);
					}
				}
			}
			else if (mAnimState == "Combo2")
			{
				if (mAnimTime >= 0.65f && mAnimTime <= 0.78f)
				{
					m_pDecal->SetDecalInfo(*m_pEnemyPos + (*m_pEnemyDir * 10.f), D3DXVECTOR3(30.f, 4.f, 30.f), 5, DECAL_BOSS_COMBO, true);


					if (m_pPlayerAnim->Get_DamagedState() == false
						|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Damaged")
						|| (m_pPlayerAnim->Get_DamagedState() == true && *m_pPlayerAnimState == "Stun"))
					{
						m_pPlayerAnim->Set_DamagedState(false);
						//m_pPlayerAnim->Set_EnemyRightDir(*m_pEnemyRightDir * -1.f);
						m_pPlayerAnim->SetJumpInfo(JUMP_INFO(3.7f, 90.f, 75.f, JUMP_BACK));
						*m_pPlayerAnimState = "DownA";
						m_pSoundMgr->PlaySound(L"Hanuman_Combo_Attack_2.wav", CHANNEL_BOSS);
					}
				}
			}
			break;
		}
	}
}

void AttackState::CollisionEX(const float timeDelta)
{
	CalDiagonalDir(m_vEnemyRightDir);
	//D3DXVECTOR3 vDir = m_vEnemyRightDir;

	if (m_pEnemyMgr->Enemy_CollisionAABB(m_pCollider, &m_vEnemyRightDir))
	{
		
		if (m_pNavigation->is_Move(*m_pEnemyPos, m_vEnemyRightDir, &m_vLineNormal))
		{
			m_pTransform->DirStep(m_vEnemy_DiagonalDir * 2.f * timeDelta);
		}
		else
		{
			m_pUtilityMgr->Cal_SlidingVector(*m_pEnemyDir, m_vLineNormal, &m_vSlide, &mSlideSpeed);
			m_vEnemyRightDir = m_vSlide * (float)mMoveSpeed * timeDelta;

			if (m_pNavigation->is_Move(*m_pEnemyPos, m_vEnemyRightDir, &m_vLineNormal) == true)
			{
				*m_pEnemyDir = m_vSlide;
				m_pTransform->GoStraight(mMoveSpeed * timeDelta);
			}
		}
	}

}

void AttackState::Boss_ComboSet()
{
	//switch (mAIMode)
	//{
	//case AI_BOSS_COMBO_0:
	//	mAIMode = AI_BOSS_COMBO_1;
	//	break;
	//case AI_BOSS_COMBO_1:
	//	mAIMode = AI_BOSS_COMBO_2;
	//	break;
	//case AI_BOSS_COMBO_2:
	//	mAIMode = AI_BOSS_COMBO_0;
	//	break;
	//default:
	//	Attack_DefaultSet();
	//	break;
	//}
	if (mAnimState == "Combo0")
	{
		mAnimState = "Combo1";
		if (m_pLeftTrail != nullptr && m_pRightTrail != nullptr)
		{
			m_pLeftTrail->SetTextureNum(7);
			m_pRightTrail->SetTextureNum(7);
		}
	}
	else if (mAnimState == "Combo1")
	{
		mAnimState = "Combo2";
		if (m_pLeftTrail != nullptr && m_pRightTrail != nullptr)
		{
			m_pLeftTrail->SetTextureNum(19);
			m_pRightTrail->SetTextureNum(19);
		}
	}
	else if (mAnimState == "Combo2")
	{
		mAnimState = "Combo2End";
	}
	else
	{
		Attack_DefaultSet();
	}
}

void AttackState::Attack_DefaultSet()
{
	if (*mEnemyAI_Info.pAttack == true)
	{
		*mEnemyAI_Info.pAttack = false;
	}

	if (*mEnemyAI_Info.pForecAttack == true)
	{
		*mEnemyAI_Info.pForecAttack = false;
	}

	*mEnemyAI_Info.pEnemy_AIMode = AI_IDLE;

	mAIMode = AI_IDLE;
	m_bAttack = false;
}

AttackState * AttackState::Create()
{
	AttackState* pInstance = new AttackState();
	return pInstance;
}

DWORD AttackState::Free()
{
	EnemyState::Free();
	return 0;
}



