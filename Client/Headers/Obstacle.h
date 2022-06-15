#ifndef OBSTACLE_H__ 
#define OBSTACLE_H__

#include "Gameobject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class DynamicMesh;
	class Shader;
	class Collider;
	class Frustum;
	class Navigation;
}

_BEGIN(Client)

class PlayerAnim;
class CameraFPS;

class Obstacle final : public Engine::GameObject
{
private:
	explicit Obstacle(LPDIRECT3DDEVICE9 pDevice);
	explicit Obstacle(const Obstacle& rhs);
	virtual ~Obstacle() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

public:
	HRESULT ReadyPrototype(const size_t type, const D3DXVECTOR3& vPos);

private:
	HRESULT ReadyComponent(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	void AnimUpdate(const float timeDelta);
	void AnimStateChange(const float timeDelta);

private:
	void TwisterUpdate(const float timeDelta);
	void SnowBallUpdate(const float timeDelta);

private:
	HRESULT SnowBall_PathLoad(const TCHAR* pFilePath);

public:
	static Obstacle* Create(LPDIRECT3DDEVICE9 pDevice, const size_t type, const D3DXVECTOR3& vPos);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::DynamicMesh* m_pMeshObject = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::Collider* m_pColliderBox = nullptr;
	Engine::Collider* m_pColliderSphere = nullptr;
	Engine::Frustum* m_pFrustum = nullptr;
	Engine::Navigation* m_pNavigation = nullptr;
	Engine::Navigation* m_pPlayerNavigation = nullptr;

private:
	Engine::Transform* m_pPlayerTransform = nullptr;
	Engine::Collider* m_pPlayerCollider = nullptr;

private:
	UtilityMgr* m_pUtilityMgr = nullptr;

private:
	D3DXVECTOR3 m_vCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vFinal_CenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3* m_pCenterPos = nullptr;
	float mRadius = 0.f;
	float mScale = 0.f;
	const D3DXVECTOR3* m_pPos = nullptr;
	const D3DXVECTOR3* m_pPlayerPos = nullptr;
	size_t mType = OBSTACLE_TYPE_END;
	const TCHAR* m_pMeshTag = nullptr;

private: // Animation Related Variable
	ANIM_CTRL_INFO* m_pAnimCtrl_Info = nullptr;
	double mAnimTime = 0.0;
	string mAnimState = "";
	string mPre_AnimState = "";
	float mAnimSpeed = 0.f;

private: // IceTwister Related Variable
	D3DXMATRIX m_matParent;
	float mTwisterRot = 0.f;
	float mTwister_RevolutionValue = 0.f;
	float mTwisterRevolution = 0.f;
	D3DXMATRIX m_matRot;
	D3DXMATRIX m_matTrans;
	PlayerAnim* m_pPlayerAnim = nullptr;
	string* m_pPlayerAnimState = nullptr;

private: // SnowBall Related Variable;
	D3DXVECTOR3 m_vLineNormal = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vSnowDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3* m_pPlaneY = nullptr;
	float mSnowBallSpeed = 0.f;
	bool m_bSnowBall = false;
	bool m_bTwister = false;
	bool m_bGate = false;
	vector<D3DXVECTOR3> mSnowBallSpace;
	int mPathIndex = 0;
	int mPath_PreIndex = -1;
	int mPathSize = 0;

private:
	const size_t* m_pPlayerIndex = nullptr;
	CameraFPS* m_pCamerFPS = nullptr;
};

_END

#endif

