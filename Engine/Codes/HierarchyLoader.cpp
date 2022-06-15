#include "HierarchyLoader.h"


_USING(Engine)

HierarchyLoader::HierarchyLoader(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

HRESULT HierarchyLoader::Init(const TCHAR * pFilePath)
{
	m_pPath = pFilePath;

	return NOERROR;
}

STDMETHODIMP HierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_DERIVED* pNewFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame, sizeof(D3DXFRAME_DERIVED));

	if (Name != nullptr)
	{
		if (FAILED(AllocateName(&pNewFrame->Name, Name)))
			return E_FAIL;
	}

	pNewFrame->combined_TransformMatrix = *D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);
	*ppNewFrame = pNewFrame;


	return NOERROR;
}

STDMETHODIMP HierarchyLoader::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED* pNewMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pNewMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	// 메시컨테이너의 이름을 보관한다.
	if (Name != nullptr)
	{
		AllocateName(&pNewMeshContainer->Name, Name);
	}

	pNewMeshContainer->MeshData.Type = pMeshData->Type;
	DWORD numFaces = pMeshData->pMesh->GetNumFaces();

	// 메시컨테이너의 각 폴리곤의 이웃폴리곤의 인덱스를 보관한다.
	pNewMeshContainer->pAdjacency = new DWORD[numFaces * 3];
	memcpy(pNewMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * numFaces * 3);

	D3DVERTEXELEMENT9 vtxElement[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		D3DDECL_END()
	};

	// 메시컨테이너의 실제 메시 데이터 타입을 셋팅한다.
	if (pNewMeshContainer->MeshData.Type == D3DXMESHTYPE_MESH)
	{
		LPD3DXMESH pMesh = pMeshData->pMesh;

		// 현재 메시에게 필요한정보들을 더 추가한다음 실제 메시컨테이너안에 담아주자.
		DWORD vtxFVF = pMesh->GetFVF();


		if (vtxFVF & D3DFVF_NORMAL)
		{
			// JH_NormalMapping DynamicMesh Mesh Clone
			if (FAILED(pMesh->CloneMesh(pMesh->GetOptions(), vtxElement, m_pDevice, &pNewMeshContainer->MeshData.pMesh)))
				return E_FAIL;
			//Safe_Release(pMesh);

			if (FAILED(D3DXComputeTangent(pNewMeshContainer->MeshData.pMesh,0, 0, 0, 0, pNewMeshContainer->pAdjacency)))
				return E_FAIL;

			//if (FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(), vtxFVF, m_pDevice, &pNewMeshContainer->MeshData.pMesh)))
			//	return E_FAIL;

			//Safe_Release(pMesh); CreateMeshContainer 함수 호출이 끝나면 자동으로 지워진다.
		}
		else
		{
			if (FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(), vtxFVF | D3DFVF_NORMAL, m_pDevice, &pNewMeshContainer->MeshData.pMesh)))
				return E_FAIL;

			if (FAILED(D3DXComputeNormals(pNewMeshContainer->MeshData.pMesh, pNewMeshContainer->pAdjacency)))
				return E_FAIL;
		}

		// Normal이 없으면 Normal을 추가해서 새로운 메시를 생성한다. 첫번째 방법
		//pMesh->CloneMeshFVF(/*새롭게 만들어질 Mesh의 옵션*/ , /*새롭게 만들어지는 메쉬의 FVF*/, /**/, /*최종적으로 보관해야할 메쉬*/ )
		/*if(FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(), vtxFVF | D3DFVF_NORMAL, Get_GraphicDev(), &m_pMesh)))
		return E_FAIL;*/
	}

	// 메시컨테이너의 서브셋은 갯수
	pNewMeshContainer->NumMaterials = NumMaterials;

	// 메시컨테이너의 서브셋정보들 
	pNewMeshContainer->pMaterials = new D3DXMATERIAL[pNewMeshContainer->NumMaterials];
	memcpy(pNewMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);

	pNewMeshContainer->ppDiffuseTex = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->ppDiffuseTex, sizeof(LPDIRECT3DTEXTURE9) * pNewMeshContainer->NumMaterials);

	pNewMeshContainer->ppSpecularTex = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->ppSpecularTex, sizeof(LPDIRECT3DTEXTURE9) * pNewMeshContainer->NumMaterials);

	pNewMeshContainer->ppNormalTex = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->ppNormalTex, sizeof(LPDIRECT3DTEXTURE9) * pNewMeshContainer->NumMaterials);

	
	for (size_t i = 0; i < pNewMeshContainer->NumMaterials; i++)
	{
		TCHAR fullPath[MAX_PATH] = L"";
		TCHAR fullSpecularPath[MAX_PATH] = L"";
		TCHAR fullNormalPath[MAX_PATH] = L"";
		TCHAR fileName[MAX_PATH] = L"";

		lstrcpy(fullPath, m_pPath);

		MultiByteToWideChar(CP_ACP, 0, pNewMeshContainer->pMaterials[i].pTextureFilename, strlen(pNewMeshContainer->pMaterials[i].pTextureFilename),
			fileName, MAX_PATH);

		lstrcat(fullPath, fileName);
		

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, fullPath, &pNewMeshContainer->ppDiffuseTex[i])))
		{
			pNewMeshContainer->ppDiffuseTex[i] = nullptr;
		}

		// SpecularTex Loading
		lstrcat(fullSpecularPath, fullPath);
		lstrcat(fullSpecularPath, L"_spec");

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, fullPath, &pNewMeshContainer->ppSpecularTex[i])))
		{
			pNewMeshContainer->ppSpecularTex[i] = nullptr;
		}

		// NormalTex Loading
		lstrcat(fullNormalPath, fullPath);
		lstrcat(fullNormalPath, L"_n");

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, fullPath, &pNewMeshContainer->ppNormalTex[i])))
		{
			pNewMeshContainer->ppNormalTex[i] = nullptr;
		}
		 
	}

	//pNewMeshContainer->MeshData.pMesh->CloneMeshFVF(pNewMeshContainer->MeshData.pMesh->GetOptions(), pNewMeshContainer->MeshData.pMesh->GetFVF(), m_pDevice, &pNewMeshContainer->pOriginalMesh);

	// JH_NormalMapping DynamicMesh originalMesh Clone 
	if (FAILED(pNewMeshContainer->MeshData.pMesh->CloneMesh(pNewMeshContainer->MeshData.pMesh->GetOptions(), vtxElement, m_pDevice, &pNewMeshContainer->pOriginalMesh)))
		return E_FAIL;


	// 피부(정점)제어를 위한 객체를 받아놓는다.
	pNewMeshContainer->pSkinInfo = pSkinInfo;
	pNewMeshContainer->pSkinInfo->AddRef(); //댕글링포인터의 위험성으로 AddRef를 해준다종료가 되면 알아서 지워준다.

	// JH_NormalMapping DynamicMesh SkinInfo SetDeclaration
	pNewMeshContainer->pSkinInfo->SetDeclaration(vtxElement);

	pNewMeshContainer->numFrames = pNewMeshContainer->pSkinInfo->GetNumBones();
	

	// 렌더링하기위해 필요한 행렬의 공간을 미리 확보해놓는다.
	pNewMeshContainer->pRenderingMatrices = new D3DXMATRIX[pNewMeshContainer->numFrames];
	ZeroMemory(pNewMeshContainer->pRenderingMatrices, sizeof(D3DXMATRIX) * pNewMeshContainer->numFrames);

	pNewMeshContainer->pOffsetMatrices = new D3DXMATRIX[pNewMeshContainer->numFrames];

	for (size_t i = 0; i < pNewMeshContainer->numFrames; i++)
	{
		pNewMeshContainer->pOffsetMatrices[i] = *pNewMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
	}

	pNewMeshContainer->ppCombined_TransformMatrices = new D3DXMATRIX*[pNewMeshContainer->numFrames];
	ZeroMemory(pNewMeshContainer->ppCombined_TransformMatrices, sizeof(D3DXMATRIX*) * pNewMeshContainer->numFrames);

	*ppNewMeshContainer = pNewMeshContainer;

	return NOERROR;
}

STDMETHODIMP HierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (pFrameToFree->pMeshContainer != nullptr)
	{
		DestroyMeshContainer(pFrameToFree->pMeshContainer);
	}
		
	if (pFrameToFree->pFrameSibling != nullptr)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}
		
	if (pFrameToFree->pFrameFirstChild != nullptr)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
		
	Safe_Delete(pFrameToFree);

	return NOERROR;
}

STDMETHODIMP HierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	Safe_Delete_Array(pMeshContainerToFree->Name);
	Safe_Release(pMeshContainerToFree->MeshData.pMesh);

	Safe_Delete_Array(pMeshContainerToFree->pMaterials);
	Safe_Delete_Array(pMeshContainerToFree->pAdjacency);
	Safe_Release(pMeshContainerToFree->pSkinInfo);

	D3DXMESHCONTAINER_DERIVED* pMeshContainer_Derived = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	Safe_Release(pMeshContainer_Derived->pOriginalMesh);

	for (size_t i = 0; i < pMeshContainer_Derived->NumMaterials; i++)
	{
		Safe_Release(pMeshContainer_Derived->ppDiffuseTex[i]);
		Safe_Release(pMeshContainer_Derived->ppSpecularTex[i]);
		Safe_Release(pMeshContainer_Derived->ppNormalTex[i]);
	}

	Safe_Delete_Array(pMeshContainer_Derived->ppSpecularTex);
	Safe_Delete_Array(pMeshContainer_Derived->ppDiffuseTex);
	Safe_Delete_Array(pMeshContainer_Derived->ppNormalTex);
	Safe_Delete_Array(pMeshContainer_Derived->pRenderingMatrices);
	Safe_Delete_Array(pMeshContainer_Derived->pOffsetMatrices);
	Safe_Delete_Array(pMeshContainer_Derived->ppCombined_TransformMatrices);

	Safe_Delete(pMeshContainerToFree);
	
	return NOERROR;
}

HRESULT HierarchyLoader::AllocateName(char ** ppSourName, const char * pDestName)
{
	if (*ppSourName != nullptr)
		return E_FAIL;

	*ppSourName = new char[strlen(pDestName) + 1];
	ZeroMemory(*ppSourName, sizeof(char) * (strlen(pDestName) + 1));

	strcpy_s(*ppSourName, strlen(pDestName) + 1, pDestName);

	return NOERROR;
}

DWORD HierarchyLoader::AddRef()
{
	return ++mRefCnt;
}

DWORD HierarchyLoader::Release()
{
	if (mRefCnt == 0)
	{
		Free();

		delete this;

		return 0;
	}

	return mRefCnt--;
}

HierarchyLoader * HierarchyLoader::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR * pFilePath)
{
	HierarchyLoader*	pInstance = new HierarchyLoader(pDevice);

	if (FAILED(pInstance->Init(pFilePath)))
	{
		_MSG_BOX("HierachyLoader Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD HierarchyLoader::Free()
{
	Safe_Release(m_pDevice);

	return 0;
}
