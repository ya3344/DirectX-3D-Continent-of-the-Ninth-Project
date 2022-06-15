#ifndef PARTICLE_MGR_H__
#define PARTICLE_MGR_H__

#include "Base.h"
#include "Include.h"


namespace Engine
{
	class GameObject;
}

_BEGIN(Client)

class Particle;

class ParticleMgr final : public::Engine::Base
{
public:
	_DECLARE_SINGLETON(ParticleMgr)

private:
	explicit ParticleMgr();
	virtual ~ParticleMgr() = default;

public:
	HRESULT Ready_ParticlePrototype(LPDIRECT3DDEVICE9 pDevice);
	UPDATE Update(const float timeDelta);
	UPDATE LastUpdate(const float timeDelta);

public:
	virtual DWORD Free() override;

private:
	vector<Particle*> mParticleSpace;

};

_END
#endif

