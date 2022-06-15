#include "TrailBuffer.h"


_USING(Engine)


TrailBuffer::TrailBuffer(LPDIRECT3DDEVICE9 pDevice)
	: VIBuffer(pDevice)
{
}


TrailBuffer::TrailBuffer(const TrailBuffer & rhs)
	: VIBuffer(rhs)
{

}


HRESULT TrailBuffer::InitBuffer(const size_t vtxNum)
{
	mVtxInfo = VTX_INFO(sizeof(VTXTEX), vtxNum, VTXTEX_FVF);
	mIdxInfo = IDX_INFO(sizeof(POLYGON16), vtxNum-2, D3DFMT_INDEX16);

	VIBuffer::CreateBuffer();

	Engine::VTXTEX* pVertex = nullptr;
	mVtxInfo.pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (size_t i = 0; i < vtxNum; )
	{
		pVertex[i].vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		pVertex[i].vTexUV = D3DXVECTOR2( (i / (vtxNum - 2.f)), 0.f);
		i++;
		pVertex[i].vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		pVertex[i].vTexUV = D3DXVECTOR2( ((i-1) / (vtxNum - 2.f)), 1.f);
		i++;

		// texUV
		// 0 2 4 6 
		// 1 3 5 7
	}

	// 정점 위치 저장 다른 클래스에서 쓰기 위함
	Set_VtxPos(pVertex);

	mVtxInfo.pVB->Unlock();

	Engine::POLYGON16* pIndex = nullptr;
	mIdxInfo.pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (WORD i = 0; i < vtxNum - 2;)
	{
		pIndex[i]._1 = i;
		pIndex[i]._2 = i + 2;
		pIndex[i]._3 = i + 3;
		i++;

		pIndex[i]._1 = i - 1;
		pIndex[i]._2 = i + 2;
		pIndex[i]._3 = i;
		i++;
	}

	mIdxInfo.pIB->Unlock();

	return NOERROR;
}

TrailBuffer * TrailBuffer::Create(size_t vtxNum, LPDIRECT3DDEVICE9 pDevice)
{
	TrailBuffer* pInstance = new TrailBuffer(pDevice);

	if (FAILED(pInstance->InitBuffer(vtxNum)))
	{
		_MSG_BOX("TrailBuffer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * TrailBuffer::Clone()
{
	return new TrailBuffer(*this);
}

DWORD TrailBuffer::Free()
{
	VIBuffer::Free();
	return 0;
}
