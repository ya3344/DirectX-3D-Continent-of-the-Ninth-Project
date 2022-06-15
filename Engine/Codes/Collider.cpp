#include "Collider.h"
#include "KeyMgr.h"

_USING(Engine)

Collider::Collider(LPDIRECT3DDEVICE9 pDevice)
	:Component(pDevice)
{
}

Collider::Collider(const Collider & rhs)
	:Component(rhs)
	, mType(rhs.mType)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pTargetMatrix(rhs.m_pTargetMatrix)
{
	m_pAdjacency->AddRef();

	if (rhs.m_pMesh != nullptr)
	{
		if (FAILED(rhs.m_pMesh->CloneMeshFVF(rhs.m_pMesh->GetOptions(), rhs.m_pMesh->GetFVF(), m_pDevice, &m_pMesh)))
			return;
		//D3DVERTEXELEMENT9 vtxElement[] =
		//{
		//	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		//	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		//	{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		//	{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		//	D3DDECL_END()
		//};

		//if (FAILED(rhs.m_pMesh->CloneMesh(rhs.m_pMesh->GetOptions(), vtxElement, m_pDevice, &m_pMesh)))
		//	return;
	}
}

HRESULT Collider::Init(TYPE type)
{
	HRESULT hr = 0;

	mType = type;

	// 원점에 위치한 사이즈 1짜리 콜라이더를 생성하자.
	switch (mType)
	{
	case TYPE_BOX:
		hr = Init_BoundingBox();
		break;
	case TYPE_SPHERE:
		hr = Init_BoundingSphere();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	return NOERROR;
}

bool Collider::Collision_EX(const Collider * pTargetCollider, D3DXVECTOR3* vPush)
{
	float width = (m_matScale._11 * 0.5f) + (pTargetCollider->m_matScale._11 * 0.5f);
	float depth = (m_matScale._33 * 0.5f) + (pTargetCollider->m_matScale._33 * 0.5f);

	D3DXVec3TransformCoord(&mColliderInfo.vSourMin, &mMeshInfo.vMin, m_pTargetMatrix);
	D3DXVec3TransformCoord(&mColliderInfo.vSourMax, &mMeshInfo.vMax, m_pTargetMatrix);

	D3DXVec3TransformCoord(&mColliderInfo.vDestMin, &pTargetCollider->mMeshInfo.vMin, pTargetCollider->m_pTargetMatrix);
	D3DXVec3TransformCoord(&mColliderInfo.vDestMax, &pTargetCollider->mMeshInfo.vMax, pTargetCollider->m_pTargetMatrix);

	D3DXVECTOR3 vSrcPos = (mColliderInfo.vSourMin + mColliderInfo.vSourMax) * 0.5f;
	D3DXVECTOR3 vDestPos = (mColliderInfo.vDestMin + mColliderInfo.vDestMax) * 0.5f;

	float distanceX = abs(vDestPos.x - vSrcPos.x);
	float distanceZ = abs(vDestPos.z - vSrcPos.z);

	if ((width >= distanceX) && (depth >= distanceZ))
	{
		//파고든거리를 구한다.
		vPush->x = width - distanceX;
		vPush->z = depth - distanceZ;

		if (vPush->x < vPush->z)
		{
			if (vSrcPos.x > vDestPos.x)
			{
				vPush->x *= -1.f;
			}
		}
		else if (vPush->x > vPush->z)
		{
			if (vSrcPos.y > vDestPos.y)
			{
				vPush->z *= -1.f;
			}
		}

		return true;
	}

	return false;
}

bool Collider::Collision_AABB(const Collider* pTargetCollider, D3DXVECTOR3* pRightDir)
{
	D3DXVec3TransformCoord(&mColliderInfo.vSourMin, &mMeshInfo.vMin, m_pTargetMatrix);
	D3DXVec3TransformCoord(&mColliderInfo.vSourMax, &mMeshInfo.vMax, m_pTargetMatrix);

	D3DXVec3TransformCoord(&mColliderInfo.vDestMin, &pTargetCollider->mMeshInfo.vMin, pTargetCollider->m_pTargetMatrix);
	D3DXVec3TransformCoord(&mColliderInfo.vDestMax, &pTargetCollider->mMeshInfo.vMax, pTargetCollider->m_pTargetMatrix);

	m_bCollision = false;

	// 가로(x축선상으로)로 안겹쳤니?!
	if (max(mColliderInfo.vSourMin.x, mColliderInfo.vDestMin.x) > min(mColliderInfo.vSourMax.x, mColliderInfo.vDestMax.x))
		return false;

	// 높이(y축선상으로)로 안겹쳤니?!	
	/*if (max(mColliderInfo.vSourMin.y, mColliderInfo.vDestMin.y) > min(mColliderInfo.vSourMax.y, mColliderInfo.vDestMax.y))
		return false;*/

	// 깊이(z축선상으로)로 안겹쳤니?!
	if (max(mColliderInfo.vSourMin.z, mColliderInfo.vDestMin.z) > min(mColliderInfo.vSourMax.z, mColliderInfo.vDestMax.z))
		return false;

	m_bCollision = true;

	if (pRightDir != nullptr)
	{
		D3DXVECTOR3 vCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		D3DXVECTOR3 vTargetCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);

		D3DXVec3TransformCoord(&vCenterPos, &m_vCenterPos, m_pTargetMatrix);
		D3DXVec3TransformCoord(&vTargetCenterPos, &pTargetCollider->m_vCenterPos, pTargetCollider->m_pTargetMatrix);
		*pRightDir = vTargetCenterPos - vCenterPos;
		D3DXVec3Normalize(pRightDir, pRightDir);
		pRightDir->y = 0.f;
	}

	return m_bCollision;
}

bool Collider::Collision_OBB(const Collider* pTargetCollider)
{
	m_bCollision = false;

	OBB_INFO OBB_Info[2];
	ZeroMemory(OBB_Info, sizeof(OBB_INFO));

	// 월드변환된 포인트
	for (size_t i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&OBB_Info[0].vPoint[i], &m_vLocalPoint[i], m_pTargetMatrix);
		D3DXVec3TransformCoord(&OBB_Info[1].vPoint[i], &pTargetCollider->m_vLocalPoint[i], pTargetCollider->m_pTargetMatrix);
	}

	OBB_Info[0].vCenter = (OBB_Info[0].vPoint[0] + OBB_Info[0].vPoint[6]) * 0.5f;
	OBB_Info[1].vCenter = (OBB_Info[1].vPoint[0] + OBB_Info[1].vPoint[6]) * 0.5f;

	// 투영을 위한 방향벡터들을 구한다.
	for (size_t i = 0; i < 2; ++i)
	{
		CalAxis(&OBB_Info[i]);
	}
		
	float distance[3];

	D3DXVECTOR3 vDir = OBB_Info[1].vCenter - OBB_Info[0].vCenter;

	for (size_t i = 0; i < 2; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			distance[0] = fabs(D3DXVec3Dot(&vDir, &OBB_Info[i].vAlignAxis[j]));

			distance[1] = fabs(D3DXVec3Dot(&OBB_Info[0].vProjAxis[0], &OBB_Info[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBB_Info[0].vProjAxis[1], &OBB_Info[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBB_Info[0].vProjAxis[2], &OBB_Info[i].vAlignAxis[j]));

			distance[2] = fabs(D3DXVec3Dot(&OBB_Info[1].vProjAxis[0], &OBB_Info[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBB_Info[1].vProjAxis[1], &OBB_Info[i].vAlignAxis[j])) +
				fabs(D3DXVec3Dot(&OBB_Info[1].vProjAxis[2], &OBB_Info[i].vAlignAxis[j]));


			// 중점과 중점을 이은 벡터를 투영시킨 길이.
			if (distance[0] > distance[1] + distance[2])
				return false;
		}
	}

	m_bCollision = true;

	return m_bCollision;
}

bool Collider::Collision_Sphere(const Collider * pTargetCollider)
{
	D3DXVECTOR3 vCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vTargetCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	D3DXVec3TransformCoord(&vCenterPos, &m_vCenterPos, m_pTargetMatrix);
	D3DXVec3TransformCoord(&vTargetCenterPos, &pTargetCollider->m_vCenterPos, pTargetCollider->m_pTargetMatrix);

	m_vTargetDir = vTargetCenterPos - vCenterPos;
	mDistance = D3DXVec3Length(&m_vTargetDir);

	if (pTargetCollider->mRadius + mRadius > mDistance)
		return true;
	
	//cout << "충돌범위: " << mDistance << endl;
	return false;
}

bool Collider::Collision_Bullet(const Collider * pTargetCollider, float targetRadius)
{
	D3DXVECTOR3 vCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vTargetCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	D3DXVec3TransformCoord(&vCenterPos, &m_vCenterPos, m_pTargetMatrix);
	D3DXVec3TransformCoord(&vTargetCenterPos, &pTargetCollider->m_vCenterPos, pTargetCollider->m_pTargetMatrix);

	if (targetRadius == 0.f)
	{
		targetRadius = pTargetCollider->mRadius * 0.5f;
	}
	m_vTargetDir = vTargetCenterPos - vCenterPos;
	mDistance = D3DXVec3Length(&m_vTargetDir);


	if (targetRadius + mRadius > mDistance)
	{
		return true;
	}

	return false;
}

void Collider::CalAxis(OBB_INFO * pOBB_Info)
{
	pOBB_Info->vProjAxis[0] = (pOBB_Info->vPoint[1] + pOBB_Info->vPoint[6]) * 0.5f - pOBB_Info->vCenter;
	pOBB_Info->vProjAxis[1] = (pOBB_Info->vPoint[1] + pOBB_Info->vPoint[4]) * 0.5f - pOBB_Info->vCenter;
	pOBB_Info->vProjAxis[2] = (pOBB_Info->vPoint[5] + pOBB_Info->vPoint[7]) * 0.5f - pOBB_Info->vCenter;

	pOBB_Info->vAlignAxis[0] = pOBB_Info->vPoint[2] - pOBB_Info->vPoint[3];
	pOBB_Info->vAlignAxis[1] = pOBB_Info->vPoint[0] - pOBB_Info->vPoint[3];
	pOBB_Info->vAlignAxis[2] = pOBB_Info->vPoint[7] - pOBB_Info->vPoint[3];

	for (size_t i = 0; i < 3; ++i)
	{
		D3DXVec3Normalize(&pOBB_Info->vAlignAxis[i], &pOBB_Info->vAlignAxis[i]);
	}
}

HRESULT Collider::SetUp_Collider(const D3DXVECTOR3 & vMin, const D3DXVECTOR3 & vMax, const D3DXMATRIX * pTargetMatrix, D3DXVECTOR3* pCenterPos, float* pRadius)
{
	if (m_pMesh == nullptr)
		return E_FAIL;

	D3DXMATRIX matResult;

	mMeshInfo.vMin = vMin;
	mMeshInfo.vMax = vMax;

	m_pTargetMatrix = pTargetMatrix;
	

	D3DXMatrixScaling(&m_matScale, vMax.x - vMin.x, vMax.y - vMin.y, vMax.z - vMin.z);
	D3DXMatrixTranslation(&m_matTrans, (vMax.x + vMin.x) * 0.5f, (vMax.y + vMin.y) * 0.5f, (vMax.z + vMin.z) * 0.5f);
	 
	matResult = m_matScale * m_matTrans;

	// 정점 하나의 크기를 얻어온다.
	size_t stride = D3DXGetFVFVertexSize(m_pMesh->GetFVF());
	void* pVertices = nullptr;

	m_pMesh->LockVertexBuffer(0, &pVertices);

	for (size_t i = 0; i < m_pMesh->GetNumVertices(); ++i)
	{
		D3DXVec3TransformCoord((D3DXVECTOR3*)((BYTE*)pVertices + (i * stride)), (D3DXVECTOR3*)((BYTE*)pVertices + (i * stride)), &matResult);
	}

	m_pMesh->UnlockVertexBuffer();

	if (mType == TYPE_SPHERE)
	{
		m_vCenterPos = *(D3DXVECTOR3*)&m_matTrans.m[3][0];
		mRadius = m_matScale._11;
		mRadius *= m_pTargetMatrix->m[1][1];
		if (pCenterPos != nullptr && pRadius != nullptr)
		{
			*pCenterPos = m_vCenterPos;
			*pRadius = mRadius;
		}

		return NOERROR;
	}

	m_vLocalPoint[0] = D3DXVECTOR3(mMeshInfo.vMin.x, mMeshInfo.vMax.y, mMeshInfo.vMin.z);
	m_vLocalPoint[1] = D3DXVECTOR3(mMeshInfo.vMax.x, mMeshInfo.vMax.y, mMeshInfo.vMin.z);
	m_vLocalPoint[2] = D3DXVECTOR3(mMeshInfo.vMax.x, mMeshInfo.vMin.y, mMeshInfo.vMin.z);
	m_vLocalPoint[3] = D3DXVECTOR3(mMeshInfo.vMin.x, mMeshInfo.vMin.y, mMeshInfo.vMin.z);

	m_vLocalPoint[4] = D3DXVECTOR3(mMeshInfo.vMin.x, mMeshInfo.vMax.y, mMeshInfo.vMax.z);
	m_vLocalPoint[5] = D3DXVECTOR3(mMeshInfo.vMax.x, mMeshInfo.vMax.y, mMeshInfo.vMax.z);
	m_vLocalPoint[6] = D3DXVECTOR3(mMeshInfo.vMax.x, mMeshInfo.vMin.y, mMeshInfo.vMax.z);
	m_vLocalPoint[7] = D3DXVECTOR3(mMeshInfo.vMin.x, mMeshInfo.vMin.y, mMeshInfo.vMax.z);

	m_vCenterPos = m_vLocalPoint[6] + m_vLocalPoint[0] * 0.5f;

	if (pCenterPos != nullptr && pRadius != nullptr)
	{
		*pCenterPos = m_vLocalPoint[6] + m_vLocalPoint[0] * 0.5f;
		D3DXVECTOR3 vRadius = m_vLocalPoint[0] - *pCenterPos;
		*pRadius = D3DXVec3Length(&vRadius);
		mRadius = *pRadius;
	}


	

	return NOERROR;
}

void Collider::Render()
{
	if (m_pMesh == nullptr)
		return;

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pDevice->SetTransform(D3DTS_WORLD, m_pTargetMatrix);

	m_pMesh->DrawSubset(0);
	
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

HRESULT Collider::Init_BoundingBox()
{
	// 박스메시를 생성한다.
	if (FAILED(D3DXCreateBox(m_pDevice, 1.f, 1.f, 1.f, &m_pMesh, &m_pAdjacency)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Collider::Init_BoundingSphere()
{
	if (FAILED(D3DXCreateSphere(m_pDevice, 1.f, 20, 20, &m_pMesh, &m_pAdjacency)))
		return E_FAIL;

	return NOERROR;
}

Collider * Collider::Create(LPDIRECT3DDEVICE9 pDevice, TYPE type)
{
	Collider* pInstance = new Collider(pDevice);

	if (FAILED(pInstance->Init(type)))
	{
		_MSG_BOX("Collider Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Component * Collider::Clone()
{
	return new Collider(*this);
}

DWORD Collider::Free()
{
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pMesh);

	Component::Free();

	return DWORD();
}
