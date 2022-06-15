#ifndef STONE_H__
#define STONE_H__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class StaticMesh;
	class Shader;
	class Material;
	class Collider;
	class Frustum;
}

_BEGIN(Client)

class Stone final : public Engine::GameObject
{
private:
	explicit Stone(LPDIRECT3DDEVICE9 pDevice);
	explicit Stone(const Stone& rhs);
	virtual ~Stone() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT ReadyPrototype() override;

private:
	HRESULT ReadyComponent();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static Stone* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::StaticMesh* m_pMeshObject = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::Collider* m_pCollider = nullptr;
	Engine::Frustum* m_pFrustum = nullptr;
};

_END

#endif

