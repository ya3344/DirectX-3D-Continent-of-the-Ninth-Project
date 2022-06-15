#include "stdafx.h"
#include "StaticObject.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"

_USING(Client)



StaticObject::StaticObject(LPDIRECT3DDEVICE9 pDevice)
	:GameObject(pDevice)
{
}

StaticObject::StaticObject(const StaticObject & rhs)
	: GameObject(rhs)
	, m_pMeshObject(rhs.m_pMeshObject)
	, mStaticObject_Info(rhs.mStaticObject_Info)
{
	m_pMeshObject->AddRef();
}

HRESULT StaticObject::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	return NOERROR;
}

UPDATE StaticObject::Update(const float timeDelta)
{
	return Engine::GameObject::Update(timeDelta);
}

UPDATE StaticObject::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (m_pFrustum->FrustumColling(&m_vCenterPos, mRadius))
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
			return UPDATE::FAIL;
	}

	return GameObject::LastUpdate(timeDelta);
}


void StaticObject::Render()
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
	pEffect->BeginPass(1);

	m_pMeshObject->Render(pEffect);

	pEffect->EndPass();
	pEffect->End();

	if (g_bColliderDraw)
	{
		m_pColliderBox->Render();
	}
}

HRESULT StaticObject::ReadyPrototype(const STATIC_OBJECT_INFO & staticObject_Info)
{
	Engine::Component* pComponent = nullptr;
	const TCHAR* pMeshTag = nullptr;
	mStaticObject_Info = staticObject_Info;

	switch (mStaticObject_Info.objectType)
	{
	case OBJECT_CRISTAL:
		pMeshTag = L"Component_Mesh_Cristal_1";
		break;
	case OBJECT_CRISTAL2:
		pMeshTag = L"Component_Mesh_Cristal_2";
		break;
	case OBEJCT_ICE_ROCK:
		pMeshTag = L"Component_Mesh_Ice_Rock_1";
		break;
	case OBJECT_LAMP_BONE_A:
		pMeshTag = L"Component_Mesh_Lamp_Bone_A";
		break;
	case OBJECT_LAMP_BONE_B:
		pMeshTag = L"Component_Mesh_Lamp_Bone_B";
		break;
	case OBJECT_LAMP_BONE_C:
		pMeshTag = L"Component_Mesh_Lamp_Bone_C";
		break;
	default:
		break;
	}

	// For.Com_Mesh
	pComponent = m_pMeshObject = (Engine::StaticMesh*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC_MESH, pMeshTag);
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(pMeshTag, pComponent)))
		return E_FAIL;



	return NOERROR;
}

HRESULT StaticObject::ReadyComponent()
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Shader
	pComponent = m_pShader = (Engine::Shader*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SHADER, L"Component_Shader_Mesh");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Shader_Mesh", pComponent)))
		return E_FAIL;

	// For.Com_Transform
	pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
	if (pComponent == nullptr)
		return E_FAIL;
	m_pTransform->Set_WorldMatrix(mStaticObject_Info.m_matWorld);

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Transform", pComponent)))
		return E_FAIL;

	// For.Com_Renderer
	pComponent = m_pRenderer = (Engine::Renderer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Renderer");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Renderer", pComponent)))
		return E_FAIL;

	// For.Com_Collider_Box
	pComponent = m_pColliderBox = (Engine::Collider*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Collider_Box");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Engine::GameObject::AddComponent(L"Component_Collider_Box", pComponent)))
		return E_FAIL;

	if (FAILED(m_pColliderBox->SetUp_Collider(m_pMeshObject->Get_MeshInfo().vMin, m_pMeshObject->Get_MeshInfo().vMax, m_pTransform->Get_WorldMatrix()
				,&m_vCenterPos, &mRadius)))
		return E_FAIL;

	D3DXVec3TransformCoord(&m_vCenterPos, &m_vCenterPos, &mStaticObject_Info.m_matWorld);
	mRadius *= mStaticObject_Info.m_matWorld.m[1][1];

	// For.Com_Frustum
	pComponent = m_pFrustum = (Engine::Frustum*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Frustum");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Frustum", pComponent)))
		return E_FAIL;
	

	return NOERROR;
}

HRESULT StaticObject::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	m_pTransform->SetUp_OnShader(pEffect, "g_matWorld", false);

	D3DXMATRIX matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	return NOERROR;
}

StaticObject * StaticObject::Create(LPDIRECT3DDEVICE9 pDevice, const STATIC_OBJECT_INFO& staticObject_Info)
{
	StaticObject* pInstance = new StaticObject(pDevice);

	if (FAILED(pInstance->ReadyPrototype(staticObject_Info)))
	{
		_MSG_BOX("StaticObject Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * StaticObject::Clone()
{
	StaticObject* pInstance = new StaticObject(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("StaticObject Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD StaticObject::Free(void)
{
	Engine::Safe_Release(m_pMeshObject);

	if (m_bClone)
	{
		Engine::Safe_Release(m_pTransform);
		Engine::Safe_Release(m_pRenderer);
		Engine::Safe_Release(m_pShader);
		Engine::Safe_Release(m_pFrustum);
		Engine::Safe_Release(m_pColliderBox);
	}

	GameObject::Free();

	return DWORD();
}
