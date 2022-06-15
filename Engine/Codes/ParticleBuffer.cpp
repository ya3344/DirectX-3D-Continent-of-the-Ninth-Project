#include "ParticleBuffer.h"

_USING(Engine)

ParticleBuffer::ParticleBuffer(LPDIRECT3DDEVICE9 pDevice)
	:VIBuffer(pDevice)
{
}

ParticleBuffer::ParticleBuffer(const ParticleBuffer & rhs)
	:VIBuffer(rhs)
{

}

HRESULT ParticleBuffer::InitBuffer()
{
	m_bParticleBuffer = true;
	mVtxInfo = VTX_INFO(sizeof(VTXPOINT), 1, D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_TEX1);

	if (FAILED(m_pDevice->CreateVertexBuffer(mVtxInfo.vtxSize * mVtxInfo.vtxNum, D3DUSAGE_DYNAMIC, mVtxInfo.vtxFVF, D3DPOOL_SYSTEMMEM, &mVtxInfo.pVB, nullptr)))
	{
		_MSG_BOX("Create Vertex Buffer Failed");
		return E_FAIL;
	}

	m_pVtxPos = new D3DXVECTOR3[mVtxInfo.vtxNum];
	ZeroMemory(m_pVtxPos, sizeof(D3DXVECTOR3)* mVtxInfo.vtxNum);

	VTXPOINT* pVertex = nullptr;
	mVtxInfo.pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVertex[0].PSize = 1.f;
	pVertex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	//pVertex[1].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.f);
	//pVertex[1].PSize = 1.f;
	//pVertex[1].vTexUV = D3DXVECTOR2(0.f, 0.f);

	//pVertex[2].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.f);
	//pVertex[2].PSize = 1.f;
	//pVertex[2].vTexUV = D3DXVECTOR2(0.f, 0.f);

	//pVertex[3].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	//pVertex[3].PSize = 1.f;
	//pVertex[3].vTexUV = D3DXVECTOR2(0.f, 0.f);

	// ���� ��ġ ���� �ٸ� Ŭ�������� ���� ����
	Set_VtxPos(pVertex);

	return NOERROR;
}

void ParticleBuffer::RenderBuffer()
{
	if (mVtxInfo.pVB == nullptr)
		return;

	// �׸��� ���� ����� ���ؽ����۸� ��ġ�� �÷����´�.
	m_pDevice->SetStreamSource(0, mVtxInfo.pVB, 0/*����տ������� �׸���*/, mVtxInfo.vtxSize/*�󸶸�ŭ �ǳʶٸ鼭 �׷��� �ϴ°�*/);
	m_pDevice->SetFVF(mVtxInfo.vtxFVF);
	m_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, mVtxInfo.vtxNum );
}

ParticleBuffer * ParticleBuffer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	ParticleBuffer* pInstance = new ParticleBuffer(pDevice);

	if (FAILED(pInstance->InitBuffer()))
	{
		_MSG_BOX("ParticleBuffer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * ParticleBuffer::Clone()
{
	return new ParticleBuffer(*this);
}

DWORD ParticleBuffer::Free()
{
	VIBuffer::Free();
	return 0;
}
