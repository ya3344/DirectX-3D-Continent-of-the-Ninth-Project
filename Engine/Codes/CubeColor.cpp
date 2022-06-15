#include "CubeColor.h"

_USING(Engine)

CubeColor::CubeColor(LPDIRECT3DDEVICE9 pDevice)
	: VIBuffer(pDevice)
{
}

CubeColor::CubeColor(const CubeColor & rhs)
	: VIBuffer(rhs)
{

}

HRESULT CubeColor::InitBuffer()
{
	mVtxInfo = VTX_INFO(sizeof(VTXCOL), 8, VTXCOL_FVF);
	mIdxInfo = IDX_INFO(sizeof(POLYGON16), 12, D3DFMT_INDEX16);

	VIBuffer::CreateBuffer();

	VTXCOL* pVertex = nullptr;
	mVtxInfo.pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVertex[0].color = D3DCOLOR_ARGB(255, 255, 255, 0);

	pVertex[1].vPos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVertex[1].color = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVertex[2].vPos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVertex[2].color = D3DCOLOR_ARGB(255, 0, 255, 0);

	pVertex[3].vPos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVertex[3].color = D3DCOLOR_ARGB(255, 255, 0, 255);

	pVertex[4].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVertex[4].color = D3DCOLOR_ARGB(255, 255, 255, 0);

	pVertex[5].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVertex[5].color = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVertex[6].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVertex[6].color = D3DCOLOR_ARGB(255, 0, 255, 0);

	pVertex[7].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVertex[7].color = D3DCOLOR_ARGB(255, 255, 0, 255);

	// 정점 위치 저장 다른 클래스에서 쓰기 위함
	Set_VtxPos(pVertex);

	mVtxInfo.pVB->Unlock();

	POLYGON16* pIndex = nullptr;
	mIdxInfo.pIB->Lock(0, 0, (void**)&pIndex, 0);

	//전면
	pIndex[0]._1 = 0;	pIndex[1]._1 = 0;
	pIndex[0]._2 = 1;	pIndex[1]._2 = 2;
	pIndex[0]._3 = 2;	pIndex[1]._3 = 3;

	//후면
	pIndex[2]._1 = 5;	pIndex[3]._1 = 5;
	pIndex[2]._2 = 4;	pIndex[3]._2 = 7;
	pIndex[2]._3 = 7;	pIndex[3]._3 = 6;

	//상단
	pIndex[4]._1 = 4;	pIndex[5]._1 = 4;
	pIndex[4]._2 = 5;	pIndex[5]._2 = 1;
	pIndex[4]._3 = 1;	pIndex[5]._3 = 0;

	//하단
	pIndex[6]._1 = 3;	pIndex[7]._1 = 3;
	pIndex[6]._2 = 2;	pIndex[7]._2 = 6;
	pIndex[6]._3 = 6;	pIndex[7]._3 = 7;

	//좌측
	pIndex[8]._1 = 4;	pIndex[9]._1 = 4;
	pIndex[8]._2 = 0;	pIndex[9]._2 = 3;
	pIndex[8]._3 = 3;	pIndex[9]._3 = 7;

	//우측
	pIndex[10]._1 = 1;	pIndex[11]._1 = 1;
	pIndex[10]._2 = 5;	pIndex[11]._2 = 6;
	pIndex[10]._3 = 6;	pIndex[11]._3 = 2;

	mIdxInfo.pIB->Unlock();

	return NOERROR;
}

CubeColor * CubeColor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CubeColor* pInstance = new CubeColor(pDevice);

	if (FAILED(pInstance->InitBuffer()))
	{
		_MSG_BOX("CubeColor Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * CubeColor::Clone()
{
	return new CubeColor(*this);
}

DWORD CubeColor::Free()
{
	VIBuffer::Free();
	return 0;
}
