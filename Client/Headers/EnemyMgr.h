#ifndef ENEMY_MGR_H__
#define ENEMY_MGR_H__

#include "Base.h"
#include "Include.h"

namespace Engine
{
	class Collider;
	class LightManager;
}

_BEGIN(Client)

class Player;

typedef struct tagEnemyTypeCount
{
	explicit tagEnemyTypeCount(const size_t babegaziWarrior = 0, const size_t babegaziAxe = 0, const size_t babegaziBow = 0,
		const size_t knoleCommander = 0, const size_t knoleWarrior = 0)
	{
		this->babegaziWarrior = babegaziWarrior;
		this->babegaziAxe = babegaziAxe;
		this->babegaziBow = babegaziBow;
		this->knoleCommander = knoleCommander;
		this->knoleWarrior = knoleWarrior;
	}

	size_t babegaziWarrior = 0;
	size_t babegaziAxe = 0;
	size_t babegaziBow = 0;
	size_t knoleCommander = 0;
	size_t knoleWarrior = 0;

}ENEMY_TYPE_COUNT;

typedef struct tagEnemySpecialCount
{
	size_t mAState = 0;
	size_t mBState = 0;
	size_t mCState = 0;
}ENEMY_SPECIAL_COUNT;

class EnemyAI;
class SoundMgr;

class EnemyMgr final : public Engine::Base
{
public:
	_DECLARE_SINGLETON(EnemyMgr);

private:
	explicit EnemyMgr();
	virtual ~EnemyMgr() = default;

public:
	HRESULT CrateEnemy(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pLayerTag);

public:
	void Enemy_AttackCount();
	void EnemyDelete();
	bool Enemy_CollisionEX(Engine::Collider* pTargetCollider, D3DXVECTOR3* pPush);
	EnemyAI* Enemy_WeaponCollision(Engine::Collider* pWeaponCollider, Engine::Collider* pWeapon_SphereCollider, int playerSkill, double animTime = 0.0);
	bool Enemy_CollisionAABB(Engine::Collider* pTargetCollider, D3DXVECTOR3* pRightDir);
	void SetUp_DissolveTextrue(LPD3DXEFFECT pEffect, const char* pNameTag);

public:
	const list<EnemyAI*>* Get_EnemyAISpace() const { return &mEnemyAI_Space; }
	size_t Get_AttackCount() const { return mAttackCount; }



private:
	void Set_EnemyAIMode(const int playerSkill, EnemyAI* pEnemyAI, double animTime);
	void PlayerGrill_SkillOperation(EnemyAI* pEnemyAI);
	void PlayerShelTechnic_SkillOperation(EnemyAI* pEnemyAI, double aniTime);
	void PlayerSpinCursh(EnemyAI* pEnemyAI);
	void PlayerJumpAttack(EnemyAI* pEnemyAI);
	void PlayerRisingAttack(EnemyAI* pEnemyAI);
	void Init_EnemyState(EnemyAI* pEnemyAI);

private:
	void EnemyGeneration(mt19937 mtRand, const ENEMY_TYPE_COUNT enemyTypeCount, const size_t naviMin, const size_t naviMax);
	void EnemySpecialIndex(const size_t specialIndex);


public:
	virtual DWORD Free() override;

private:
	enum ENEMY_GENERATION_COUNT
	{
		A_SECTOR_NUM = 5,
		B_SECTOR_ACCESS_NUM = 4,
		B_SECTOR_NUM = 5,
		C_SECTOR_NUM = 4,
		C_SECTOR_AFTER_NUM = 4,
		DOOR_SECTOR_NUM = 15,
		BOSS_SECTOR_NUM = 10,
	
	};

private:
	vector<wstring> mProtoName_Space;
	TCHAR mComponentName_Buf[MAX_PATH] = L"";
	TCHAR mProtoName_Buf[MAX_PATH] = L"";

private:
	size_t mEnemyType = 0;
	size_t mNaviIndex = 0;
	size_t mNaviMinIndex = 0;
	size_t mNaviMaxIndex = 0;

private:
	int mEnemy_AIMode = -1;
	size_t mDamagedPercent = 0;
	size_t mSkillPercent = 0;
	size_t mAttackCount = 0;
	
private:
	list<EnemyAI*> mEnemyAI_Space;
	ENEMY_TYPE_COUNT mEnemyTypeCount;
	size_t mSpecialIndex = 0;
	ENEMY_SPECIAL_COUNT mSpecialCount;
	Engine::LightManager* m_pLightMgr = nullptr;
	SoundMgr* m_pSoundMgr = nullptr;

private:
	LPDIRECT3DTEXTURE9 m_pDissolveTex = nullptr;

private:
	Player* m_pPlayer = nullptr;

};

_END
#endif

