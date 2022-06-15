#include "stdafx.h"
#include "EnemyAI.h"
#include "ComponentMgr.h"
#include "MoveState.h"
#include "AttackState.h"
#include "DamagedState.h"
#include "Trail.h"
#include "ObjectMgr.h"

_USING(Client)

EnemyAI::EnemyAI(LPDIRECT3DDEVICE9 pDevice)
	:Component(pDevice)
	, m_pEnemyMgr(EnemyMgr::GetInstance())
	, m_pUtilityMgr(UtilityMgr::GetInstance())
{
	//m_pEnemyMgr->AddRef();
	m_pUtilityMgr->AddRef();
}

EnemyAI::EnemyAI(const EnemyAI & rhs)
	:Component(rhs)
	,m_pEnemyMgr(EnemyMgr::GetInstance())
	,m_pUtilityMgr(UtilityMgr::GetInstance())
{
	//m_pEnemyMgr->AddRef();
	m_pUtilityMgr->AddRef();
}

HRESULT EnemyAI::Init()
{
	return NOERROR;
}

HRESULT EnemyAI::SetInit(const size_t enemyType, Engine::DynamicMesh * pMesh, Engine::Transform * pTransform, Engine::Transform * pPlayerTransform, Engine::Navigation * pNavigation)
{	
	m_pMoveState = MoveState::Create();
	m_pAttackState = AttackState::Create();
	m_pDamagedState = DamagedState::Create();

	m_pEnemyState = m_pMoveState;
	
	if (m_pEnemyState == nullptr)
	{
		_MSG_BOX("EnemyAI SetInit Failed!");
		Engine::Safe_Release(m_pMoveState);
		return E_FAIL;
	}

	m_pTransform = pTransform;
	m_pTransform->AddRef();

	m_pPlayerTransform = pPlayerTransform;
	m_pPlayerTransform->AddRef();

	mEnemy_AIMode = AI_IDLE;
	mEnemyType = enemyType;

	m_pMoveState->SetInit(ENEMY_AI_INFO(mEnemyType, &m_bForceAttack, &m_bAttack, &m_bDamaged, &mEnemy_AIMode, &mHP, &m_bDie, &mNaviMinIndex, &mNaviMaxIndex, &mBossHP_Count),
		pMesh, pTransform, pPlayerTransform, pNavigation);
	m_pAttackState->SetInit(ENEMY_AI_INFO(mEnemyType, &m_bForceAttack, &m_bAttack, &m_bDamaged, &mEnemy_AIMode, &mHP, &m_bDie, &mNaviMinIndex, &mNaviMaxIndex, &mBossHP_Count),
		pMesh, pTransform, pPlayerTransform, pNavigation);
	m_pDamagedState->SetInit(ENEMY_AI_INFO(mEnemyType, &m_bForceAttack, &m_bAttack, &m_bDamaged, &mEnemy_AIMode, &mHP, &m_bDie, &mNaviMinIndex, &mNaviMaxIndex, &mBossHP_Count),
		pMesh, pTransform, pPlayerTransform, pNavigation);

	m_pEnemyPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);
	m_pPlayerPos = m_pPlayerTransform->GetInfo(Engine::Transform::INFO_POSITION);
	m_pTransform->SetUpdate_CenterPos(true);

	mBossHP_Count = 3;

	return NOERROR;
}

HRESULT EnemyAI::SetCollider(Engine::Collider * pCollider, Engine::Collider* pSphereCollider, Engine::Collider * pPlayerCollider, Engine::Collider* pWeaponCollider)
{
	m_pCollider = pCollider;
	if (m_pCollider == nullptr)
	{
		_MSG_BOX("EnemyAI m_pCollider Failed!");
		return E_FAIL;
	}
	m_pCollider->AddRef();

	m_pSphereCollider = pSphereCollider;
	if (m_pSphereCollider == nullptr)
	{
		_MSG_BOX("EnemyAI m_pSphereCollider Failed!");
		return E_FAIL;
	}
	m_pSphereCollider->AddRef();

	m_pPlayerCollider = pPlayerCollider;
	if (m_pPlayerCollider == nullptr)
	{
		_MSG_BOX("EnemyAI m_pPlayerCollider Failed!");
		return E_FAIL;
	}
	m_pPlayerCollider->AddRef();

	m_pMoveState->SetCollider(m_pCollider, m_pPlayerCollider, pWeaponCollider);
	m_pAttackState->SetCollider(m_pCollider, m_pPlayerCollider, pWeaponCollider);
	m_pDamagedState->SetCollider(m_pCollider, m_pPlayerCollider, pWeaponCollider);

	return NOERROR;
}

HRESULT EnemyAI::AIState_Update(const float timeDelta)
{

	if (Engine::KeyMgr::GetInstance()->KeyDown(DIK_7))
	{
		if (mEnemyType == HUMAN_BOSS)
		{
			mEnemy_AIMode = AI_IDLE;
			m_bAttack = false;
		}
	}

	if (m_pEnemyState->Get_PreAIMdoe() == AI_DIE)
	{
		mEnemy_AIMode = AI_DIE;
		m_pEnemyState = m_pDamagedState;
		m_pEnemyMgr->EnemyDelete();
		return NOERROR;
	}
	//if (mHP <= 0)
	//{
	//	m_pEnemyState = m_pDamagedState;
	//	return NOERROR;
	//}
	mDistance = m_pUtilityMgr->Cal_TargetDistance(*m_pEnemyPos, *m_pPlayerPos);
	//cout << "°Å¸®=" << mDistance << endl;
	switch (mEnemyType)
	{
	case BABEGAZI_BOW:
		if (mEnemy_AIMode >= AI_DAMAGED)
		{
			if (m_bForceAttack == false && m_bDamaged == false && m_bAttack == false)
			{
				if (mEnemy_AIMode != mPre_EnemyAIMode)
				{
					if (mEnemy_AIMode != AI_DAMAGED && mEnemy_AIMode != AI_DAMAGED_LEFT && mEnemy_AIMode != AI_DAMAGED_RIGHT && mEnemy_AIMode != AI_STUN&& mEnemy_AIMode != AI_STAND_DOWN)
					{
						m_bDamaged = true;
					}
					m_pEnemyState = m_pDamagedState;
					m_pEnemyState->PreAISet(mEnemy_AIMode);
					m_pPreEnemyState = m_pEnemyState;
					mPre_EnemyAIMode = mEnemy_AIMode;
				}
			}
		}
		else if (mEnemy_AIMode == AI_IDLE)
		{
			if (mDistance >= BOW_ATTACK_MIN_RANGE && mDistance <= BOW_ATTACK_MAX_RANGE)
			{
				m_bAttack = true;
				m_pEnemyState = m_pAttackState;
				if (m_pPreEnemyState != m_pEnemyState)
				{
					m_pEnemyState->PreAISet();
					m_pPreEnemyState = m_pEnemyState;
				}
			}
			else if (m_bAttack == false)
			{
				m_pEnemyState = m_pMoveState;
				if (m_pPreEnemyState != m_pEnemyState)
				{
					m_pEnemyState->PreAISet();
					m_pPreEnemyState = m_pEnemyState;
				}
			}
			mPre_EnemyAIMode = mEnemy_AIMode;
		}
		break;
	case HUMAN_BOSS:
		if (mEnemy_AIMode >= AI_DAMAGED)
		{
			if (m_bForceAttack == false && m_bDamaged == false && m_bAttack == false)
			{
				if (mEnemy_AIMode != mPre_EnemyAIMode)
				{
					if (mEnemy_AIMode != AI_DAMAGED_LEFT)
					{
						m_bDamaged = true;
					}

					m_pEnemyState = m_pDamagedState;
					m_pEnemyState->PreAISet(mEnemy_AIMode);
					m_pPreEnemyState = m_pEnemyState;
					mPre_EnemyAIMode = mEnemy_AIMode;
				}
			}
		}
		else if (mEnemy_AIMode == AI_IDLE)
		{
			if (m_pCollider->Collision_AABB(m_pPlayerCollider, nullptr))
			{
				m_bAttack = true;
				m_pEnemyState = m_pAttackState;
				if (m_pPreEnemyState != m_pEnemyState)
				{
					if (m_pPreEnemyState->Get_AIMode() == AI_SPECIAL_CHASE)
					{
						m_pEnemyState->PreAISet(AI_SPECIAL_CHASE);
					}
					else
					{
						m_pEnemyState->PreAISet();
					}
					
					m_pPreEnemyState = m_pEnemyState;
				}
			}
			else if (m_bAttack == false)
			{
				m_pEnemyState = m_pMoveState;
				if (m_pPreEnemyState != m_pEnemyState)
				{
					m_pEnemyState->PreAISet();
					m_pPreEnemyState = m_pEnemyState;
				}
			}
			mPre_EnemyAIMode = mEnemy_AIMode;
		}
		break;
	default:
		if (mEnemy_AIMode >= AI_DAMAGED)
		{
			if (m_bForceAttack == false && m_bDamaged == false && m_bAttack == false)
			{
				if (mEnemy_AIMode != mPre_EnemyAIMode)
				{
					if (mEnemy_AIMode != AI_DAMAGED && mEnemy_AIMode != AI_DAMAGED_LEFT && mEnemy_AIMode != AI_DAMAGED_RIGHT && mEnemy_AIMode != AI_STUN && mEnemy_AIMode != AI_STAND_DOWN)
					{
						m_bDamaged = true;
					}
					
					m_pEnemyState = m_pDamagedState;
					m_pEnemyState->PreAISet(mEnemy_AIMode);
					m_pPreEnemyState = m_pEnemyState;
					mPre_EnemyAIMode = mEnemy_AIMode;
				}
			}
		}
		else if (mEnemy_AIMode == AI_IDLE)
		{
			if (m_pCollider->Collision_AABB(m_pPlayerCollider, nullptr) 
				&& m_pEnemyMgr->Get_AttackCount() <= 6)
			{
				m_bAttack = true;
				m_pEnemyState = m_pAttackState;
				if (m_pPreEnemyState != m_pEnemyState)
				{
					m_pEnemyState->PreAISet();
					m_pPreEnemyState = m_pEnemyState;
				}
			}
			else if (m_bAttack == false)
			{
				m_pEnemyState = m_pMoveState;
				if (m_pPreEnemyState != m_pEnemyState)
				{
					m_pEnemyState->PreAISet();
					m_pPreEnemyState = m_pEnemyState;
				}
			}
			mPre_EnemyAIMode = mEnemy_AIMode;
		}
		break;
	}

	
	return NOERROR;
}

void EnemyAI::AIState(const float timeDelta)
{
	if (m_pEnemyState->Get_PreAIMdoe() == AI_DIE)
		return;

	if (m_pEnemyState != nullptr)
	{
		m_pEnemyState->AIState(timeDelta);
		m_pEnemyState->AISet();
	}

}

void EnemyAI::AI_StateChange(const float timeDelta)
{
	/*if (m_pEnemyState->Get_PreAIMdoe() == AI_STATE_END)
		return;*/
	

	if (m_pEnemyState != nullptr)
	{
		m_pEnemyState->AI_StateChange(timeDelta);
	}
	
}

void EnemyAI::AIUpdate(const float timeDelta)
{
	if (m_pEnemyState->Get_PreAIMdoe() == AI_DIE)
		return;

	if (m_pEnemyState != nullptr)
	{
		m_pEnemyState->AIUpdate(timeDelta);


	}
}

EnemyAI * EnemyAI::Create(PDIRECT3DDEVICE9 pDevice)
{
	EnemyAI* pInstance = new EnemyAI(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("EnemyAI Created Failed!!!");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::Component * EnemyAI::Clone()
{
	return new EnemyAI(*this);
}

DWORD EnemyAI::Free()
{
	//Engine::Safe_Release(m_pEnemyMgr);
	Engine::Safe_Release(m_pUtilityMgr);

	if (m_bClone)
	{
		Engine::Safe_Release(m_pMoveState);
		Engine::Safe_Release(m_pAttackState);
		Engine::Safe_Release(m_pDamagedState);
		Engine::Safe_Release(m_pCollider);
		Engine::Safe_Release(m_pSphereCollider);
		Engine::Safe_Release(m_pPlayerCollider);
		Engine::Safe_Release(m_pTransform);
		Engine::Safe_Release(m_pPlayerTransform);
	}

	Component::Free();
	return 0;
}