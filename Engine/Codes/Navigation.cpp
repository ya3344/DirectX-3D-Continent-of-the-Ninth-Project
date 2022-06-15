#include "Navigation.h"
#include "Cell.h"
#include "KeyMgr.h"

_USING(Engine)

Navigation::Navigation(LPDIRECT3DDEVICE9 pDevice)
	:Component(pDevice)
{
}

Navigation::Navigation(const Navigation & rhs)
	:Component(rhs)
	,mCellSpace(rhs.mCellSpace)
	,mPointSpace(rhs.mPointSpace)
{
	for (Cell* pCell : mCellSpace)
	{
		pCell->AddRef();
	}
}

HRESULT Navigation::Init(const TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	size_t naviSize = 0;
	size_t index = 0;
	D3DXVECTOR3 centerPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 point[POINT_END];
	size_t cellState = CELL_NORMAL;

	DWORD byte = 0;

	ReadFile(hFile, &naviSize, sizeof(size_t), &byte, nullptr);
	mCellSpace.reserve(naviSize);

	for (size_t i = 0; i < naviSize; i++)
	{
		ReadFile(hFile, &index, sizeof(size_t), &byte, nullptr);

		for (size_t i = 0; i < POINT_END; i++)
		{
			ReadFile(hFile, &point[i], sizeof(D3DXVECTOR3), &byte, nullptr);
		}

		ReadFile(hFile, &centerPos, sizeof(D3DXVECTOR3), &byte, nullptr);
		ReadFile(hFile, &cellState, sizeof(size_t), &byte, nullptr);

		LoadCell(point, index, centerPos, cellState);
	}

	CloseHandle(hFile);
	SetUp_Neighbor();

	return NOERROR;
}

HRESULT Navigation::Render()
{


	if (KeyMgr::GetInstance()->KeyDown(DIK_2))
	{
		m_bDraw = !m_bDraw;
	}

	if (m_bDraw == false)
		return NOERROR;

	for (Cell* pCell : mCellSpace)
	{
		if (pCell != nullptr)
		{
			pCell->Render();
		}
	}

	return NOERROR;
}

HRESULT Navigation::LoadCell(const D3DXVECTOR3 * pPickedPos, const size_t index, const D3DXVECTOR3 & vCenterPos, const size_t cellState)
{
	D3DXVECTOR3 vRange = D3DXVECTOR3(0.f, 0.f, 0.f);

	for (size_t i = 0; i < POINT_END; i++)
	{
		m_pPoint[i] = nullptr;
	}

	for (size_t i = 0; i < POINT_END; i++)
	{
		m_bAdjacent = false;

		for (D3DXVECTOR3* pPoint : mPointSpace)
		{
			vRange = pPickedPos[i] - *pPoint;

			if (D3DXVec3Length(&vRange) <= CELL_SIZE)
			{
				m_pPoint[i] = pPoint;
				m_bAdjacent = true;
				break;
			}
		}

		if (m_bAdjacent == false)
		{
			if (m_pPoint[i] != nullptr)
			{
				_MSG_BOX("Adjacent Error!!!!!");
				return E_FAIL;
			}
			m_pPoint[i] = new D3DXVECTOR3;
			*m_pPoint[i] = pPickedPos[i];
			mPointSpace.emplace_back(m_pPoint[i]);
		}
	}

	Cell* pCell = Cell::Create(m_pDevice, m_pPoint, index);
	if (pCell == nullptr)
		return E_FAIL;

	pCell->Set_CellState(cellState);
	pCell->SetCenterPos(vCenterPos);
	mCellSpace.emplace_back(pCell);

	return NOERROR;
}

bool Navigation::is_Move(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vDir, D3DXVECTOR3* pLineNormal)
{
	D3DXVECTOR3 vResultPos = vPos + vDir;

	Cell::NEIGHBOR neighborID;

	bool bIn = mCellSpace[mCurrentIndex]->is_InCell(&neighborID, vResultPos, pLineNormal);
	Cal_IntersectPlaneY(vResultPos);

	if (bIn)
		return true;
	else
	{
		const Cell* pNeighbor = mCellSpace[mCurrentIndex]->GetNeighbor(neighborID);
		
		if (pNeighbor != nullptr)
		{
			mCurrentIndex = pNeighbor->GetIndex();
			//cout << "NaviIndex =" << mCurrentIndex << endl;
		
			return is_Move(vPos, vDir, pLineNormal);
		}
		else
			return false;
	}
}

bool Navigation::CameraMove(const D3DXVECTOR2& vResultPos, D3DXVECTOR2* pLineA, D3DXVECTOR2* pLineB)
{
	Cell::NEIGHBOR neighborID;

	bool bIn = mCellSpace[mCurrentIndex]->is_InLineCell(&neighborID, vResultPos, pLineA, pLineB);

	if (bIn)
		return true;
	else
	{ 
		const Cell* pNeighbor = mCellSpace[mCurrentIndex]->GetNeighbor(neighborID);

		if (pNeighbor != nullptr)
		{
			mCurrentIndex = pNeighbor->GetIndex();
			return true;
		}
		else
			return false;
	}
}

void Navigation::Cal_IntersectPlaneY(const D3DXVECTOR3& vPos)
{
	mCellSpace[mCurrentIndex]->Cal_IntersectPlaneY(vPos);
	GetPlaneY();
}

void Navigation::Cal_JumpIndex()
{
	if (mCellSpace[mCurrentIndex]->Get_CellState() == CELL_JUMP)
	{
		if (mCurrentIndex >= 136 && mCurrentIndex <= 141)
		{
			mCurrentIndex = 142;
		}
		else if (mCurrentIndex >= 142 && mCurrentIndex <= 147)
		{
			mCurrentIndex = 136;
		}

	}
}

bool Navigation::is_InCell(const D3DXVECTOR3 & vPos)
{
	return mCellSpace[mCurrentIndex]->is_InCell(vPos);
}

size_t Navigation::Get_CellState()
{
	return mCellSpace[mCurrentIndex]->Get_CellState();
}

D3DXVECTOR3 * Navigation::GetPlaneY()
{
	m_vPlaneY = mCellSpace[mCurrentIndex]->GetPlaneY();

	return &m_vPlaneY;
}


const D3DXVECTOR3 & Navigation::GetCenterPos(const size_t index)
{
	if (index == 0)
	{
		return mCellSpace[mCurrentIndex]->GetCenterPos();
	}

	return mCellSpace[index]->GetCenterPos();
}

float Navigation::Get_CenterPosY()
{
	return mCellSpace[mCurrentIndex]->GetCenterPos().y;
}

HRESULT Navigation::SetUp_Neighbor()
{
	for (size_t i = 0; i < mCellSpace.size(); i++)
	{
		for (size_t j = 0; j < mCellSpace.size(); j++)
		{
			if (mCellSpace[i] == mCellSpace[j])
				continue;

			if (mCellSpace[j]->ComparePoint(*mCellSpace[i]->GetPoint(POINT_A), *mCellSpace[i]->GetPoint(POINT_B)))
				mCellSpace[i]->SetNeighbor(Cell::NEIGHBOR_AB, mCellSpace[j]);

			if (mCellSpace[j]->ComparePoint(*mCellSpace[i]->GetPoint(POINT_B), *mCellSpace[i]->GetPoint(POINT_C)))
				mCellSpace[i]->SetNeighbor(Cell::NEIGHBOR_BC, mCellSpace[j]);

			if (mCellSpace[j]->ComparePoint(*mCellSpace[i]->GetPoint(POINT_C), *mCellSpace[i]->GetPoint(POINT_A)))
				mCellSpace[i]->SetNeighbor(Cell::NEIGHBOR_CA, mCellSpace[j]);
		}
	}
	return NOERROR;
}

Navigation * Navigation::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR * pFilePath)
{
	Navigation*	pInstance = new Navigation(pDevice);

	if (FAILED(pInstance->Init(pFilePath)))
	{
		_MSG_BOX("Navigation Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Component * Navigation::Clone()
{
	return new Navigation(*this);
}

DWORD Navigation::Free()
{
	for (Cell* pCell : mCellSpace)
	{
		Safe_Release(pCell);
	}
	mCellSpace.clear();

	if (m_bClone == false)
	{
		for (D3DXVECTOR3* pPoint : mPointSpace)
		{
			Safe_Delete(pPoint);
		}
		mPointSpace.clear();
	}

	Component::Free();

	return DWORD();
}
