#include "Frustum.h"

_USING(Engine)

Frustum::Frustum(LPDIRECT3DDEVICE9 pDevice)
	:Component(pDevice)
{
}

HRESULT Frustum::Init()
{
	// In.Projection Space 절두체를 만든다
	m_vPoint[0] = D3DXVECTOR3(-1.f, 1.f, 0.f);
	m_vPoint[1] = D3DXVECTOR3(1.f, 1.f, 0.f);
	m_vPoint[2] = D3DXVECTOR3(1.f, -1.f, 0.f);
	m_vPoint[3] = D3DXVECTOR3(-1.f, -1.f, 0.f);

	m_vPoint[4] = D3DXVECTOR3(-1.f, 1.f, 1.f);
	m_vPoint[5] = D3DXVECTOR3(1.f, 1.f, 1.f);
	m_vPoint[6] = D3DXVECTOR3(1.f, -1.f, 1.f);
	m_vPoint[7] = D3DXVECTOR3(-1.f, -1.f, 1.f);

	return NOERROR;
}

HRESULT Frustum::Transform_WorldSpace()
{
	D3DXVECTOR3 vPoint[8];
	memcpy(vPoint, m_vPoint, sizeof(D3DXVECTOR3) * 8);

	D3DXMATRIX matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	m_vCamPos = D3DXVECTOR3(&matView.m[3][0]);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	// 바로 월드 스페이스로 변환한다.
	for (size_t i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj); // 뷰스페이스로
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView); // 월드스페이스로
	}

	// +x
	D3DXPlaneFromPoints(&mPlane[0], &vPoint[1], &vPoint[5], &vPoint[6]);

	// -x
	D3DXPlaneFromPoints(&mPlane[1], &vPoint[4], &vPoint[0], &vPoint[3]);

	// +y
	D3DXPlaneFromPoints(&mPlane[2], &vPoint[4], &vPoint[5], &vPoint[1]);

	// -y
	D3DXPlaneFromPoints(&mPlane[3], &vPoint[3], &vPoint[2], &vPoint[6]);

	// +z
	D3DXPlaneFromPoints(&mPlane[4], &vPoint[5], &vPoint[4], &vPoint[7]);

	// -z
	D3DXPlaneFromPoints(&mPlane[5], &vPoint[0], &vPoint[1], &vPoint[2]);

	return NOERROR;
}

bool Frustum::FrustumColling(const D3DXVECTOR3* pWorldPos, const float radius)
{
	// 한번이라도 평면 밖에 있냐 ? !
	/*ax + by + cz + d > 0
	m_Plane[0].a * vWorldPos.x + m_Plane[0].b * vWorldPos.y*/
	for (size_t i = 0; i < 6; i++)
	{
		if (radius < D3DXPlaneDotCoord(&mPlane[i], pWorldPos))
			return false;
	}

	return true;
}

Frustum * Frustum::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Frustum* pInstance = new Frustum(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Frustum Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * Frustum::Clone()
{
	AddRef();

	return this;
}

DWORD Frustum::Free()
{
	Component::Free();

	return DWORD();
}
