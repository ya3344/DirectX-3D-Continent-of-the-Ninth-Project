#include "StaticMesh.h"

_USING(Engine)

StaticMesh::StaticMesh(LPDIRECT3DDEVICE9 pDevice)
	:Mesh(pDevice)
{
}

StaticMesh::StaticMesh(const StaticMesh & rhs)
	:Mesh(rhs)
	,m_pMesh(rhs.m_pMesh)
	, mShaderTexture(rhs.mShaderTexture)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubset(rhs.m_pSubset)
	, mNumSubset(rhs.mNumSubset)
	, mMeshInfo(rhs.mMeshInfo)
{
	m_pMesh->AddRef();
	m_pAdjacency->AddRef();
	m_pSubset->AddRef();
}

HRESULT StaticMesh::Init(const TCHAR * pFilePath, const TCHAR * pFileName)
{
	// 메시를 로드합니다.
	TCHAR fullPath[MAX_PATH] = L"";

	lstrcpy(fullPath, pFilePath);
	lstrcat(fullPath, pFileName);

	LPD3DXMESH pMesh = nullptr;

	// ppAdjacency : 정수 배열, 배열의 갯수(폴리곤 갯수 * 3)
	if (FAILED(D3DXLoadMeshFromX(fullPath, D3DXMESH_MANAGED, m_pDevice, &m_pAdjacency, &m_pSubset, nullptr, &mNumSubset, &pMesh)))
		return E_FAIL;

	
	mShaderTexture.ppDiffuseTex = new LPDIRECT3DTEXTURE9[mNumSubset];
	ZeroMemory(mShaderTexture.ppDiffuseTex, sizeof(LPDIRECT3DTEXTURE9) * mNumSubset);

	mShaderTexture.ppSpecularTex = new LPDIRECT3DTEXTURE9[mNumSubset];
	ZeroMemory(mShaderTexture.ppSpecularTex, sizeof(LPDIRECT3DTEXTURE9) * mNumSubset);

	mShaderTexture.ppNormalTex = new LPDIRECT3DTEXTURE9[mNumSubset];
	ZeroMemory(mShaderTexture.ppNormalTex, sizeof(LPDIRECT3DTEXTURE9) * mNumSubset);


	//DWORD* pNewAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
	//if (FAILED(pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, (const DWORD*)m_pAdjacency->GetBufferPointer(), pNewAdjacency, nullptr, nullptr)))
	//{
	//	_MSG_BOX("Mesh Optimize Failed");
	//}
	//else
	//{
	//	memcpy(m_pAdjacency->GetBufferPointer(), pNewAdjacency, sizeof(DWORD) * pMesh->GetNumFaces() * 3);
	//}
	//Safe_Delete_Array(pNewAdjacency);

	// 서브셋의 정보를 가지고 있는D3DXMATERIAL구조체 배열의 가장 앞 주소.
	D3DXMATERIAL* pSubset = (D3DXMATERIAL*)m_pSubset->GetBufferPointer();

	for (size_t i = 0; i < mNumSubset; i++)
	{
		TCHAR fullPath[MAX_PATH] = L"";
		TCHAR fileTempName[MAX_PATH] = L"";
		TCHAR fileName[MAX_PATH] = L"";
		TCHAR fullSpecularPath[MAX_PATH] = L"";
		TCHAR fullNormalPath[MAX_PATH] = L"";

		// For.Texture Info
		lstrcpy(fullPath, pFilePath);

		MultiByteToWideChar(CP_ACP, 0, pSubset[i].pTextureFilename, strlen(pSubset[i].pTextureFilename)
			, fileName, MAX_PATH);

		lstrcat(fullPath, fileName);

		lstrcpyn(fileTempName, fullPath, lstrlen(fullPath) - 3);

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, fullPath, &mShaderTexture.ppDiffuseTex[i])))
		{
			mShaderTexture.ppDiffuseTex[i] = nullptr;
		}
			
		// SpecularTex Loading
		lstrcat(fullSpecularPath, fileTempName);
		lstrcat(fullSpecularPath, L"_spec.dds");

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, fullSpecularPath, &mShaderTexture.ppSpecularTex[i])))
		{
			mShaderTexture.ppSpecularTex[i] = nullptr;
		}

		// NormalTex Loading
		lstrcat(fullNormalPath, fileTempName);
		lstrcat(fullNormalPath, L"_n.dds");

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, fullNormalPath, &mShaderTexture.ppNormalTex[i])))
		{
			mShaderTexture.ppNormalTex[i] = nullptr;
		}
	}

	//메시를 이루는 정점에 노멀벡터가 있냐없냐?
	
	// 메시를 이루고있는 정점의 fvf를 |연산으로 누적시켜놓은 정수를 리턴한다.
	DWORD vtxFVF = pMesh->GetFVF();

	// 정점의 fvf정보 하나를 가진다.  MAX_FVF_DECL_SIZE//정점하나에 가질수있는 FVF 정보
	D3DVERTEXELEMENT9 oriVtxElement[MAX_FVF_DECL_SIZE];
	ZeroMemory(oriVtxElement, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	// 메시를 이루고 있는 정점의 fvf를 배열에 각각 담아서 리턴한다.
	if (FAILED(pMesh->GetDeclaration(oriVtxElement)))
		return E_FAIL;


	WORD offset = 0;

	for (size_t i = 0; i < MAX_FVF_DECL_SIZE; i++)
	{
		if (D3DDECLUSAGE_POSITION == oriVtxElement[i].Usage)
		{
			offset = oriVtxElement[i].Offset;
			break;
		}
	}

	void* pVertices = nullptr;

	pMesh->LockVertexBuffer(0, &pVertices);

	if (FAILED(D3DXComputeBoundingBox((D3DXVECTOR3*)((BYTE*)pVertices + offset), pMesh->GetNumVertices(), D3DXGetFVFVertexSize(pMesh->GetFVF()), &mMeshInfo.vMin, &mMeshInfo.vMax)))
		return E_FAIL;

	pMesh->UnlockVertexBuffer();



	/*
	FVF정보를 알려주는
	stream -> 하드웨어 성능을 높이기위해 따로 부르기위해
	offset -> 0번째부터 어디 위치가있는냐 구조체 패딩이아니고 실질적인 크기
	type -> 자료형의 type이 무엇이냐
	Method -> fvf정보의 특징 Default 0
	Usage -> D3DDECLUSAGE_TEXCOORD
	UsageIndex -> COLOR0, COLOR1  0번쨰다 1번쨰다
	*/

	//	typedef struct _D3DVERTEXELEMENT9
	//	{
	//		WORD    Stream;     // Stream index
	//		WORD    Offset;     // Offset in the stream in bytes
	//		BYTE    Type;       // Data type
	//		BYTE    Method;     // Processing method
	//		BYTE    Usage;      // Semantics
	//		BYTE    UsageIndex; // Semantic index
	//	} D3DVERTEXELEMENT9, *LPD3DVERTEXELEMENT9;
	//
	//	// This is used to initialize the last vertex element in a vertex declaration
	//	// array
	//	//
	//#define D3DDECL_END() {0xFF,0,D3DDECLTYPE_UNUSED,0,0,0}

	//m_pMesh = pMesh;
	D3DVERTEXELEMENT9 vtxElement[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		D3DDECL_END()
	};


	// JH_NormalMapping StaticMesh Mesh Clone
	//	m_pMesh->CloneMeshFVF(pMesh->GetOptions(), m_pMesh->GetFVF(), m_pDevice, &m_pMesh);
	if (FAILED(pMesh->CloneMesh(pMesh->GetOptions(), vtxElement, m_pDevice, &m_pMesh)))
		return E_FAIL;

	Safe_Release(pMesh);

	if (FAILED(D3DXComputeTangent(m_pMesh, 0, 0, 0, 0, (const DWORD*)m_pAdjacency->GetBufferPointer())))
		return E_FAIL;





	return NOERROR;
}

void StaticMesh::Render(LPD3DXEFFECT pEffect)
{
	if (m_pMesh == nullptr)
		return;

	for (size_t i = 0; i < mNumSubset ; i++)
	{
		pEffect->SetTexture("g_MeshDiffuseTex", mShaderTexture.ppDiffuseTex[i]);
		pEffect->SetTexture("g_MeshSpecularTex", mShaderTexture.ppSpecularTex[i]);
		pEffect->SetTexture("g_MeshNormalTex", mShaderTexture.ppNormalTex[i]);

		pEffect->CommitChanges();

		m_pMesh->DrawSubset(i);
	}
}

StaticMesh * StaticMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR * pFilePath, const TCHAR * pFileName)
{
	StaticMesh*	pInstance = new StaticMesh(pDevice);

	if (FAILED(pInstance->Init(pFilePath, pFileName)))
	{
		_MSG_BOX("StaticMesh Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Component * StaticMesh::Clone()
{
	return new StaticMesh(*this);
}

DWORD StaticMesh::Free()
{
	Safe_Release(m_pMesh);
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubset);

	if (m_bClone == false)
	{
		for (size_t i = 0; i < mNumSubset; i++)
		{
			Safe_Release(mShaderTexture.ppDiffuseTex[i]);
			Safe_Release(mShaderTexture.ppSpecularTex[i]);
			Safe_Release(mShaderTexture.ppNormalTex[i]);
		}

		Safe_Delete_Array(mShaderTexture.ppDiffuseTex);
		Safe_Delete_Array(mShaderTexture.ppSpecularTex);
		Safe_Delete_Array(mShaderTexture.ppNormalTex);
	//	Safe_Delete_Array(m_pMaterials);
	}


	Mesh::Free();

	return DWORD();
}
