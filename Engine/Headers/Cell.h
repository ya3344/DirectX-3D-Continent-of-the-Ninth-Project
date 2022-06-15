#ifndef CELL_H__
#define CELL_H__

#include "Base.h"

_BEGIN(Engine)

class Cell final : public Base
{
public:
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };

private:
	explicit Cell(LPDIRECT3DDEVICE9 pDevice);
	virtual ~Cell() = default;
	
public:
	HRESULT Init(D3DXVECTOR3** ppPoint, const size_t index);
	HRESULT Render();

public:
	bool ComparePoint(const D3DXVECTOR3& vFrist, const D3DXVECTOR3& vSecond);
	bool is_InCell(NEIGHBOR* pNeighborID, const D3DXVECTOR3& vTargetPos, D3DXVECTOR3* pLineNormal);
	bool is_InLineCell(NEIGHBOR* pNeighborID, const D3DXVECTOR2 & vTargetPos, D3DXVECTOR2* pLineA, D3DXVECTOR2* pLineB);
	bool is_InCell(const D3DXVECTOR3& vPos);
	void Cal_IntersectPlaneY(const D3DXVECTOR3& vPos);

public:
	void SetNeighbor(NEIGHBOR neighbor, Cell* pCell) { m_pNeighbor[neighbor] = pCell; }
	void SetCenterPos(const D3DXVECTOR3& vCenterPos) { mCenterPos = vCenterPos; }
	void Set_CellState(size_t cellState) { mCellState = cellState; }

public:
	size_t GetIndex() const { return mIndex; }
	D3DXVECTOR3* GetPoint(size_t point) { return m_pPoint[point]; }
	const Cell* GetNeighbor(NEIGHBOR neighbor) const { return m_pNeighbor[neighbor]; }
	D3DXVECTOR3& GetCenterPos() { return mCenterPos; }
	size_t Get_CellState() const { return mCellState; }
	D3DXVECTOR3& GetPlaneY() { return mPlaneY; }

public:
	static Cell* Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3** ppPoint, const size_t index);
	virtual DWORD Free() override;

private:
	static constexpr float Y_RANGE = 50.f;
private:
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
	LPD3DXLINE m_pLine;
	D3DXVECTOR3* m_pPoint[POINT_END];
	Cell* m_pNeighbor[NEIGHBOR_END];
	size_t mIndex = 0;
	size_t mCellState = CELL_NORMAL;

private:
	D3DXVECTOR3 mCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXPLANE mPlane;
	D3DXVECTOR3 mPlaneY = D3DXVECTOR3(0.f, 0.f, 0.f);
};

_END
#endif

