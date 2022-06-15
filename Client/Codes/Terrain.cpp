#include "stdafx.h"
#include "Terrain.h"
#include "ComponentMgr.h"
#include "Include.h"

_USING(Client)

Terrain::Terrain(LPDIRECT3DDEVICE9 pDevice)
	: GameObject(pDevice)
	, mScale(1.f)
{

}

Terrain::Terrain(const Terrain & rhs)
	: GameObject(rhs)
	, m_pMeshObject(rhs.m_pMeshObject)
	, mScale(rhs.mScale)
{
	m_pMeshObject->AddRef();
}

HRESULT Terrain::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	//m_pTransform->SetScale(D3DXVECTOR3(mScale, mScale, mScale));

	return NOERROR;
}

UPDATE Terrain::Update(const float timeDelta)
{
	return GameObject::Update(timeDelta);
}

UPDATE Terrain::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (m_pFrustum->FrustumColling(&m_vCenter, mRadius))
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
			return UPDATE::FAIL;
	}

	return GameObject::LastUpdate(timeDelta);
}

void Terrain::Render()
{
	if (m_pMeshObject == nullptr || m_pTransform == nullptr || m_pShader == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pMeshObject->Render(pEffect);

	pEffect->EndPass();
	pEffect->End();

	if (g_bColliderDraw)
	{
		m_pCollider->Render();
	}
	
}

HRESULT Terrain::ReadyPrototype(const TCHAR* pMeshName)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Mesh
	pComponent = m_pMeshObject = (Engine::StaticMesh*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SOULVALLEY, pMeshName);
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(pMeshName, pComponent)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Terrain::ReadyComponent()
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Transform
	pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Transform", pComponent)))
		return E_FAIL;

	// For.Com_Renderer
	pComponent = m_pRenderer = (Engine::Renderer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Renderer");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Renderer", pComponent)))
		return E_FAIL;

	// For.Com_Shader
	pComponent = m_pShader = (Engine::Shader*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SHADER, L"Component_Shader_Mesh");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Engine::GameObject::AddComponent(L"Component_Shader_Mesh", pComponent)))
		return E_FAIL;

	// For.Com_Collider
	pComponent = m_pCollider = (Engine::Collider*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Collider_Box");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Collider_Box", pComponent)))
		return E_FAIL;

	if (FAILED(m_pCollider->SetUp_Collider(m_pMeshObject->Get_MeshInfo().vMin, m_pMeshObject->Get_MeshInfo().vMax, m_pTransform->Get_WorldMatrix(), 
		&m_vCenter, &mRadius)))
		return E_FAIL;

	mRadius += 1.f;

	// For.Com_Frustum
	pComponent = m_pFrustum = (Engine::Frustum*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Frustum");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Frustum", pComponent)))
		return E_FAIL;


	return NOERROR;
}

HRESULT Terrain::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	m_pTransform->SetUp_OnShader(pEffect, "g_matWorld");

	D3DXMATRIX matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	//pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(1.f, -1.f, 1.f, 0.f));
	//pEffect->SetVector("g_vLightDiffuse", &D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));
	//pEffect->SetVector("g_vLightSpecular", &D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));

	//pEffect->SetFloat("g_fPower", 20);

	//D3DXMatrixInverse(&matView, nullptr, &matView);
	//pEffect->SetVector("g_vCamPosition", (D3DXVECTOR4*)&matView.m[3][0]);
	
	return NOERROR;
}

Terrain * Terrain::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshName)
{
	Terrain* pInstance = new Terrain(pDevice);

	if (FAILED(pInstance->ReadyPrototype(pMeshName)))
	{
		_MSG_BOX("Terrain Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Terrain::Clone()
{
	Terrain* pInstance = new Terrain(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Terrain Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Terrain::Free(void)
{
	Engine::Safe_Release(m_pMeshObject);

	if (m_bClone)
	{
		Engine::Safe_Release(m_pTransform);
		Engine::Safe_Release(m_pRenderer);
		Engine::Safe_Release(m_pShader);
		Engine::Safe_Release(m_pCollider);
		Engine::Safe_Release(m_pFrustum);
	}

	Engine::GameObject::Free();

	return 0;
}
