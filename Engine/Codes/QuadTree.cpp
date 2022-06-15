#include "QuadTree.h"
#include "ComponentMgr.h"

_USING(Engine)

QuadTree::QuadTree(LPDIRECT3DDEVICE9 pDevice)
	: VIBuffer(pDevice)
{
	
}

HRESULT QuadTree::Init()
{
	if (FAILED(ReadyComponent()))
	{
		_MSG_BOX("QuadTree ReadyComponent Failed!");
	}
	
	return NOERROR;
}

HRESULT QuadTree::Recursive_QuadTree(QUAD_TREE_IDX** ppQuadTree_Idx)
{
	// 쿼드트리관련 정보 계산
	CalInfo(ppQuadTree_Idx);

	if ((*ppQuadTree_Idx)->distance <= 1.f) // 나눌때까지 나눔
		return NOERROR;
	
	
	for (size_t i = 0; i < CHILD_END; i++)
	{
		(*ppQuadTree_Idx)->pQuadTreeIdx[i] = new QUAD_TREE_IDX;
		if ((*ppQuadTree_Idx)->pQuadTreeIdx[i] == nullptr)
			return E_FAIL;
	}
	
	// 왼쪽 하단
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_LB]->leftBottom = (*ppQuadTree_Idx)->leftBottom;
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_LB]->leftTop = (size_t)(((*ppQuadTree_Idx)->leftBottom + (*ppQuadTree_Idx)->leftTop) * 0.5f);
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_LB]->rightBottom = (size_t)(((*ppQuadTree_Idx)->leftBottom + (*ppQuadTree_Idx)->rightBottom) * 0.5f);
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_LB]->rightTop = (size_t)(((((*ppQuadTree_Idx)->leftTop + (*ppQuadTree_Idx)->rightTop) * 0.5f) + (*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_LB]->rightBottom) * 0.5f);
	
	Recursive_QuadTree(&(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_LB]);
		
	// 오른쪽 하단
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_RB]->leftBottom = (size_t)(((*ppQuadTree_Idx)->leftBottom + (*ppQuadTree_Idx)->rightBottom) * 0.5f);
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_RB]->leftTop = (size_t)(((((*ppQuadTree_Idx)->leftTop + (*ppQuadTree_Idx)->rightTop) * 0.5f) + (*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_RB]->leftBottom) * 0.5f);
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_RB]->rightBottom = (*ppQuadTree_Idx)->rightBottom;
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_RB]->rightTop = (size_t)(((*ppQuadTree_Idx)->rightBottom + (*ppQuadTree_Idx)->rightTop) * 0.5f);

	Recursive_QuadTree(&(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_RB]);
	
	// 왼쪽 상단
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_LT]->leftBottom = (size_t)(((*ppQuadTree_Idx)->leftBottom + (*ppQuadTree_Idx)->leftTop) * 0.5f);
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_LT]->leftTop = (*ppQuadTree_Idx)->leftTop;
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_LT]->rightBottom = (size_t)(((((*ppQuadTree_Idx)->rightBottom + (*ppQuadTree_Idx)->rightTop) * 0.5f) + (*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_LT]->leftBottom) * 0.5f);
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_LT]->rightTop = (size_t)(((*ppQuadTree_Idx)->leftTop + (*ppQuadTree_Idx)->rightTop) * 0.5f);
	
	Recursive_QuadTree(&(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_LT]);
	
	// 오른쪽 상단
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_RT]->leftTop = (size_t)(((*ppQuadTree_Idx)->leftTop + (*ppQuadTree_Idx)->rightTop) * 0.5f);
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_RT]->rightTop = (*ppQuadTree_Idx)->rightTop;
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_RT]->rightBottom = (size_t)(((*ppQuadTree_Idx)->rightTop + (*ppQuadTree_Idx)->rightBottom) * 0.5f);
	(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_RT]->leftBottom = (size_t)(((((*ppQuadTree_Idx)->leftBottom + (*ppQuadTree_Idx)->leftTop) * 0.5f) + (*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_RT]->rightBottom) * 0.5f);
	
	Recursive_QuadTree(&(*ppQuadTree_Idx)->pQuadTreeIdx[CHILD_RT]);

	return NOERROR;
}

void QuadTree::Recursive_QuadTreeCulling(const QUAD_TREE_IDX * pQuadTree_Idx, size_t * pPoloygonCnt)
{
	mRange = D3DXVec3Length(&(*m_pCamPos - m_pVtxPos[pQuadTree_Idx->centerIdx]));
	mRange *= 0.2f;
	if (mRange <= 2.f)
	{
		mRange = 2.f;
	}

	if (pQuadTree_Idx->distance <= mRange)
	{
		Cal_MinQuadTree(pQuadTree_Idx, pPoloygonCnt);
	}
	else
	{
		if (m_pFrustum->FrustumColling(&m_pVtxPos[pQuadTree_Idx->centerIdx], pQuadTree_Idx->radius))
		{
			// 왼쪽 하단
			if (pQuadTree_Idx->pQuadTreeIdx[CHILD_LB] != nullptr)
			{
				Recursive_QuadTreeCulling(pQuadTree_Idx->pQuadTreeIdx[CHILD_LB], pPoloygonCnt);
			}
			
			// 오른쪽 하단
			if (pQuadTree_Idx->pQuadTreeIdx[CHILD_RB] != nullptr)
			{
				Recursive_QuadTreeCulling(pQuadTree_Idx->pQuadTreeIdx[CHILD_RB], pPoloygonCnt);
			}

			// 왼쪽 상단
			if (pQuadTree_Idx->pQuadTreeIdx[CHILD_LT] != nullptr)
			{
				Recursive_QuadTreeCulling(pQuadTree_Idx->pQuadTreeIdx[CHILD_LT], pPoloygonCnt);
			}

			// 오른쪽 상단
			if (pQuadTree_Idx->pQuadTreeIdx[CHILD_RT] != nullptr)
			{
				Recursive_QuadTreeCulling(pQuadTree_Idx->pQuadTreeIdx[CHILD_RT], pPoloygonCnt);
			}
		}
	}
}

void QuadTree::Recursive_QuadTreeDelete(QUAD_TREE_IDX * pQuadTree_Idx)
{
	if (pQuadTree_Idx == nullptr)
		return;
	
	if (pQuadTree_Idx->pQuadTreeIdx[CHILD_LT] != nullptr)
	{
		Recursive_QuadTreeDelete(pQuadTree_Idx->pQuadTreeIdx[CHILD_LT]);
	}

	if (pQuadTree_Idx->pQuadTreeIdx[CHILD_RT] != nullptr)
	{
		Recursive_QuadTreeDelete(pQuadTree_Idx->pQuadTreeIdx[CHILD_RT]);
	}

	if (pQuadTree_Idx->pQuadTreeIdx[CHILD_LB] != nullptr)
	{
		Recursive_QuadTreeDelete(pQuadTree_Idx->pQuadTreeIdx[CHILD_LB]);
	}

	if (pQuadTree_Idx->pQuadTreeIdx[CHILD_RB] != nullptr)
	{
		Recursive_QuadTreeDelete(pQuadTree_Idx->pQuadTreeIdx[CHILD_RB]);
	}

	Safe_Delete(pQuadTree_Idx);
}

void QuadTree::Cal_MinQuadTree(const QUAD_TREE_IDX* pQuadTree_Idx,  size_t * pPoloygonCnt)
{
	// 왼쪽 하단
	Culling(pQuadTree_Idx->pQuadTreeIdx[CHILD_LB], pPoloygonCnt);

	// 오른쪽 하단
	Culling(pQuadTree_Idx->pQuadTreeIdx[CHILD_RB], pPoloygonCnt);

	// 왼쪽 상단
	Culling(pQuadTree_Idx->pQuadTreeIdx[CHILD_LT], pPoloygonCnt);

	// 오른쪽 상단
	Culling(pQuadTree_Idx->pQuadTreeIdx[CHILD_RT], pPoloygonCnt);

}

void QuadTree::Culling(const QUAD_TREE_IDX * pQuadTree_Idx, size_t * pPoloygonCnt)
{
	POLYGON16* pIndex = nullptr;
	m_pIdxInfo->pIB->Lock(0, 0, (void**)&pIndex, 0);

	if (m_pFrustum->FrustumColling(&m_pVtxPos[pQuadTree_Idx->leftTop]) ||
		m_pFrustum->FrustumColling(&m_pVtxPos[pQuadTree_Idx->rightTop]) ||
		m_pFrustum->FrustumColling(&m_pVtxPos[pQuadTree_Idx->rightBottom]))
	{
		pIndex[*pPoloygonCnt]._1 = (WORD)(pQuadTree_Idx->leftTop);
		pIndex[*pPoloygonCnt]._2 = (WORD)(pQuadTree_Idx->rightTop);
		pIndex[*pPoloygonCnt]._3 = (WORD)(pQuadTree_Idx->rightBottom);
		++*pPoloygonCnt;
	}

	if (m_pFrustum->FrustumColling(&m_pVtxPos[pQuadTree_Idx->leftTop]) ||
		m_pFrustum->FrustumColling(&m_pVtxPos[pQuadTree_Idx->rightBottom]) ||
		m_pFrustum->FrustumColling(&m_pVtxPos[pQuadTree_Idx->leftBottom]))
	{
		pIndex[*pPoloygonCnt]._1 = (WORD)(pQuadTree_Idx->leftTop);
		pIndex[*pPoloygonCnt]._2 = (WORD)(pQuadTree_Idx->rightBottom);
		pIndex[*pPoloygonCnt]._3 = (WORD)(pQuadTree_Idx->leftBottom);
		++*pPoloygonCnt;
	}

	m_pIdxInfo->pIB->Unlock();
}

void QuadTree::CalInfo(QUAD_TREE_IDX** ppQuadTree_Idx)
{
	D3DXVECTOR3 vLeftTop = D3DXVECTOR3(m_pVtxPos[(*ppQuadTree_Idx)->leftTop].x, 0.f, m_pVtxPos[(*ppQuadTree_Idx)->leftTop].z);
	D3DXVECTOR3 vRightTop = D3DXVECTOR3(m_pVtxPos[(*ppQuadTree_Idx)->rightTop].x, 0.f, m_pVtxPos[(*ppQuadTree_Idx)->rightTop].z);
	(*ppQuadTree_Idx)->centerIdx = (size_t)(((*ppQuadTree_Idx)->leftTop + (*ppQuadTree_Idx)->rightBottom) * 0.5f);
	D3DXVECTOR3 vCenter = D3DXVECTOR3(m_pVtxPos[mCenterIdx].x, 0.f, m_pVtxPos[mCenterIdx].z);

	m_vRadius = vRightTop - vCenter;
	m_vDistance = vRightTop - vLeftTop;
	(*ppQuadTree_Idx)->radius = D3DXVec3Length(&m_vRadius);
	(*ppQuadTree_Idx)->distance = D3DXVec3Length(&m_vDistance);

}

HRESULT QuadTree::ReadyComponent()
{
	// FrusutumColling
	m_pFrustum = (Frustum*)ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Frustum");
	if (m_pFrustum == nullptr)
		return E_FAIL;

	m_pCamPos = m_pFrustum->Get_CamPos();

	return NO_ERROR;
}


QuadTree * QuadTree::Create(LPDIRECT3DDEVICE9 pDevice)
{
	QuadTree* pInstance = new QuadTree(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Frustum Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * QuadTree::Clone()
{
	AddRef();
	m_bClone = true;
	return this;
}

DWORD QuadTree::Free()
{
	Safe_Release(m_pFrustum);
	VIBuffer::Free();

	return DWORD();
}
