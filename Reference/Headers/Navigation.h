#pragma once

#include "Component.h"

_BEGIN(Engine)

class Cell;

class _ENGINE_DLL Navigation final : public Component
{
private:
	explicit Navigation(LPDIRECT3DDEVICE9 pDevice);
	explicit Navigation(const Navigation& rhs);
	virtual ~Navigation() = default;

public:
	HRESULT Init(const TCHAR* pFilePath);
	HRESULT Render();
	HRESULT LoadCell(const D3DXVECTOR3 *pPickedPos, const size_t index, const D3DXVECTOR3& vCenterPos, const size_t cellState);
	
public:
	void Setup_CurrentIndex(const size_t index) { mCurrentIndex = index; }
	bool is_Move(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vDir, D3DXVECTOR3* pLineNormal);
	bool CameraMove(const D3DXVECTOR2& vResultPos, D3DXVECTOR2* pLineA, D3DXVECTOR2* pLineB);
	void Cal_IntersectPlaneY(const D3DXVECTOR3& vPos);
	void Cal_JumpIndex();
	bool is_InCell(const D3DXVECTOR3& vPos);

public:
	const D3DXVECTOR3& GetCenterPos(const size_t index = 0);
	float Get_CenterPosY();
	size_t Get_CellState();
	const size_t* Get_CurrentIndex() const { return &mCurrentIndex; }
	D3DXVECTOR3* GetPlaneY();

private:
	HRESULT SetUp_Neighbor();

public:
	static Navigation* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath);
	virtual Component* Clone() override;
	virtual DWORD Free() override;

private:
	static constexpr float CELL_SIZE = 1.f;

private:
	vector<Cell*> mCellSpace;
	size_t mCurrentIndex = 0;
	D3DXVECTOR3* m_pPoint[POINT_END];

private:
	bool m_bAdjacent = false;
	list<D3DXVECTOR3*> mPointSpace;
	D3DXVECTOR3 m_vPlaneY = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vTempNormal = D3DXVECTOR3(0.f, 0.f, 0.f);

private:
	bool m_bDraw = false;
};

_END