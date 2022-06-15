#ifndef ENEMY_AI_H__
#define ENEMY_AI_H__

#include "Component.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class DynamicMesh;
	class Navigation;
	class Collider;
}

_BEGIN(Client)

class EnemyState;
class MoveState;
class AttackState;
class DamagedState;

class EnemyAI : public Engine::Component
{
private:
	explicit EnemyAI(LPDIRECT3DDEVICE9 pDevice);
	explicit EnemyAI(const EnemyAI& rhs);
	virtual ~EnemyAI() = default;

public:
	HRESULT Init();
	HRESULT SetInit(const size_t enemyType, Engine::DynamicMesh* pMesh, Engine::Transform* pTransform, Engine::Transform* pTargetTransform, Engine::Navigation* pNavigation);
	HRESULT SetCollider(Engine::Collider* pCollider, Engine::Collider* pSphereCollider, Engine::Collider* pPlayerCollider, Engine::Collider* pWeaponCollider);

public:
	void Set_AIMode(const int AIMode) { mEnemy_AIMode = AIMode; }
	void Set_ForceAttack(const bool bForceAttack) { m_bForceAttack = bForceAttack; }
	void SetAttack(const bool bAttack) { m_bAttack = bAttack; }
	void SetDamaged(const bool bDamaged) { m_bDamaged = bDamaged; }
	void SetSpecialIndex(const size_t specialIndex) { mSpecialIndex = specialIndex; }
	void SetNaviMinIndex(const size_t naviMinIndex) { mNaviMinIndex = naviMinIndex; }
	void SetNaviMaxIndex(const size_t naviMaxIndex) { mNaviMaxIndex = naviMaxIndex; }

public:
	bool Get_IsAttack() const { return m_bAttack; }
	bool Get_IsForceAttack() const { return m_bForceAttack; }
	bool GetDamaged() const { return m_bDamaged; }
	size_t Get_EnemyType() const { return mEnemyType; }
	int Get_EnemyAIMode() const { return mEnemy_AIMode; }
	int GetPreEnemy_AIMode() const { return mPre_EnemyAIMode;  }
	size_t GetSpecialIndex() const { return mSpecialIndex; }
	size_t GetNaviMinIndex() const { return mNaviMinIndex; }
	size_t GetNaviMaxIndex() const { return mNaviMaxIndex; }
	const D3DXVECTOR3* GetEnemyPos() const { return m_pEnemyPos; }

public:
	const float* GetEnemyHP() const { return &mHP; }
	const size_t* GetEnemyHP_Count() const { return &mBossHP_Count; }

public:
	Engine::Collider* GetCollider() const { return m_pCollider; }
	Engine::Collider* Get_SphereCollider() const { return m_pSphereCollider; }

public:
	HRESULT AIState_Update(const float timeDelta);
	void AIState(const float timeDelta);
	void AI_StateChange(const float timeDelta);
	void AIUpdate(const float timeDelta);

public:
	static EnemyAI* Create(PDIRECT3DDEVICE9 pDevice);
	virtual Component * Clone() override;
	virtual DWORD Free() override;

private:
	static constexpr float BOW_ATTACK_MAX_RANGE = 30.f;
	static constexpr float BOW_ATTACK_MIN_RANGE = 15.f;

private:
	Engine::Collider* m_pCollider = nullptr;
	Engine::Collider* m_pSphereCollider = nullptr;
	Engine::Collider* m_pPlayerCollider = nullptr;
	Engine::Transform* m_pTransform = nullptr;
	Engine::Transform* m_pPlayerTransform = nullptr;

private:
	EnemyMgr* m_pEnemyMgr = nullptr;
	UtilityMgr* m_pUtilityMgr = nullptr;

private:
	EnemyState* m_pEnemyState = nullptr;
	EnemyState* m_pPreEnemyState = nullptr;
	MoveState* m_pMoveState = nullptr;
	DamagedState* m_pDamagedState = nullptr;
	AttackState* m_pAttackState = nullptr;

private:
	const D3DXVECTOR3* m_pEnemyPos = nullptr;
	const D3DXVECTOR3* m_pPlayerPos = nullptr;

private:
	int mEnemy_AIMode = -1;
	int mPre_EnemyAIMode = -1;

private:
	size_t mEnemyType = 0;
	size_t mSpecialIndex = 0;
	size_t mNaviMinIndex = 0;
	size_t mNaviMaxIndex = 0;
	bool m_bForceAttack = false;
	bool m_bAttack = false;
	bool m_bDamaged = false;
	
private:
	float mDistance = 0,f;

protected: // Die Related Variable;
	float mHP = 0.f;
	size_t mBossHP_Count = 0;
	bool m_bDie = false;
	
	
};

_END

#endif

