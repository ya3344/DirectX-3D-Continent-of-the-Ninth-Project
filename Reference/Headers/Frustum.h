#ifndef FRUSTUM_H__
#define FRUSTUM_H__

#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL Frustum final : public Component
{
private:
	explicit Frustum(LPDIRECT3DDEVICE9 pDevice);
	virtual ~Frustum() = default;

public:
	HRESULT Init();
	HRESULT Transform_WorldSpace();
	bool FrustumColling(const D3DXVECTOR3* pWorldPos, const float radius = 0.f);
	const D3DXVECTOR3* Get_CamPos() const { return &m_vCamPos; }

public:
	static Frustum* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Component* Clone();
	virtual DWORD Free();

private:
	D3DXVECTOR3 m_vPoint[8];
	D3DXPLANE mPlane[6];
	D3DXVECTOR3 m_vCamPos = D3DXVECTOR3(0.f, 0.f, 0.f);


};

_END

#endif

