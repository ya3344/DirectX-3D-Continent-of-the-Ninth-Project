#include "stdafx.h"
#include "EnemyMgr.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "Enemy.h"
#include "EnemyAi.h"
#include "LightManager.h"
#include "Player.h"

_USING(Client)

_IMPLEMENT_SINGLETON(EnemyMgr);

EnemyMgr::EnemyMgr()
{
	m_pLightMgr = Engine::LightManager::GetInstance();
	m_pSoundMgr = SoundMgr::GetInstance();
}

HRESULT EnemyMgr::CrateEnemy(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pLayerTag)
{
	Engine::GameObject* pGameObject = nullptr;
	Engine::Component* pComponent = nullptr;
	Engine::Collider* pEnemyCollider = nullptr;
	EnemyAI* pEnmeyAI = nullptr;

	// For.Component_AI
	pComponent = EnemyAI::Create(pDevice);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SOULVALLEY_ENEMY, L"Component_AI_Enemy", pComponent)))
		return E_FAIL;

	size_t enemyTotalNum = A_SECTOR_NUM + B_SECTOR_ACCESS_NUM + B_SECTOR_NUM + C_SECTOR_NUM + C_SECTOR_AFTER_NUM + DOOR_SECTOR_NUM + BOSS_SECTOR_NUM + 1;

	mProtoName_Space.reserve(enemyTotalNum);

	// Player
	m_pPlayer = (Player *)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_PLAYER, L"Layer_Player", L"Prototype_GameObject_Player");

	// For.Dissolve Textrue Load 
	if (FAILED(D3DXCreateTextureFromFile(pDevice, L"../Bin/Resources/Textures/etc/Dissolve1.png", &m_pDissolveTex)))
		return E_FAIL;

	for (size_t i = 0; i < enemyTotalNum; i++)
	{
		mt19937 mtRand((size_t)(rand()));

		if (i < A_SECTOR_NUM)
		{
			// ACave Navi = 45 ~ 55 BaegazaWarrior = 3, BaegazaAxe = 2
			EnemyGeneration(mtRand, ENEMY_TYPE_COUNT(3, 2), 45, 55);
			mSpecialIndex = LIGHT_A_STATE;
			mNaviMinIndex = 42;
			mNaviMaxIndex = 55;
		}
		else if (i < A_SECTOR_NUM + B_SECTOR_ACCESS_NUM)
		{
			EnemyGeneration(mtRand, ENEMY_TYPE_COUNT(0, 2, 2), 150, 166);
			mSpecialIndex = LIGHT_NORMAL_STATE;
			mNaviMinIndex = 150;
			mNaviMaxIndex = 165;
		}
		else if (i < A_SECTOR_NUM + B_SECTOR_ACCESS_NUM + B_SECTOR_NUM)
		{
			EnemyGeneration(mtRand, ENEMY_TYPE_COUNT(1, 1, 3), 217, 222);
			mSpecialIndex = LIGHT_B_STATE;
			mNaviMinIndex = 215;
			mNaviMaxIndex = 224;
		}
		else if (i < A_SECTOR_NUM + B_SECTOR_ACCESS_NUM + B_SECTOR_NUM + C_SECTOR_NUM)
		{
			EnemyGeneration(mtRand, ENEMY_TYPE_COUNT(0, 0, 0, 2, 2), 203, 210);
			mSpecialIndex = LIGHT_C_STATE;
			mNaviMinIndex = 199;
			mNaviMaxIndex = 210;
		}
		else if (i < A_SECTOR_NUM + B_SECTOR_ACCESS_NUM + B_SECTOR_NUM + C_SECTOR_NUM + C_SECTOR_AFTER_NUM)
		{
			EnemyGeneration(mtRand, ENEMY_TYPE_COUNT(0, 0, 1, 1, 1), 98, 107);
			mSpecialIndex = LIGHT_NORMAL_STATE;
			mNaviMinIndex = 98;
			mNaviMaxIndex = 106;
		}
		else if (i < A_SECTOR_NUM + B_SECTOR_ACCESS_NUM + B_SECTOR_NUM + C_SECTOR_NUM + C_SECTOR_AFTER_NUM + DOOR_SECTOR_NUM)
		{
			EnemyGeneration(mtRand, ENEMY_TYPE_COUNT(7, 6, 2, 0, 0), 70, 80);
			mSpecialIndex = LIGHT_NORMAL_STATE;
			mNaviMinIndex = 70;
			mNaviMaxIndex = 80;
		}
		else if (i < A_SECTOR_NUM + B_SECTOR_ACCESS_NUM + B_SECTOR_NUM + C_SECTOR_NUM + C_SECTOR_AFTER_NUM + DOOR_SECTOR_NUM + BOSS_SECTOR_NUM)
		{
			EnemyGeneration(mtRand, ENEMY_TYPE_COUNT(2, 3, 0, 3, 2), 253, 289);
			mSpecialIndex = LIGHT_NORMAL_STATE;
			mNaviMinIndex = 253;
			mNaviMaxIndex = 284;
		}
		else
		{
			mEnemyType = HUMAN_BOSS;
			mNaviIndex = 390;
			mSpecialIndex = LIGHT_BOSS_STATE;
			mNaviMinIndex = 353;
			mNaviMaxIndex = 390;

		}

		
		switch (mEnemyType)
		{
		case BABEGAZI_WARRIOR:
			wsprintf(mComponentName_Buf, L"Component_Mesh_Babegazi_Warrior", i);
			wsprintf(mProtoName_Buf, L"Prototype_GameObject_Babegazi_Warrior%d", i);
			break;
		case BABEGAZI_AXE:
			wsprintf(mComponentName_Buf, L"Component_Mesh_Babegazi_Axe", i);
			wsprintf(mProtoName_Buf, L"Prototype_GameObject_Babegazi_Axe%d", i);
			break;
		case BABEGAZI_BOW:
			wsprintf(mComponentName_Buf, L"Component_Mesh_Babegazi_Bow", i);
			wsprintf(mProtoName_Buf, L"Prototype_GameObject_Babegazi_Bow%d", i);
			break;
		case KNOLE_COMMANDER:
			wsprintf(mComponentName_Buf, L"Component_Mesh_Knole_Commander", i);
			wsprintf(mProtoName_Buf, L"Prototype_GameObject_Knole_Commander%d", i);
			break;
		case KNOLE_WARRIOR:
			wsprintf(mComponentName_Buf, L"Component_Mesh_Knole_Warrior", i);
			wsprintf(mProtoName_Buf, L"Prototype_GameObject_Knole_Warrior%d", i);
			break;
		case HUMAN_BOSS:
			wsprintf(mComponentName_Buf, L"Component_Mesh_HumanBoss", i);
			wsprintf(mProtoName_Buf, L"Prototype_GameObject_HumanBoss", i);
			break;
		default:
			assert(false && "CrateEnemy");
			break;
		}
		
		mProtoName_Space.emplace_back(mProtoName_Buf);

		pGameObject = Enemy::Create(pDevice, mComponentName_Buf, mNaviIndex, mEnemyType );
		if (pGameObject == nullptr)
			return E_FAIL;
		
		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(mProtoName_Space[i].c_str(), SCENE_SOULVALLEY_ENEMY, pGameObject)))
			return E_FAIL;

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_SOULVALLEY_ENEMY, mProtoName_Space[i].c_str(), SCENE_SOULVALLEY_ENEMY, pLayerTag)))
			return E_FAIL;

		pEnmeyAI = (EnemyAI*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_SOULVALLEY_ENEMY, pLayerTag, L"Component_AI_Enemy", mProtoName_Space[i].c_str());
		pEnmeyAI->SetSpecialIndex(mSpecialIndex);
		pEnmeyAI->SetNaviMinIndex(mNaviMinIndex);
		pEnmeyAI->SetNaviMaxIndex(mNaviMaxIndex);

		if (pEnmeyAI != nullptr)
		{
			mEnemyAI_Space.emplace_back(pEnmeyAI);
		}
	}
	return NOERROR;
}

void EnemyMgr::Enemy_AttackCount()
{
	size_t attackCount = 0;

	for (EnemyAI* pEnemyAI : mEnemyAI_Space)
	{
		if (pEnemyAI->Get_EnemyType() == BABEGAZI_BOW)
			continue;

		if (pEnemyAI->Get_IsAttack())
		{
			++attackCount;
		}
	}

	mAttackCount = attackCount;
	//cout << "AttackCount= " << mAttackCount << endl;
}

void EnemyMgr::EnemyDelete()
{
	auto iterBegin = mEnemyAI_Space.begin();
	auto iterEnd = mEnemyAI_Space.end();
	EnemyAI* pEnemyAI = nullptr;

	while(iterBegin != iterEnd)
	{
		pEnemyAI = *iterBegin;
		if (pEnemyAI->Get_EnemyAIMode() == AI_DIE)
		{
			switch (pEnemyAI->GetSpecialIndex())
			{
			case LIGHT_A_STATE:
				EnemySpecialIndex(LIGHT_A_STATE);
				break;
			case LIGHT_B_STATE:
				EnemySpecialIndex(LIGHT_B_STATE);
				break;
			case LIGHT_C_STATE:
				EnemySpecialIndex(LIGHT_C_STATE);
				break;
			default:
				break;
			}

			iterBegin = mEnemyAI_Space.erase(iterBegin);
			return;
		}
		else
		{
			++iterBegin;
		}
	}
}

bool EnemyMgr::Enemy_CollisionEX(Engine::Collider* pTargetCollider, D3DXVECTOR3* pPush)
{
	for (EnemyAI* pEnemyAI : mEnemyAI_Space)
	{
		if (pEnemyAI->Get_IsAttack())
			continue;

		if (pEnemyAI->GetCollider() == pTargetCollider)
			continue;

		if (pEnemyAI->GetCollider()->Collision_EX(pTargetCollider, pPush))
		{
			return true;
		}
	}


	return false;
}

EnemyAI* EnemyMgr::Enemy_WeaponCollision(Engine::Collider * pWeaponCollider, Engine::Collider* pWeapon_SphereCollider, int playerSkill, double animTime)
{
	for (EnemyAI* pEnemyAI : mEnemyAI_Space)
	{
		if (pWeapon_SphereCollider->Collision_Sphere(pEnemyAI->Get_SphereCollider()))
		{
			// 맞는 확률 및 강제공격하는 변수
			if (pWeaponCollider->Collision_OBB(pEnemyAI->GetCollider()))
			{
				if (pEnemyAI != nullptr)
				{
					Set_EnemyAIMode(playerSkill, pEnemyAI, animTime);
					/*if (pEnemyAI->Get_ForceAttack() == false)
					{
						pEnemyAI->Set_AIMode(mEnemy_AIMode);
					}*/

					if (pEnemyAI->Get_IsForceAttack() == false)
					{
						Set_EnemyAIMode(playerSkill, pEnemyAI, animTime);
						pEnemyAI->Set_AIMode(mEnemy_AIMode);
					}
					else if (playerSkill == PLAYER_SPINCRUSH || playerSkill == PLAYER_JUMPATTACK || playerSkill == PLAYER_SHELTECHNIC)
					{
						Set_EnemyAIMode(playerSkill, pEnemyAI, animTime);
						pEnemyAI->Set_AIMode(mEnemy_AIMode);

						if (pEnemyAI->Get_IsForceAttack())
						{
							pEnemyAI->Set_ForceAttack(false);
						}
					}

				}
				else
				{
					_MSG_BOX("Enemy_WeaponCollision EnemyAI nullptr!!");
					return nullptr;
				}

			}
		}
	}

	return nullptr;
}

bool EnemyMgr::Enemy_CollisionAABB(Engine::Collider * pTargetCollider, D3DXVECTOR3* pRightDir)
{
	for (EnemyAI* pEnemyAI : mEnemyAI_Space)
	{
		if (pEnemyAI->GetCollider() == pTargetCollider)
			continue;

		if (pEnemyAI->GetCollider()->Collision_AABB(pTargetCollider, pRightDir))
			return true;
	}


	return false;
}

void EnemyMgr::SetUp_DissolveTextrue(LPD3DXEFFECT pEffect, const char * pNameTag)
{
	pEffect->SetTexture(pNameTag, m_pDissolveTex);
}

void EnemyMgr::Set_EnemyAIMode(const int playerSkill, EnemyAI* pEnemyAI, double animTime)
{
	mt19937 mtRand((size_t)(rand()));
	uniform_int_distribution<size_t> range(1, 10);

	mSkillPercent = range(mtRand);

	switch (playerSkill)
	{
	case PLAYER_GAIL:
		PlayerGrill_SkillOperation(pEnemyAI);
		break;
	case PLAYER_SHELTECHNIC:
		PlayerShelTechnic_SkillOperation(pEnemyAI, animTime);
		break;
	case PLAYER_SPINCRUSH:
		PlayerSpinCursh(pEnemyAI);
		break;
	case PLAYER_JUMPATTACK:
		PlayerJumpAttack(pEnemyAI);
		break;
	case PLAYER_RISINGATTACK:
		PlayerRisingAttack(pEnemyAI);
		break;
	default:
		assert(false && "Enemy_WeaponCollision playerSkill!!");
		break;
	}


	if (pEnemyAI->Get_IsAttack())
	{
		pEnemyAI->SetAttack(false);
	}

	if (pEnemyAI->GetPreEnemy_AIMode() == AI_STUN)
	{
		pEnemyAI->SetDamaged(false);
	}

}

void EnemyMgr::PlayerGrill_SkillOperation(EnemyAI* pEnemyAI)
{
	switch (pEnemyAI->Get_EnemyType())
	{
	case BABEGAZI_WARRIOR:
		if (mSkillPercent <= 7)
		{
			mEnemy_AIMode = AI_DAMAGED;
		}
		else
		{
			mEnemy_AIMode = AI_DEFENCE;
		}
		break;
	case BABEGAZI_AXE:
	case BABEGAZI_BOW:
		if (mSkillPercent <= 9)
		{
			mEnemy_AIMode = AI_DAMAGED;
		}
		else
		{
			mEnemy_AIMode = AI_DESH_BACK;
		}
		break;
	case KNOLE_COMMANDER:
	case KNOLE_WARRIOR:
		if (mSkillPercent <= 2)
		{
			mEnemy_AIMode = AI_DAMAGED_LEFT;
		}
		else if (mSkillPercent > 2 && mSkillPercent <= 4)
		{
			mEnemy_AIMode = AI_DAMAGED_RIGHT;
		}
		else if (mSkillPercent <= 7)
		{
			mEnemy_AIMode = AI_DAMAGED;
		}
		else
		{
			mEnemy_AIMode = AI_DEFENCE;
		}
		break;
	case HUMAN_BOSS:
		if (mSkillPercent <= 3)
		{
			mEnemy_AIMode = AI_DAMAGED_LEFT;
		}
		else
		{
			mEnemy_AIMode = AI_IDLE;
		}
		break;
	default:
		assert(false && "EnemyMgr PlayerSpinCursh!!");
		break;
	}
}

void EnemyMgr::PlayerShelTechnic_SkillOperation(EnemyAI * pEnemyAI, double animTime)
{
	switch (pEnemyAI->Get_EnemyType())
	{
	case BABEGAZI_WARRIOR:
		if (animTime <= 4.5f)
		{
			if (mSkillPercent <= 4)
			{
				mEnemy_AIMode = AI_DAMAGED_LEFT;
			}
			else if (mSkillPercent <= 8)
			{
				mEnemy_AIMode = AI_DAMAGED_RIGHT;
			}
			else
			{
				mEnemy_AIMode = AI_DEFENCE;
			}
		}
		else 
		{
			mEnemy_AIMode = AI_DAMAGE_SWO;
			//pEnemyAI->SetDamaged(false);
			//Init_EnemyState(pEnemyAI);
		}
		break;
	case BABEGAZI_AXE:
		if (animTime <= 4.5f)
		{
			if (mSkillPercent <= 4)
			{
				mEnemy_AIMode = AI_DAMAGED_LEFT;
			}
			else if (mSkillPercent <= 9)
			{
				mEnemy_AIMode = AI_DAMAGED_RIGHT;
			}
			else
			{
				mEnemy_AIMode = AI_DESH_BACK;
			}
		}
		else
		{
			mEnemy_AIMode = AI_DAMAGE_SWO;
			//pEnemyAI->SetDamaged(false);
			//Init_EnemyState(pEnemyAI);
		}

		//if (animTime <= 4.0)
		//{
		//	if (mSkillPercent <= 2)
		//	{
		//		mEnemy_AIMode = AI_DAMAGED_LEFT;
		//	}
		//	else if (mSkillPercent <= 5)
		//	{
		//		mEnemy_AIMode = AI_DAMAGED_RIGHT;
		//	}
		//	else
		//	{
		//		mEnemy_AIMode = AI_DESH_BACK;
		//	}
		//}
		//else if (animTime > 4.0 && animTime <= 5.0)
		//{
		//	mEnemy_AIMode = AI_DAMAGE_SWO;
		//	//pEnemyAI->SetDamaged(false);
		//	//Init_EnemyState(pEnemyAI);
		//}
		break;
	case BABEGAZI_BOW :
		if (animTime <= 5.5)
		{
			if (mSkillPercent <= 4)
			{
				mEnemy_AIMode = AI_DAMAGED_LEFT;
			}
			else if (mSkillPercent > 4 && mSkillPercent <= 9)
			{
				mEnemy_AIMode = AI_DAMAGED_RIGHT;
			}
			else
			{
				mEnemy_AIMode = AI_DESH_BACK;
			}
		}
		else
		{
			mEnemy_AIMode = AI_STAND_DOWN;
			//pEnemyAI->SetDamaged(false);
			
			//Init_EnemyState(pEnemyAI);
		}
		break;
	
	case KNOLE_WARRIOR: // 5
		if (animTime <= 5.5f)
		{
			if (mSkillPercent <= 3)
			{
				mEnemy_AIMode = AI_DAMAGED_LEFT;
			}
			else if (mSkillPercent <= 6)
			{
				mEnemy_AIMode = AI_DEFENCE;
			}
			else
			{
				mEnemy_AIMode = AI_DAMAGED_RIGHT;
			}
		}
		else 
		{
			mEnemy_AIMode = AI_DAM_UFA;
			//pEnemyAI->SetDamaged(false);
		}
		break;

	case KNOLE_COMMANDER:
		if (animTime <= 5.5)
		{
			if (mSkillPercent <= 3)
			{
				mEnemy_AIMode = AI_DAMAGED_LEFT;
			}
			else if (mSkillPercent <= 6)
			{
				mEnemy_AIMode = AI_DEFENCE;
			}
			else
			{
				mEnemy_AIMode = AI_DAMAGED_RIGHT;
			}
		}
		else
		{
			mEnemy_AIMode = AI_DAM_UFA;
			//pEnemyAI->SetDamaged(false);
			//Init_EnemyState(pEnemyAI);
		}
		break;
	case HUMAN_BOSS:
		if (animTime <= 5.5f)
		{
			if (mSkillPercent <= 5)
			{
				mEnemy_AIMode = AI_DAMAGED_LEFT;
			}
		}
		else
		{
			if (mSkillPercent <= 3)
			{
				mEnemy_AIMode = AI_ROUND_PUNCH;
			}
			else
			{
				mEnemy_AIMode = AI_DAMAGED_LEFT;
			}
			//cout << mSkillPercent << endl;
			//pEnemyAI->SetDamaged(false);
			//Init_EnemyState(pEnemyAI);
		}
		break;
	default:
		assert(false && "EnemyMgr PlayerShelTechnic_SkillOperation!!");
		break;
	}
}

void EnemyMgr::PlayerSpinCursh(EnemyAI * pEnemyAI)
{
	switch (pEnemyAI->Get_EnemyType())
	{
	case BABEGAZI_WARRIOR:
		mEnemy_AIMode = AI_SPIN_A;
		m_pSoundMgr->PlaySound(L"F_Hit0.wav", CHANNEL_BABE_WARRIOR);
		break;
	case BABEGAZI_BOW:
		mEnemy_AIMode = AI_SPIN_A;
		m_pSoundMgr->PlaySound(L"F_Hit0.wav", CHANNEL_BABE_BOW);
		break;
	case BABEGAZI_AXE:
		mEnemy_AIMode = AI_SPIN_A;
		m_pSoundMgr->PlaySound(L"F_Hit0.wav", CHANNEL_BABE_AXE);
		break;
	case KNOLE_COMMANDER:
	case KNOLE_WARRIOR:
		if (mSkillPercent <= 7)
		{
			mEnemy_AIMode = AI_SPIN_A;
			m_pSoundMgr->PlaySound(L"F_Hit0.wav", CHANNEL_KNOLE_COMMANDER);
		}
		else
		{
			mEnemy_AIMode = AI_DEFENCE;
		}
		//Init_EnemyState(pEnemyAI);
		break;
	case HUMAN_BOSS:
		//cout << "SkillPercent=" <<  mSkillPercent << endl;
		if (mSkillPercent <= 5)
		{
			mEnemy_AIMode = AI_BOSS_STAND_DOWN;
		}
		else
		{
			mEnemy_AIMode = AI_DAMAGED_LEFT;
		}
		break;
	default:
		assert(false && "EnemyMgr PlayerSpinCursh!!");
		break;
	}
}

void EnemyMgr::PlayerJumpAttack(EnemyAI * pEnemyAI)
{
	switch (pEnemyAI->Get_EnemyType())
	{
	case BABEGAZI_WARRIOR:
	case BABEGAZI_AXE:
	case BABEGAZI_BOW:
		if (mSkillPercent <= 7)
		{
			mEnemy_AIMode = AI_STUN;
		}
		else
		{
			mEnemy_AIMode = AI_DESH_BACK;
		}
		break;
	case KNOLE_COMMANDER:
	case KNOLE_WARRIOR:
		if (mSkillPercent <= 7)
		{
			mEnemy_AIMode = AI_STUN;
		}
		else
		{
			mEnemy_AIMode = AI_REATTACK;
		}
		//Init_EnemyState(pEnemyAI);
		break;
	case HUMAN_BOSS:
		if (mSkillPercent <= 5)
		{
			mEnemy_AIMode = AI_STUN;
		}
		else
		{
			mEnemy_AIMode = AI_DAMAGED_LEFT;
		}
		break;
	default:
		assert(false && "EnemyMgr PlayerSpinCursh!!");
		break;
	}
}

void EnemyMgr::PlayerRisingAttack(EnemyAI * pEnemyAI)
{
	switch (pEnemyAI->Get_EnemyType())
	{
	case BABEGAZI_WARRIOR:
	case BABEGAZI_AXE:
	case BABEGAZI_BOW:
		mEnemy_AIMode = AI_STUN;
		break;
	case KNOLE_COMMANDER:
	case KNOLE_WARRIOR:
		if (mSkillPercent <= 5)
		{
			mEnemy_AIMode = AI_DAMAGED_LEFT;
		}
		else
		{
			mEnemy_AIMode = AI_STUN;
		}
		//Init_EnemyState(pEnemyAI);
		break;
	case HUMAN_BOSS:
		if (mSkillPercent <= 5)
		{
			mEnemy_AIMode = AI_DAMAGED_LEFT;
		}
		else
		{
			mEnemy_AIMode = AI_STUN;
		}
		break;
	default:
		assert(false && "EnemyMgr PlayerSpinCursh!!");
		break;
	}
}

void EnemyMgr::Init_EnemyState(EnemyAI* pEnemyAI)
{
	//pEnemyAI->SetDamaged(false);
	pEnemyAI->Set_ForceAttack(false);
}

void EnemyMgr::EnemyGeneration(mt19937 mtRand, const ENEMY_TYPE_COUNT enemyTypeCount, const size_t naviMin, const size_t naviMax)
{

	uniform_int_distribution<size_t> naviIndex(naviMin, naviMax);

	if (mEnemyTypeCount.babegaziWarrior < enemyTypeCount.babegaziWarrior)
	{
		mEnemyType = BABEGAZI_WARRIOR;
		++mEnemyTypeCount.babegaziWarrior;
	}
	else if(mEnemyTypeCount.babegaziAxe < enemyTypeCount.babegaziAxe)
	{
		mEnemyType = BABEGAZI_AXE;
		++mEnemyTypeCount.babegaziAxe;
	}
	else if (mEnemyTypeCount.babegaziBow < enemyTypeCount.babegaziBow)
	{
		mEnemyType = BABEGAZI_BOW;
		++mEnemyTypeCount.babegaziBow;
	}
	else if (mEnemyTypeCount.knoleCommander < enemyTypeCount.knoleCommander)
	{
		mEnemyType = KNOLE_COMMANDER;
		++mEnemyTypeCount.knoleCommander;
	}
	else if (mEnemyTypeCount.knoleWarrior < enemyTypeCount.knoleWarrior)
	{
		mEnemyType = KNOLE_WARRIOR;
		++mEnemyTypeCount.knoleWarrior;
	}

	if (mEnemyTypeCount.babegaziWarrior == enemyTypeCount.babegaziWarrior
		&& mEnemyTypeCount.babegaziAxe == enemyTypeCount.babegaziAxe
		&& mEnemyTypeCount.babegaziBow == enemyTypeCount.babegaziBow
		&& mEnemyTypeCount.knoleCommander == enemyTypeCount.knoleCommander
		&& mEnemyTypeCount.knoleWarrior == enemyTypeCount.knoleWarrior)
	{
		ZeroMemory(&mEnemyTypeCount, sizeof(mEnemyTypeCount));
	}

	mNaviIndex = naviIndex(mtRand);
}

void EnemyMgr::EnemySpecialIndex(const size_t specialIndex)
{
	switch (specialIndex)
	{
	case LIGHT_A_STATE:
		++mSpecialCount.mAState;
		if (mSpecialCount.mAState == A_SECTOR_NUM - 2)
		{
			m_pLightMgr->ChangeLightColor(LIGHT_A_STATE);
		}
		break;
	case LIGHT_B_STATE:
		++mSpecialCount.mBState;
		if (mSpecialCount.mBState == B_SECTOR_NUM - 2)
		{
			m_pLightMgr->ChangeLightColor(LIGHT_B_STATE);
		}
		break;
	case LIGHT_C_STATE:
		++mSpecialCount.mCState;
		if (mSpecialCount.mCState == C_SECTOR_NUM - 2)
		{
			m_pLightMgr->ChangeLightColor(LIGHT_C_STATE);
		}
		break;
	default:
		break;
	}
}



DWORD EnemyMgr::Free()
{
	mEnemyAI_Space.clear();
	mProtoName_Space.clear();
	Engine::Safe_Release(m_pDissolveTex);
	return 0;
}


