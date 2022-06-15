#ifndef PARTICLE_H__
#define PARTICLE_H__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class ParticleBuffer;
	class Texture;
	class Picking;
	class Shader;
	class TargetManager;
}

_BEGIN(Client)

class Particle : public Engine::GameObject
{
private:
	explicit Particle(LPDIRECT3DDEVICE9 pDevice);
	explicit Particle(const Particle& rhs);
	virtual ~Particle() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	HRESULT ReadyPrototype() override;

private:
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	HRESULT ReadyComponent(void);
	HRESULT SetUp_BillBoard(void);


public:
	static Particle* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::ParticleBuffer* m_pParticleBuffer = nullptr;
	Engine::Texture* m_pTexture = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::TargetManager* m_pTargetMgr = nullptr;
	Engine::Transform* m_pPlayerTransform = nullptr;

private:
	const D3DXVECTOR3* m_pPlayerPos = nullptr;

private:
	float mFrame = 0.f;
	LPD3DXEFFECT m_pEffect = nullptr;

private:
	D3DXVECTOR4 m_vColor = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	D3DXVECTOR3 m_vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vRandomPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	float mX_TimeValue = 0.f;
	float mX_Value = 0.f;
	float mZ_TimeValue = 0.f;
	float mZ_Value = 0.f;
};

#endif

_END

