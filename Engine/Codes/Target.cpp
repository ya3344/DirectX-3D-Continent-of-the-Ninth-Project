#include "Target.h"

_USING(Engine)

Target::Target(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

HRESULT Target::Init(const size_t width, const size_t height, D3DFORMAT format, D3DXCOLOR color)
{
	// 우선 텍스쳐를 먼저 생성하자.
	if (FAILED(D3DXCreateTexture(m_pDevice, width, height, 1, D3DUSAGE_RENDERTARGET, format, D3DPOOL_DEFAULT, &m_pTargetTexture)))
		return E_FAIL;

	// 텍스쳐로부터 렌더타겟의 정보를 얻어온다.
	if (FAILED(m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface)))
		return E_FAIL;

	mClearColor = color;

	return NOERROR;
}

HRESULT Target::ClearTarget()
{
	if (m_pDevice == nullptr)
		return E_FAIL;

	m_pDevice->GetRenderTarget(0, &m_pOldSurface);

	if (FAILED(m_pDevice->SetRenderTarget(0, m_pTargetSurface)))
		return E_FAIL;

	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, mClearColor, 1.f, 0);

	m_pDevice->SetRenderTarget(0, m_pOldSurface);

	Safe_Release(m_pOldSurface);

	return NOERROR;
}

HRESULT Target::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName)
{
	if (m_pTargetTexture == nullptr)
		return E_FAIL;

	pEffect->SetTexture(pConstantName, m_pTargetTexture);

	return NOERROR;
}

HRESULT Target::SetUp_RenderTarget(const size_t index)
{
	if (m_pDevice == nullptr)
		return E_FAIL;

	m_pDevice->GetRenderTarget(index, &m_pOldSurface);

	if (FAILED(m_pDevice->SetRenderTarget(index, m_pTargetSurface)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Target::Release_RenderTarget(const size_t index)
{

	if (GetKeyState('Q') & 0x8000)
	{
		if (index == 1)
		{
			D3DXSaveTextureToFile(L"../Bin/Target.jpg", D3DXIFF_JPG, m_pTargetTexture, nullptr);
		}
	}

	if (FAILED(m_pDevice->SetRenderTarget(index, m_pOldSurface)))
		return E_FAIL;

	Safe_Release(m_pOldSurface);

	return NOERROR;
}

HRESULT Target::Ready_BufferDebug(const float startX, const float startY, const float sizeX, const float sizeY)
{
	if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(VTX_VIEWPORT_TEX) * 4, 0, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTX_VIEWPORT_TEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = D3DXVECTOR4(startX, startY, 0.f, 1.f);
	pVertices[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVertices[1].vPosition = D3DXVECTOR4(startX + sizeX, startY, 0.f, 1.f);
	pVertices[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVertices[2].vPosition = D3DXVECTOR4(startX + sizeX, startY + sizeY, 0.f, 1.f);
	pVertices[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVertices[3].vPosition = D3DXVECTOR4(startX, startY + sizeY, 0.f, 1.f);
	pVertices[3].vTexUV = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();

	if (FAILED(m_pDevice->CreateIndexBuffer(sizeof(POLYGON16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	POLYGON16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();


	return NOERROR;
}

void Target::Render_BufferDebug()
{
	m_pDevice->SetTexture(0, m_pTargetTexture);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTX_VIEWPORT_TEX));
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

Target * Target::Create(LPDIRECT3DDEVICE9 pDevice, const size_t width, const size_t height, D3DFORMAT format, D3DXCOLOR color)
{
	Target*	pInstance = new Target(pDevice);

	if (FAILED(pInstance->Init(width, height, format, color)))
	{
		_MSG_BOX("Target Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Target::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	Safe_Release(m_pTargetSurface);
	Safe_Release(m_pTargetTexture);
	Safe_Release(m_pDevice);

	return 0;
}
