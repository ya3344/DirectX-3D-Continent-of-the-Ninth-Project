#include "TerrainBuffer.h"
#include "ComponentMgr.h"

_USING(Engine)

TerrainBuffer::TerrainBuffer(LPDIRECT3DDEVICE9 pDevice)
	: VIBuffer(pDevice)
{

}

TerrainBuffer::TerrainBuffer(const TerrainBuffer & rhs)
	: VIBuffer(rhs)
	, m_pFrustum(rhs.m_pFrustum)
	, m_pQuadTree(rhs.m_pQuadTree)
	, mVtxNumX(rhs.mVtxNumX)
	, mVtxNumZ(rhs.mVtxNumZ)
	, m_pQuadTree_Idx(rhs.m_pQuadTree_Idx)
{
}


HRESULT TerrainBuffer::InitBuffer(const TCHAR * pTerrainInfo, const TCHAR * pHeightMap)
{
	if (FAILED(FileRead(pTerrainInfo, pHeightMap)))
	{
		_MSG_BOX("TerrainBuffer FileRead Failed!");
	}

	if (FAILED(ReadyComponent()))
	{
		_MSG_BOX("TerrainBuffer TerrainBuffer Failed!");
	}

	mVtxInfo = VTX_INFO(sizeof(VTXNORMAL_TEX), mVtxNumX * mVtxNumZ, VTXNORMAL_TEX_FVF, mVtxNumX, mVtxNumZ);
	mIdxInfo = IDX_INFO(sizeof(POLYGON16), (mVtxNumX - 1) * (mVtxNumZ - 1) * 2, D3DFMT_INDEX16);

	VIBuffer::CreateBuffer();

	VTXNORMAL_TEX* pVertex = nullptr;

	mVtxInfo.pVB->Lock(0, 0, (void**)&pVertex, 0);

	size_t index = 0;
	D3DXVECTOR3 vNormal = { 0.f, 0.f, 0.f };

	for (size_t z = 0; z < mVtxNumZ; ++z)
	{
		for (size_t x = 0; x < mVtxNumX; ++x)
		{
			index = z * mVtxNumX + x;

			float y = (m_pPixel[index] & 0x000000ff) / 10.f;

			pVertex[index].vPos = D3DXVECTOR3(float(x) * mInterval, y, float(z) * mInterval);
			pVertex[index].vTexUV = D3DXVECTOR2(x / (mVtxNumX - 1.f) * 20.f , z / (mVtxNumZ - 1.f) * 20.f);
		}
	}

	POLYGON16* pIndex = nullptr;
	mIdxInfo.pIB->Lock(0, 0, (void**)&pIndex, 0);

	size_t polygonCnt = 0;

	for (size_t z = 0; z < mVtxNumZ - 1; ++z)
	{
		for (size_t x = 0; x < mVtxNumX - 1; ++x)
		{
			index = z * mVtxNumX + x;

			pIndex[polygonCnt]._1 = (WORD)(index + mVtxNumX);
			pIndex[polygonCnt]._2 = (WORD)(index + mVtxNumX + 1);
			pIndex[polygonCnt]._3 = (WORD)(index + 1);
			
			// Cal Noraml Vector
			Cal_NormalVector(&vNormal, &pVertex[index + mVtxNumX].vPos, &pVertex[index + mVtxNumX + 1].vPos, &pVertex[index + 1].vPos);
			pVertex[pIndex[polygonCnt]._1].vNormal += vNormal;
			pVertex[pIndex[polygonCnt]._2].vNormal += vNormal;
			pVertex[pIndex[polygonCnt]._3].vNormal += vNormal;

			++polygonCnt;

			pIndex[polygonCnt]._1 = (WORD)(index + mVtxNumX);
			pIndex[polygonCnt]._2 = (WORD)(index + 1);
			pIndex[polygonCnt]._3 = (WORD)(index);
			
			// Cal Normal Vector
			Cal_NormalVector(&vNormal, &pVertex[index + mVtxNumX].vPos, &pVertex[index + 1].vPos, &pVertex[index].vPos);
			pVertex[pIndex[polygonCnt]._1].vNormal += vNormal;
			pVertex[pIndex[polygonCnt]._2].vNormal += vNormal;
			pVertex[pIndex[polygonCnt]._3].vNormal += vNormal;

			++polygonCnt;
		}
	}

	// 노말벡터를 단위벡터로 모두 계산
	for (size_t i = 0; i < mVtxInfo.vtxNum; i++)
	{
		D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);
	}

	// 정점 위치 저장 다른 클래스에서 쓰기 위함
	Set_VtxPos(pVertex);

	mIdxInfo.pIB->Unlock();
	mVtxInfo.pVB->Unlock();
	Safe_Delete_Array(m_pPixel);

	// 쿼드트리에 버텍스버퍼를 담는다.
	m_pQuadTree->SetVtxPos(m_pVtxPos);
	m_pQuadTree->SetIdxInfo(&mIdxInfo);

	// 쿼드트리 분할 
	m_pQuadTree_Idx = new QUAD_TREE_IDX(0, (mVtxNumZ - 1)*mVtxNumX, mVtxNumX - 1,
		((mVtxNumZ - 1)*mVtxNumX) + (mVtxNumX - 1));
	if (m_pQuadTree_Idx == nullptr)
		return E_FAIL; 

	m_pQuadTree->Recursive_QuadTree(&m_pQuadTree_Idx);

	return NO_ERROR;
}

UPDATE TerrainBuffer::FrustumColling()
{
	size_t index = 0;
	size_t polygonCnt = 0;

	POLYGON16* pIndex = nullptr;
	mIdxInfo.pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (size_t z = 0; z < mVtxNumZ - 1; ++z)
	{
		for (size_t x = 0; x < mVtxNumX - 1; ++x)
		{
			index = z * mVtxNumX + x;

			if (m_pFrustum->FrustumColling(&m_pVtxPos[index + mVtxNumX]) ||
				m_pFrustum->FrustumColling(&m_pVtxPos[index + mVtxNumX + 1]) ||
				m_pFrustum->FrustumColling(&m_pVtxPos[index + 1]))
			{
				pIndex[polygonCnt]._1 = (WORD)(index + mVtxNumX);
				pIndex[polygonCnt]._2 = (WORD)(index + mVtxNumX + 1);
				pIndex[polygonCnt]._3 = (WORD)(index + 1);
				++polygonCnt;
			}

			if (m_pFrustum->FrustumColling(&m_pVtxPos[index + mVtxNumX]) ||
				m_pFrustum->FrustumColling(&m_pVtxPos[index + 1]) ||
				m_pFrustum->FrustumColling(&m_pVtxPos[index]))
			{
				pIndex[polygonCnt]._1 = (WORD)(index + mVtxNumX);
				pIndex[polygonCnt]._2 = (WORD)(index + 1);
				pIndex[polygonCnt]._3 = (WORD)(index);
				++polygonCnt;
			}
		}
	}

	mIdxInfo.polygonNum = polygonCnt;
	mIdxInfo.pIB->Unlock();

	return UPDATE::NORMAL;
}

UPDATE TerrainBuffer::QuadTreeColling()
{
	mPolygonCnt = 0;
	m_pQuadTree->Recursive_QuadTreeCulling(m_pQuadTree_Idx, &mPolygonCnt);
	mIdxInfo.polygonNum = mPolygonCnt;

	return UPDATE::NORMAL;
}

HRESULT TerrainBuffer::ReadyComponent()
{
	// FrusutumColling
	m_pFrustum = (Frustum*)ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Frustum");
	if (m_pFrustum == nullptr)
		return E_FAIL;

	// QuadTree
	m_pQuadTree = (QuadTree*)ComponentMgr::GetInstance()->Clone(SCENE_STAGE, L"Component_QuadTree");
	if (m_pQuadTree == nullptr)
		return E_FAIL;

	return NO_ERROR;
}

HRESULT TerrainBuffer::FileRead(const TCHAR * pTerrainInfo, const TCHAR * pHeightMap)
{
	HANDLE hFile;
	DWORD dwByte = 0;

	hFile = CreateFile(pTerrainInfo, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	ReadFile(hFile, &mVtxNumX, sizeof(size_t), &dwByte, nullptr);
	if (dwByte == 0)
		return E_FAIL;

	ReadFile(hFile, &mVtxNumZ, sizeof(size_t), &dwByte, nullptr);
	if (dwByte == 0)
		return E_FAIL;

	ReadFile(hFile, &mInterval, sizeof(float), &dwByte, nullptr);
	if (dwByte == 0)
		return E_FAIL;

	CloseHandle(hFile);

	hFile = CreateFile(pHeightMap, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	ReadFile(hFile, &fileHeader, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	if (0 == dwByte)
		return E_FAIL;

	ReadFile(hFile, &infoHeader, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
	if (0 == dwByte)
		return E_FAIL;

	m_pPixel = new DWORD[infoHeader.biWidth * infoHeader.biHeight];

	ReadFile(hFile, m_pPixel, sizeof(DWORD) * infoHeader.biWidth * infoHeader.biHeight, &dwByte, nullptr);

	if (0 == dwByte)
		return E_FAIL;


	CloseHandle(hFile);

	return NO_ERROR;
}

void TerrainBuffer::Cal_NormalVector(D3DXVECTOR3 * pOut, D3DXVECTOR3 * p0, D3DXVECTOR3 * p1, D3DXVECTOR3 * p2)
{
	D3DXVECTOR3 vU = *p1 - *p0;
	D3DXVECTOR3 vV = *p2 - *p0;

	D3DXVec3Cross(pOut, &vU, &vV);
	D3DXVec3Normalize(pOut, pOut);
}


TerrainBuffer * TerrainBuffer::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR * pTerrainInfo, const TCHAR * pHeightMap)
{
	TerrainBuffer* pInstance = new TerrainBuffer(pDevice);

	if (FAILED(pInstance->InitBuffer(pTerrainInfo, pHeightMap)))
	{
		_MSG_BOX("Terrain_Buffer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * TerrainBuffer::Clone()
{
	return new TerrainBuffer(*this);
}

DWORD TerrainBuffer::Free(void)
{
	Safe_Release(m_pFrustum);
	Safe_Release(m_pQuadTree);

	if (m_bClone == false)
	{
		m_pQuadTree->Recursive_QuadTreeDelete(m_pQuadTree_Idx);
	}

	m_pQuadTree_Idx = nullptr;
	VIBuffer::Free();

	return 0;
}
