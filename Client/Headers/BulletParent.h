#ifndef BULLET_PARENT_H__
#define BULLET_PARENT_H__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class StaticMesh;
	class Shader;
	class Collider;
	class Navigation;
	class Collider;
	class DynamicMesh;
}


_BEGIN(Client)
class PlayerAnim;
class EnemyAI;


class BulletParent : public Engine::GameObject
{
protected:
	explicit BulletParent(LPDIRECT3DDEVICE9 pDevice);
	explicit BulletParent(const BulletParent& rhs);
	virtual ~BulletParent() = default;

public:
	HRESULT InitBullet(size_t bulletType, const D3DXVECTOR3& vBulletDir = D3DXVECTOR3(0.f, 0.f, 0.f), const D3DXVECTOR3& vBulletPos = D3DXVECTOR3(0.f ,0.f, 0.f));
	virtual UPDATE Update(const float timeDelta) = 0;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

public:
	const D3DXVECTOR3* Get_BulletPos() const { return m_pBulletPos; }
	const bool* GetDieState() const { return &m_bDie; }
	const size_t GetBulletType() const { return mBulletType; }

protected:
	HRESULT ReadyPrototype(const TCHAR* pMeshName);


private:
	HRESULT ReadyComponent();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	virtual GameObject* Clone() override;
	virtual GameObject * BulletClone(size_t bulletType, const D3DXVECTOR3& vBulletDir = D3DXVECTOR3(0.f, 0.f, 0.f), const D3DXVECTOR3& vBulletPos = D3DXVECTOR3(0.f, 0.f, 0.f)) = 0;
	virtual DWORD Free(void) override;

protected:
	Engine::Transform* m_pPlayerTransform = nullptr;
	Engine::Collider* m_pPlayerCollider = nullptr;
	PlayerAnim* m_pPlayerAnim = nullptr;
	string* m_pAnimState = nullptr;

protected:
	D3DXVECTOR3* m_pPlayerPos = nullptr;
	D3DXVECTOR3* m_pPlayerDir = nullptr;
	D3DXVECTOR3* m_pBulletPos = nullptr;
	D3DXVECTOR3* m_pBulletDir = nullptr;
	D3DXMATRIX* m_pHandMatrix = nullptr;
	Engine::DynamicMesh* m_pPlayerMesh = nullptr;

protected:
	D3DXVECTOR3 m_vBulletDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vBullet_RightDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vBulletPos = D3DXVECTOR3(0.f, 0.f, 0.f);

protected:
	float mBulletTime = 0.f;
	D3DXVECTOR3* m_pPlaneY = nullptr;

protected:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::StaticMesh* m_pMeshObject = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::Collider* m_pColliderSphere = nullptr;
	Engine::Navigation* m_pNavigation = nullptr;

protected:
	EnemyMgr* m_pEnemyMgr = nullptr;
	UtilityMgr* m_pUtilityMgr = nullptr;
	const list<EnemyAI*>* m_pEnemyAI_Space = nullptr;
	D3DXVECTOR3 m_vEnemyPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3* m_pEnemyPos = nullptr;

protected:
	size_t mBulletType = BULLET_END;
	bool m_bDie = false;
	float mTargetDegree = 0.f;

protected: // IceMissile Related Variable;
	D3DXVECTOR3 m_vIceMissile_Range = D3DXVECTOR3(0.f, 0.f, 0.f);

};

_END

#endif

