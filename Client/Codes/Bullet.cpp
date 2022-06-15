#include "stdafx.h"
#include "Bullet.h"
#include "ComponentMgr.h"
#include "PlayerAnim.h"
#include "EnemyAI.h"

_USING(Client)

Bullet::Bullet(LPDIRECT3DDEVICE9 pDevice)
	:BulletParent(pDevice)
{
}

Bullet::Bullet(const Bullet & rhs)
	:BulletParent(rhs)
{

}

UPDATE Bullet::Update(const float timeDelta)
{
	if (m_bDie)
		return UPDATE::DIE;

	switch (mBulletType)
	{
	case BULLET_BOOM:
		ParabolaAttack(timeDelta, 4.5f, 90.f, 75.f);
		BoomCollision();
		//for (EnemyAI* pEnemyAI : mEnemyAI_Space)
		//{
		//	m_bCollision = m_pColliderSphere->Collision_Bullet(pEnemyAI->Get_SphereCollider(), 20.f);
		//	if (m_bCollision)
		//	{
		//		Enemy_AnimationSet(pEnemyAI);
		//		m_bDie = true;
		//		break;
		//	}
		//}
		
		break;
	case BULLET_SNIPER:
		TargetReady(timeDelta);
		SniperAttack(timeDelta);
		SniperCollision();
		break;
	case BULLET_ARROW:
		ArrowAttack(timeDelta);
		PlayerCollision(timeDelta);
		break;
	case BULLET_ICEMISSILE:
		IceMissileAttack(timeDelta);
		MissileCollision();
		break;
	case BULLET_ICEMISSILE2:
		IceMissileAttack(timeDelta);
		MissileCollision();
		break;
	default:
		assert(false && "BulletParent Update default!");
		break;
	}



	return Engine::GameObject::Update(timeDelta);
}

void Bullet::ParabolaAttack(const float timeDelta, const float velocity, const float sinDegree, const float cosDegree)
{
	if (m_bDie)
		return;

	mBulletTime += (timeDelta * 2.f);

	//m_vBulletDir = *m_pPlayerDir;
	D3DXVec3Normalize(&m_vBulletDir, &m_vBulletDir);

	m_vBulletPos.z = m_vBulletDir.z * velocity * cosf(D3DXToRadian(cosDegree)) * mBulletTime;
	m_vBulletPos.x = m_vBulletDir.x * velocity * cosf(D3DXToRadian(cosDegree)) * mBulletTime;
	m_vBulletPos.y = (velocity * sinf(D3DXToRadian(sinDegree)) * mBulletTime) - (0.5f  * 9.8f  * mBulletTime * mBulletTime);

	m_pTransform->DirStep(m_vBulletPos, true);


	if (m_pBulletPos->y < m_pPlayerPos->y - 5.f)
	{
		cout << "ÃÑ¾Ë ÅÍÁü" << endl;
		//m_bDie = true;
		m_bBoomDie = true;
	}
}

void Bullet::SniperAttack(const float timeDelta)
{
	if (m_bDie || m_bSniperShot == false)
		return;

	m_vTargetDir = m_pUtilityMgr->CalTargetDir(*m_pBulletPos, *m_pTargetPos);
	m_pTransform->DirStep(m_vTargetDir * BULLET_SPEED * timeDelta, true);
}

void Bullet::TargetReady(const float timeDelta)
{
	if (m_bSniperShot == true || m_bDie == true)
		return;

	EnemyAI* pEnemyAI = nullptr;

	auto iterBegin = m_pEnemyAI_Space->begin();
	auto iterEnd = m_pEnemyAI_Space->end();

	m_vBulletDir = *m_pPlayerDir;
	D3DXVec3Normalize(&m_vBulletDir, &m_vBulletDir);
	//m_vBulletDir.y += 0.01f;
	m_pTransform->DirStep(m_vBulletDir * 50 * timeDelta, true);

	for (; iterBegin != iterEnd; iterBegin++)
	{
		pEnemyAI = (*iterBegin);

		m_vEnemyPos = *pEnemyAI->GetCollider()->Get_TargetPos();
		mDistance = m_pUtilityMgr->Cal_TargetDistance(*m_pBulletPos, m_vEnemyPos);
		
		if (mDistance <= SNIPER_RANGE)
		{
			m_pTargetPos = pEnemyAI->GetCollider()->Get_TargetPos();
			/*m_vTargetDir = m_pUtilityMgr->CalTargetDir(*m_pBulletPos, *m_pTargetPos);
			m_bCross = m_pUtilityMgr->Cal_DirCross(*m_pBulletDir, m_vTargetDir);*/
			mShortDistance = mDistance;
			m_bSniperShot = true;
			break;
		}
	}

	mDistance = m_pUtilityMgr->Cal_TargetDistance(*m_pBulletPos, *m_pPlayerPos);

	if (mDistance >= BULLET_DIE_DISTANCE)
	{
	
		m_bDie = true;
	}
		
}

void Bullet::ArrowAttack(const float timeDelta)
{
	if (m_bDie)
		return;
	
	while (mTargetDegree >= 2.f)
	{
		m_pTransform->SetRotation(D3DXVECTOR3(0.f, 2.f, 0.f));
		mTargetDegree = m_pUtilityMgr->CalDirDegree(*m_pBulletDir, m_vBulletDir);
	}

	m_pTransform->Arrow_GoStraight(70.f * timeDelta);
	//m_pTransform->Arrow_GoStraight(5.f * timeDelta);

	mDistance = m_pUtilityMgr->Cal_TargetDistance(*m_pBulletPos, m_vBulletPos);
	if (mDistance >= BULLET_DIE_DISTANCE)
	{
		m_bDie = true;
	}
		
}

void Bullet::IceMissileAttack(const float timeDelta)
{
	if (m_bDie)
		return;

	m_vTargetDir = D3DXVECTOR3(0.f, -1.f, 0.f);

	D3DXVECTOR3 vLimitPos = *m_pPlayerPos;
	if (mBulletType == BULLET_ICEMISSILE)
	{
		vLimitPos.y += 2.f;
	}
	else if (mBulletType == BULLET_ICEMISSILE2)
	{
		vLimitPos.y += 5.5f;
	}

	if (m_pBulletPos->y > vLimitPos.y)
	{
		m_pTransform->DirStep(m_vTargetDir * ICE_MISSILE_SPEED * timeDelta, false);
	}
	else
	{
		mIceMissile_Time += timeDelta * 10.f;
		if (mIceMissile_Time > 7.f)
		{
			m_pTransform->DirStep(m_vTargetDir * ICE_MISSILE_SPEED * 0.3f * timeDelta, false);

			if (m_pBulletPos->y < vLimitPos.y - 10.f)
			{
				m_bDie = true;
				//cout << "¹Ì»çÀÏ »èÁ¦´Ù" << endl;
			}
		}
	}
}

void Bullet::SniperCollision()
{
	EnemyAI* pEnemyAI = nullptr;

	auto iterBegin = m_pEnemyAI_Space->begin();
	auto iterEnd = m_pEnemyAI_Space->end();

	for (; iterBegin != iterEnd; iterBegin++)
	{
		pEnemyAI = (*iterBegin);

		if (pEnemyAI->Get_EnemyType() == HUMAN_BOSS)
		{
			m_bCollision = m_pColliderSphere->Collision_Bullet(pEnemyAI->Get_SphereCollider(), 6.f);
		}
		else
		{
			m_bCollision = m_pColliderSphere->Collision_Bullet(pEnemyAI->Get_SphereCollider(), 0.1f);
		}
		
		if (m_bCollision)
		{
			Enemy_AnimationSet(pEnemyAI);
			m_bDie = true;
			break;
		}
	}
}

void Bullet::BoomCollision()
{
	EnemyAI* pEnemyAI = nullptr;
	float distance = 0.f;

	auto iterBegin = m_pEnemyAI_Space->begin();
	auto iterEnd = m_pEnemyAI_Space->end();

	for (; iterBegin != iterEnd; iterBegin++)
	{
		pEnemyAI = (*iterBegin);

		m_bCollision = m_pColliderSphere->Collision_Bullet(pEnemyAI->Get_SphereCollider());
		if (m_bBoomDie)
		{
			distance = m_pUtilityMgr->Cal_TargetDistance(*m_pBulletPos, *pEnemyAI->GetEnemyPos());

			if (distance <= 20.f)
			{
				Enemy_AnimationSet(pEnemyAI);
			}
		}
		else if (m_bCollision)
		{
			Enemy_AnimationSet(pEnemyAI);
		}

		
	}

	if (m_bBoomDie)
	{
		m_bDie = true;
	}
}

void Bullet::MissileCollision()
{
	m_bCollision = m_pColliderSphere->Collision_Bullet(m_pPlayerCollider, 5.f);

	mMissileCollision_Time += gTimeDelta * 10.f;

	if (m_bCollision)
	{
		if (mMissileCollision_Time > 1.f)
		{
			mMissileCollision_Time = 0.f;
			return;
		}
		if (m_pPlayerAnim->Get_DamagedCount() <= 4)
		{
			if (m_pPlayerAnim->Get_DamagedState() == false)
			{
				*m_pAnimState = "Damaged";
			}
		}
		else
		{
			if (m_pPlayerAnim->Get_DamagedState() == false)
			{
				*m_pAnimState = "Stun";
			}
		}
	}

}

void Bullet::Enemy_AnimationSet(EnemyAI * pEnemyAI)
{
	switch (pEnemyAI->Get_EnemyType())
	{
	case BABEGAZI_WARRIOR:
	case BABEGAZI_BOW:
	case BABEGAZI_AXE:
		pEnemyAI->Set_AIMode(AI_SPIN_A);

		break;
	case KNOLE_COMMANDER:
	case KNOLE_WARRIOR:
		pEnemyAI->Set_AIMode(AI_DAM_UFA);
		break;
	case HUMAN_BOSS:
		pEnemyAI->Set_AIMode(AI_ROUND_PUNCH);
		break;
	default:
		assert(false && "Bullet Enemy_AnimationSet!!");
		break;
	}

	pEnemyAI->SetDamaged(false);
	pEnemyAI->Set_ForceAttack(false);
	pEnemyAI->SetAttack(false);
}

void Bullet::PlayerCollision(const float timeDelta)
{
	mt19937 mtRand((size_t)(rand()));
	uniform_int_distribution<size_t> range(1, 10);
	mAttack_SucessPercent = range(mtRand);
	m_bCollision = m_pColliderSphere->Collision_Bullet(m_pPlayerCollider, 2.f);

	if (m_pAnimState->find("Gail") != string::npos)
	{
		mAttack_SucessValue = 1;
	}
	else
	{
		mAttack_SucessValue = 3;
	}

	if (m_bCollision)
	{
		m_bDie = true;
		if (mAttack_SucessPercent <= mAttack_SucessValue)
		{
			if (m_pPlayerAnim->Get_DamagedCount() <= 4)
			{
				if (m_pPlayerAnim->Get_DamagedState() == false)
				{
					*m_pAnimState = "Damaged";
				}
			}
			else
			{
				if (m_pPlayerAnim->Get_DamagedState() == false)
				{
					*m_pAnimState = "Stun";
				}
			}
		}
	}
}


Bullet * Bullet::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR * pMeshName)
{
	Bullet* pInstance = new Bullet(pDevice);

	if (FAILED(pInstance->ReadyPrototype(pMeshName)))
	{
		_MSG_BOX("Bullet Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Bullet::BulletClone(size_t bulletType, const D3DXVECTOR3& vBulletDir, const D3DXVECTOR3& vBulletPos)
{
	Bullet* pInstance = new Bullet(*this);

	if (FAILED(pInstance->InitBullet(bulletType, vBulletDir, vBulletPos)))
	{
		_MSG_BOX("Bullet Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


DWORD Bullet::Free(void)
{
	BulletParent::Free();

	return 0;
}

