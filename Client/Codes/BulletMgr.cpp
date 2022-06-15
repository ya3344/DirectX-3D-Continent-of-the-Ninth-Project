#include "stdafx.h"
#include "BulletMgr.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "Bullet.h"

_USING(Client)
_IMPLEMENT_SINGLETON(BulletMgr);

BulletMgr::BulletMgr()
{
}


HRESULT BulletMgr::Ready_BulletPrototype(LPDIRECT3DDEVICE9 pDevice)
{
	Engine::GameObject* pGameObject = nullptr;

	pGameObject = Bullet::Create(pDevice, L"Component_FireCritical");
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_Bullet", SCENE_BULLET, pGameObject)))
		return E_FAIL;

	m_pEffectMgr = EffectMgr::GetInstance();

	return NOERROR;
}

HRESULT BulletMgr::Ready_ArrowPrototype(LPDIRECT3DDEVICE9 pDevice)
{
	Engine::GameObject* pGameObject = nullptr;

	pGameObject = Bullet::Create(pDevice, L"Component_Arrow");
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_Arrow", SCENE_BULLET, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT BulletMgr::Ready_BossPrototype(LPDIRECT3DDEVICE9 pDevice)
{
	Engine::GameObject* pGameObject = nullptr;

	pGameObject = Bullet::Create(pDevice, L"Component_Ice_Missile_1");
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_Ice_Missile1", SCENE_BULLET, pGameObject)))
		return E_FAIL;

	pGameObject = Bullet::Create(pDevice, L"Component_Ice_Missile_2");
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_Ice_Missile2", SCENE_BULLET, pGameObject)))
		return E_FAIL;

	return NOERROR;

}

UPDATE BulletMgr::Update(const float timeDelta)
{
	auto iterBegin = m_pBulletSpace.begin();
	auto iterEnd = m_pBulletSpace.end();
	Bullet* pBullet = nullptr;

	for (; iterBegin != iterEnd;)
	{
		pBullet = *iterBegin;

		if (pBullet == nullptr)
			return UPDATE::FAIL;

		mUpdate = pBullet->Update(timeDelta);

		if (mUpdate == UPDATE::DIE)
		{
			EffectCreate(pBullet);
			Engine::Safe_Release(pBullet);
			iterBegin = m_pBulletSpace.erase(iterBegin);
		}
		else
		{
			++iterBegin;
		}
	}

	return UPDATE::NORMAL;
}

UPDATE BulletMgr::LastUpdate(const float timeDelta)
{
	for (Bullet* pBullet : m_pBulletSpace)
	{
		if (pBullet == nullptr)
			return UPDATE::FAIL;

		pBullet->LastUpdate(timeDelta);
	//	pBullet->Render();
	}
	return UPDATE::NORMAL;
}

void BulletMgr::AddBullet(const TCHAR* pBulletTag, size_t bulletType, const D3DXVECTOR3& vBulletDir, const D3DXVECTOR3& vBulletPos)
{
	Bullet* pBulletPrototype = (Bullet*)Engine::ObjectMgr::GetInstance()->FindPrototype(SCENE_BULLET, pBulletTag);
	if (pBulletPrototype == nullptr)
	{
		_MSG_BOX("BulletPrototype Find Failed!");
		return;
	}

	Bullet* pBullet = (Bullet*)pBulletPrototype->BulletClone(bulletType, vBulletDir, vBulletPos);

	if (pBullet != nullptr)
	{
		if (bulletType == BULLET_BOOM)
		{
			m_pEffectMgr->CreateEffect(L"Prototype_Effect_Bullet", EFFECT_INFO(pBullet->Get_BulletPos(), D3DXVECTOR3(10.f, 10.f, 10.f), EFFECT_HOLDING_BULLET, pBullet->GetDieState()));
		}
		else if (bulletType == BULLET_SNIPER)
		{
			m_pEffectMgr->CreateEffect(L"Prototype_Effect_SniperBullet", EFFECT_INFO(pBullet->Get_BulletPos(), D3DXVECTOR3(7.f, 7.f, 7.f), EFFECT_SNIPER_BULLET, pBullet->GetDieState()));
		}
	
		m_pBulletSpace.emplace_back(pBullet);
	}

}

void BulletMgr::IceMisile(const TCHAR * pBulletTag, size_t bulletType, const size_t misileNum, const D3DXVECTOR3 & vBulletDir, const D3DXVECTOR3 & vBulletPos)
{
	Bullet* pBulletPrototype = (Bullet*)Engine::ObjectMgr::GetInstance()->FindPrototype(SCENE_BULLET, pBulletTag);
	if (pBulletPrototype == nullptr)
	{
		_MSG_BOX("BulletPrototype Find Failed!");
		return;
	}

	for (size_t i = 0; i < misileNum; i++)
	{
		m_pBulletSpace.emplace_back((Bullet*)pBulletPrototype->BulletClone(bulletType, vBulletDir, vBulletPos));
	}
}

void BulletMgr::EffectCreate(const Bullet* pBullet)
{
	switch (pBullet->GetBulletType())
	{
	case BULLET_BOOM:
		m_pEffectMgr->CreateEffect(L"Prototype_Effect_Explosion", EFFECT_INFO(pBullet->Get_BulletPos(), D3DXVECTOR3(25.f, 25.f, 25.f), EFFECT_EXPLOSION));
		m_pEffectMgr->CreateEffect(L"Prototype_Effect_Fire", EFFECT_INFO(pBullet->Get_BulletPos(), D3DXVECTOR3(25.f, 25.f, 25.f), EFFECT_EXPLOSION));
		break;
	case EFFECT_SNIPER_BULLET:
		m_pEffectMgr->CreateEffect(L"Prototype_Effect_BlueFire", EFFECT_INFO(pBullet->Get_BulletPos(), D3DXVECTOR3(25.f, 25.f, 25.f), EFFECT_SNIPER_EXPLOSION));
		break;
	case BULLET_ARROW:
		m_pEffectMgr->CreateEffect(L"Prototype_Effect_Fire", EFFECT_INFO(pBullet->Get_BulletPos(), D3DXVECTOR3(3.f, 3.f, 3.f), EFFECT_ARROW_EXPLOSION));
		break;
	default:
		break;
	}
}

DWORD BulletMgr::Free()
{
	for (Bullet* pBullet : m_pBulletSpace)
	{
		Engine::Safe_Release(pBullet);
	}
	m_pBulletSpace.clear();
	return 0;
}
