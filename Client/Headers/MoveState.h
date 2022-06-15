#ifndef MOVE_STATE_H__
#define MOVE_STATE_H__

#include "EnemyState.h"

_BEGIN(Client)

class MoveState final : public EnemyState
{
private:
	explicit MoveState();
	virtual ~MoveState() = default;

public:
	virtual void AIState(const float timeDelta) override;
	virtual void AISet() override;
	virtual void AI_StateChange(const float timeDelta) override;
	virtual void AIUpdate(const float timeDelta) override;
	virtual void Collision(const float timeDelta) override;

private:
	void CalWalk(const float timeDelta);
	void CalChase(const float timeDelta);
	void MoveDir(const float timeDelta);
	void Cal_SlidngVector();

public:
	static MoveState* Create();
	virtual DWORD Free() override;

private:
	enum ENEMY_VALUE
	{
		OPERATION_RANGE = 250,
		DETECT_RANGE = 45,
		DETECT_SHORT_RANGE = 20,
		DETECT_BOW_RANGE = 50,
		DETECT_BOW_AWAY_RANGE = 20,
		DETECT_BOSS_RANGE = 110, // 100
		DETECT_BOSS_SHORT_RANGE = 55, // 50
		MOVE_SPEED = 12,
		MOVE_WALK = 9,
	};

private:
	

private:
	size_t mRandomMove_Speed = 0;
	
	size_t mRandomChase_Speed = 0;

private:
	float mMoveTime = 0.f;
	float mRandomTime = 0.f;

	float mRangeAssitant = 0.f;
	float mRange = 0.f;
	float mShortRange = 0.f;
	float mPushDistance = 0.f;

private:
	bool m_bRotation = false;
	size_t mSelectMode = 0;

private:
	bool m_bBossInit = false;
	float mBossTime = 0.f;


};

_END

#endif

