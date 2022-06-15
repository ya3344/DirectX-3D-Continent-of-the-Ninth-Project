#ifndef MISTIC_WEAPON_H__
#define MISTIC_WEAPON_H__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class StaticMesh;
	class Shader;
	class Collider;
	class DynamicMesh;
}

_BEGIN(Client)

class Trail;

class MisticWeapon final : public Engine::GameObject
{
private:
	explicit MisticWeapon(LPDIRECT3DDEVICE9 pDevice);
	explicit MisticWeapon(const MisticWeapon& rhs);
	virtual ~MisticWeapon() = default;

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
	static MisticWeapon* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private: 
	D3DXMATRIX* m_pHandMatrix = nullptr;
	Engine::DynamicMesh* m_pPlayerMesh = nullptr;
	Engine::Transform* m_pPlayerTransform = nullptr;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::StaticMesh* m_pMeshObject = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::Collider* m_pColliderBox = nullptr;
	Engine::Collider* m_pColliderSphere = nullptr;

private: // Trail Related Variable
	Trail* m_pTrail = nullptr;
	D3DXVECTOR3 m_vTrailPosA;
	D3DXVECTOR3 m_vTrailPosB;

	
};

_END

#endif

