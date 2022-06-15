#ifndef PLAYER_ANIM_H__
#define PLAYER_ANIM_H__

#include "PlayerCommand.h"

_BEGIN(Client)


class PlayerAnim : public PlayerCommand
{
private:
	explicit PlayerAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit PlayerAnim(const PlayerAnim& rhs);
	virtual ~PlayerAnim() = default;

public:
	HRESULT Init();

public: // AnimState Related Function
	virtual HRESULT KeyCheck(const float timeDelta) override;
	virtual void AnimStateChange(const float timeDelta) override;
	virtual void AnimUpdate(const float timeDelta) override;

private:
	void AnimChange();

private:
	void Move(const float timeDelta);
	void AttackCollision();

public:
	void Set_DamagedState(const bool bDamaged) { m_bDamaged = bDamaged; }
	size_t Get_DamagedCount() const { return mDamgedCount; }
	bool Get_DamagedState() const { return m_bDamaged; }

public:
	void Set_EnemyRightDir(const D3DXVECTOR3& vEnemyRightDir) { m_vEnemyRightDir = vEnemyRightDir; }
	void SetJumpInfo(const JUMP_INFO& jumpInfo) { mJumpInfo = jumpInfo; }

public:
	static PlayerAnim* Create(PDIRECT3DDEVICE9 pDevice);
	virtual Component * Clone() override;
	virtual DWORD Free() override;

private:
	enum
	{
		FORWARD_SPEED = 25,
		BACKWARD_SPEED = -20,
	};

private:
	static constexpr float JUMP_VELOCITY = 2.7f;

private:
	double mCorrectionTime = 0.0;
	double mGailTime = 0.0;

private:
	bool m_bHoldingShot = false;
	bool m_bAttack = false;
	
	size_t mDamgedCount = 0;

private: // Trail Related Variable;
	size_t mTrail_TextureNum = 0;
	D3DXVECTOR4 m_vTrailColor = D3DXVECTOR4(0.f, 0.f, 0.f, 0.f);

private:
	D3DXVECTOR3 m_vEnemyRightDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	JUMP_INFO mJumpInfo = {};
};

_END
#endif

