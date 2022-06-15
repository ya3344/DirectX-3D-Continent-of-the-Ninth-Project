#include "Transform.h"
#include "VIBuffer.h"

_USING(Engine);



Transform::Transform(LPDIRECT3DDEVICE9 pDevice)
	: Component(pDevice)
{
}

Transform::Transform(const Transform & rhs)
	: Component(rhs)
	, mMatrixInfo(rhs.mMatrixInfo)
{

}

HRESULT Transform::Init()
{
	return NOERROR;
}

UPDATE Transform::UpdateTransform()
{
	mMatrixInfo.matFinal = mMatrixInfo.matWorld * mMatrixInfo.matParnet;
	return UPDATE::NORMAL;
}

UPDATE Transform::Cal_NotRotMatrix()
{
	mMatrixInfo.matWorld_NotRot = mMatrixInfo.matWorld;

	// 회전상태를 제외한 행렬을 셋팅해놓자.
	D3DXVECTOR3 vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);

	vRight *= D3DXVec3Length((D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_RIGHT][0]);
	vUp *= D3DXVec3Length((D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_UP][0]);
	vLook *= D3DXVec3Length((D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_LOOK][0]);

	memcpy(&mMatrixInfo.matWorld_NotRot.m[INFO_RIGHT][0], &vRight, sizeof(D3DXVECTOR3));
	memcpy(&mMatrixInfo.matWorld_NotRot.m[INFO_UP][0], &vUp, sizeof(D3DXVECTOR3));
	memcpy(&mMatrixInfo.matWorld_NotRot.m[INFO_LOOK][0], &vLook, sizeof(D3DXVECTOR3));

	return UPDATE::NORMAL;
}

UPDATE Transform::Cal_FinalNotRot_Matrix()
{
	mMatrixInfo.matFinal_NotRot = mMatrixInfo.matFinal;

	// 회전상태를 제외한 행렬을 셋팅해놓자.
	D3DXVECTOR3 vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);

	vRight *= D3DXVec3Length((D3DXVECTOR3*)&mMatrixInfo.matFinal.m[INFO_RIGHT][0]);
	vUp *= D3DXVec3Length((D3DXVECTOR3*)&mMatrixInfo.matFinal.m[INFO_UP][0]);
	vLook *= D3DXVec3Length((D3DXVECTOR3*)&mMatrixInfo.matFinal.m[INFO_LOOK][0]);

	memcpy(&mMatrixInfo.matFinal_NotRot.m[INFO_RIGHT][0], &vRight, sizeof(D3DXVECTOR3));
	memcpy(&mMatrixInfo.matFinal_NotRot.m[INFO_UP][0], &vUp, sizeof(D3DXVECTOR3));
	memcpy(&mMatrixInfo.matFinal_NotRot.m[INFO_LOOK][0], &vLook, sizeof(D3DXVECTOR3));

	return UPDATE::NORMAL;
}

HRESULT Transform::SetUp_World()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &mMatrixInfo.matFinal);

	return NOERROR;
}

HRESULT Transform::SetUp_ParentMatrix(const D3DXMATRIX* pParentMatrix)
{
	mMatrixInfo.matParnet = *pParentMatrix;

	return NOERROR;
}

HRESULT Transform::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName, bool bComplexMesh)
{
	if (bComplexMesh)
	{
		pEffect->SetMatrix(pConstantName, &mMatrixInfo.matFinal);
	}
	else
	{
		pEffect->SetMatrix(pConstantName, &mMatrixInfo.matWorld);
	}
	
	return NOERROR;
}

D3DXVECTOR3 * Transform::GetInfo(INFO info)
{
	return (D3DXVECTOR3*)&mMatrixInfo.matWorld.m[info][0];
}

D3DXVECTOR3 Transform::GetScale()
{
	return D3DXVECTOR3(D3DXVec3Length((D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_RIGHT][0])
		, D3DXVec3Length((D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_UP][0])
		, D3DXVec3Length((D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_LOOK][0]));
}

void Transform::SetPos(const D3DXVECTOR3 & pVector)
{
	D3DXMatrixTranslation(&mMatrixInfo.matTrans, pVector.x, pVector.y, pVector.z);
	Combine_WorldMatrix();
	
}

void Transform::TargetStep(D3DXVECTOR3 vTargetPos, const float speedPerSec)
{
	D3DXVECTOR3 vPosition = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_POSITION][0];

	D3DXVECTOR3 vTargetDir = vTargetPos - vPosition;
	D3DXVec3Normalize(&vTargetDir, &vTargetDir);
	vPosition += vTargetDir * speedPerSec;

	D3DXMatrixTranslation(&mMatrixInfo.matTrans, vPosition.x, vPosition.y, vPosition.z);
	Combine_WorldMatrix();
}

void Transform::GoStraight(const float speedPerSec)
{
	D3DXVECTOR3 vPosition = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_POSITION][0];
	D3DXVECTOR3 vLook = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_LOOK][0];
	D3DXVec3Normalize(&vLook, &vLook);

	vPosition += vLook * speedPerSec;
	vPosition.y = m_pPlaneY->y;
	//cout << m_pPlaneY->y << endl;

	D3DXMatrixTranslation(&mMatrixInfo.matTrans, vPosition.x, vPosition.y, vPosition.z);
	Combine_WorldMatrix();
}

void Transform::Arrow_GoStraight(const float speedPerSec)
{
	D3DXVECTOR3 vPosition = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_POSITION][0];
	D3DXVECTOR3 vLook = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_LOOK][0];
	D3DXVec3Normalize(&vLook, &vLook);

	vPosition += vLook * speedPerSec;

	D3DXMatrixTranslation(&mMatrixInfo.matTrans, vPosition.x, vPosition.y, vPosition.z);
	Combine_WorldMatrix();
}

void Transform::RightStep(const float speedPerSec)
{
	D3DXVECTOR3 vPosition = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_POSITION][0];
	D3DXVECTOR3 vRight = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_RIGHT][0];
	D3DXVec3Normalize(&vRight, &vRight);

	vPosition += vRight * speedPerSec;
	vPosition.y = m_pPlaneY->y;

	D3DXMatrixTranslation(&mMatrixInfo.matTrans, vPosition.x, vPosition.y, vPosition.z);
	Combine_WorldMatrix();
}

void Transform::LeftStep(const float speedPerSec)
{
	D3DXVECTOR3 vPosition = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_POSITION][0];
	D3DXVECTOR3 vLeft = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_RIGHT][0] * -1.f;
	D3DXVec3Normalize(&vLeft, &vLeft);

	vPosition += vLeft * speedPerSec;
	vPosition.y = m_pPlaneY->y;

	D3DXMatrixTranslation(&mMatrixInfo.matTrans, vPosition.x, vPosition.y, vPosition.z);
	Combine_WorldMatrix();
}

void Transform::CalJump(D3DXVECTOR3* pJumpDir, const size_t jumpState, const float velocity, const float speedPerSec, const float sinDegree, const float cosDegree, const D3DXVECTOR3* pEnemyDir)
{
	D3DXVECTOR3 vDir = D3DXVECTOR3(0.f, 0.f, 0.f);

	switch (jumpState)
	{
	case JUMP_FORWARD:
		vDir = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_LOOK][0];
		break;
	case JUMP_BACK:
		vDir = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_LOOK][0];
		vDir *= -1.f;
		break;
	case JUMP_LEFT:
		vDir = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_RIGHT][0];
		vDir *= -1.f;
		break;
	case JUMP_RIGHT:
		vDir = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_RIGHT][0];
		break;
	case JUMP_DIR:
		if (pEnemyDir != nullptr)
		{
			vDir = *pEnemyDir;
		}
		break;
	default:
		vDir = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_LOOK][0];
		break;
	}

	D3DXVec3Normalize(&vDir, &vDir);
 
	pJumpDir->z = vDir.z * velocity * cosf(D3DXToRadian(cosDegree)) * speedPerSec;
	pJumpDir->x = vDir.x * velocity * cosf(D3DXToRadian(cosDegree)) * speedPerSec;
	pJumpDir->y = (velocity * sinf(D3DXToRadian(sinDegree)) * speedPerSec) - (0.5f  * 9.8f  * speedPerSec * speedPerSec);
}

void Transform::DirStep(const D3DXVECTOR3 & vDir, const bool bNotPlaneY)
{
	D3DXVECTOR3 vPosition = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_POSITION][0];

	vPosition += vDir;

	if (bNotPlaneY == false)
	{
		if (vPosition.y <= m_pPlaneY->y)
		{
			vPosition.y = m_pPlaneY->y - 0.1f;
		}
	}

	D3DXMatrixTranslation(&mMatrixInfo.matTrans, vPosition.x, vPosition.y, vPosition.z);
	Combine_WorldMatrix();

}

void Transform::SnowBallStep(const D3DXVECTOR3 & vDir)
{
	D3DXVECTOR3 vPosition = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_POSITION][0];

	vPosition += vDir;
	vPosition.y = m_pPlaneY->y - 1.f;

	D3DXMatrixTranslation(&mMatrixInfo.matTrans, vPosition.x, vPosition.y, vPosition.z);
	Combine_WorldMatrix();
}

void Transform::Gravity(const float speedPerSec)
{
	D3DXVECTOR3 vPosition = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_POSITION][0];
	vPosition.y -=  speedPerSec;

	if (vPosition.y <= m_pPlaneY->y)
	{
		vPosition.y = m_pPlaneY->y;
	}

	D3DXMatrixTranslation(&mMatrixInfo.matTrans, vPosition.x, vPosition.y, vPosition.z);
	Combine_WorldMatrix();
}

void Transform::SetScale(const D3DXVECTOR3& vScale)
{
	D3DXMatrixScaling(&mMatrixInfo.matScale, vScale.x, vScale.y, vScale.z);
	Combine_WorldMatrix();
}

void Transform::SetRotation(const D3DXVECTOR3 & vRot)
{
	m_vRot += vRot;
	D3DXQuaternionRotationYawPitchRoll(&mQuatRot, D3DXToRadian(m_vRot.y), D3DXToRadian(m_vRot.x), D3DXToRadian(m_vRot.z));
	D3DXMatrixRotationQuaternion(&mMatrixInfo.matRotation, &mQuatRot);
	Combine_WorldMatrix();
}



void Transform::Set_FixRotation(const D3DXVECTOR3 & vRot)
{
	m_vRot = vRot;
	D3DXQuaternionRotationYawPitchRoll(&mQuatRot, D3DXToRadian(vRot.y), D3DXToRadian(m_vRot.x), D3DXToRadian(m_vRot.z));
	D3DXMatrixRotationQuaternion(&mMatrixInfo.matRotation, &mQuatRot);
	Combine_WorldMatrix();
}

void Transform::SetInfo(INFO info, const D3DXVECTOR3* pVector)
{
	memcpy(&mMatrixInfo.matWorld.m[info][0], pVector, sizeof(D3DXVECTOR3));
}

void Transform::Combine_WorldMatrix()
{
	mMatrixInfo.matWorld = mMatrixInfo.matScale * mMatrixInfo.matRotation * mMatrixInfo.matTrans;

	if (m_bUpdate_CenterPos == true)
	{
		D3DXVec3TransformCoord(&m_vCenter_FinalPos, &m_vCenterPos, &mMatrixInfo.matWorld);
	}
	
}

HRESULT Transform::ComputeY_OnBuffer(Engine::VIBuffer * pBuffer)
{
	const D3DXVECTOR3* pVtxPos = pBuffer->Get_VtxPos();

	if (pVtxPos == nullptr)
		return E_FAIL;

	float interval = 1.f;

	size_t numX = pBuffer->Get_VtxInfo().vtxNumX;

	D3DXVECTOR3 vPosition = *(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_POSITION][0];

	size_t currentIdx = size_t(vPosition.z / interval) * numX + size_t(vPosition.x / interval);

	float ratioX = (vPosition.x - pVtxPos[currentIdx + numX].x) / interval;
	float ratioZ = (pVtxPos[currentIdx + numX].z - vPosition.z) / interval;

	size_t indices[4] = { currentIdx + numX, currentIdx + numX + 1, currentIdx + 1, currentIdx };

	if (ratioX > ratioZ) // 오른쪽 위 삼각형에 있을때.
	{
		
		vPosition.y = pVtxPos[indices[0]].y + (pVtxPos[indices[1]].y - pVtxPos[indices[0]].y) * ratioX
			+ (pVtxPos[indices[2]].y - pVtxPos[indices[1]].y) * ratioZ;
	}
	else // 왼쪽 하단 삼각형안에 있을때.
	{
		vPosition.y = pVtxPos[indices[0]].y + (pVtxPos[indices[3]].y - pVtxPos[indices[0]].y) * ratioZ
			+ (pVtxPos[indices[2]].y - pVtxPos[indices[3]].y) * ratioX;
	}

	D3DXMatrixTranslation(&mMatrixInfo.matTrans, vPosition.x, vPosition.y, vPosition.z);
	Combine_WorldMatrix();

	return NOERROR;
}

void Transform::Combine_WorldMatrix(const D3DXMATRIX & matrix)
{
	mMatrixInfo.matWorld = mMatrixInfo.matWorld * matrix;
	
}

Transform * Transform::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Transform*	pInstance = new Transform(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Transform Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * Transform::Clone()
{
	return new Transform(*this);
}

DWORD Transform::Free(void)
{
	Component::Free();
	return 0;
}
