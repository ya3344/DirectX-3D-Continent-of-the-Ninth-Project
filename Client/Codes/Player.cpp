#include "stdafx.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "PlayerAnim.h"


_USING(Client)

Player::Player(LPDIRECT3DDEVICE9 pDevice)
	: Engine::GameObject(pDevice)
{
}

Player::Player(const Player & rhs)
	: Engine::GameObject(rhs)
{
}


HRESULT Player::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	m_pNavigation->Setup_CurrentIndex(5); // 380 , 340
	m_pTransform->SetPos(m_pNavigation->GetCenterPos(5));
	m_pTransform->SetRotation(D3DXVECTOR3(0.f, 45.f, 0.f));

	mPlayerInfo.totalEXP = 150.f;
	mPlayerInfo.level = 1;

	m_pAnimState = m_pAnim->GetAnimState();

	

	return NOERROR;
}

UPDATE Player::Update(const float timeDelta)
{
	m_pAnim->KeyCheck(timeDelta);
	m_pAnim->AnimStateChange(timeDelta);
	m_pAnim->AnimUpdate(timeDelta);

	m_pTransform->Cal_NotRotMatrix();


/*	if (m_pKeyMgr->KeyDown(DIK_L))
	{
		SetEXP(100.f);
	}
	else */if (m_pKeyMgr->KeyDown(DIK_H))
	{
		SetHP(20.f); 
	}
	else if (m_pKeyMgr->KeyDown(DIK_M))
	{
		SetMP(20.f);
	}


	//if (m_pKeyMgr->KeyDown(DIK_H))
	//{
	//	SetMP(20.f);
	//}
	//else if (m_pKeyMgr->KeyDown(DIK_M))
	//{
	//	SetMP(-20.f);
	//}

	// 69 ~97
	return Engine::GameObject::Update(timeDelta);
}

UPDATE Player::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;


	return Engine::GameObject::LastUpdate(timeDelta);
}

void Player::Render()
{
	if (m_pMesh == nullptr  || m_pTransform == nullptr  || m_pShader == nullptr)
		return;

	// 뷰랑 투영도 다 가지고 있는 상태.
	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(1);

	m_pMesh->Render(pEffect);

	pEffect->EndPass();

	pEffect->End();

	m_pNavigation->Render();

	if (g_bColliderDraw)
	{
		m_pCollider->Render();
	}

}

HRESULT Player::ReadyPrototype()
{
	Engine::Component* pComponent = nullptr;

	// For.Component_Anim_Player
	pComponent = PlayerAnim::Create(m_pDevice);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_PLAYER, L"Component_Anim_Player", pComponent)))
		return E_FAIL;
	
	return NOERROR;
}

void Player::SetEXP(const float EXP)
{
	mPlayerInfo.EXP += (EXP / mPlayerInfo.totalEXP);

	if (mPlayerInfo.EXP >= 1.f)
	{
		++mPlayerInfo.level;
		mPlayerInfo.totalEXP *= 1.2f;
		mPlayerInfo.EXP = mPlayerInfo.EXP - 1.f;
		mPlayerInfo.HP = 1.f;
		mPlayerInfo.MP = 1.f;
		mPlayerInfo.bLevelUpBack = true;
		m_pAnim->Set_DamagedState(false);
		*m_pAnimState = "LevelUp";

		//cout << "Level: " << mPlayerInfo.level << endl;
	}
}

void Player::SetHP(const float HP)
{
	mPlayerInfo.HP += (HP / 100.f);

	if (mPlayerInfo.HP >= 1.f)
	{
		mPlayerInfo.HP = 1.f;
	}
	else if (mPlayerInfo.HP <= 0.f)
	{
		mPlayerInfo.HP = 0.f;
	}
}

void Player::SetMP(const float MP)
{
	mPlayerInfo.MP += (MP / 100.f);

	if (mPlayerInfo.MP >= 1.f)
	{
		mPlayerInfo.MP = 1.f;
	}
	else if (mPlayerInfo.MP <= 0.f)
	{
		mPlayerInfo.MP = 0.f;
	}
}

HRESULT Player::ReadyComponent(void)
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
		pComponent = m_pMesh = (Engine::DynamicMesh*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_PLAYER, L"Component_Mesh_Player");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_Mesh_Player", pComponent)))
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

	// For.Com_Collider
	{
		pComponent = m_pCollider = (Engine::Collider*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Collider_Box");

		if (pComponent == nullptr)
			return E_FAIL;
		if (FAILED(Engine::GameObject::AddComponent(L"Component_Collider_Box", pComponent)))
			return E_FAIL;

		if (FAILED(m_pCollider->SetUp_Collider(D3DXVECTOR3(-3.f, 0.f, -3.f), D3DXVECTOR3(3.f, 6.f, 3.f), m_pTransform->Get_WorldMatrix_NotRot(), &m_vCenterPos, &mRadius)))
			return E_FAIL;
	}

	// For.Com_Navi
	{
		pComponent = m_pNavigation = (Engine::Navigation*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Navigation");
		if (nullptr == pComponent)
			return E_FAIL;
		if (FAILED(Engine::GameObject::AddComponent(L"Component_Navigation", pComponent)))
			return E_FAIL;
	}

	// For.Com_Anim 맨마지막에 호출
	{
		pComponent = m_pAnim = (PlayerAnim*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_PLAYER, L"Component_Anim_Player");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::GameObject::AddComponent(L"Component_Anim_Player", pComponent)))
			return E_FAIL;

		m_pTrail = (Trail*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_TRAIL, L"Layer_Trail", L"Prototype_GameObject_PlayerTrail");

		if (m_pTrail == nullptr)
			return E_FAIL;

		m_pAnim->SetInit(m_pMesh, m_pTransform, m_pNavigation, this, m_pTrail);
	}



	return NOERROR;
}

HRESULT Player::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	m_pTransform->SetUp_OnShader(pEffect, "g_matWorld", false);

	D3DXMATRIX matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);


	return NO_ERROR;
}

Player * Player::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Player* pInstance = new Player(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Player Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Player::Clone()
{
	Player*	pInstance = new Player(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Player Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Player::Free(void)
{
	if (m_bClone)
	{
		Engine::Safe_Release(m_pTransform);
		Engine::Safe_Release(m_pRenderer);
		Engine::Safe_Release(m_pMesh);
		Engine::Safe_Release(m_pShader);
		Engine::Safe_Release(m_pCollider);
		Engine::Safe_Release(m_pAnim);
		Engine::Safe_Release(m_pNavigation);
	}

	GameObject::Free();

	return DWORD();
}
