#include "Cell.h"
_USING(Engine)

Cell::Cell(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

HRESULT Cell::Init(D3DXVECTOR3** ppPoint, const size_t index)
{
	mIndex = index;
	for (size_t i = 0; i < NEIGHBOR::NEIGHBOR_END; i++)
	{
		m_pNeighbor[i] = nullptr;
	}

	for (size_t i = 0; i < POINT_END; i++)
	{
		m_pPoint[i] = ppPoint[i];
	}

	if (FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Cell::Render()
{
	if (m_pLine == nullptr)
		return E_FAIL;

	D3DXVECTOR3 vPoint[4];

	for (size_t i = 0; i < POINT_END; i++)
	{
		vPoint[i] = *m_pPoint[i];
		vPoint[i].y = m_pPoint[i]->y + 1.f;
	}

	vPoint[3] = *m_pPoint[0];
	vPoint[3].y = m_pPoint[0]->y + 1.f;

	D3DXMATRIX matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pLine->SetWidth(4.f);

	for (size_t i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
		if (0.f > vPoint[i].z)
			vPoint[i].z = 0.f;
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	D3DXMATRIX matTmp;

	m_pLine->Begin();

	switch (mCellState)
	{
	case CELL_NORMAL:
		m_pLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTmp), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		break;
	case CELL_JUMP:
		m_pLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTmp), D3DXCOLOR(0.f, 1.f, 1.f, 1.f));
		break;
	default:
		m_pLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTmp), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		break;
	}

	m_pLine->End();

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	return NOERROR;
}

bool Cell::ComparePoint(const D3DXVECTOR3& vFrist, const D3DXVECTOR3& vSecond)
{
	if (*m_pPoint[POINT_A] == vFrist)
	{
		if (*m_pPoint[POINT_B] == vSecond)
			return true;
		if (*m_pPoint[POINT_C] == vSecond)
			return true;
	}

	if (*m_pPoint[POINT_B] == vFrist)
	{
		if (*m_pPoint[POINT_A] == vSecond)
			return true;
		if (*m_pPoint[POINT_C] == vSecond)
			return true;
	}

	if (*m_pPoint[POINT_C] == vFrist)
	{
		if (*m_pPoint[POINT_A] == vSecond)
			return true;
		if (*m_pPoint[POINT_B] == vSecond)
			return true;
	}

	return false;
}

bool Cell::is_InCell(NEIGHBOR * pNeighborID, const D3DXVECTOR3& vTargetPos, D3DXVECTOR3* pLineNormal)
{
	D3DXVECTOR2	vLineDir, vNormal;
	D3DXVECTOR2 vCompare;

	vLineDir = D3DXVECTOR2(m_pPoint[POINT_B]->x, m_pPoint[POINT_B]->z) - D3DXVECTOR2(m_pPoint[POINT_A]->x, m_pPoint[POINT_A]->z);
	vNormal = D3DXVECTOR2(vLineDir.y * -1.f, vLineDir.x);

	*pLineNormal = D3DXVECTOR3(vNormal.x, m_pPoint[POINT_B]->y, vNormal.y);

	vCompare = D3DXVECTOR2(vTargetPos.x, vTargetPos.z) - D3DXVECTOR2(m_pPoint[POINT_A]->x, m_pPoint[POINT_A]->z);

	if (D3DXVec2Dot(D3DXVec2Normalize(&vNormal, &vNormal), D3DXVec2Normalize(&vCompare, &vCompare)) > 0)
	{
		*pNeighborID = NEIGHBOR_AB;
		return false;
	}

	vLineDir = D3DXVECTOR2(m_pPoint[POINT_C]->x, m_pPoint[POINT_C]->z) - D3DXVECTOR2(m_pPoint[POINT_B]->x, m_pPoint[POINT_B]->z);
	vNormal = D3DXVECTOR2(vLineDir.y * -1.f, vLineDir.x);

	*pLineNormal = D3DXVECTOR3(vNormal.x, m_pPoint[POINT_C]->y, vNormal.y);

	vCompare = D3DXVECTOR2(vTargetPos.x, vTargetPos.z) - D3DXVECTOR2(m_pPoint[POINT_B]->x, m_pPoint[POINT_B]->z);

	if (D3DXVec2Dot(D3DXVec2Normalize(&vNormal, &vNormal), D3DXVec2Normalize(&vCompare, &vCompare)) > 0)
	{
		*pNeighborID = NEIGHBOR_BC;
		return false;
	}

	vLineDir = D3DXVECTOR2(m_pPoint[POINT_A]->x, m_pPoint[POINT_A]->z) - D3DXVECTOR2(m_pPoint[POINT_C]->x, m_pPoint[POINT_C]->z);
	vNormal = D3DXVECTOR2(vLineDir.y * -1.f, vLineDir.x);

	*pLineNormal = D3DXVECTOR3(vNormal.x, m_pPoint[POINT_A]->y, vNormal.y);

	vCompare = D3DXVECTOR2(vTargetPos.x, vTargetPos.z) - D3DXVECTOR2(m_pPoint[POINT_C]->x, m_pPoint[POINT_C]->z);

	if (D3DXVec2Dot(D3DXVec2Normalize(&vNormal, &vNormal), D3DXVec2Normalize(&vCompare, &vCompare)) > 0)
	{
		*pNeighborID = NEIGHBOR_CA;
		return false;
	}

	return true;
}

bool Cell::is_InLineCell(NEIGHBOR * pNeighborID, const D3DXVECTOR2 & vTargetPos, D3DXVECTOR2* pLineA, D3DXVECTOR2* pLineB)
{
	D3DXVECTOR2	vLineDir, vNormal;
	D3DXVECTOR2 vCompare;

	vLineDir = D3DXVECTOR2(m_pPoint[POINT_B]->x, m_pPoint[POINT_B]->z) - D3DXVECTOR2(m_pPoint[POINT_A]->x, m_pPoint[POINT_A]->z);
	vNormal = D3DXVECTOR2(vLineDir.y * -1.f, vLineDir.x);

	*pLineA = D3DXVECTOR2(m_pPoint[POINT_B]->x, m_pPoint[POINT_B]->z);
	*pLineB = D3DXVECTOR2(m_pPoint[POINT_A]->x, m_pPoint[POINT_A]->z);

	vCompare = vTargetPos - D3DXVECTOR2(m_pPoint[POINT_A]->x, m_pPoint[POINT_A]->z);

	if (D3DXVec2Dot(D3DXVec2Normalize(&vNormal, &vNormal), D3DXVec2Normalize(&vCompare, &vCompare)) > 0)
	{
		*pNeighborID = NEIGHBOR_AB;
		return false;
	}

	vLineDir = D3DXVECTOR2(m_pPoint[POINT_C]->x, m_pPoint[POINT_C]->z) - D3DXVECTOR2(m_pPoint[POINT_B]->x, m_pPoint[POINT_B]->z);
	vNormal = D3DXVECTOR2(vLineDir.y * -1.f, vLineDir.x);

	*pLineA = D3DXVECTOR2(m_pPoint[POINT_C]->x, m_pPoint[POINT_C]->z);
	*pLineB = D3DXVECTOR2(m_pPoint[POINT_B]->x, m_pPoint[POINT_B]->z);

	vCompare = vTargetPos - D3DXVECTOR2(m_pPoint[POINT_B]->x, m_pPoint[POINT_B]->z);

	if (D3DXVec2Dot(D3DXVec2Normalize(&vNormal, &vNormal), D3DXVec2Normalize(&vCompare, &vCompare)) > 0)
	{
		*pNeighborID = NEIGHBOR_BC;
		return false;
	}

	vLineDir = D3DXVECTOR2(m_pPoint[POINT_A]->x, m_pPoint[POINT_A]->z) - D3DXVECTOR2(m_pPoint[POINT_C]->x, m_pPoint[POINT_C]->z);
	vNormal = D3DXVECTOR2(vLineDir.y * -1.f, vLineDir.x);

	*pLineA = D3DXVECTOR2(m_pPoint[POINT_A]->x, m_pPoint[POINT_A]->z);
	*pLineB = D3DXVECTOR2(m_pPoint[POINT_C]->x, m_pPoint[POINT_C]->z);

	vCompare = vTargetPos - D3DXVECTOR2(m_pPoint[POINT_C]->x, m_pPoint[POINT_C]->z);

	if (D3DXVec2Dot(D3DXVec2Normalize(&vNormal, &vNormal), D3DXVec2Normalize(&vCompare, &vCompare)) > 0)
	{
		*pNeighborID = NEIGHBOR_CA;
		return false;
	}

	return true;
}

bool Cell::is_InCell(const D3DXVECTOR3 & vPos)
{
	D3DXVECTOR2	vLineDir, vNormal;
	D3DXVECTOR2 vCompare;

	vLineDir = D3DXVECTOR2(m_pPoint[POINT_B]->x, m_pPoint[POINT_B]->z) - D3DXVECTOR2(m_pPoint[POINT_A]->x, m_pPoint[POINT_A]->z);
	vNormal = D3DXVECTOR2(vLineDir.y * -1.f, vLineDir.x);

	vCompare = D3DXVECTOR2(vPos.x, vPos.z) - D3DXVECTOR2(m_pPoint[POINT_A]->x, m_pPoint[POINT_A]->z);

	if (D3DXVec2Dot(D3DXVec2Normalize(&vNormal, &vNormal), D3DXVec2Normalize(&vCompare, &vCompare)) > 0)
	{
		return false;
	}

	vLineDir = D3DXVECTOR2(m_pPoint[POINT_C]->x, m_pPoint[POINT_C]->z) - D3DXVECTOR2(m_pPoint[POINT_B]->x, m_pPoint[POINT_B]->z);
	vNormal = D3DXVECTOR2(vLineDir.y * -1.f, vLineDir.x);


	vCompare = D3DXVECTOR2(vPos.x, vPos.z) - D3DXVECTOR2(m_pPoint[POINT_B]->x, m_pPoint[POINT_B]->z);

	if (D3DXVec2Dot(D3DXVec2Normalize(&vNormal, &vNormal), D3DXVec2Normalize(&vCompare, &vCompare)) > 0)
	{
		return false;
	}

	vLineDir = D3DXVECTOR2(m_pPoint[POINT_A]->x, m_pPoint[POINT_A]->z) - D3DXVECTOR2(m_pPoint[POINT_C]->x, m_pPoint[POINT_C]->z);
	vNormal = D3DXVECTOR2(vLineDir.y * -1.f, vLineDir.x);

	vCompare = D3DXVECTOR2(vPos.x, vPos.z) - D3DXVECTOR2(m_pPoint[POINT_C]->x, m_pPoint[POINT_C]->z);

	if (D3DXVec2Dot(D3DXVec2Normalize(&vNormal, &vNormal), D3DXVec2Normalize(&vCompare, &vCompare)) > 0)
	{
		return false;
	}

	return true;
}

void Cell::Cal_IntersectPlaneY(const D3DXVECTOR3& vPos)
{
	D3DXVECTOR3 vStatPoint = D3DXVECTOR3(vPos.x, vPos.y + Y_RANGE, vPos.z);
	D3DXVECTOR3 vEndPoint = D3DXVECTOR3(vPos.x, vPos.y - Y_RANGE, vPos.z);

	D3DXPlaneFromPoints(&mPlane, m_pPoint[POINT_A], m_pPoint[POINT_B], m_pPoint[POINT_C]);

	D3DXPlaneIntersectLine(&mPlaneY, &mPlane, &vStatPoint, &vEndPoint);

	//pPos->y = mPlaneY.y;
}

Cell * Cell::Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3** ppPoint, const size_t index)
{
	Cell* pInstance = new Cell(pDevice);

	if (FAILED(pInstance->Init(ppPoint, index)))
	{
		_MSG_BOX("Cell Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Cell::Free()
{
	Safe_Release(m_pLine);
	Safe_Release(m_pDevice);

	return DWORD();
}
