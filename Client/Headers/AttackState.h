#ifndef ATTACK_STATE_H__
#define ATTACK_STATE_H__

#include "EnemyState.h"

_BEGIN(Client)

class AttackState final : public EnemyState
{
private:
	explicit AttackState();
	virtual ~AttackState() = default;

public:
	virtual void AIState(const float timeDelta) override;
	virtual void AISet() override;
	virtual void AI_StateChange(const float timeDelta) override;
	virtual void AIUpdate(const float timeDelta) override;
	virtual void Collision(const float timeDelta) override;

private:
	void CollisionEX(const float timeDelta);

private:
	void Boss_ComboSet();
	void Attack_DefaultSet();

public:
	static AttackState* Create();
	virtual DWORD Free() override;

private:
	size_t mAttackPercent = 0;
	size_t mAttackValue = 0;
	size_t mAttack_SucessPercent = 0;
	size_t mAttack_CriticalPercent = 0;
	float mAttackRange = 0.f;

private:
	bool m_bArrow = false;
	double mAttackTime = 0.0;
	bool m_bAttack = false;
	JUMP_INFO mJumpInfo;
	size_t mTextureNum = 0;
};

_END

#endif

