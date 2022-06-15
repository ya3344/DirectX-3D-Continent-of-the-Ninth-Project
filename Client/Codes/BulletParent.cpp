#include "stdafx.h"
#include "BulletParent.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "PlayerAnim.h"
#include "EnemyAI.h"

_USING(Client)


BulletParent::BulletParent(LPDIRECT3DDEVICE9 pDevice)
	:GameObject(pDevice)
	,m_pEnemyMgr(EnemyMgr::GetInstance())
	,m_pUtilityMgr(UtilityMgr::GetInstance())
{
	//m_pEnemyMgr->AddRef();
	m_pUtilityMgr->AddRef();
}

BulletParent::BulletParent(const BulletParent & rhs)
	: GameObject(rhs)
	, m_pMeshObject(rhs.m_pMeshObject)
	, m_pEnemyMgr(EnemyMgr::GetInstance())
	, m_pUtilityMgr(UtilityMgr::GetInstance())
{
	//m_pEnemyMgr->AddRef();
	m_pUtilityMgr->AddRef();
	m_pMeshObject->AddRef();
}

HRESULT BulletParent::InitBullet(size_t bulletType, const D3DXVECTOR3& vBulletDir, const D3DXVECTOR3& vBulletPos)
{
	mBulletType = bulletType;

	if (FAILED(ReadyComponent()))
		return E_FAIL;

	{
		m_pPlayerTransform = ((Engine::Transform*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Transform"));
		if (m_pPlayerTransform == nullptr)
			return E_FAIL;

		m_pPlayerCollider = ((Engine::Collider*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Collider_Box"));
		if (m_pPlayerCollider == nullptr)
			return E_FAIL;

		m_pPlayerMesh = ((Engine::DynamicMesh*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Mesh_Player"));
		if (m_pPlayerMesh == nullptr)
			return E_FAIL;

		m_pHandMatrix = m_pPlayerMesh->Get_BoneMatrix("_86");
		if (m_pHandMatrix == nullptr)
			return E_FAIL;
	}

	m_pPlayerDir = m_pPlayerTransform->GetInfo(Engine::Transform::INFO_LOOK);
	m_pPlayerPos = m_pPlayerTransform->GetInfo(Engine::Transform::INFO_POSITION);

	m_pBulletPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);
	m_pBulletDir = m_pTransform->GetInfo(Engine::Transform::INFO_LOOK);

	m_pPlaneY = m_pNavigation->GetPlaneY();
	m_pTransform->SetPlaneY(m_pPlaneY); // 주소전달

	m_pEnemyAI_Space = m_pEnemyMgr->Get_EnemyAISpace();


	mt19937 mtRand((int)(rand()));
	uniform_real_distribution<float> range(-15.f, 15.f);

	// BulletPos Setting
	switch (mBulletType)
	{
	case BULLET_BOOM:
		m_vBulletDir = *m_pPlayerDir;
		D3DXVec3Normalize(&m_vBulletDir, &m_vBulletDir);
		m_vBullet_RightDir = *m_pPlayerTransform->GetInfo(Engine::Transform::INFO_RIGHT);
		D3DXVec3Normalize(&m_vBullet_RightDir, &m_vBullet_RightDir);

		m_vBulletPos = *m_pPlayerPos + (m_vBulletDir * 7.5f) + (-m_vBullet_RightDir * 1.f);
		m_vBulletPos.y += 4.5f;

		m_pTransform->SetScale(D3DXVECTOR3(0.6f, 0.6f, 0.6f));
		m_pTransform->SetPos(m_vBulletPos);
		break;
	case BULLET_SNIPER:
		m_vBulletDir = *m_pPlayerDir;
		D3DXVec3Normalize(&m_vBulletDir, &m_vBulletDir);
		m_vBullet_RightDir = *m_pPlayerTransform->GetInfo(Engine::Transform::INFO_RIGHT);
		D3DXVec3Normalize(&m_vBullet_RightDir, &m_vBullet_RightDir);

		m_vBulletPos = *m_pPlayerPos + (m_vBulletDir * 7.5f) + (-m_vBullet_RightDir * 1.5f);
		m_vBulletPos.y += 2.f;

		m_pTransform->SetScale(D3DXVECTOR3(0.2f, 0.2f, 0.2f));
		m_pTransform->SetPos(m_vBulletPos);
		break;
	case BULLET_ARROW:
		//vLook.y += m_pHandMatrix->m[3][1];
		m_vBulletDir = *m_pPlayerPos;
		//m_vBulletDir = *(D3DXVECTOR3*)&m_pHandMatrix->m[3][0];
		m_vBulletPos = vBulletPos;
		m_vBulletDir = m_pUtilityMgr->CalTargetDir(m_vBulletPos, m_vBulletDir);

		m_vBulletPos.y += 3.5f;
		m_pTransform->SetPos(m_vBulletPos);
		mTargetDegree = m_pUtilityMgr->CalDirDegree(*m_pBulletDir, m_vBulletDir);
		break;
	case BULLET_ICEMISSILE:
		m_vBulletPos = vBulletPos;
		m_vIceMissile_Range.x = range(mtRand);
		m_vIceMissile_Range.y = range(mtRand);
		m_vIceMissile_Range.z = range(mtRand);
		
		m_vBulletPos += m_vIceMissile_Range;
		m_vBulletPos += vBulletDir * 20.f;
		m_vBulletPos.y += 35.f;
		m_pTransform->SetPos(m_vBulletPos);
		m_pTransform->SetRotation(D3DXVECTOR3(90.f, 0.f, 0.f));
		m_pTransform->SetScale(D3DXVECTOR3(2.f, 2.f, 2.f));
		break;
	case BULLET_ICEMISSILE2:
		m_vBulletPos = vBulletPos;
		m_vIceMissile_Range.x = range(mtRand);
		m_vIceMissile_Range.y = range(mtRand);
		m_vIceMissile_Range.z = range(mtRand);

		m_vBulletPos += m_vIceMissile_Range;
		m_vBulletPos += vBulletDir * 20.f;
		m_vBulletPos.y += 35.f;
		m_pTransform->SetPos(m_vBulletPos);
		m_pTransform->SetRotation(D3DXVECTOR3(180.f, 0.f, 0.f));
		//m_pTransform->SetScale(D3DXVECTOR3(2.f, 2.f, 2.f));
		break;
	default:
		assert(false && "BulletParent InitBullet default!");
		break;
	}
	
	return NOERROR;
}

UPDATE BulletParent::LastUpdate(const float timeDelta)
{
	if (m_bDie)
		return UPDATE::DIE;

	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;

	return GameObject::LastUpdate(timeDelta);
}

void BulletParent::Render()
{
	if (m_pMeshObject == nullptr || m_pTransform == nullptr || m_pShader == nullptr)
		return;

	if (mBulletType == BULLET_BOOM || mBulletType == BULLET_SNIPER)
		return;

	if (m_bDie)
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
		m_pColliderSphere->Render();
	}
}



HRESULT BulletParent::ReadyPrototype(const TCHAR * pMeshName)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Mesh
	pComponent = m_pMeshObject = (Engine::StaticMesh*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_BULLET, pMeshName);
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(pMeshName, pComponent)))
		return E_FAIL;

	return NOERROR;
}

HRESULT BulletParent::ReadyComponent()
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
	if (FAILED(GameObject::AddComponent(L"Component_Shader_Mesh", pComponent)))
		return E_FAIL;

	// For.Com_Navigation
	{
		pComponent = m_pNavigation = (Engine::Navigation*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Navigation");
		if (nullptr == pComponent)
			return E_FAIL;
		if (FAILED(Engine::GameObject::AddComponent(L"Component_Navigation", pComponent)))
			return E_FAIL;
	}

	// For.Com_Collider Sphere
	{
		pComponent = m_pColliderSphere = (Engine::Collider*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Collider_Sphere");
		if (pComponent == nullptr)
			return E_FAIL;
		if (FAILED(Engine::GameObject::AddComponent(L"Component_Collider_Sphere", pComponent)))
			return E_FAIL;

		switch (mBulletType)
		{
		case BULLET_ARROW:
			if (FAILED(m_pColliderSphere->SetUp_Collider(D3DXVECTOR3(-0.3f, -0.3f, -0.5f), D3DXVECTOR3(0.3f, 0.3f, 0.f), m_pTransform->Get_WorldMatrix())))
				return E_FAIL;
			break;
		case BULLET_BOOM:
			if (FAILED(m_pColliderSphere->SetUp_Collider(m_pMeshObject->Get_MeshInfo().vMin, m_pMeshObject->Get_MeshInfo().vMax, m_pTransform->Get_WorldMatrix())))
				return E_FAIL;
			break;
		case BULLET_ICEMISSILE:
			if (FAILED(m_pColliderSphere->SetUp_Collider(m_pMeshObject->Get_MeshInfo().vMin, m_pMeshObject->Get_MeshInfo().vMax, m_pTransform->Get_WorldMatrix())))
				return E_FAIL;
			break;
		case BULLET_ICEMISSILE2:
			if (FAILED(m_pColliderSphere->SetUp_Collider(m_pMeshObject->Get_MeshInfo().vMin, m_pMeshObject->Get_MeshInfo().vMax, m_pTransform->Get_WorldMatrix())))
				return E_FAIL;
			break;
		default:
			if (FAILED(m_pColliderSphere->SetUp_Collider(m_pMeshObject->Get_MeshInfo().vMin * 0.5f, m_pMeshObject->Get_MeshInfo().vMax * 0.5f, m_pTransform->Get_WorldMatrix())))
				return E_FAIL;
			break;
		}

	}

	{
		//Component_Player_Anim
		m_pPlayerAnim = (PlayerAnim*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Anim_Player", L"Prototype_GameObject_Player");
		m_pPlayerAnim->AddRef();

		m_pAnimState = m_pPlayerAnim->GetAnimState();
	}

	return NOERROR;
}

HRESULT BulletParent::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

Engine::GameObject * BulletParent::Clone()
{
	return nullptr;
}

DWORD BulletParent::Free(void)
{
	Engine::Safe_Release(m_pMeshObject);
	//Engine::Safe_Release(m_pEnemyMgr);
	Engine::Safe_Release(m_pUtilityMgr);

	if (m_bClone)
	{
		Engine::Safe_Release(m_pTransform);
		Engine::Safe_Release(m_pRenderer);
		Engine::Safe_Release(m_pShader);
		Engine::Safe_Release(m_pColliderSphere);
		Engine::Safe_Release(m_pNavigation);
		Engine::Safe_Release(m_pPlayerAnim);
	}

	GameObject::Free();

	return DWORD();
}
