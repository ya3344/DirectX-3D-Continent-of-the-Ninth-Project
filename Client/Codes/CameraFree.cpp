#include "stdafx.h"
#include "CameraFree.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"

_USING(Client)

CameraFree::CameraFree(LPDIRECT3DDEVICE9 pDevice)
	: Camera(pDevice)
{
}


CameraFree::CameraFree(const CameraFree& rhs)
	: Camera(rhs)
{

	mViewInfo.vEye = D3DXVECTOR3(-122.f, 38.f, 273.f);

	m_pPlayerTransform = (Engine::Transform*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Transform", L"Prototype_GameObject_Player");
	if (m_pPlayerTransform != nullptr)
	{
		mViewInfo.vEye = *m_pPlayerTransform->GetInfo(Engine::Transform::INFO_POSITION);
		mViewInfo.vEye.y += 30.f;
	}

}

HRESULT CameraFree::Init()
{
	return E_NOTIMPL;
}

HRESULT CameraFree::ReadyPrototype(const D3DXVECTOR3 & vEye, const D3DXVECTOR3 & vAt, const D3DXVECTOR3 & vAxisY, const float fovY, const float aspect, const float projNear, const float projFar)
{
	if (FAILED(Camera::ReadyPrototype(vEye, vAt, vAxisY, fovY, aspect, projNear, projFar)))
		return E_FAIL;

	return NOERROR;
}

UPDATE CameraFree::Update(const float timeDelta)
{
	if (m_pKeyMgr == nullptr)
		return UPDATE::FAIL;

	float keySpeed = 100.f;
	float mouseSpeed = 15.f;
	// ÀüÁø
	if (m_pKeyMgr->KeyPressing(DIK_I))
	{
		D3DXVECTOR3 vLook;
		memcpy(&vLook, &m_matWorld.m[2][0], sizeof(D3DXVECTOR3));

		mViewInfo.vEye += *D3DXVec3Normalize(&vLook, &vLook) * keySpeed * timeDelta;
		mViewInfo.vAt += *D3DXVec3Normalize(&vLook, &vLook) * keySpeed * timeDelta;
	}

	if (m_pKeyMgr->KeyPressing(DIK_K))
	{
		D3DXVECTOR3	vLook;
		memcpy(&vLook, &m_matWorld.m[2][0], sizeof(D3DXVECTOR3));

		mViewInfo.vEye += *D3DXVec3Normalize(&vLook, &vLook) * -keySpeed * timeDelta;
		mViewInfo.vAt += *D3DXVec3Normalize(&vLook, &vLook) * -keySpeed * timeDelta;
	}

	if (m_pKeyMgr->KeyPressing(DIK_J))
	{
		D3DXVECTOR3	vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

		mViewInfo.vEye += *D3DXVec3Normalize(&vRight, &vRight) * -keySpeed * timeDelta;
		mViewInfo.vAt += *D3DXVec3Normalize(&vRight, &vRight) * -keySpeed * timeDelta;
	}

	if (m_pKeyMgr->KeyPressing(DIK_L))
	{
		D3DXVECTOR3		vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

		mViewInfo.vEye += *D3DXVec3Normalize(&vRight, &vRight) * keySpeed * timeDelta;
		mViewInfo.vAt += *D3DXVec3Normalize(&vRight, &vRight) * keySpeed * timeDelta;
	}

	D3DXVECTOR3	vLook;
	memcpy(&vLook, &m_matWorld.m[2][0], sizeof(D3DXVECTOR3));

	long moveValue = 0;

	if (moveValue = m_pKeyMgr->MouseMove(Engine::KeyMgr::VERTICAL))
	{
		D3DXVECTOR3 vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

		D3DXMATRIX matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(moveValue * timeDelta) * mouseSpeed);

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		mViewInfo.vAt = mViewInfo.vEye + vLook;
	}

	if (moveValue = m_pKeyMgr->MouseMove(Engine::KeyMgr::HORIZONTAL))
	{
		D3DXMATRIX matRot;

		D3DXMatrixRotationY(&matRot, D3DXToRadian(moveValue * timeDelta) * mouseSpeed);

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		mViewInfo.vAt = mViewInfo.vEye + vLook;
	}


	return UPDATE::NORMAL;
}

UPDATE CameraFree::LastUpdate(const float timeDelta)
{
	Camera::LastUpdate(timeDelta);

	return UPDATE::NORMAL;
}

CameraFree * CameraFree::Create(LPDIRECT3DDEVICE9 pDevce, const D3DXVECTOR3 & vEye, const D3DXVECTOR3 & vAt, const D3DXVECTOR3 & vAxisY, const float fovY, const float aspect, const float projNear, const float projFar)
{
	CameraFree* pInstance = new CameraFree(pDevce);

	if (FAILED(pInstance->ReadyPrototype(vEye, vAt, vAxisY, fovY, aspect, projNear, projFar)))
	{
		_MSG_BOX("Camera_Free Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * CameraFree::Clone()
{
	return new CameraFree(*this);
}

DWORD CameraFree::Free(void)
{
	Camera::Free();

	return DWORD();
}