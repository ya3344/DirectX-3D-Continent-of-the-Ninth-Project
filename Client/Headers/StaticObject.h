#ifndef STATIC_OBJECT_H__
#define STATIC_OBJECT_H__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class StaticMesh;
	class Shader;
	class Collider;
	class Frustum;
}

_BEGIN(Client)

class StaticObject final : public Engine::GameObject
{
private:
	explicit StaticObject(LPDIRECT3DDEVICE9 pDevice);
	explicit StaticObject(const StaticObject& rhs);
	virtual ~StaticObject() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

public:
	HRESULT ReadyPrototype(const STATIC_OBJECT_INFO& staticObject_Info);

private:
	HRESULT ReadyComponent();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static StaticObject* Create(LPDIRECT3DDEVICE9 pDevice, const STATIC_OBJECT_INFO& staticObject_Info);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;


private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::StaticMesh* m_pMeshObject = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::Collider* m_pColliderBox = nullptr;
	Engine::Frustum* m_pFrustum = nullptr;

private:
	STATIC_OBJECT_INFO mStaticObject_Info;

private:
	D3DXVECTOR3 m_vCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	float mRadius = 0.f;
	float mScale = 0.f;
};

_END

#endif

