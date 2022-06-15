#include "stdafx.h"
#include "MisticWeapon.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "PlayerAnim.h"
#include "Trail.h"
#include "BackLogo.h"


_USING(Client)

MisticWeapon::MisticWeapon(LPDIRECT3DDEVICE9 pDevice)
	:GameObject(pDevice)
{
}

MisticWeapon::MisticWeapon(const MisticWeapon & rhs)
	: GameObject(rhs)
{
}

HRESULT MisticWeapon::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;
	
	m_pPlayerMesh = ((Engine::DynamicMesh*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Mesh_Player"));
	if (m_pPlayerMesh == nullptr)
		return E_FAIL;

	m_pHandMatrix = m_pPlayerMesh->Get_BoneMatrix("_86");
	if (m_pHandMatrix == nullptr)
		return E_FAIL;

	m_pPlayerTransform = ((Engine::Transform*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Transform"));
	if (m_pPlayerTransform == nullptr)
		return E_FAIL;

	PlayerAnim* pPlayerAnim = ((PlayerAnim*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Anim_Player"));
	if (pPlayerAnim == nullptr)
		return E_FAIL;

	if (FAILED(pPlayerAnim->Set_WeaponCollider(m_pColliderBox, m_pColliderSphere)))
		return E_FAIL;

	m_pTransform->SetScale(D3DXVECTOR3(0.025f, 0.025f, 0.025f));
	m_pTransform->SetPos(D3DXVECTOR3(0.f, 0.001f, -0.11f));

	m_pTrail = (Trail*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_TRAIL, L"Layer_Trail", L"Prototype_GameObject_PlayerTrail");
	
	m_vTrailPosA = m_pMeshObject->Get_MeshInfo().vMin;
	m_vTrailPosB = m_pMeshObject->Get_MeshInfo().vMax;

	//m_vTrailPosA = D3DXVECTOR3(0.08757f, 0.1490367f, -0.8199937f);
	//m_vTrailPosA = D3DXVECTOR3(0.10757052f, 0.1490367f, -0.8199936f); // 길이가 긴 트레일 
	m_vTrailPosA = D3DXVECTOR3(0.1375712f, -0.9109630f, 3.5600037f); // 길이가 짧은 트레일 
	m_vTrailPosB = D3DXVECTOR3(0.29257065f, -0.9811105f, 7.345501f);
	m_pTrail->SetInitPoint(m_vTrailPosA, m_vTrailPosB);
	m_pTrail->SetTrailState(true);
	m_pTrail->SetColor(D3DXVECTOR4(1.3f, 1.3f, 1.3f, 0.f));
	return NOERROR;
}

UPDATE MisticWeapon::Update(const float timeDelta)
{
#ifdef _DEBUG
#ifdef BONE_TEST
	static int count = 0;
	string temp;
	char buffer[5] = "";
	if (m_pKeyMgr->KeyDown(DIK_1))
	{
		++count;
		if (count > 133)
		{
			count = 0;
		}
		
		temp += "_";
		_itoa_s(count, buffer, 10);
		temp += buffer;
		cout << "Bone Name: " << temp.c_str() << endl;
		m_pHandMatrix = m_pPlayerMesh->Get_BoneMatrix(temp.c_str());
	}
#endif

#ifdef TRAIL_NOT_OPERATION
	m_pTrail->SetTrailState(false);
#endif
#endif
	
	D3DXMATRIX matParent = *m_pHandMatrix * *m_pPlayerTransform->Get_WorldMatrix();

	m_pTransform->SetUp_ParentMatrix(&matParent);
	m_pTransform->UpdateTransform();

	if (m_pTrail != nullptr)
	{
		//if (m_pKeyMgr->KeyPressing(DIK_NUMPAD4))
		//{
		//	m_vTrailPosA.x += 0.05f;
		////	m_vTrailPosB.x += 0.05f;
		//	//m_pTrail->SetInitPoint(m_vTrailPosA, m_vTrailPosB);
		//}
		//if (m_pKeyMgr->KeyPressing(DIK_NUMPAD1))
		//{
		//	m_vTrailPosA.x -= 0.02f;
		////	m_vTrailPosB.x -= 0.01f;

		//	//m_pTrail->SetInitPoint(m_vTrailPosA, m_vTrailPosB);
		//}
		//if (m_pKeyMgr->KeyPressing(DIK_NUMPAD5))
		//{
		//	m_vTrailPosA.y += 0.02f;
		////	m_vTrailPosB.z += 0.05f;

		//	//m_pTrail->SetInitPoint(m_vTrailPosA, m_vTrailPosB);
		//}
		//if (m_pKeyMgr->KeyPressing(DIK_NUMPAD2))
		//{
		//	m_vTrailPosA.y -= 0.02f;
		////	m_vTrailPosB.y -= 0.01f;

		//	//m_pTrail->SetInitPoint(m_vTrailPosA, m_vTrailPosB);
		//}
		//if (m_pKeyMgr->KeyPressing(DIK_NUMPAD6))
		//{
		//	m_vTrailPosA.z += 0.02f;
		//	//m_vTrailPosB.z += 0.01f;

		//	//m_pTrail->SetInitPoint(m_vTrailPosA, m_vTrailPosB);
		//}
		//if (m_pKeyMgr->KeyPressing(DIK_NUMPAD3))
		//{
		//	m_vTrailPosA.z -= 0.02f;
		//	//m_vTrailPosB.z -= 0.01f;
		//}

		//m_pTrail->SetInitPoint(m_vTrailPosA, m_vTrailPosB);
		m_pTrail->SetTargetMatrix(*m_pTransform->Get_FinalMatrix());
	}


	return Engine::GameObject::Update(timeDelta);
}

UPDATE MisticWeapon::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;



	return GameObject::LastUpdate(timeDelta);
}

void MisticWeapon::Render()
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
		m_pColliderBox->Render();
		m_pColliderSphere->Render();
	}

}

HRESULT MisticWeapon::ReadyPrototype()
{
	return NOERROR;
}

HRESULT MisticWeapon::ReadyComponent()
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
	pComponent = m_pMeshObject = (Engine::StaticMesh*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_PLAYER, L"Component_Mesh_MisticWeapon");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Mesh_MisticWeapon", pComponent)))
		return E_FAIL;

	// For.Com_Shader
	pComponent = m_pShader = (Engine::Shader*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SHADER, L"Component_Shader_Mesh");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Shader_Mesh", pComponent)))
		return E_FAIL;

	// For.Com_Collider Box
	pComponent = m_pColliderBox = (Engine::Collider*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Collider_Box");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Collider_Box", pComponent)))
		return E_FAIL;

	if (FAILED(m_pColliderBox->SetUp_Collider(D3DXVECTOR3(-1.0f, -2.3f, 1.f), D3DXVECTOR3(1.1f, 0.9f, 7.5f), m_pTransform->Get_FinalMatrix())))
		return E_FAIL;

	// For.Com_Collider Sphere
	{
		pComponent = m_pColliderSphere = (Engine::Collider*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Collider_Sphere");
		if (pComponent == nullptr)
			return E_FAIL;
		if (FAILED(Engine::GameObject::AddComponent(L"Component_Collider_Sphere", pComponent)))
			return E_FAIL;

		if (FAILED(m_pColliderSphere->SetUp_Collider(D3DXVECTOR3(-1.5f, -1.5f, 2.f), D3DXVECTOR3(1.5f, 1.5f, 5.f), m_pTransform->Get_FinalMatrix())))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT MisticWeapon::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	m_pTransform->SetUp_OnShader(pEffect, "g_matWorld", true);

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

MisticWeapon * MisticWeapon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	MisticWeapon* pInstance = new MisticWeapon(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("MisticWeapon Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * MisticWeapon::Clone()
{
	MisticWeapon* pInstance = new MisticWeapon(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("MisticWeapon Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD MisticWeapon::Free(void)
{
	if (m_bClone)
	{
		Engine::Safe_Release(m_pTransform);
		Engine::Safe_Release(m_pRenderer);
		Engine::Safe_Release(m_pMeshObject);
		Engine::Safe_Release(m_pShader);
		Engine::Safe_Release(m_pColliderBox);
		Engine::Safe_Release(m_pColliderSphere);
	}

	GameObject::Free();

	return DWORD();
}
