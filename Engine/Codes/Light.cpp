#include "..\Headers\Light.h"
#include "KeyMgr.h"

_USING(Engine)

Light::Light(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

HRESULT Light::Init(const LIGHT_INFO& lgihtInfo)
{
	mLightInfo = lgihtInfo;

	D3DVIEWPORT9 ViewPort;
	m_pDevice->GetViewport(&ViewPort);

	if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(VTX_VIEWPORT_TEX) * 4, 0, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTX_VIEWPORT_TEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = D3DXVECTOR4(0.0f - 0.5f, 0.f - 0.5f, 0.f, 1.f);
	pVertices[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVertices[1].vPosition = D3DXVECTOR4((float)ViewPort.Width - 0.5f, 0.0f -0.5f, 0.f, 1.f);
	pVertices[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVertices[2].vPosition = D3DXVECTOR4((float)ViewPort.Width - 0.5f, (float)ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertices[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVertices[3].vPosition = D3DXVECTOR4(0.f - 0.5f, (float)ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertices[3].vTexUV = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();

	if (FAILED(m_pDevice->CreateIndexBuffer(sizeof(POLYGON16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	POLYGON16* pIndices = nullptr;

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

void Light::Render(LPD3DXEFFECT pEffect)
{
	size_t passIdx = 0;

	D3DXMATRIX matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	m_vCamPosition = *(D3DXVECTOR3*)&matView.m[3][0];

	pEffect->SetVector("g_vCamPosition", (D3DXVECTOR4*)&matView.m[3][0]);

	if (mLightInfo.Type == D3DLIGHT_DIRECTIONAL)
	{
		passIdx = 0;
		pEffect->SetVector("g_vLightDir", (D3DXVECTOR4*)&mLightInfo.Direction);
	}
	else if (mLightInfo.Type == D3DLIGHT_POINT)
	{
		passIdx = 1;
		pEffect->SetVector("g_vLightPos", (D3DXVECTOR4*)&mLightInfo.Position);

		mDistance = CalCameraDistance();

		if (mDistance > LIGHT_OPTIMIZATION_RANGE + mLightInfo.Range)
			return;

		pEffect->SetFloat("g_fRange", mLightInfo.Range);
	}

	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)&mLightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)&mLightInfo.Ambient);
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)&mLightInfo.Specular);

	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	pEffect->BeginPass(passIdx);

	pEffect->CommitChanges();

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTX_VIEWPORT_TEX));
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
}

void Light::BackBuffer_Render()
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTX_VIEWPORT_TEX));
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

float Light::CalCameraDistance()
{
	D3DXVECTOR3 vLightPosition = (D3DXVECTOR3)mLightInfo.Position;
	D3DXVECTOR3 vCamPosition = m_vCamPosition;
	float distance = 0.f;

	vLightPosition.y = 0;
	vCamPosition.y = 0;

	m_vDistance = vLightPosition - vCamPosition;
	distance = D3DXVec3Length(&m_vDistance);

	return distance;
}

Light * Light::Create(LPDIRECT3DDEVICE9 pDevice, const LIGHT_INFO& lgihtInfo)
{
	Light* pInstance = new Light(pDevice);

	if (FAILED(pInstance->Init(lgihtInfo)))
	{
		_MSG_BOX("Light Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Light::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pDevice);

	return DWORD();
}
