#ifndef TERRAIN_H__
#define TERRAIN_H__

#include "GameObject.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class Shader;
	class StaticMesh;
	class Collider;
	class Frustum;
}

_BEGIN(Client)

class Terrain final : public Engine::GameObject
{
private:
	explicit Terrain(LPDIRECT3DDEVICE9 pDevice);
	explicit Terrain(const Terrain& rhs);
	virtual ~Terrain() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

public:
	HRESULT ReadyPrototype(const TCHAR* pMeshName);

private:
	HRESULT ReadyComponent();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static Terrain* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshName);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private:
	D3DXVECTOR3 m_vCenter = D3DXVECTOR3(0.f, 0.f, 0.f);
	float mRadius = 0.f;
	float mScale = 0.f;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::StaticMesh* m_pMeshObject = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::Collider* m_pCollider = nullptr;
	Engine::Frustum* m_pFrustum = nullptr;


};

_END

#endif // TERRAIN_H__
