#ifndef CAMERA_FPS_H__
#define CAMERA_FPS_H__

#include "Camera.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Navigation;
	class DynamicMesh;
}

_BEGIN(Client)

class PlayerAnim;

class CameraFPS final : public Engine::Camera
{
private:
	explicit CameraFPS(LPDIRECT3DDEVICE9 pDevice);
	explicit CameraFPS(const CameraFPS& rhs);
	virtual ~CameraFPS() = default;

public:
	virtual HRESULT Init() override;
	virtual HRESULT ReadyPrototype(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, const D3DXVECTOR3& vAxisY,
		const float fovY, const float aspect, const float projNear, const float projFar) override;

	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	
public:
	void SetCameraMode(size_t cameraMode) { mCameraMode = cameraMode; }
	void SetBossPos(const D3DXVECTOR3* pBossPos) { m_pBossPos = pBossPos; }
	void SetBossDir(const D3DXVECTOR3* pBossDir) { m_pBossDir = pBossDir; }

private:
	D3DXVECTOR2 Cal_IntersectPoint(const D3DXVECTOR2& lineA, const D3DXVECTOR2& lineB, const D3DXVECTOR2& lineC, const D3DXVECTOR2& lineD);


private:
	void NormalMode(const D3DXVECTOR3* pTargetDir, const float dir = -1.f);
	void GateMode(const D3DXVECTOR3* pTargetDir);
	void BossActionMode();
	void LockOnTarget_Mode();
	inline void Cal_CameraFPS();

public:
	static CameraFPS* Create(LPDIRECT3DDEVICE9 pDevce,
		const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, const D3DXVECTOR3& vAxisY,
		const float fovY = D3DXToRadian(60.0f), const float aspect = float(WIN_SIZEX) / WIN_SIZEY, const float projNear = 0.2f, const float projFar = 1000.f); // 투영행렬을 만들기 위한 정보.

	virtual GameObject * Clone() override;
	virtual DWORD Free(void) override;

private:
	enum CAMER_CONSTANT
	{
		CAMERA_DISTANCE = 35, //25 
		NAVI_DISTANCE= 15, // 15
		CAMERA_Y = 17, // 13 
		CAMERA_FPS_DISTANCE = 15, //25 45
		CAMERA_FPS_Y = 13, // 13 20
	};

private:
	Engine::Transform* m_pPlayerTransform = nullptr;
	PlayerAnim* m_pPlayerAnim = nullptr;
	Engine::Navigation* m_pNavigation = nullptr;
	Engine::Navigation* m_pPlayerNavigation = nullptr;
	UtilityMgr* m_pUtilityMgr = nullptr;

private:
	const D3DXVECTOR3* m_pPlayerDir = nullptr;
	const D3DXVECTOR3* m_pPlayerPos = nullptr;
	const D3DXVECTOR3* m_pPlayerRight = nullptr;
	D3DXMATRIX m_matRot;
	D3DXMATRIX* m_pHandMatrix = nullptr;
	Engine::DynamicMesh* m_pPlayerMesh = nullptr;

private:
	D3DXVECTOR3 m_vCameraPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vPlayer_PrePos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vTargetDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vNaviPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	float mCameraDistance = 0.f;

private:
	float mAngle = 0.f;
	bool m_bModeChange = false;

private:
	D3DXVECTOR2 m_vLineA = D3DXVECTOR2(0.f, 0.f);
	D3DXVECTOR2 m_vLineB = D3DXVECTOR2(0.f, 0.f);
	D3DXVECTOR3 m_vLIneNoraml = D3DXVECTOR3(0.f, 0.f, 0.f);
	bool m_bIntersect = false;

private:
	 string* m_pAnimState = nullptr;
	 const double* m_pAnimTime = nullptr;

private:
	D3DXVECTOR3 m_vActionEye = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vActionAt = D3DXVECTOR3(0.f, 0.f, 0.f);
	const size_t* m_pPlayerIndex = nullptr;
	size_t mCameraMode = 0;

private:
	float mCameraY = 0.f;
	float mPlayerDistance = 0.f;
	const D3DXVECTOR3* m_pBossPos = nullptr;
	const D3DXVECTOR3* m_pBossDir = nullptr;

};

_END

#endif

