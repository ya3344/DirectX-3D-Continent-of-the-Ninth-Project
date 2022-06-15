#ifndef ENEMY_H__ 
#define ENEMY_H__

#include "Gameobject.h"
#include "Include.h"


namespace Engine
{
	class Transform;
	class Renderer;
	class DynamicMesh;
	class Shader;
	class Collider;
	class Navigation;
	class Frustum;
}

_BEGIN(Client)

class EnemyAI;
class Trail;

class Enemy final : public Engine::GameObject
{
private:
	explicit Enemy(LPDIRECT3DDEVICE9 pDevice);
	explicit Enemy(const Enemy& rhs);
	virtual ~Enemy() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	HRESULT ReadyPrototype(const TCHAR* pMeshName, size_t naviIndex, size_t enemyType);

public:
	const EnemyAI* GetEnemyAI() const { return m_pEnemyAI; }

private:
	HRESULT ReadyComponent(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static Enemy* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshName, size_t naviIndex, size_t enemyType);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Transform* m_pTargetTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::DynamicMesh* m_pMesh = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::Collider* m_pColliderBox = nullptr;
	Engine::Collider* m_pColliderSphere = nullptr;
	Engine::Navigation* m_pNavigation = nullptr;
	Engine::Frustum* m_pFrustum = nullptr;

private:
	EnemyMgr* m_pEnemyMgr = nullptr;

private:
	LPD3DXEFFECT m_pEffect = nullptr;

private:
	D3DXVECTOR3 m_vCenter = D3DXVECTOR3(0.f, 0.f, 0.f);
	const D3DXVECTOR3* m_pCenterPos = nullptr;
	float mRadius = 0.f;
	float mDistroyTime = 0.f;

private:
	EnemyAI* m_pEnemyAI = nullptr;
	size_t mNaviIndex = 0;
	size_t mEnemyType = 0;
	const TCHAR* m_pMeshName = nullptr;
	size_t mShaderIndex = 0;

private:
	enum OPTIMIZATION
	{
		RENDER_RANGE = 200,
	};


private: // Trail Related Variable
	Trail* m_pLeftTrail = nullptr;
	Trail* m_pRightTrail = nullptr;
	D3DXVECTOR3 m_vTrailPosA;
	D3DXVECTOR3 m_vTrailPosB;
	D3DXMATRIX* m_pHandLeftMatrix = nullptr;
	D3DXMATRIX* m_pHandRightMatrix = nullptr;
	size_t mTrail_TextureNum = 0;

private:
	D3DXVECTOR3* m_pEnemyPos = nullptr;
	const D3DXVECTOR3* m_pPlayerPos = nullptr;
	UtilityMgr* m_pUtilityMgr = nullptr;
};

_END

#endif

