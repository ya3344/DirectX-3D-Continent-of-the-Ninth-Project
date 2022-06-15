#ifndef TERRAIN_BUFFER_H__
#define TERRAIN_BUFFER_H__

#include "VIBuffer.h"
#include "QuadTreeStruct.h"

_BEGIN(Engine)

class Frustum;
class QuadTree;

class _ENGINE_DLL TerrainBuffer final : public VIBuffer
{
private:
	explicit TerrainBuffer(LPDIRECT3DDEVICE9 pDevice);
	explicit TerrainBuffer(const TerrainBuffer& rhs);
	virtual ~TerrainBuffer() = default;

public:
	HRESULT InitBuffer(const TCHAR* pTerrainInfo, const TCHAR* pHeightMap);
	UPDATE FrustumColling();
	UPDATE QuadTreeColling();

private:
	HRESULT ReadyComponent();
	HRESULT FileRead(const TCHAR* pTerrainInfo, const TCHAR* pHeightMap);
	void Cal_NormalVector(D3DXVECTOR3* pOut, D3DXVECTOR3* p0, D3DXVECTOR3* p1, D3DXVECTOR3* p2);

public:
	static TerrainBuffer* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pTerrainInfo, const TCHAR* pHeightMap);
	virtual Component* Clone() override;
	virtual DWORD Free() override;

private:
	size_t mVtxNumX = 0;
	size_t mVtxNumZ = 0;
	float mInterval = 0.f;

private:
	DWORD* m_pPixel = nullptr;
	Frustum* m_pFrustum = nullptr;
	QuadTree* m_pQuadTree = nullptr;
	QUAD_TREE_IDX* m_pQuadTree_Idx = nullptr;
	size_t mPolygonCnt = 0;

};

_END

#endif //TERRAIN_BUFFER_H__

