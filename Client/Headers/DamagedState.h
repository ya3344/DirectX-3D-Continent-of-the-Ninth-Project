#ifndef DAMAGED_STATE_H__
#define DAMAGED_STATE_H__

#include "EnemyState.h"

_BEGIN(Client)

class DamagedState final : public EnemyState
{
private:
	explicit DamagedState();
	virtual ~DamagedState() = default;

public:
	virtual void AIState(const float timeDelta) override;
	virtual void AISet() override;
	virtual void AI_StateChange(const float timeDelta) override;
	virtual void AIUpdate(const float timeDelta) override;
	virtual void Collision(const float timeDelta) override;

private:
	void AIChange();
	void AI_Init();

public:
	static DamagedState* Create();
	virtual DWORD Free() override;

private:
	float mPushSpeed = 0.f;

private:
	size_t mForceAttack_Percent = 0;
	size_t mForceAttack_Value = 0;
	float mDamaged_Value = 0.f;

private:
	D3DXVECTOR3* pBonePos = nullptr;
	bool m_bRealDie = false;
	float mStunSpeed = 0.f;
	float mDamagedTime = 0.f;

};

_END

#endif

