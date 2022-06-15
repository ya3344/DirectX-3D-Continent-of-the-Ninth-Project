#include "stdafx.h"
#include "Enemy.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "EnemyAI.h"
#include "TimerMgr.h"
#include "Trail.h"
#include "BackLogo.h"
_USING(Client)

Enemy::Enemy(LPDIRECT3DDEVICE9 pDevice)
	: Engine::GameObject(pDevice)
{
}

Enemy::Enemy(const Enemy & rhs)
	: Engine::GameObject(rhs)
	, mNaviIndex(rhs.mNaviIndex)
	, mEnemyType(rhs.mEnemyType)
	, m_pMeshName(rhs.m_pMeshName)
{
	m_pEnemyMgr = EnemyMgr::GetInstance();
}

HRESULT Enemy::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	m_pNavigation->Setup_CurrentIndex(mNaviIndex);
	m_pTransform->SetPos(m_pNavigation->GetCenterPos(mNaviIndex));

	switch (mEnemyType)
	{
	case KNOLE_COMMANDER:
	case KNOLE_WARRIOR:
	case HUMAN_BOSS:
		mShaderIndex = 1;
		break;
	default:
		mShaderIndex = 0;
		break;
	}

	m_pUtilityMgr = UtilityMgr::GetInstance();
	m_pEnemyPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);
	m_pPlayerPos = m_pTargetTransform->GetInfo(Engine::Transform::INFO_POSITION);

	// Trail
	if (mEnemyType == HUMAN_BOSS)
	{
		m_pLeftTrail = (Trail*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_TRAIL, L"Layer_Trail", L"Prototype_GameObject_Boss_LeftTrail");
		m_pRightTrail = (Trail*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_TRAIL, L"Layer_Trail", L"Prototype_GameObject_Boss_RightTrail");

		m_pLeftTrail->SetColor(D3DXVECTOR4(1.4f, 1.4f, 1.4f, 0.f));
		m_pRightTrail->SetColor(D3DXVECTOR4(1.4f, 1.4f, 1.4f, 0.f));

		m_vTrailPosA = D3DXVECTOR3(-0.1f, -0.1f, -0.1f); // 길이가 짧은 트레일 
		m_vTrailPosB = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		m_pLeftTrail->SetInitPoint(m_vTrailPosA, m_vTrailPosB);
		m_pLeftTrail->SetTrailState(true);
		m_pLeftTrail->SetTextureNum(1);
		m_pHandLeftMatrix = m_pMesh->Get_BoneMatrix("_26");

		m_pRightTrail->SetInitPoint(m_vTrailPosA, m_vTrailPosB);
		m_pRightTrail->SetTrailState(true);
		m_pRightTrail->SetTextureNum(1);
		m_pHandRightMatrix = m_pMesh->Get_BoneMatrix("_44");
	}

	return NOERROR;
}

UPDATE Enemy::Update(const float timeDelta)
{
#ifdef _DEBUG
#ifdef TRAIL_NOT_OPERATION
	m_pRightTrail->SetTrailState(false);
	m_pLeftTrail->SetTrailState(false);
#endif
#endif

	m_pTransform->Cal_NotRotMatrix();
	return Engine::GameObject::Update(timeDelta);
}

UPDATE Enemy::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (m_pCenterPos == nullptr)
		return UPDATE::FAIL;

	if (m_pFrustum->FrustumColling(m_pCenterPos, mRadius))
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
			return UPDATE::FAIL;
	}

	return Engine::GameObject::LastUpdate(timeDelta);
}

void Enemy::Render()
{
	if (m_pMesh == nullptr || m_pTransform == nullptr || m_pShader == nullptr)
		return;

	if (mDistroyTime >= 1.f)
		return;

	if (m_pUtilityMgr->Cal_TargetDistance(*m_pEnemyPos, *m_pPlayerPos, true) > RENDER_RANGE)
		return;

	if (m_pEnemyAI->Get_EnemyAIMode() == AI_DIE)
	{
		mShaderIndex = 2;
		mDistroyTime += 0.6f * gTimeDelta;
		m_pEffect->SetFloat("g_fDestroyTime", mDistroyTime);
		m_pEnemyMgr->SetUp_DissolveTextrue(m_pEffect, "g_DissolveTex");
	}


	m_pEnemyAI->AIState_Update(gTimeDelta);
	m_pEnemyAI->AIState(gTimeDelta);
	m_pEnemyAI->AI_StateChange(gTimeDelta);
	m_pEnemyAI->AIUpdate(gTimeDelta);

	if (m_pLeftTrail != nullptr && m_pRightTrail != nullptr)
	{
		if (m_pHandLeftMatrix != nullptr)
		{
			m_pLeftTrail->SetTargetMatrix((*m_pHandLeftMatrix) * (*m_pTransform->Get_WorldMatrix()));
		}
		if (m_pHandRightMatrix != nullptr)
		{
			m_pRightTrail->SetTargetMatrix((*m_pHandRightMatrix) * (*m_pTransform->Get_WorldMatrix()));
		}
	}


	if (FAILED(SetUp_ConstantTable(m_pEffect)))
		return;

	m_pEffect->Begin(nullptr, 0);

	m_pEffect->BeginPass(mShaderIndex);

	m_pMesh->Render(m_pEffect);

	m_pEffect->EndPass();

	m_pEffect->End();

	m_pNavigation->Render();

	if (g_bColliderDraw)
	{
		m_pColliderBox->Render();
		m_pColliderSphere->Render();
	}
}

HRESULT Enemy::ReadyPrototype(const TCHAR* pMeshName, size_t naviIndex, size_t enemyType)
{
	mNaviIndex = naviIndex;
	mEnemyType = enemyType;
	m_pMeshName = pMeshName;

	return NOERROR;
}

HRESULT Enemy::ReadyComponent(void)
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

	// For.Com_Mesh
	{
		pComponent = m_pMesh = (Engine::DynamicMesh*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SOULVALLEY_ENEMY, m_pMeshName);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(m_pMeshName, pComponent)))
			return E_FAIL;
	}

	// For.Com_Shader
	{
		pComponent = m_pShader = (Engine::Shader*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SHADER, L"Component_Shader_Mesh");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_Shader_Mesh", pComponent)))
			return E_FAIL;
	}

	// For.Com_Collider Box
	{
		pComponent = m_pColliderBox = (Engine::Collider*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Collider_Box");
		if (pComponent == nullptr)
			return E_FAIL;
		if (FAILED(Engine::GameObject::AddComponent(L"Component_Collider_Box", pComponent)))
			return E_FAIL;

		switch (mEnemyType)
		{
		case BABEGAZI_AXE:
		case BABEGAZI_BOW:
		case BABEGAZI_WARRIOR:
			if (FAILED(m_pColliderBox->SetUp_Collider(D3DXVECTOR3(-2.f, 0.f, -2.f), D3DXVECTOR3(2.f, 5.f, 2.f), m_pTransform->Get_WorldMatrix_NotRot()
			, &m_vCenter, &mRadius)))
				return E_FAIL;
			break;
		case KNOLE_COMMANDER:
		case KNOLE_WARRIOR:
			if (FAILED(m_pColliderBox->SetUp_Collider(D3DXVECTOR3(-3.5f, 0.f, -3.5f), D3DXVECTOR3(3.5f, 6.5f, 3.5f), m_pTransform->Get_WorldMatrix_NotRot()
			, &m_vCenter, &mRadius)))
				return E_FAIL;
			break;
		case HUMAN_BOSS:
			if (FAILED(m_pColliderBox->SetUp_Collider(D3DXVECTOR3(-6.f, 0.f, -6.f), D3DXVECTOR3(11.f, 20.f, 11.f), m_pTransform->Get_WorldMatrix_NotRot()
				, &m_vCenter, &mRadius)))
				return E_FAIL;
			break;
		default:
			if (FAILED(m_pColliderBox->SetUp_Collider(D3DXVECTOR3(-2.f, 0.f, -2.f), D3DXVECTOR3(2.f, 5.f, 2.f), m_pTransform->Get_WorldMatrix_NotRot()
			, &m_vCenter, &mRadius)))
				return E_FAIL;
			break;
		}

		m_pTransform->Set_CenterPos(m_vCenter);
		m_pCenterPos = m_pTransform->Get_CenterPos();

	}

	// For.Com_Collider Sphere
	{
		pComponent = m_pColliderSphere = (Engine::Collider*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Collider_Sphere");
		if (pComponent == nullptr)
			return E_FAIL;
		if (FAILED(Engine::GameObject::AddComponent(L"Component_Collider_Sphere", pComponent)))
			return E_FAIL;


		switch (mEnemyType)
		{
		case BABEGAZI_AXE:
		case BABEGAZI_BOW:
		case BABEGAZI_WARRIOR:
			if (FAILED(m_pColliderSphere->SetUp_Collider(D3DXVECTOR3(-1.5f, 1.f, -1.5f), D3DXVECTOR3(1.5f, 4.f, 1.5f), m_pTransform->Get_WorldMatrix())))
				return E_FAIL;
			break;
		case KNOLE_COMMANDER:
		case KNOLE_WARRIOR:
			if (FAILED(m_pColliderSphere->SetUp_Collider(D3DXVECTOR3(-2.f, 1.f, -2.f), D3DXVECTOR3(2.f, 5.f, 2.f), m_pTransform->Get_WorldMatrix())))
				return E_FAIL;
			break;
		case HUMAN_BOSS:
			if (FAILED(m_pColliderSphere->SetUp_Collider(D3DXVECTOR3(-5.f, 1.f, -5.f), D3DXVECTOR3(7.f, 13.f, 7.f), m_pTransform->Get_WorldMatrix())))
				return E_FAIL;
			break;
		default:
			if (FAILED(m_pColliderSphere->SetUp_Collider(D3DXVECTOR3(-1.5f, 1.f, -1.5f), D3DXVECTOR3(1.5f, 4.f, 1.5f), m_pTransform->Get_WorldMatrix())))
				return E_FAIL;
			break;
		}

		// 뷰랑 투영도 다 가지고 있는 상태.
		m_pEffect = m_pShader->Get_EffectHandle();
		if (m_pEffect == nullptr)
			return E_FAIL;
	}

	// For.Com_Navigation
	{
		pComponent = m_pNavigation = (Engine::Navigation*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Navigation");
		if (nullptr == pComponent)
			return E_FAIL;
		if (FAILED(Engine::GameObject::AddComponent(L"Component_Navigation", pComponent)))
			return E_FAIL;
	}

	// For.Com_AI
	{
		pComponent = m_pEnemyAI = (EnemyAI*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SOULVALLEY_ENEMY, L"Component_AI_Enemy");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_AI_Enemy", pComponent)))
			return E_FAIL;

		m_pTargetTransform = (Engine::Transform*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Transform", L"Prototype_GameObject_Player");
		m_pTargetTransform->AddRef();

		m_pEnemyAI->SetInit(mEnemyType, m_pMesh, m_pTransform, m_pTargetTransform, m_pNavigation);

	
		Engine::Collider* pPlayerCollider = (Engine::Collider*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Collider_Box", L"Prototype_GameObject_Player");
		Engine::Collider* pWeaponCollider = (Engine::Collider*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Collider_Box", L"Prototype_GameObject_MisticWeapon");
		m_pEnemyAI->SetCollider(m_pColliderBox, m_pColliderSphere, pPlayerCollider, pWeaponCollider);
	}

	{
		// For.Com_Frustum
		pComponent = m_pFrustum = (Engine::Frustum*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Frustum");
		if (pComponent == nullptr)
			return E_FAIL;
		if (FAILED(GameObject::AddComponent(L"Component_Frustum", pComponent)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT Enemy::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	m_pTransform->SetUp_OnShader(pEffect, "g_matWorld");

	D3DXMATRIX matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);


	

	return NO_ERROR;
}

Enemy * Enemy::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshName, size_t naviIndex, size_t enemyType)
{
	Enemy* pInstance = new Enemy(pDevice);

	if (FAILED(pInstance->ReadyPrototype(pMeshName, naviIndex, enemyType)))
	{
		_MSG_BOX("Enemy Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Enemy::Clone()
{
	Enemy*	pInstance = new Enemy(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Enemy Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Enemy::Free(void)
{
	if (m_bClone)
	{
		Engine::Safe_Release(m_pTransform);
		Engine::Safe_Release(m_pRenderer);
		Engine::Safe_Release(m_pMesh);
		Engine::Safe_Release(m_pShader);
		Engine::Safe_Release(m_pColliderBox);
		Engine::Safe_Release(m_pColliderSphere);
		Engine::Safe_Release(m_pNavigation);
		Engine::Safe_Release(m_pEnemyAI);
		Engine::Safe_Release(m_pTargetTransform);
		Engine::Safe_Release(m_pFrustum);
		
	}

	GameObject::Free();

	return DWORD();
}
