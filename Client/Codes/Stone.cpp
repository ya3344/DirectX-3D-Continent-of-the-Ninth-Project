#include "stdafx.h"
#include "Stone.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"


_USING(Client)

Stone::Stone(LPDIRECT3DDEVICE9 pDevice)
	:GameObject(pDevice)
{
}

Stone::Stone(const Stone & rhs)
	:GameObject(rhs)
{
}

HRESULT Stone::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	m_pTransform->SetScale(D3DXVECTOR3(3.f, 3.f, 3.f));
	m_pTransform->SetPos(D3DXVECTOR3((float)(rand() % 20), 0.f, (float)(rand() % 20)));

	return NOERROR;
}

UPDATE Stone::Update(const float timeDelta)
{
	/*Engine::Component* pTerrainBuffer = Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_STAGE, L"Layer_BackGround", L"Component_Buffer_Terrain");

	if (pTerrainBuffer == nullptr)
		return UPDATE::FAIL;

	m_pTransform->ComputeY_OnBuffer((Engine::VIBuffer*)pTerrainBuffer);*/

	return GameObject::Update(timeDelta);
}

UPDATE Stone::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;
	bool m_bCollision = false;

	Engine::Collider *pPlayerCollider = (Engine::Collider*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Collider_Box", L"MisticWeapon");
	if ( m_bCollision = m_pCollider->Collision_OBB(pPlayerCollider))
	{
#ifdef _DEBUG
#ifdef COLLISION_CHECK
		cout << "Collision Check" << endl;
#endif
#endif
	}
	if (m_pFrustum->FrustumColling(m_pTransform->GetInfo(Engine::Transform::INFO_POSITION), 1.f))
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
			return UPDATE::FAIL;
	}



	return GameObject::LastUpdate(timeDelta);
}

void Stone::Render()
{
	if (m_pMeshObject == nullptr || m_pTransform == nullptr || m_pShader == nullptr)
		return;


	// 뷰랑 투영도 다 가지고 있는 상태.
	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pMeshObject->Render(pEffect);

	pEffect->EndPass();
	pEffect->End();

	if (g_bColliderDraw)
	{
		m_pCollider->Render();
	}

}

HRESULT Stone::ReadyPrototype()
{
	return NOERROR;
}

HRESULT Stone::ReadyComponent()
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

	// For.Com_Mesh
	pComponent = m_pMeshObject = (Engine::StaticMesh*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STAGE, L"Component_Mesh_Stone");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Mesh_Stone", pComponent)))
		return E_FAIL;

	// For.Com_Shader
	pComponent = m_pShader = (Engine::Shader*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SHADER, L"Component_Shader_Mesh");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Shader_Mesh", pComponent)))
		return E_FAIL;

	// For.Com_Collider
	pComponent = m_pCollider = (Engine::Collider*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Collider_Box");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Collider_Box", pComponent)))
		return E_FAIL;

	if (FAILED(m_pCollider->SetUp_Collider(m_pMeshObject->Get_MeshInfo().vMin, m_pMeshObject->Get_MeshInfo().vMax, m_pTransform->Get_WorldMatrix())))
		return E_FAIL;

	// For.Com_Frustum
	pComponent = m_pFrustum = (Engine::Frustum*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Frustum");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Frustum", pComponent)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Stone::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	m_pTransform->SetUp_OnShader(pEffect, "g_matWorld");

	D3DXMATRIX matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(1.f, -1.f, 1.f, 0.f));
	pEffect->SetVector("g_vLightDiffuse", &D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));
	pEffect->SetVector("g_vLightSpecular", &D3DXVECTOR4(1.f, 1.f, 0.f, 1.f));

	pEffect->SetFloat("g_fPower", 20);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (D3DXVECTOR4*)&matView.m[3][0]);


	return NOERROR;
}

Stone * Stone::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Stone*	pInstance = new Stone(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Stone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Stone::Clone()
{
	Stone*	pInstance = new Stone(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Stone Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Stone::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pMeshObject);
	Engine::Safe_Release(m_pShader);
	Engine::Safe_Release(m_pCollider);
	Engine::Safe_Release(m_pFrustum);

	GameObject::Free();

	return DWORD();
}
