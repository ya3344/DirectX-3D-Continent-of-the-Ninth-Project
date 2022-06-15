#ifndef PLAYER_H__ 
#define PLAYER_H__

#include "Gameobject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class DynamicMesh;
	class Shader;
	class Collider;
	class Navigation;
	
}

_BEGIN(Client)

class PlayerAnim;
class Trail;

class Player final : public Engine::GameObject
{
private:
	explicit Player(LPDIRECT3DDEVICE9 pDevice);
	explicit Player(const Player& rhs);
	virtual ~Player() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT ReadyPrototype() override;

public:
	PLAYER_INFO* Get_PlayerInfo() { return &mPlayerInfo; }

public:
	void SetEXP(const float EXP);
	void SetHP(const float HP);
	void SetMP(const float MP);


private:
	HRESULT ReadyComponent(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static Player* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private:
	PlayerAnim* m_pAnim = nullptr;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::DynamicMesh* m_pMesh = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::Collider* m_pCollider = nullptr;
	Engine::Navigation* m_pNavigation = nullptr;

private:
	D3DXVECTOR3 m_vCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	float mRadius = 0.f;

private:
	PLAYER_INFO mPlayerInfo;
	string* m_pAnimState = nullptr;
	Trail* m_pTrail = nullptr;
};

_END

#endif

