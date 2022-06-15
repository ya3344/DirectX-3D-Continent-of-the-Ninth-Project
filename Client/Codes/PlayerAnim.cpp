#include "stdafx.h"
#include "PlayerAnim.h"
#include "ComponentMgr.h"
#include "EnemyAI.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "Trail.h"
#include "PlayerDecal.h"

_USING(Client)

PlayerAnim::PlayerAnim(LPDIRECT3DDEVICE9 pDevice)
	:PlayerCommand(pDevice)
{
}

PlayerAnim::PlayerAnim(const PlayerAnim & rhs)
	:PlayerCommand(rhs)
{
}

HRESULT PlayerAnim::Init()
{


	return NOERROR;
}

HRESULT PlayerAnim::KeyCheck(const float timeDelta)
{
	if (m_bDamaged)
		return NOERROR;

	if (mAnimState == "LockOnTargetShot" || mAnimState == "DownA" || mAnimState == "Damaged" || mAnimState == "RisingAttack"
		|| mAnimState == "LevelUp")
		return NOERROR;

	if (mAnimState.find("Run") != string::npos)
	{
		mAnimState = "BattleIdle";
	}

	{ // Move
		if (m_pKeyMgr->KeyPressing(DIK_UP))
		{
			if (mAnimState.find("Gail") == string::npos)
			{
				mAnimState = "RunF_In";
				mAnimInfo = ANIM_INFO(-1, 1.3f);
			}
		
			if (mAnimState != "GailEnd" && mAnimState != "GailReady")
			{
				CalMove(FORWARD_SPEED, 1.4f, timeDelta);
			}
			
		}

		if (m_pKeyMgr->KeyPressing(DIK_DOWN))
		{
			if (mAnimState.find("Gail") == string::npos)
			{
				mAnimState = "RunB_In";
				mAnimInfo = ANIM_INFO(-1, 1.3f);
			}

			if (mAnimState != "GailEnd" && mAnimState != "GailReady")
			{
				CalMove(BACKWARD_SPEED, -1.2f, timeDelta);
			}
		}

		if (m_pKeyMgr->KeyPressing(DIK_LEFT))
		{
			m_pTransform->SetRotation(D3DXVECTOR3(0.f, -200.0f * timeDelta, 0.f));
		}

		if (m_pKeyMgr->KeyPressing(DIK_RIGHT))
		{
			m_pTransform->SetRotation(D3DXVECTOR3(0.f, 200.0f * timeDelta, 0.f));
		}

		if (m_pKeyMgr->KeyPressing(DIK_NUMPAD7))
		{
			m_pTransform->SetRotation(D3DXVECTOR3(0.f, -200.0f * timeDelta, 0.f));
		}

		if (m_pKeyMgr->KeyPressing(DIK_NUMPAD9))
		{
			m_pTransform->SetRotation(D3DXVECTOR3(0.f, 200.0f * timeDelta, 0.f));
		}
	}



	{ 
		// Jump
		if (m_pKeyMgr->KeyDown(DIK_W))
		{
			if (mAnimState == "JumpFB_In")
			{
				mJumpSpeed *= 1.7f;
				mJumpVelocity *= 1.4f;
				m_pEffectMgr->CreateEffect(L"Prototype_Effect_PlayerMove", EFFECT_INFO(nullptr, D3DXVECTOR3(10.f, 10.f, 10.f), EFFECT_PLAYER_MOVE));
			}
			else
			{
				mJumpSpeed = 0.9f;
				mJumpVelocity = JUMP_VELOCITY;
			}

			mAnimInfo = ANIM_INFO(-1, 1.1f);
			mAnimState = "JumpFB_In";
			m_bJump = true;
			mJumpTime = 0.f;

			m_pSoundMgr->PlaySound(L"WitchJump.wav", CHANNEL::CHANNEL_PLAYER);
		}
		
		if (m_pKeyMgr->KeyDown(DIK_S))
		{
			{
				mJumpSpeed = 0.9f;
				mJumpVelocity = JUMP_VELOCITY;
			}

			mAnimInfo = ANIM_INFO(-1, 1.1f);
			mAnimState = "JumpBB_In";
			m_bJump = true;
			mJumpTime = 0.f;

			m_pSoundMgr->PlaySound(L"WitchJump.wav", CHANNEL::CHANNEL_PLAYER);
		}

		if (m_pKeyMgr->KeyDown(DIK_A))
		{
			mAnimInfo = ANIM_INFO(-1, 1.1f);
			mAnimState = "JumpLB_In";
			m_bJump = true;
			mJumpVelocity = JUMP_VELOCITY;
			mJumpTime = 0.f;
			mJumpSpeed = 0.9f;

			m_pSoundMgr->PlaySound(L"WitchJump.wav", CHANNEL::CHANNEL_PLAYER);
		}

		if (m_pKeyMgr->KeyDown(DIK_D))
		{
			mAnimInfo = ANIM_INFO(-1, 1.1f);
			mAnimState = "JumpRB_In";
			m_bJump = true;
			mJumpVelocity = JUMP_VELOCITY;
			mJumpTime = 0.f;
			mJumpSpeed = 0.9f;

			m_pSoundMgr->PlaySound(L"WitchJump.wav", CHANNEL::CHANNEL_PLAYER);
		}

	}

	{ // Dash
		if (m_pKeyMgr->KeyDown(DIK_NUMPAD8))
		{
			mAnimInfo = ANIM_INFO(-1, 1.7f);
			mAnimState = "DashF";

			m_pSoundMgr->PlaySound(L"WitchJump.wav", CHANNEL::CHANNEL_PLAYER);
		}

		if (m_pKeyMgr->KeyDown(DIK_NUMPAD5))
		{
			mAnimInfo = ANIM_INFO(-1, 1.5f);
			mAnimState = "QuickStandB";

			m_pSoundMgr->PlaySound(L"WitchJump.wav", CHANNEL::CHANNEL_PLAYER);
		}

		if (m_pKeyMgr->KeyDown(DIK_NUMPAD4))
		{
			mAnimInfo = ANIM_INFO(-1, 2.f);
			mAnimState = "DashL";

			m_pSoundMgr->PlaySound(L"WitchJump.wav", CHANNEL::CHANNEL_PLAYER);
		}

		if (m_pKeyMgr->KeyDown(DIK_NUMPAD6))
		{
			mAnimInfo = ANIM_INFO(-1, 2.f);
			mAnimState = "DashR";

			m_pSoundMgr->PlaySound(L"WitchJump.wav", CHANNEL::CHANNEL_PLAYER);
		}
	}

	{ // Normal Attack
		if (m_pKeyMgr->KeyDown(DIK_Z))
		{
			mAnimInfo = ANIM_INFO(-1, 1.5f);
			mAnimState = "StunAttack";

			m_pTrail->SetTextureNum(18);
			//m_pTrail->SetColor(D3DXVECTOR4(0.3f, 0.3f, 0.0f, 0.0f));
			m_pPlayer->SetMP(-5.f);
		}
		//HoldingShot2
		if (m_pKeyMgr->KeyDown(DIK_X))
		{
			m_pTrail->SetTextureNum(19); // 19, 8
			mAnimInfo = ANIM_INFO(-1, 1.1f);
			mAnimState = "SpinCrushC";
			m_pPlayer->SetMP(-10.f);
		}

		if (m_pKeyMgr->KeyDown(DIK_C))
		{
			m_pTrail->SetTextureNum(6); // 5, 7, 8
			mAnimInfo = ANIM_INFO(-1, 2.7f);
			mAnimState = "AttShelTechnic3";
			m_pPlayer->SetMP(-7.f);
		}

		if (m_pKeyMgr->KeyDown(DIK_V))
		{
			m_pTrail->SetTextureNum(5); // 5, 7, 8
			//m_pTrail->SetColor(D3DXVECTOR4(0.1f, 0.f, 0.2f, 0.0f));
			mAnimInfo = ANIM_INFO(0, 1.5f);
			mAnimState = "GailReady";
			mGailTime = 0.0;
			m_pPlayer->SetMP(-7.f);
		}
	}

	{ // Special Attack
		if (m_pKeyMgr->KeyPressing(DIK_Z) & m_pKeyMgr->KeyPressing(DIK_LSHIFT))
		{
			mAnimInfo = ANIM_INFO(0, 1.f);
			mAnimState = "LockOnTargetReady";
			g_bMainBase = true;
		}
		if (m_pKeyMgr->KeyPressing(DIK_X) & m_pKeyMgr->KeyPressing(DIK_LSHIFT))
		{
			mAnimInfo = ANIM_INFO(-1, 1.f);
			mAnimState = "UpperKick1E";
		}
		if (m_pKeyMgr->KeyPressing(DIK_C) & m_pKeyMgr->KeyPressing(DIK_LSHIFT))
		{
			mAnimInfo = ANIM_INFO(-1, 1.2f);
			mAnimState = "HoldingShot2";
			if (m_bHoldingShot == false)
			{
				m_bHoldingShot = true;
			}
		}
		if (m_pKeyMgr->KeyPressing(DIK_V) & m_pKeyMgr->KeyPressing(DIK_LSHIFT))
		{
			//m_pTrail->SetTextureNum(8);
			mAnimInfo = ANIM_INFO(0, 1.f);
			mAnimState = "FireReady";
		}
		if (m_pKeyMgr->KeyPressing(DIK_SPACE) & m_pKeyMgr->KeyPressing(DIK_LSHIFT))
		{
			mAnimInfo = ANIM_INFO(-1, 1.f);
			mAnimState = "RisingAttack";
		}

	}

	return NOERROR;
}

void PlayerAnim::AnimStateChange(const float timeDelta)
{
	if (mPre_AnimState != mAnimState)
	{
		m_bAttack = false;
		AnimChange();
	}

	if (mAnimState.find("Jump") != string::npos)
	{
		if (mAnimTime >= 0.17)
		{
			if (m_pPos->y <= m_pPlaneY->y)
			{
				mAnimState = "BattleIdle";
				mAnimTime = 0.0;
			}
		}

	}

	if (mAnimTime >= ((m_pAnimCtrl_Info->animPeriod) - mCorrectionTime))
	{
		if (mAnimInfo.attackCount == -1 /*|| mAnimInfo.attackCount == 2*/)
		{
			if (mAnimState.find("Jump") != string::npos)
			{
				if (m_pPos->y <= m_pPlaneY->y)
				{
					mAnimState = "BattleIdle";
				}
			}
			else
			{
				mAnimState = "BattleIdle";
			}
			mAnimTime = 0.0;
		}
		else
		{
			LinkAnimation(mAnimInfo.attackCount);
		}

		m_bAttack = false;
		m_bDamaged = false;
		//mAnimTime = 0.0;
	}
}


void PlayerAnim::AnimUpdate(const float timeDelta)
{
	mAnimTime += mAnimInfo.animSpeed *timeDelta;
	
	if (LockOnTarget_Operation(timeDelta))
	{
		if (mAnimTime <= (m_pAnimCtrl_Info->animPeriod))
		{
			if (m_bTargetOnShot)
			{
				if (FAILED(m_pMesh->PlayAnimation(mAnimInfo.animSpeed * timeDelta)))
				{
					_MSG_BOX("Player PlayAnimation FAILED!");
				}
			}
		}
		else
		{
			m_pAnimCtrl_Info = m_pMesh->Set_AnimaByName("LockOnTargetShot");
		}
	}
	else
	{
		if (FAILED(m_pMesh->PlayAnimation(mAnimInfo.animSpeed * timeDelta)))
		{
			_MSG_BOX("Player PlayAnimation FAILED!");
		}
	}

	if ((mAnimState.find("Gail") != string::npos))
	{
		mGailTime += mAnimInfo.animSpeed *timeDelta;
		//cout << mGailTime << endl;
	}

	// 이동
	Move(timeDelta);

	// 충돌체크
	AttackCollision();
}

void PlayerAnim::AnimChange()
{
	if (m_bDamaged == false)
	{
		m_pAnimCtrl_Info = m_pMesh->Set_AnimaByName(mAnimState.c_str());
	}
	
	if ((mAnimState.find("Ready") != string::npos) ||
		(mAnimState.find("Shot") != string::npos))
	{
		mCorrectionTime = 0.0;
		*m_pAnimCtrl_Info->pDuration = 0.05;
	}
	else if (mAnimState == "LockOnTargetEnd")
	{
		mCorrectionTime = (mAnimInfo.animSpeed * m_pAnimCtrl_Info->animPeriod * 0.5);
		*m_pAnimCtrl_Info->pDuration = 0.25;
	}
	else if (mAnimState == "GailReady")
	{
		mCorrectionTime = (mAnimInfo.animSpeed * m_pAnimCtrl_Info->animPeriod * 0.2);
		*m_pAnimCtrl_Info->pDuration = 0.25;
		
	}
	else if (mAnimState == "GailEnd")
	{
		mCorrectionTime = (mAnimInfo.animSpeed * m_pAnimCtrl_Info->animPeriod * 0.03);
		*m_pAnimCtrl_Info->pDuration = 0.05;
	}
	else if (mAnimState == "FireEnd")
	{
		mCorrectionTime = (mAnimInfo.animSpeed * m_pAnimCtrl_Info->animPeriod * 0.1);
		*m_pAnimCtrl_Info->pDuration = 0.1;
	}
	else if (mAnimState == "QuickStandB")
	{
		mCorrectionTime = (mAnimInfo.animSpeed * m_pAnimCtrl_Info->animPeriod * 0.05);
		*m_pAnimCtrl_Info->pDuration = 0.05;
	}
	else if (mAnimState == "UpperKick1E")
	{
		mCorrectionTime = (mAnimInfo.animSpeed * m_pAnimCtrl_Info->animPeriod * 0.05);
		*m_pAnimCtrl_Info->pDuration = 0.05;
		m_pPlayer->SetMP(-10.f);
	}
	else if (mAnimState == "Damaged")
	{
		m_bDamaged = true;
		++mDamgedCount;
		mAnimInfo = ANIM_INFO(-1, 2.5f);
		m_pPlayer->SetHP(-5.f);
		m_pSoundMgr->PlaySound(L"WitchDamaged2.wav", CHANNEL::CHANNEL_PLAYER);
	}
	else if (mAnimState == "Stun")
	{
		m_bDamaged = true;
		mDamgedCount = 0;
		mAnimInfo = ANIM_INFO(-1, 0.8f);
		m_pPlayer->SetHP(-10.f);
		m_pSoundMgr->PlaySound(L"WitchDamaged2.wav", CHANNEL::CHANNEL_PLAYER);

	}
	else if (mAnimState == "DownA")
	{
		m_bDamaged = true;
		mAnimInfo = ANIM_INFO(2, 1.f);
		mJumpTime = 0.f;
		m_pPlayer->SetHP(-10.f);
		m_pSoundMgr->PlaySound(L"WitchDamaged.wav", CHANNEL::CHANNEL_PLAYER);
		m_pSoundMgr->PlaySound(L"Floor_Hit.wav", CHANNEL::CHANNEL_PLAYER);
		
	}
	else if (mAnimState == "DownC")
	{
		m_bDamaged = true;
	}
	else if (mAnimState == "RisingAttack")
	{
		mCorrectionTime = (mAnimInfo.animSpeed * m_pAnimCtrl_Info->animPeriod * 0.05);
		*m_pAnimCtrl_Info->pDuration = 0.25;
		if (m_pBossDir != nullptr)
		{
			float dirDegree = m_pUtilityMgr->CalDirDegree(*m_pBossDir * -1.f , *m_pDir);
			m_pTransform->SetRotation(D3DXVECTOR3(0.f, dirDegree, 0.f));
		}
		m_bDamaged = true;
	}
	else if (mAnimState == "LevelUp")
	{
		mAnimInfo = ANIM_INFO(-1, 2.f);
		m_bDamaged = true;
		mCorrectionTime = (mAnimInfo.animSpeed * m_pAnimCtrl_Info->animPeriod * 0.02);
		*m_pAnimCtrl_Info->pDuration = 0.25;

		m_pSoundMgr->PlaySound(L"LevelUp.wav", CHANNEL_PLAYER);

	}
	else if (mAnimState == "SpinCrushC")
	{
		mCorrectionTime = (mAnimInfo.animSpeed * m_pAnimCtrl_Info->animPeriod * 0.04);
		*m_pAnimCtrl_Info->pDuration = 0.25;
	}
	else
	{
		mCorrectionTime = (mAnimInfo.animSpeed * m_pAnimCtrl_Info->animPeriod * 0.02);
		*m_pAnimCtrl_Info->pDuration = 0.25;
	}
		
	/*	cout << "AnimState=" << m_pAnimState << endl;
	cout << "AnimPeriod=" << m_pAnimCtrl_Info->animPeriod << endl;
	cout << "Duration=" << *m_pAnimCtrl_Info->pDuration << endl;*/
	mPre_AnimState = mAnimState;
	mAnimTime = 0.0;
}

void PlayerAnim::Move(const float timeDelta)
{
	if (mAnimState == "DashF")
	{
		CalMove(FORWARD_SPEED * 1.5f, mAnimInfo.animSpeed, timeDelta);
	}
	else if (mAnimState == "QuickStandB")
	{
		if (mAnimTime >= 0.2f && mAnimTime <= 0.9f)
		{
			CalMove(FORWARD_SPEED * -1.5f, mAnimInfo.animSpeed, timeDelta);
		}
	}
	else if (mAnimState == "DashR")
	{
		if (mAnimTime >= 0.2f && mAnimTime <= 1.1f)
		{
			CalMove(FORWARD_SPEED * 1.5f, mAnimInfo.animSpeed, timeDelta, true);
		}
	}
	else if (mAnimState == "DashL")
	{
		if (mAnimTime >= 0.2f && mAnimTime <= 1.1f)
		{
			CalMove(FORWARD_SPEED * -1.5f, mAnimInfo.animSpeed, timeDelta, true);
		}
	}
	else if (mAnimState == "JumpFB_In")
	{
		Jump(timeDelta, JUMP_FORWARD);
	}
	else if (mAnimState == "JumpBB_In")
	{
		Jump(timeDelta, JUMP_BACK);
	}
	else if (mAnimState == "JumpRB_In")
	{
		Jump(timeDelta, JUMP_RIGHT);
	}
	else if (mAnimState == "JumpLB_In")
	{
		Jump(timeDelta, JUMP_LEFT);
	}
	else if (mAnimState == "HoldingShot2" || mAnimState == "SpinCrushC")
	{
		if (mAnimTime >= 0.07f && mAnimTime <= 0.2f)
		{
			CalMove(20.f, mAnimInfo.animSpeed, timeDelta);
		}
	}
	else if (mAnimState == "DownA")
	{
		//if(mJumpInfo.jumpState 
		//DamagedJump(timeDelta, JUMP_BACK, 2.5f, 90.f, 75.f);
		DamagedJump(timeDelta, mJumpInfo.jumpState, mJumpInfo.jumpVelocity, mJumpInfo.sinDegree, mJumpInfo.cosDegree, &m_vEnemyRightDir);
	}
}

void PlayerAnim::AttackCollision()
{
	EnemyAI* pEnemyAI = nullptr;

	if ((mAnimState.find("Gail") != string::npos))
	{
		if (mGailTime >= 0.4 && mGailTime <= 3.5)
		{
			if (m_bAttack == false)
			{
				//WitchSpiralSwing // WitchBlare
				m_pSoundMgr->PlaySound(L"WitchViperVoice1.wav", CHANNEL_PLAYER);
				m_bAttack = true;
			}
			m_pDecal->SetDecalInfo(*m_pPos /*+ (*m_pEnemyRightDir * 5.f)*/, D3DXVECTOR3(11.f, 4.f, 11.f), 9, DECAL_PALYER_GAIL, true);
			pEnemyAI = m_pEnemyMgr->Enemy_WeaponCollision(m_pWeaponCollider, m_pWeapon_SphereCollider, PLAYER_GAIL);
		}
	}
	else if (mAnimState == "AttShelTechnic3")
	{
		if (mAnimTime >= 0.5 && mAnimTime <= 0.7)
		{
			m_pSoundMgr->PlaySound(L"WitchSwing1.wav", CHANNEL_PLAYER);
		}
		else if (mAnimTime >= 3.5 && mAnimTime <= 3.7)
		{
			m_pSoundMgr->PlaySound(L"WitchSwing1.wav", CHANNEL_PLAYER);
		}
		else if (mAnimTime >= 5.5 && mAnimTime <= 5.6)
		{
			m_pSoundMgr->PlaySound(L"WitchHit.wav", CHANNEL_PLAYER);
		}
		if (mAnimTime >= 0.5 && mAnimTime <= 6.0)
		{
			m_pDecal->SetDecalInfo(*m_pPos, D3DXVECTOR3(11.f, 4.f, 11.f), 10, DECAL_PALYER_ATTSHEL, true, -2.f);
			pEnemyAI = m_pEnemyMgr->Enemy_WeaponCollision(m_pWeaponCollider, m_pWeapon_SphereCollider, PLAYER_SHELTECHNIC, mAnimTime);
			if (m_bAttack == false)
			{


				//m_pSoundMgr->PlaySound(L"WitchFledge.wav", CHANNEL_PLAYER);
				m_bAttack = true;
			}
		}
	}
	else if (mAnimState == "SpinCrushC")
	{
		m_pDecal->SetDecalInfo(*m_pPos /*+ (*m_pEnemyRightDir * 5.f)*/, D3DXVECTOR3(11.f, 4.f, 11.f), 13, DECAL_PLAYER_SPIN_CRUSH_ATTACK, true);
		
		if (mAnimTime >= 0.5 && mAnimTime <= 0.53)
		{
			if (m_bAttack == false)
			{
				m_pSoundMgr->PlaySound(L"WitchSpinTurnA.wav", CHANNEL_PLAYER);
				pEnemyAI = m_pEnemyMgr->Enemy_WeaponCollision(m_pWeaponCollider, m_pWeapon_SphereCollider, PLAYER_SPINCRUSH);
				m_bAttack = true;
			}
			
		}
	}
	else if (mAnimState == "UpperKick1E")
	{
		m_pDecal->SetDecalInfo(*m_pPos /*+ (*m_pEnemyRightDir * 5.f)*/, D3DXVECTOR3(9.f, 3.f, 9.f), 14, DECAL_PLAYER_UPPERKICK, true, -1.f);

		if (mAnimTime >= 0.3 && mAnimTime <= 0.33)
		{
			if (m_bAttack == false)
			{
				m_pSoundMgr->PlaySound(L"WitchTricky.wav", CHANNEL_PLAYER);
				pEnemyAI = m_pEnemyMgr->Enemy_WeaponCollision(m_pWeaponCollider, m_pWeapon_SphereCollider, PLAYER_SPINCRUSH);
				m_bAttack = true;
			}
		}
	}
	else if (mAnimState == "StunAttack")
	{
		m_pDecal->SetDecalInfo(*m_pPos, D3DXVECTOR3(9.f, 3.f, 9.f), 15, DECAL_PLAYER_STUN_ATTACK, true);
		if (mAnimTime >= 0.8 && mAnimTime <= 1.0)
		{
			if (m_bAttack == false)
			{
				m_pSoundMgr->PlaySound(L"WitchHit.wav", CHANNEL_PLAYER);
				pEnemyAI = m_pEnemyMgr->Enemy_WeaponCollision(m_pWeaponCollider, m_pWeapon_SphereCollider, PLAYER_JUMPATTACK);
				m_bAttack = true;
			}
		}
	}
	else if (mAnimState == "RisingAttack")
	{
		m_pDecal->SetDecalInfo(*m_pPos, D3DXVECTOR3(11.f, 4.f, 11.f), 12, DECAL_PALYER_RISING_ATTACK, true);
		if (mAnimTime >= 0.8)
		{
			if (m_bAttack == false)
			{
				m_pSoundMgr->PlaySound(L"WitchFledgeVoice2.wav", CHANNEL_PLAYER);
				pEnemyAI = m_pEnemyMgr->Enemy_WeaponCollision(m_pWeaponCollider, m_pWeapon_SphereCollider, PLAYER_RISINGATTACK);
				m_bAttack = true;
			}
		}


	}
	else if (mAnimState == "HoldingShot2")
	{
		m_pDecal->SetDecalInfo(*m_pPos, D3DXVECTOR3(9.f, 3.f, 9.f), 16, DECAL_PALYER_HOLDING_SHOT, true, -1.f);
		if (m_bHoldingShot == true)
		{
			if (mAnimTime >= 1.3)
			{
				m_pSoundMgr->PlaySound(L"WitchDarkPollingWind2.wav", CHANNEL_PLAYER);
				m_pBulletMgr->AddBullet(L"Prototype_GameObject_Bullet", BULLET_BOOM);
				m_bHoldingShot = false;
			}
		}
	}
	else if (mAnimState == "LockOnTargetShot")
	{
		if (m_bTargetOnShot_Bulelt == true)
		{
			if (mAnimTime >= 0.24)
			{
				m_pSoundMgr->PlaySound(L"WitchSpinTurnB.wav", CHANNEL_PLAYER);
				m_pBulletMgr->AddBullet(L"Prototype_GameObject_Bullet", BULLET_SNIPER);
				m_bTargetOnShot_Bulelt = false;
			}
		}
	}
}



PlayerAnim * PlayerAnim::Create(PDIRECT3DDEVICE9 pDevice)
{
	PlayerAnim* pInstance = new PlayerAnim(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("PlayerAnim Created Failed!!!");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::Component * PlayerAnim::Clone()
{
	return new PlayerAnim(*this);
}

DWORD PlayerAnim::Free()
{
	PlayerCommand::Free();
	return 0;
}
