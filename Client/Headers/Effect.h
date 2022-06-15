#ifndef EFFECT_H__
#define EFFECT_H__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class RectTex;
	class Texture;
	class Picking;
	class Shader;
	class TargetManager;
}

_BEGIN(Client)

class Effect : public Engine::GameObject
{
private:
	explicit Effect(LPDIRECT3DDEVICE9 pDevice);
	explicit Effect(const Effect& rhs);
	virtual ~Effect() = default;

public:
	virtual HRESULT Init() override;
	HRESULT EffectInit(const EFFECT_INFO& effectInfo);
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	HRESULT ReadyPrototype(const EFFECT_INIT_INFO& effect_InitInfo);

public:
	bool GetDieState() const { return m_bDie; }
private:
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	HRESULT ReadyComponent(void);
	HRESULT SetUp_BillBoard(void);

private:
	void AtlasUpdate(const float timeDelta);

public:
	static Effect* Create(LPDIRECT3DDEVICE9 pDevice, const EFFECT_INIT_INFO& effect_InitInfo);
	virtual GameObject* Clone() override;
	GameObject* EffectClone(const EFFECT_INFO& effectInfo);
	virtual DWORD Free(void) override;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::RectTex* m_pRectTex = nullptr;
	Engine::Texture* m_pTexture = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::TargetManager* m_pTargetMgr = nullptr;
	Engine::Transform* m_pPlayerTransform = nullptr;
	SoundMgr* m_pSoundMgr = nullptr;

private:
	float mFrame = 0.f;
	LPD3DXEFFECT m_pEffect = nullptr;

private:
	const D3DXVECTOR3* m_pPlayerPos = nullptr;
	EFFECT_INIT_INFO mEffect_InitInfo;
	EFFECT_INFO mEffectInfo;
	bool m_bDie = false;


private:
	float mTimeCheckX = 0.f;
	float mTimeCheckY = 0.f;
	size_t mEffectCount = 0;
	D3DXVECTOR4 m_vColor = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	D3DXVECTOR3 m_vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
};

#endif

_END

