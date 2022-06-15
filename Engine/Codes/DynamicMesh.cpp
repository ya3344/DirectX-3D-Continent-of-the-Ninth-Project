#include "DynamicMesh.h"
#include "HierarchyLoader.h"
#include "AnimCtrl.h"

_USING(Engine)


DynamicMesh::DynamicMesh(LPDIRECT3DDEVICE9 pDevice)
	:Mesh(pDevice)
{
}

DynamicMesh::DynamicMesh(const DynamicMesh & rhs)
	:Mesh(rhs)
	,m_pLoader(rhs.m_pLoader)
	,m_pRootFrame(rhs.m_pRootFrame)
	,mMeshContainer_Space(rhs.mMeshContainer_Space)
	,m_pAnimCtrl(rhs.m_pAnimCtrl->Clone())
{
	m_pLoader->AddRef();


}

HRESULT DynamicMesh::Init(const TCHAR * pFilePath, const TCHAR * pFileName)
{
	// 메시를 로드합니다.
	TCHAR fullPath[MAX_PATH] = L"";

	lstrcpy(fullPath, pFilePath);
	lstrcat(fullPath, pFileName);

	// Loader생성한다.
	m_pLoader = HierarchyLoader::Create(m_pDevice, pFilePath);
	if (m_pLoader == nullptr)
		return E_FAIL;

	LPD3DXANIMATIONCONTROLLER pAnimCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(fullPath, D3DXMESH_MANAGED, m_pDevice, m_pLoader, nullptr, &m_pRootFrame, &pAnimCtrl)))
		return E_FAIL;

	m_pAnimCtrl = AnimCtrl::Create(m_pDevice, pAnimCtrl);
	if (m_pAnimCtrl == nullptr)
		return E_FAIL;

	Safe_Release(pAnimCtrl);

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	if (FAILED(Recursive_CombinedTransformMatrix(m_pRootFrame, &matIdentity)))
		return E_FAIL;

	if (FAILED(Recursive_SetMeshContainer(m_pRootFrame)))
		return E_FAIL;


	for (D3DXMESHCONTAINER_DERIVED* pMeshContainer : mMeshContainer_Space)
	{
		if (FAILED(SetUp_CombinedTransformMatrixPointer(pMeshContainer)))
			return E_FAIL;
	}

	return NOERROR;
}

void DynamicMesh::Render(LPD3DXEFFECT pEffect)
{
	for (D3DXMESHCONTAINER_DERIVED* pMeshContainer : mMeshContainer_Space)
	{
		for (size_t i = 0; i < pMeshContainer->numFrames; i++)
		{
			// 피부들을 뼈에 상태에 맞게끔 변환하는 과정.
			// OffsetMatrix * CombinedMatrix
			pMeshContainer->pRenderingMatrices[i] = pMeshContainer->pOffsetMatrices[i] * *pMeshContainer->ppCombined_TransformMatrices[i];
		}

		void* pSourVertices, *pDestVertices;

		// 앞으로 절대 변형되지않을 메시의 정점배열을 얻어와.
		pMeshContainer->pOriginalMesh->LockVertexBuffer(0, &pSourVertices);

		// 계속 변형하며 그릴 메시의 정점배열을 얻어와.
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVertices);
	
		// 구해놓은 행렬을  Sour정점에 곱한다음에 그 결과를 Dest정점에 대입해.
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrices, nullptr, pSourVertices, pDestVertices);
		pMeshContainer->pOriginalMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

		for (size_t i = 0; i < pMeshContainer->NumMaterials; i++)
		{
			pEffect->SetTexture("g_MeshDiffuseTex", pMeshContainer->ppDiffuseTex[i]);
			pEffect->SetTexture("g_MeshSpecularTex", pMeshContainer->ppSpecularTex[i]);
			pEffect->SetTexture("g_MeshNormalTex", pMeshContainer->ppNormalTex[i]);

			pEffect->CommitChanges();

			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
}

D3DXMATRIX * DynamicMesh::Get_BoneMatrix(const char * pBoneName) const
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
	if (nullptr == pFrame)
		return nullptr;

	return &pFrame->combined_TransformMatrix;
}

D3DXVECTOR3 * DynamicMesh::Get_BonePos(const char * pBoneName) const
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
	if (nullptr == pFrame)
		return nullptr;

	return (D3DXVECTOR3*)&pFrame->combined_TransformMatrix.m[3];
}

HRESULT DynamicMesh::Recursive_CombinedTransformMatrix(D3DXFRAME * pFrame, const D3DXMATRIX * pParentMatrix)
{
	D3DXFRAME_DERIVED* pFrameDerived = (D3DXFRAME_DERIVED*)pFrame;
	pFrameDerived->combined_TransformMatrix = pFrameDerived->TransformationMatrix * *pParentMatrix;

	if (pFrameDerived->pFrameSibling != nullptr)
	{
		Recursive_CombinedTransformMatrix(pFrameDerived->pFrameSibling, pParentMatrix);
	}
		
	if (pFrameDerived->pFrameFirstChild != nullptr)
	{
		Recursive_CombinedTransformMatrix(pFrameDerived->pFrameFirstChild, &pFrameDerived->combined_TransformMatrix);
	}

	return NO_ERROR;
}

HRESULT DynamicMesh::Recursive_SetMeshContainer(D3DXFRAME * pFrame)
{
	if (pFrame == nullptr)
		return E_FAIL;

	if (pFrame->pMeshContainer != nullptr)
	{
		mMeshContainer_Space.emplace_back((D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer);
	}

	if (pFrame->pFrameSibling != nullptr)
	{
		Recursive_SetMeshContainer(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != nullptr)
	{
		Recursive_SetMeshContainer(pFrame->pFrameFirstChild);
	}

	return NOERROR;
}

HRESULT DynamicMesh::SetUp_CombinedTransformMatrixPointer(D3DXMESHCONTAINER_DERIVED * pMeshContainer)
{
	for (size_t i = 0; i < pMeshContainer->numFrames; i++)
	{
		const char* pFrameName = pMeshContainer->pSkinInfo->GetBoneName(i);
		if (pFrameName == nullptr)
			return E_FAIL;

		D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
		if (pFrame == nullptr)
			return E_FAIL;

		pMeshContainer->ppCombined_TransformMatrices[i] = &pFrame->combined_TransformMatrix;
	}


	return NOERROR;
}


ANIM_CTRL_INFO* DynamicMesh::Set_AnimaByName(const char * pAnimName)
{
	if (m_pAnimCtrl == nullptr)
		return nullptr;

	m_pAnimCtrl->Set_AnimByName(pAnimName);
	
	return m_pAnimCtrl->Get_AnimCtrlInfo();
}

HRESULT DynamicMesh::PlayAnimation(const float timeDelta)
{
	if (m_pAnimCtrl == nullptr)
		return E_FAIL;

	m_pAnimCtrl->PlayAnim(timeDelta);

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	if (FAILED(Recursive_CombinedTransformMatrix(m_pRootFrame, &matIdentity)))
		return E_FAIL;
	
	
	return NOERROR;
}

DynamicMesh * DynamicMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR * pFilePath, const TCHAR * pFileName)
{
	DynamicMesh* pInstance = new DynamicMesh(pDevice);

	if (FAILED(pInstance->Init(pFilePath, pFileName)))
	{
		_MSG_BOX("DynamicMesh Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Component * DynamicMesh::Clone()
{
	return new DynamicMesh(*this);
}

DWORD DynamicMesh::Free()
{
	if (m_bClone == false)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
	}

	Safe_Release(m_pAnimCtrl);
	Safe_Release(m_pLoader); 

	Mesh::Free();

	return DWORD();
}
