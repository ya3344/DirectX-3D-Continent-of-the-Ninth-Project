#ifndef STRUCT_H__
#define STRUCT_H__

typedef struct tag_EnemyAI_Info
{
	tag_EnemyAI_Info() {};
	explicit tag_EnemyAI_Info(size_t enemyType, bool* pForceAttack, bool* pAttack, bool* pDamaged, int* pEnemy_AIMode, float* pHP, bool* pDie, size_t* pNaviMinIndex, size_t* pNaviMaxIndex, size_t* pBoss_HPCount)
	{
		this->enemyType = enemyType;
		this->pForecAttack = pForceAttack;
		this->pAttack = pAttack;
		this->pDamaged = pDamaged;
		this->pEnemy_AIMode = pEnemy_AIMode;
		this->pHP = pHP;
		this->pDie = pDie;
		this->pNaviMinIndex = pNaviMinIndex;
		this->pNaviMaxIndex = pNaviMaxIndex;
		this->pBoss_HPCount = pBoss_HPCount;
	}
	
	size_t enemyType = 0;
	size_t* pNaviMinIndex = nullptr;
	size_t* pNaviMaxIndex = nullptr;
	bool* pForecAttack = nullptr;
	bool* pAttack = nullptr;
	bool* pDamaged = nullptr;
	int* pEnemy_AIMode = nullptr;
	float* pHP = nullptr;
	bool* pDie = nullptr;
	size_t* pBoss_HPCount = nullptr;

}ENEMY_AI_INFO;

typedef struct tag_StaticObjectInfo
{
	tag_StaticObjectInfo()
	{
		D3DXMatrixIdentity(&m_matWorld);
	}
	tag_StaticObjectInfo(const size_t objectType, const D3DXMATRIX& m_matWorld)
	{
		this->objectType = objectType;
		this->m_matWorld = m_matWorld;
	}

	size_t objectType = 0;
	D3DXMATRIX m_matWorld;
	D3DXVECTOR3 m_vRot = D3DXVECTOR3(0.f, 0.f, 0.f);

}STATIC_OBJECT_INFO;


typedef struct tagUIInfo
{
	tagUIInfo() {};
	explicit tagUIInfo(const float X, const float Y, const float width, const float height, const size_t type = UI_NORMAL)
	{
		this->X = WIN_SIZEX * X;
		this->Y = WIN_SIZEY * Y;
		this->width = WIN_SIZEX * width;
		this->height = WIN_SIZEY * height;
		this->type = type;
	}

	float X = 0.f;
	float Y = 0.f;
	float width = 0.f;
	float height = 0.f;
	size_t type = 0;

}UI_INFO;


typedef struct tagPlayerInfo
{
	tagPlayerInfo() {}
	explicit tagPlayerInfo(const float HP, const float MP, const int level, const float EXP)
	{
		this->HP = HP;
		this->MP = MP;
		this->level = level;
		this->EXP = EXP;
	}

	float HP = 0;
	float MP = 0;
	int level = 0;
	int preLevel = 0;
	float EXP = 0.f;
	float totalEXP = 0.f;
	bool bLevelUP = false;
	bool bLevelUpBack = false;

}PLAYER_INFO;

typedef struct tagJumpInfo
{
	tagJumpInfo() {}
	explicit tagJumpInfo(const float jumpVelocity, const float sinDegree, const float cosDegree, const size_t jumpState)
	{
		this->jumpVelocity = jumpVelocity;
		this->sinDegree = sinDegree;
		this->cosDegree = cosDegree;
		this->jumpState = jumpState;
	}

	float jumpVelocity = 0.f;
	float sinDegree = 0.f;
	float cosDegree = 0.f;
	size_t jumpState = JUMP_STATE_END;

}JUMP_INFO;

typedef struct Effect_InitInfo
{
	Effect_InitInfo() {}
	explicit Effect_InitInfo(const TCHAR* pTextureName, const size_t imageSizeX, const size_t imageSizeY, const size_t imageNumX, const size_t imageNumY, const float frameSpeed, const bool bForver = false)
	{
		this->pTextureName = pTextureName;
		this->imageSizeX = imageSizeX;
		this->imageSizeY = imageSizeY;
		this->imageNumX = imageNumX;
		this->imageNumY = imageNumY;
		this->frameSpeed = frameSpeed;
		this->bForever = bForever;
	}

	const TCHAR* pTextureName = nullptr;
	size_t imageSizeX = 0;
	size_t imageSizeY = 0;
	size_t imageNumX = 0;
	size_t imageNumY = 0;
	float frameSpeed = 0.f;
	bool bForever = false;

	int frameX = 0;
	int frameY = 0;
	float UV_PercenTageX = 0.f;
	float UV_PercentageY = 0.f;

}EFFECT_INIT_INFO;

typedef struct Effect_Info
{
	Effect_Info() {}
	explicit Effect_Info(const D3DXVECTOR3* pPos, const D3DXVECTOR3& vScale, const size_t effectType , const bool* pDie = nullptr)
	{
		this->pPos = pPos;
		this->vScale = vScale;
		this->effectType = effectType;
		this->pDie = pDie;
	}

	const D3DXVECTOR3* pPos = nullptr;
	const bool* pDie = nullptr;
	D3DXVECTOR3 vScale = { 0.f, 0.f, 0.f };
	size_t effectType = 0;

}EFFECT_INFO;

#endif