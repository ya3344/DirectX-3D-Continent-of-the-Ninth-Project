#include "CubeTex.h"

_USING(Engine)

CubeTex::CubeTex(LPDIRECT3DDEVICE9 pDevice)
	: VIBuffer(pDevice)
{

}

CubeTex::CubeTex(const CubeTex & rhs)
	: VIBuffer(rhs)
{

}

HRESULT CubeTex::InitBuffer()
{
	mVtxInfo = VTX_INFO(sizeof(VTXCUBE), 8, VTXFVF_CUBE);
	mIdxInfo = IDX_INFO(sizeof(POLYGON16), 12, D3DFMT_INDEX16);

	VIBuffer::CreateBuffer();

	VTXCUBE* pVertex = nullptr;
	mVtxInfo.pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVertex[1].vPos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVertex[2].vPos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVertex[3].vPos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);

	pVertex[4].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVertex[5].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVertex[6].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVertex[7].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	for (DWORD i = 0; i < mVtxInfo.vtxNum; ++i)
		pVertex[i].vTex = pVertex[i].vPos;

	// 정점 위치 저장 다른 클래스에서 쓰기 위함
	Set_VtxPos(pVertex);

	mVtxInfo.pVB->Unlock();

	POLYGON16* pIndex = nullptr;
	mIdxInfo.pIB->Lock(0, 0, (void**)&pIndex, 0);



	// +x
	pIndex[0]._1 = 1; pIndex[0]._2 = 5; pIndex[0]._3 = 6;
	pIndex[1]._1 = 1; pIndex[1]._2 = 6; pIndex[1]._3 = 2;

	// -x
	pIndex[2]._1 = 4; pIndex[2]._2 = 0; pIndex[2]._3 = 3;
	pIndex[3]._1 = 4; pIndex[3]._2 = 3; pIndex[3]._3 = 7;

	// +y
	pIndex[4]._1 = 4; pIndex[4]._2 = 5; pIndex[4]._3 = 1;
	pIndex[5]._1 = 4; pIndex[5]._2 = 1; pIndex[5]._3 = 0;

	// -y
	pIndex[6]._1 = 3; pIndex[6]._2 = 2; pIndex[6]._3 = 6;
	pIndex[7]._1 = 3; pIndex[7]._2 = 6; pIndex[7]._3 = 7;

	// +z
	pIndex[8]._1 = 7; pIndex[8]._2 = 6; pIndex[8]._3 = 5;
	pIndex[9]._1 = 7; pIndex[9]._2 = 5; pIndex[9]._3 = 4;

	// -z
	pIndex[10]._1 = 0; pIndex[10]._2 = 1; pIndex[10]._3 = 2;
	pIndex[11]._1 = 0; pIndex[11]._2 = 2; pIndex[11]._3 = 3;

	//전면
	//pIndex[0]._1 = 0;	pIndex[1]._1 = 0;
	//pIndex[0]._2 = 1;	pIndex[1]._2 = 2;
	//pIndex[0]._3 = 2;	pIndex[1]._3 = 3;

	////후면
	//pIndex[2]._1 = 5;	pIndex[3]._1 = 5;
	//pIndex[2]._2 = 4;	pIndex[3]._2 = 7;
	//pIndex[2]._3 = 7;	pIndex[3]._3 = 6;

	////상단
	//pIndex[4]._1 = 4;	pIndex[5]._1 = 4;
	//pIndex[4]._2 = 5;	pIndex[5]._2 = 1;
	//pIndex[4]._3 = 1;	pIndex[5]._3 = 0;

	////하단
	//pIndex[6]._1 = 3;	pIndex[7]._1 = 3;
	//pIndex[6]._2 = 2;	pIndex[7]._2 = 6;
	//pIndex[6]._3 = 6;	pIndex[7]._3 = 7;

	////좌측
	//pIndex[8]._1 = 4;	pIndex[9]._1 = 4;
	//pIndex[8]._2 = 0;	pIndex[9]._2 = 3;
	//pIndex[8]._3 = 3;	pIndex[9]._3 = 7;

	////우측
	//pIndex[10]._1 = 1;	pIndex[11]._1 = 1;
	//pIndex[10]._2 = 5;	pIndex[11]._2 = 6;
	//pIndex[10]._3 = 6;	pIndex[11]._3 = 2;

	mIdxInfo.pIB->Unlock();

	return NOERROR;
}

CubeTex * CubeTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CubeTex* pInstance = new CubeTex(pDevice);

	if (FAILED(pInstance->InitBuffer()))
	{
		_MSG_BOX("CubeTex Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * CubeTex::Clone()
{
	return new CubeTex(*this);
}

DWORD CubeTex::Free()
{
	VIBuffer::Free();
	return 0;
}
