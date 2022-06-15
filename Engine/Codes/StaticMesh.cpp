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
	// �޽ø� �ε��մϴ�.
	TCHAR fullPath[MAX_PATH] = L"";

	lstrcpy(fullPath, pFilePath);
	lstrcat(fullPath, pFileName);

	LPD3DXMESH pMesh = nullptr;

	// ppAdjacency : ���� �迭, �迭�� ����(������ ���� * 3)
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

	// ������� ������ ������ �ִ�D3DXMATERIAL����ü �迭�� ���� �� �ּ�.
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

	//�޽ø� �̷�� ������ ��ֺ��Ͱ� �ֳľ���?
	
	// �޽ø� �̷���ִ� ������ fvf�� |�������� �������ѳ��� ������ �����Ѵ�.
	DWORD vtxFVF = pMesh->GetFVF();

	// ������ fvf���� �ϳ��� ������.  MAX_FVF_DECL_SIZE//�����ϳ��� �������ִ� FVF ����
	D3DVERTEXELEMENT9 oriVtxElement[MAX_FVF_DECL_SIZE];
	ZeroMemory(oriVtxElement, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	// �޽ø� �̷�� �ִ� ������ fvf�� �迭�� ���� ��Ƽ� �����Ѵ�.
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
	FVF������ �˷��ִ�
	stream -> �ϵ���� ������ ���̱����� ���� �θ�������
	offset -> 0��°���� ��� ��ġ���ִ³� ����ü �е��̾ƴϰ� �������� ũ��
	type -> �ڷ����� type�� �����̳�
	Method -> fvf������ Ư¡ Default 0
	Usage -> D3DDECLUSAGE_TEXCOORD
	UsageIndex -> COLOR0, COLOR1  0������ 1������
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
