#include "stdafx.h"
#include "EffectMgr.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "Effect.h"

_USING(Client)
_IMPLEMENT_SINGLETON(EffectMgr);


EffectMgr::EffectMgr()
{

}

HRESULT EffectMgr::Ready_EffectPrototype(LPDIRECT3DDEVICE9 pDevice)
{
	Engine::GameObject* pGameObject = nullptr;


	pGameObject = Effect::Create(pDevice, EFFECT_INIT_INFO(L"Component_Effect_Fire", 1280, 640, 8, 3, 80.f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Effect_Fire", SCENE_EFFECT, pGameObject)))
		return E_FAIL;

	pGameObject = Effect::Create(pDevice, EFFECT_INIT_INFO(L"Component_Effect_BlueFire", 768, 768, 4, 4, 80.f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Effect_BlueFire", SCENE_EFFECT, pGameObject)))
		return E_FAIL;

	pGameObject = Effect::Create(pDevice, EFFECT_INIT_INFO(L"Component_Effect_Bullet", 768, 768, 4, 4, 60.f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Effect_Bullet", SCENE_EFFECT, pGameObject)))
		return E_FAIL;

	pGameObject = Effect::Create(pDevice, EFFECT_INIT_INFO(L"Component_Effect_SniperBullet", 768, 768, 4, 4, 60.f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Effect_SniperBullet", SCENE_EFFECT, pGameObject)))
		return E_FAIL;

	pGameObject = Effect::Create(pDevice, EFFECT_INIT_INFO(L"Component_Effect_Explosion", 1280, 640, 8, 4, 60.f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Effect_Explosion", SCENE_EFFECT, pGameObject)))
		return E_FAIL;

	pGameObject = Effect::Create(pDevice, EFFECT_INIT_INFO(L"Component_Effect_PlayerMove", 768, 768, 4, 4, 80.f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Effect_PlayerMove", SCENE_EFFECT, pGameObject)))
		return E_FAIL;

	pGameObject = Effect::Create(pDevice, EFFECT_INIT_INFO(L"Component_Effect_Blood", 1280, 640, 8, 4, 60.f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Effect_Blood", SCENE_EFFECT, pGameObject)))
		return E_FAIL;

	pGameObject = Effect::Create(pDevice, EFFECT_INIT_INFO(L"Component_Effect_BlueBlood", 768, 768, 4, 4, 45.f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Effect_BlueBlood", SCENE_EFFECT, pGameObject)))
		return E_FAIL;

	pGameObject = Effect::Create(pDevice, EFFECT_INIT_INFO(L"Component_Effect_PurpleBlood", 768, 768, 4, 4, 45.f));
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Effect_PurpleBlood", SCENE_EFFECT, pGameObject)))
		return E_FAIL;

	
	return NOERROR;
}

UPDATE EffectMgr::Update(const float timeDelta)
{
	for (Effect* pEffect : mEffectSpace)
	{
		pEffect->Update(timeDelta);
	}

	EffectDelete();

	return UPDATE::NORMAL;
}

UPDATE EffectMgr::LastUpdate(const float timeDelta)
{
	for (Effect* pEffect : mEffectSpace)
	{
		pEffect->LastUpdate(timeDelta);
	}
	return UPDATE::NORMAL;
}

void EffectMgr::EffectDelete()
{
	auto iterBegin = mEffectSpace.begin();
	auto iterEnd = mEffectSpace.end();
	Effect* pEffect = nullptr;

	for (; iterBegin != iterEnd;)
	{
		pEffect = (Effect*)*iterBegin;

		if (pEffect->GetDieState() == true)
		{
			Engine::Safe_Release(pEffect);
			iterBegin = mEffectSpace.erase(iterBegin);
		}
		else
		{
			++iterBegin;
		}
	}
}

HRESULT EffectMgr::CreateEffect(const TCHAR * pEffectTag, const EFFECT_INFO& effectInfo)
{
	Effect* pEffectPrototype = (Effect*)Engine::ObjectMgr::GetInstance()->FindPrototype(SCENE_EFFECT, pEffectTag);
	if (pEffectPrototype == nullptr)
	{
		_MSG_BOX("EffectPrototype Find Failed!");
		return E_FAIL;
	}

	mEffectSpace.emplace_back((Effect*)pEffectPrototype->EffectClone(effectInfo));

	return NOERROR;
}

DWORD EffectMgr::Free()
{
	for (Effect* pEffect : mEffectSpace)
	{
		Engine::Safe_Release(pEffect);
	}
	mEffectSpace.clear();

	return 0;
}
