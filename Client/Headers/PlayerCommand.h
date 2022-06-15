#ifndef PLAYER_COMMAND_H__
#define PLAYER_COMMAND_H__

#include "Component.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class DynamicMesh;
	class Navigation;
	class Collider;
	class KeyMgr;
}

_BEGIN(Client)

class EnemyMgr;
class Player;
class Trail;
class DecalBox;
class EffectMgr;

typedef struct tagAnimInfo
{
	tagAnimInfo() {}
	tagAnimInfo(int attackCount, float animSpeed)
	{
		this->attackCount = attackCount;
		this->animSpeed = animSpeed;
	}
	
	int attackCount = -1;
	float animSpeed = 1.f;

}ANIM_INFO;

class PlayerCommand : public Engine::Component
{
protected:
	explicit PlayerCommand(LPDIRECT3DDEVICE9 pDevice);
	explicit PlayerCommand(const PlayerCommand& rhs);
	virtual ~PlayerCommand() = default;

public:
	HRESULT SetInit(Engine::DynamicMesh* pMesh, Engine::Transform* pTransform, Engine::Navigation* pNavigation, Player* pPlayer, Trail* pTrail);
	HRESULT Set_WeaponCollider(Engine::Collider* pWeaponCollider, Engine::Collider* pWeapon_SphereCollider);

public: // AnimState Related Function
	virtual HRESULT KeyCheck(const float timeDelta) = 0;
	virtual void AnimStateChange(const float timeDelta) = 0;
	virtual void AnimUpdate(const float timeDelta) = 0;

public:
	string* GetAnimState() { return &mAnimState; }
	const double* GetAnimTime() const { return &mAnimTime; }
	void SetBossDir(const D3DXVECTOR3* pBossDir) { m_pBossDir = pBossDir; }

protected:
	void LinkAnimation(int attackCount);
	void Cal_SlidngVector();
	void CalMove(const float dirSpeed, const float animSpeed, const float timeDelta, const bool bRightStep = false);
	void Gravity(const float timeDelta);
	void Jump(const float timeDelta, const size_t jumpState);
	void DamagedJump(const float timeDelta, const size_t jumpState, const float jumpVelocity, const float sinDegree, const float cosDegree, const D3DXVECTOR3* pEnemyDir = nullptr);
	
protected:
	bool LockOnTarget_Operation(const float timeDelta);

public:
	virtual Component * Clone() = 0;
	virtual DWORD Free() override;

protected: // Animation Related Variable
	ANIM_CTRL_INFO* m_pAnimCtrl_Info = nullptr;
	ANIM_INFO mAnimInfo;
	double mAnimTime = 0.0;
	string mAnimState = "";
	string mPre_AnimState = "";

protected:
	Engine::Transform* m_pTransform = nullptr;
	Engine::DynamicMesh* m_pMesh = nullptr;
	Engine::Navigation* m_pNavigation = nullptr;
	Engine::Collider* m_pWeaponCollider = nullptr;
	Engine::Collider* m_pWeapon_SphereCollider = nullptr;

protected:
	Engine::KeyMgr* m_pKeyMgr = nullptr;
	UtilityMgr* m_pUtilityMgr = nullptr;
	EnemyMgr* m_pEnemyMgr = nullptr;
	EffectMgr* m_pEffectMgr = nullptr;
	SoundMgr* m_pSoundMgr = nullptr;

protected: 
	/*float mForwardSpeed = 0.f;
	float mBackwardSpeed = 0.f;*/

protected: // Jump Related Variable
	D3DXVECTOR3 m_vJumpDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	bool m_bJump = false;
	D3DXVECTOR3* m_pPlaneY = nullptr; 
	float mJumpTime = 0.f;
	float mJumpVelocity = 0.f;
	float mJumpSpeed = 0.f;

protected: // Move Info Related Variable
	D3DXVECTOR3* m_pDir = nullptr;
	D3DXVECTOR3* m_pRightDir = nullptr;
	D3DXVECTOR3 m_vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3* m_pPos = nullptr;

protected: // Bullet Info Related Variable
	BulletMgr* m_pBulletMgr = nullptr;
	Player* m_pPlayer = nullptr;

protected:
	bool m_bTargetOnShot = false;
	bool m_bTargetOnShot_Bulelt = false;
	bool m_bDamaged = false;

protected:
	Trail* m_pTrail = nullptr;
	DecalBox* m_pDecal = nullptr;

private: // Slide Vector Related Variable
	D3DXVECTOR3 m_vLineNormal = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vSlide = D3DXVECTOR3(0.f, 0.f, 0.f);
	float mSlideSpeed = 1.f;

private:
	size_t mLinkAnim_Percent = 0;
	size_t mGailCount = 0;

protected:
	const D3DXVECTOR3* m_pBossDir = nullptr;



};

_END
#endif

