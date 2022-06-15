#include "stdafx.h"
#include "ParticleMgr.h"
#include "Particle.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"

_USING(Client)
_IMPLEMENT_SINGLETON(ParticleMgr);

ParticleMgr::ParticleMgr()
{
}


HRESULT ParticleMgr::Ready_ParticlePrototype(LPDIRECT3DDEVICE9 pDevice)
{
	Engine::GameObject* pGameObject = nullptr;

	pGameObject = Particle::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_GameObject_Particle", SCENE_EFFECT, pGameObject)))
		return E_FAIL;

	mParticleSpace.reserve(300);

	for (size_t i = 0; i < 300; i++)
	{
		mParticleSpace.emplace_back((Particle*)pGameObject->Clone());
	}

	return NOERROR;
}

UPDATE ParticleMgr::Update(const float timeDelta)
{
	for (Particle* pParticle : mParticleSpace)
	{
		pParticle->Update(timeDelta);
	}


	return UPDATE::NORMAL;
}

UPDATE ParticleMgr::LastUpdate(const float timeDelta)
{
	for (Particle* pParticle : mParticleSpace)
	{
		pParticle->LastUpdate(timeDelta);
	}

	return UPDATE::NORMAL;
}

DWORD ParticleMgr::Free()
{
	for (Particle* pParticle : mParticleSpace)
	{
		Engine::Safe_Release(pParticle);
	}

	return 0;
}
