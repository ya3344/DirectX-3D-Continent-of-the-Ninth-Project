#include "VIBuffer.h"

_USING(Engine)

VIBuffer::VIBuffer(LPDIRECT3DDEVICE9 pDevice)
	: Component(pDevice)
{

}

VIBuffer::VIBuffer(const VIBuffer& rhs)
	: Component(rhs)
	, m_bParticleBuffer(rhs.m_bParticleBuffer)
	, mVtxInfo(rhs.mVtxInfo)
	, mIdxInfo(rhs.mIdxInfo)
	, m_pVtxPos(rhs.m_pVtxPos)
{
	mVtxInfo.pVB->AddRef();
	if (m_bParticleBuffer == false)
	{
		mIdxInfo.pIB->AddRef();
	}
}

HRESULT VIBuffer::CreateBuffer()
{
	if (FAILED(m_pDevice->CreateVertexBuffer(mVtxInfo.vtxSize * mVtxInfo.vtxNum, D3DUSAGE_DYNAMIC, mVtxInfo.vtxFVF, D3DPOOL_SYSTEMMEM, &mVtxInfo.pVB, nullptr)))
	{
		_MSG_BOX("Create Vertex Buffer Failed");
		return E_FAIL;
	}
	
	if (FAILED(m_pDevice->CreateIndexBuffer(mIdxInfo.polygonSize * mIdxInfo.polygonNum, 0, mIdxInfo.idxFmt, D3DPOOL_MANAGED, &mIdxInfo.pIB, nullptr)))
	{
		_MSG_BOX("Create Index Buffer Failed");
		return E_FAIL;
	}

	m_pVtxPos = new D3DXVECTOR3[mVtxInfo.vtxNum];
	ZeroMemory(m_pVtxPos, sizeof(D3DXVECTOR3)* mVtxInfo.vtxNum);

	return NOERROR;
}

void VIBuffer::Render()
{
	if (mVtxInfo.pVB == nullptr || mIdxInfo.pIB == nullptr)
		return;

	// �׸��� ���� ����� ���ؽ����۸� ��ġ�� �÷����´�.
	m_pDevice->SetStreamSource(0, mVtxInfo.pVB, 0/*����տ������� �׸���*/, mVtxInfo.vtxSize/*�󸶸�ŭ �ǳʶٸ鼭 �׷��� �ϴ°�*/);
	m_pDevice->SetIndices(mIdxInfo.pIB);
	m_pDevice->SetFVF(mVtxInfo.vtxFVF);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mVtxInfo.vtxNum, 0, mIdxInfo.polygonNum);
}

void VIBuffer::Set_VtxPos(const void * pVtxData)
{
	if (mVtxInfo.pVB == nullptr)
		return;

	for (size_t i = 0; i < mVtxInfo.vtxNum; i++)
	{
		m_pVtxPos[i] = *(D3DXVECTOR3*)((byte*)pVtxData + (i * mVtxInfo.vtxSize));
	}
}

DWORD VIBuffer::Free()
{
	DWORD refCnt = 0;

	if (m_bClone == false)
	{
		Safe_Delete_Array(m_pVtxPos);
		m_pVtxPos = nullptr;
	}

	// ������ �������� ������ �������Ƿ� ���⼭ refcnt�� 1�̻� �����ִ°��� üũ�� �� �ִ�.
	if (refCnt = Safe_Release(mVtxInfo.pVB))
	{
		if (!m_bClone)
			_MSG_BOX("m_pVB Release Failed!");
	}
	if (refCnt = Safe_Release(mIdxInfo.pIB))
	{
		if (!m_bClone)
			_MSG_BOX("m_pIB Release Failed!");
	}
	Component::Free();
	return 0;
}

