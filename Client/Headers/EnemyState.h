#ifndef ENEMY_STATE_H__
#define ENEMY_STATE_H__

#include "Base.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class DynamicMesh;
	class Navigation;
	class Collider;
	class LightManager;
}

_BEGIN(Client)

class PlayerAnim;
class Player;
class Trail;
class DecalBox;
class CameraFPS;
class EffectMgr;


class EnemyState : public Engine::Base
{
protected:
	explicit EnemyState();
	virtual ~EnemyState() = default;

public:
	HRESULT SetInit(const ENEMY_AI_INFO enemy_AIInfo, Engine::DynamicMesh* pMesh, Engine::Transform* pTransform, Engine::Transform* pPlayerTransform, Engine::Navigation* pNavigation);
	HRESULT SetCollider(Engine::Collider* pCollider, Engine::Collider* pPlayerCollider, Engine::Collider* pWeaponCollider);

public:
	virtual void AIState(const float timeDelta) = 0;
	virtual void AISet() = 0;
	virtual void AI_StateChange(const float timeDelta) = 0;
	virtual void AIUpdate(const float timeDelta) = 0;
	virtual void Collision(const float timeDelta) = 0;

public:
	void PreAISet(const int AIMode = -1);
//	bool GetDie() const { return m_bDie; }
	int Get_AIMode() const { return mAIMode; }
	int Get_PreAIMdoe() const { return mPre_AIMode; }
//	float GetHP() const { return mHP; }

private:
	void DamagedEffect();

protected:
	void ChangeInit();
	void CalDir(const float timeDelta);
	void Gravity(const float timeDelta);
	void Jump(const float timeDelta, const size_t jumpState, const float jumpVelocity, const float sinDegree, const float cosDegree);
	void GoStraight(const float timeDelta, const float speed);
	void SetDamged(const float damaged);

protected:
	void CalDiagonalDir(const D3DXVECTOR3& vRightDir);

private:
	void Set_HPInit();

public:
	virtual DWORD Free() override;

private:
	enum ENEMY_HP_VALUE
	{
		BOSS_ORIGINAL_HP = 500,
	};

protected: // Animation Related Variable
	string mAnimState = "";
	string mPreAnimState = "";
	ANIM_CTRL_INFO* m_pAnimCtrl_Info = nullptr;
	double mAnimTime = 0.0;
	float mAnimSpeed = 0.f;
	double mCorrectionTime = 0.0;
	float mAccumulationTime = 0.f;
	
protected: // Jump Related Variable
	float mJumpTime = 0.f;
	D3DXVECTOR3 m_vJumpDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	bool m_bJump = false;

protected: // AI Related Variable
	ENEMY_AI_INFO mEnemyAI_Info;
	int mAIMode = AI_IDLE;
	int mPre_AIMode = -1;

protected: // Move Info Related Variable;
	D3DXVECTOR3* m_pPlaneY = nullptr;
	D3DXVECTOR3* m_pEnemyPos = nullptr;
	D3DXVECTOR3* m_pEnemyDir = nullptr;
	D3DXVECTOR3* m_pEnemyRightDir = nullptr;

	const D3DXVECTOR3* m_pPlayerPos = nullptr;
	const D3DXVECTOR3* m_pPlayerDir = nullptr;
	size_t mMoveSpeed = 0;
	float mSlideSpeed = 0.f;
	D3DXVECTOR3 m_vSlide = D3DXVECTOR3(0.f, 0.f, 0.f);

protected:
	D3DXVECTOR3 m_vEnemyRightDir;
	D3DXVECTOR3 m_vEnemy_DiagonalDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vPreDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vLineNormal = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vPush = D3DXVECTOR3(0.f, 0.f, 0.f);

protected:
	bool m_bCross = false;
	float mTargetDegree = 0.f;
	float mRotSpeed = 0.f;
	float mDistance = 0.f;

protected:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Transform* m_pPlayerTransform = nullptr;
	Engine::DynamicMesh* m_pMesh = nullptr;
	Engine::Navigation* m_pNavigation = nullptr;

protected:
	Engine::Collider* m_pCollider = nullptr;
	Engine::Collider* m_pPlayerCollider = nullptr;
	Engine::Collider* m_pWeaponCollider = nullptr;
	Engine::LightManager* m_pLightMgr = nullptr;

protected:
	EnemyMgr* m_pEnemyMgr = nullptr;
	UtilityMgr* m_pUtilityMgr = nullptr;
	BulletMgr* m_pBulletMgr = nullptr;
	UIMgr* m_pUIMgr = nullptr;
	EffectMgr* m_pEffectMgr = nullptr;
	SoundMgr* m_pSoundMgr = nullptr;

protected: // PlayerAnim Related Variable;
	PlayerAnim* m_pPlayerAnim = nullptr;
	string* m_pPlayerAnimState = nullptr;
	Player* m_pPlayer = nullptr;
	const PLAYER_INFO* m_pPlayerInfo = nullptr;
	const size_t* m_pEnemyIndex = nullptr;
	CameraFPS* m_pCamerFPS = nullptr;

private:
	float mDamagedValue = 0.f;

protected: // Trail Related Variable
	Trail* m_pLeftTrail = nullptr;
	Trail* m_pRightTrail = nullptr;

protected:
	DecalBox* m_pDecal = nullptr;

private:
	enum EFFECT_DAMGED_TYPE
	{
		EFFECT_RED = 1,
		EFFECT_BLUE,
		EFFECT_PURPLE,
	};

private:
	D3DXVECTOR3 m_vEffect_DamgedPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	int mEffectRand = 0;
	D3DXVECTOR3 m_vEffectScale = D3DXVECTOR3(0.f, 0.f, 0.f);
};

_END

#endif

