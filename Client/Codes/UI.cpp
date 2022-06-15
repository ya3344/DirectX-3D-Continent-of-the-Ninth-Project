#include "stdafx.h"
#include "UI.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "EnemyAI.h"

_USING(Client)



UI::UI(LPDIRECT3DDEVICE9 pDevice)
	:GameObject(pDevice)
{

}

UI::UI(const UI & rhs)
	: GameObject(rhs)
{
}


HRESULT UI::ReadyPrototype()
{
	return NOERROR;
}

HRESULT UI::Init()
{
	return NOERROR;
}

HRESULT UI::InitUI(const TCHAR* pTextureTag, const UI_INFO & UI_Info, const bool bGet_NotObject)
{
	m_pTextureTag = pTextureTag;

	if (FAILED(ReadyComponent()))
		return E_FAIL;

	mUIInfo = UI_Info;

	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._11 = UI_Info.width;
	m_matWorld._22 = UI_Info.height;
	m_matWorld._41 = UI_Info.X - (WIN_SIZEX * 0.5f);
	m_matWorld._42 = -UI_Info.Y + (WIN_SIZEY * 0.5f);


	// 단순한 직교투영을 위한 행렬을 8생성해주낟.

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, WIN_SIZEX, WIN_SIZEY, 0.0f, 1.f);

	if (bGet_NotObject == false)
	{
		m_pPlayer = (Player *)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_PLAYER, L"Layer_Player", L"Prototype_GameObject_Player");
		if (m_pPlayer != nullptr)
		{
			m_pPlayerInfo = m_pPlayer->Get_PlayerInfo();

			if (m_pPlayerInfo == nullptr)
				return E_FAIL;
		}

		EnemyAI* pEnemyAI = (EnemyAI*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_SOULVALLEY_ENEMY, L"Layer_Enemy", L"Component_AI_Enemy", L"Prototype_GameObject_HumanBoss");
		if (pEnemyAI != nullptr)
		{
			m_pBossHP = pEnemyAI->GetEnemyHP();
			mBossOriginalHP = *m_pBossHP;
			m_pBossCount = pEnemyAI->GetEnemyHP_Count();
			if (m_pBossHP == nullptr)
				return E_FAIL;
		}


		//Component_Player_Navigation
		m_pPlayerNavigation = (Engine::Navigation*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Navigation", L"Prototype_GameObject_Player");
		m_pPlayerIndex = m_pPlayerNavigation->Get_CurrentIndex();

		m_pSoundMgr = SoundMgr::GetInstance();
	}
	



	mAlphaValue = 1.f;
	mAlphaTestValue = 85.f;

	return NOERROR;
}

UPDATE UI::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_UI, this)))
		return UPDATE::FAIL;

	return GameObject::LastUpdate(timeDelta);
}

HRESULT UI::ReadyComponent()
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
	pComponent = m_pShader = (Engine::Shader*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SHADER, L"Component_Shader_UI");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Shader_UI", pComponent)))
		return E_FAIL;

	m_pEffect = m_pShader->Get_EffectHandle();
	if (m_pEffect == nullptr)
		return E_FAIL;

	// For.Com_Texture
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_UI, m_pTextureTag);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(m_pTextureTag, pComponent)))
		return E_FAIL;

	// For.Com_RectTex
	pComponent = m_pRectTex = (Engine::RectTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_RectTex");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_RectTex", pComponent)))
		return E_FAIL;

	return NOERROR;
}

Engine::GameObject * UI::Clone()
{
	return nullptr;
}


DWORD UI::Free(void)
{
	if (m_bClone)
	{
		Engine::Safe_Release(m_pTransform);
		Engine::Safe_Release(m_pRenderer);
		Engine::Safe_Release(m_pShader);
		Engine::Safe_Release(m_pTexture);
		Engine::Safe_Release(m_pRectTex);
	}

	GameObject::Free();

	return DWORD();
}

Engine::GameObject * UI::MainUI_Clone(const TCHAR * pTextureTag, const UI_INFO & UI_Info)
{
	return nullptr;
}

Engine::GameObject * UI::LogoUI_Clone(const TCHAR * pTextureTag, const UI_INFO & UI_Info, const bool bNot_Object)
{
	return nullptr;
}

Engine::GameObject * UI::BossUI_Clone(const TCHAR * pTextureTag, const UI_INFO & UI_Info)
{
	return nullptr;
}

Engine::GameObject * UI::ComboUI_Clone(const TCHAR * pTextureTag, const UI_INFO & UI_Info, size_t* pComboCount)
{
	return nullptr;
}

Engine::GameObject * UI::NumberingUI_Clone(const TCHAR * pTextureTag, const D3DXMATRIX& pEnemyWorldMatrix, const float damageValue, const UI_INFO& UI_Info)
{
	return nullptr;
}



