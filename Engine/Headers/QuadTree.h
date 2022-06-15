#ifndef QUADTREE_H__
#define QUADTREE_H__

#include "VIBuffer.h"
#include "QuadTreeStruct.h"

_BEGIN(Engine)

class Frustum;

class _ENGINE_DLL QuadTree final : public VIBuffer
{
private:
	explicit QuadTree(LPDIRECT3DDEVICE9 pDevice);
	virtual ~QuadTree() = default;

public:
	HRESULT Init();

public:
	HRESULT Recursive_QuadTree(QUAD_TREE_IDX** ppQuadTree_Idx);
	void Recursive_QuadTreeCulling(const QUAD_TREE_IDX* pQuadTree_Idx, size_t* pPoloygonCnt);
	void Recursive_QuadTreeDelete(QUAD_TREE_IDX * pQuadTree_Idx);

public:
	void Cal_MinQuadTree(const QUAD_TREE_IDX* pQuadTree_Idx, size_t* pPoloygonCnt);
	void Culling(const QUAD_TREE_IDX* pQuadTree_Idx, size_t* pPoloygonCnt);
	void CalInfo(QUAD_TREE_IDX** ppQuadTree_Idx);

public:
	void SetVtxPos(D3DXVECTOR3* pVtxPos) { m_pVtxPos = pVtxPos; }
	void SetIdxInfo(IDX_INFO* pIdxInfo) { m_pIdxInfo = pIdxInfo; }

private:
	HRESULT ReadyComponent();

public:
	static QuadTree* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Component* Clone();
	virtual DWORD Free();

private:
	static constexpr float MIN_RANGE = 1.5f;

private:
	Frustum * m_pFrustum = nullptr;

private: // QuadTree Related Variable
	float mRange = 0.f;
	float mRadius = 1.f;
	float mDistance = 0.f;
	size_t mCenterIdx = 0;

private:
	D3DXVECTOR3 m_vRadius = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vDistance = D3DXVECTOR3(0.f, 0.f, 0.f);
	const D3DXVECTOR3* m_pCamPos = nullptr;
	QUAD_TREE_IDX* m_pQuadTree_Idx = nullptr;

private: // Buffer Related Variable
	size_t mPolygonCnt = 0;
	IDX_INFO* m_pIdxInfo = nullptr;
};

_END

#endif

