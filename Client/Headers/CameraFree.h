#ifndef CAMERA_FREE_H__
#define CAMERA_FREE_H__

#include "Camera.h"
#include "Include.h"

namespace Engine
{
	class Transform;
}

_BEGIN(Client)

class CameraFree final : public Engine::Camera
{
private:
	explicit CameraFree(LPDIRECT3DDEVICE9 pDevice);
	explicit CameraFree(const CameraFree& rhs);
	virtual ~CameraFree() = default;

public:
	virtual HRESULT Init() override;
	virtual HRESULT ReadyPrototype(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, const D3DXVECTOR3& vAxisY,
		const float fovY, const float aspect, const float projNear, const float projFar) override;

	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;

public:
	static CameraFree* Create(LPDIRECT3DDEVICE9 pDevce,
		const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, const D3DXVECTOR3& vAxisY,
		const float fovY = D3DXToRadian(60.0f), const float aspect = float(WIN_SIZEX) / WIN_SIZEY, const float projNear = 0.2f, const float projFar = 1000.f); // ��������� ����� ���� ����.

	virtual GameObject * Clone() override;
	virtual DWORD Free(void) override;

private:
	Engine::Transform* m_pPlayerTransform = nullptr;
};

_END
#endif // CAMERA_FREE_H__

