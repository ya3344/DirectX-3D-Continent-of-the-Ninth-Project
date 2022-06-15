#include "stdafx.h"
#include "DecalBox.h"
#include "ComponentMgr.h"
#include "TargetManager.h"

_USING(Client)

DecalBox::DecalBox(LPDIRECT3DDEVICE9 pDevice)
	:GameObject(pDevice)
{

}

DecalBox::DecalBox(const DecalBox & rhs)
	:GameObject(rhs)
{

}

HRESULT DecalBox::ReadyPrototype()
{
	return NOERROR;
}

HRESULT DecalBox::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	m_bDecal = false;

	m_pMatWorld = m_pTransform->Get_WorldMatrix();
	
	m_pTargetMgr = Engine::TargetManager::GetInstance();

	return NOERROR;
}

UPDATE DecalBox::LastUpdate(const float timeDelta)
{
	if (m_bDecal == false)
		return UPDATE::NORMAL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_ALPHA, this)))
		return UPDATE::FAIL;

	return Engine::GameObject::LastUpdate(timeDelta);
}

void DecalBox::SetTargetMatrix(const D3DXMATRIX * pTargetMatrix)
{
	{ m_pTransform->Set_WorldMatrix(*pTargetMatrix); }
	
}

void DecalBox::SetDecalPos(const D3DXVECTOR3 & vPos)
{
	m_pTransform->SetPos(vPos);
}

void DecalBox::SetScale(const D3DXVECTOR3 & vScale)
{
	m_pTransform->SetScale(vScale);
}

HRESULT DecalBox::ReadyComponent(void)
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
		pComponent = m_pShader = (Engine::Shader*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SHADER, L"Component_Shader_Decal");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_Shader_Decal", pComponent)))
			return E_FAIL;

		m_pEffect = m_pShader->Get_EffectHandle();
	}

	// For.Com_Texture
	{
		// For.Com_Texture
		pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_EFFECT, L"Component_Decal");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_Decal", pComponent)))
			return E_FAIL;
	}

	// For.Com_CubeBuffer
	{
		pComponent = m_pCubeBuffer = (Engine::CubeTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Buffer_CubeTexture");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_Buffer_CubeTexture", pComponent)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT DecalBox::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	m_pTransform->SetUp_OnShader(pEffect, "g_matWorld");

	D3DXMATRIX matWorld, matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	matWorld = *m_pMatWorld;
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);

	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);
	pEffect->SetMatrix("g_matWorldInv", &matWorld);

	pEffect->SetVector("g_vColor", &m_vColor);
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", mTextureNum);
	pEffect->SetFloat("g_fAlpha", mAlpha);

	m_pTargetMgr->SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTex");

	return NOERROR;
}


DWORD DecalBox::Free()
{
	if (m_bClone)
	{
		Engine::Safe_Release(m_pTransform);
		Engine::Safe_Release(m_pShader);
		Engine::Safe_Release(m_pRenderer);
		Engine::Safe_Release(m_pTexture);
		Engine::Safe_Release(m_pCubeBuffer);
	}

	GameObject::Free();

	return DWORD();
}
