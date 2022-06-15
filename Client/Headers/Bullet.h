#ifndef BULLET_H__
#define BULLET_H__

#include "BulletParent.h"

_BEGIN(Client)

class Bullet final : public BulletParent
{
private:
	explicit Bullet(LPDIRECT3DDEVICE9 pDevice);
	explicit Bullet(const Bullet& rhs);
	virtual ~Bullet() = default;

public:
	virtual UPDATE Update(const float timeDelta) override;
	// BulletParent을(를) 통해 상속됨

private:
	void ParabolaAttack(const float timeDelta, const float velocity, const float sinDegree, const float cosDegree);
	void SniperAttack(const float timeDelta);
	void TargetReady(const float timeDelta);
	void ArrowAttack(const float timeDelta);
	void IceMissileAttack(const float timeDelta);
	void SniperCollision();
	void BoomCollision();
	void MissileCollision();
	void Enemy_AnimationSet(EnemyAI* pEnemyAI);
	void PlayerCollision(const float timeDelta);

public:
	static Bullet* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshName);
	virtual DWORD Free(void) override;
	virtual GameObject * BulletClone(size_t bulletType, const D3DXVECTOR3& vBulletDir = D3DXVECTOR3(0.f, 0.f, 0.f), const D3DXVECTOR3& vBulletPos = D3DXVECTOR3(0.f, 0.f, 0.f)) override;

private:
	enum BULLET_CONSTANT
	{
		SNIPER_RANGE = 6,
		BULLET_SPEED = 70,
		BULLET_DIE_DISTANCE = 130,
		ICE_MISSILE_SPEED = 100,
	};


private:
	const D3DXVECTOR3* m_pTargetPos = nullptr;
	D3DXVECTOR3 m_vTargetPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vTargetDir = D3DXVECTOR3(0.f, 0.f, 0.f);

private:
	bool m_bSniperShot = false;
	bool m_bCollision = false;
	bool m_bCross = false;

private:
	float mDistance;
	float mShortDistance = 100000.f;

private: //Arrow Related Variable
	size_t mAttack_SucessPercent = 0;
	size_t mAttack_SucessValue = 0;

private:
	float mIceMissile_Time = 0.f;
	float mMissileCollision_Time = 0.f;
	bool m_bBoomDie = false;


};

_END

#endif

