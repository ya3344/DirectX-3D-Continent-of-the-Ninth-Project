#include "stdafx.h"
#include "Trail.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "VIBuffer.h"
#include "Layer.h"

_USING(Client)

Trail::Trail(LPDIRECT3DDEVICE9 pDevice)
	:GameObject(pDevice)
{

}

Trail::Trail(const Trail & rhs)
	: GameObject(rhs)
	, m_pBufferTag(rhs.m_pBufferTag)
{

}

HRESULT Trail::ReadyPrototype(const TCHAR* pBufferTag)
{
	m_pBufferTag = pBufferTag;

	return NOERROR;
}

HRESULT Trail::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	m_pMatWorld = m_pTransform->Get_WorldMatrix();
	m_bTrail = false;
	mTextureNum = 0;
	mTrailTimeGap = 0.02f;

	return NOERROR;
}



UPDATE Trail::Update(const float timeDelta)
{
	if (m_bTrail == false)
		return UPDATE::NORMAL;

	mTrail_DeltaTime += timeDelta;

	if (mTrail_DeltaTime > mTrailTimeGap)
	{
		Engine::VTXTEX* pVertex;
		D3DXVECTOR3 vNewPos[2];

		m_pVtxInfo->pVB->Lock(0, 0, (void**)&pVertex, 0);

		D3DXVECTOR3 vInitPosA = D3DXVECTOR3(10.f, 0.f, 0.f);
		D3DXVECTOR3 vInitPosB = D3DXVECTOR3(20.f, 0.f, 0.f);

		D3DXVec3TransformCoord(&vNewPos[0], &m_vInitPosA, m_pMatWorld);
		D3DXVec3TransformCoord(&vNewPos[1], &m_vInitPosB, m_pMatWorld);

		for (int i = m_pVtxInfo->vtxNum - 1; i > 1; )
		{
			pVertex[i].vPos = pVertex[i - 2].vPos;
			--i;
			pVertex[i].vPos = pVertex[i - 2].vPos;
			--i;
		}

		pVertex[0].vPos = vNewPos[0];
		pVertex[1].vPos = vNewPos[1];

		m_pVtxInfo->pVB->Unlock();

		mTrail_DeltaTime = 0.f;
	}

	return Engine::GameObject::Update(timeDelta);
}

UPDATE Trail::LastUpdate(const float timeDelta)
{
	if (m_bTrail == false)
		return UPDATE::NORMAL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_ALPHA, this)))
		return UPDATE::FAIL;

	return Engine::GameObject::LastUpdate(timeDelta);
}

void Trail::Render()
{
	if (m_pTransform == nullptr || m_pShader == nullptr)
		return;

	if (FAILED(SetUp_ConstantTable(m_pEffect)))
		return;

	m_pEffect->Begin(nullptr, 0);

	m_pEffect->BeginPass(0);

	m_pTrailBuffer->Render();

	m_pEffect->EndPass();

	m_pEffect->End();

}

void Trail::SetTargetMatrix(const D3DXMATRIX& targetMatrix)
{
	m_pTransform->Set_WorldMatrix(targetMatrix);
}

void Trail::SetInitPoint(const D3DXVECTOR3 & vInitPosA, const D3DXVECTOR3 & vInitPosB)
{
	m_vInitPosA = vInitPosA;
	m_vInitPosB = vInitPosB;
}

HRESULT Trail::ReadyComponent(void)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Transform
	{
		pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
		if (pComponent == nullptr)
			return E_FAIL;
		if (FAILED(Engine::GameObject::AddComponent(L"Component_Transform", pComponent)))
			return E_FAIL;
	}

	// For.Com_Renderer
	{
		pComponent = m_pRenderer = (Engine::Renderer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Renderer");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_Renderer", pComponent)))
			return E_FAIL;
	}

	// For.Com_Shader
	{
		pComponent = m_pShader = (Engine::Shader*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SHADER, L"Component_Shader_Trail");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_Shader_Trail", pComponent)))
			return E_FAIL;

		m_pEffect = m_pShader->Get_EffectHandle();
	}

	// For.Com_Texture
	{
		// For.Com_Texture
		pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_TRAIL, L"Component_Trail");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_Trail", pComponent)))
			return E_FAIL;
	}

	// For.Com_TrailBuffer
	{
		pComponent = m_pTrailBuffer = (Engine::TrailBuffer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, m_pBufferTag);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(m_pBufferTag, pComponent)))
			return E_FAIL;

		m_pVtxInfo = m_pTrailBuffer->Get_VtxInfo_Pointer();
		if (m_pVtxInfo == nullptr)
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT Trail::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	//m_pTransform->SetUp_OnShader(pEffect, "g_matWorld");
	D3DXMATRIX matTemp;
	D3DXMatrixIdentity(&matTemp);
	m_pEffect->SetMatrix("g_matWorld", &matTemp);

	D3DXMATRIX matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetVector("g_vColor", &m_vColor);
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", mTextureNum);

	return NOERROR;
}

Trail * Trail::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pBufferTag)
{
	Trail* pInstance = new Trail(pDevice);

	if (FAILED(pInstance->ReadyPrototype(pBufferTag)))
	{
		_MSG_BOX("Trail Created Failed!!!");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Trail::Clone()
{
	Trail* pInstance = new Trail(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Trail Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Trail::Free()
{
	if (m_bClone)
	{
		Engine::Safe_Release(m_pTransform);
		Engine::Safe_Release(m_pShader);
		Engine::Safe_Release(m_pRenderer);
		Engine::Safe_Release(m_pTexture);
		Engine::Safe_Release(m_pTrailBuffer);
	}

	GameObject::Free();

	return DWORD();
}

