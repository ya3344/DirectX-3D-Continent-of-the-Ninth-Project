#include "stdafx.h"
#include "UIMgr.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "MainUI.h"
#include "NumberUI.h"
#include "ComboUI.h"
#include "BossUI.h"
#include "LogoUI.h"

_USING(Client)
_IMPLEMENT_SINGLETON(UIMgr);

UIMgr::UIMgr()
{
}

HRESULT UIMgr::Ready_UIPrototype(LPDIRECT3DDEVICE9 pDevice)
{
	Engine::GameObject* pGameObject = nullptr;

	pGameObject = MainUI::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Main_UI", SCENE_UI, pGameObject)))
		return E_FAIL;

	pGameObject = NumberUI::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Numbering_UI", SCENE_UI, pGameObject)))
		return E_FAIL;

	pGameObject = ComboUI::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Combo_UI", SCENE_UI, pGameObject)))
		return E_FAIL;

	pGameObject = BossUI::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Boss_UI", SCENE_UI, pGameObject)))
		return E_FAIL;

	pGameObject = LogoUI::Create(pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(L"Prototype_Logo_UI", SCENE_LOGO, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

UPDATE UIMgr::Update(const float timeDelta)
{
	for (UI* pUI : mUISpace)
	{
		pUI->Update(timeDelta);
	}

	for (UI* pUI : mNumberingSpace)
	{
		pUI->Update(timeDelta);
	}

	//if (Engine::KeyMgr::GetInstance()->KeyDown(DIK_P))
	//{
	//	AddComboCout();

	//}

	NumberIngDelete();
	//cout << "NumberIngSize: " << mNumberingSpace.size() << endl;
	return UPDATE::NORMAL;
}

UPDATE UIMgr::LastUpdate(const float timeDelta)
{
	for (UI* pUI : mUISpace)
	{
		pUI->LastUpdate(timeDelta);
	}

	for (UI* pUI : mNumberingSpace)
	{
		pUI->LastUpdate(timeDelta);
	}

	return UPDATE::NORMAL;
}

void UIMgr::NumberIngDelete()
{
	auto iterBegin = mNumberingSpace.begin();
	auto iterEnd = mNumberingSpace.end();
	NumberUI* pUI = nullptr;

	for (; iterBegin != iterEnd;)
	{
		pUI = (NumberUI*)*iterBegin;

		if (pUI->GetNumberingSate() == false)
		{
			Engine::Safe_Release(pUI);
			iterBegin = mNumberingSpace.erase(iterBegin);
		}
		else
		{
			++iterBegin;
		}
	}
}

void UIMgr::LogoDelte()
{
	auto iterBegin = mUISpace.begin();
	auto iterEnd = mUISpace.end();
	LogoUI* pUI = nullptr;

	for (; iterBegin != iterEnd;)
	{
		pUI = (LogoUI*)*iterBegin;

		if (pUI->GetNotObject() == true)
		{
			Engine::Safe_Release(pUI);
			iterBegin = mUISpace.erase(iterBegin);
		}
		else
		{
			++iterBegin;
		}
	}
}

HRESULT UIMgr::MainUI_Create()
{
	LogoDelte();

	{
		if (FAILED(Create_MainUI(L"Prototype_Main_UI", L"Component_UI_Main_Base", UI_INFO(0.5f, 0.918f, 0.75f, 0.16f, UI_MAIN_BASE))))
			return E_FAIL;

		if (FAILED(Create_MainUI(L"Prototype_Main_UI", L"Component_UI_Main_Exp_Gage", UI_INFO(0.527f, 0.9933f, 0.655f, 0.005f, UI_EXP_GAUAGE))))
			return E_FAIL;

		if (FAILED(Create_MainUI(L"Prototype_Main_UI", L"Component_UI_PlayerState_LevelUp_Back", UI_INFO(0.5f, 0.2f, 0.25f, 0.08f, UI_LEVEL_UP_BACK))))
			return E_FAIL;

		if (FAILED(Create_MainUI(L"Prototype_Main_UI", L"Component_UI_PlayerState_LevelUp", UI_INFO(0.5f, 0.2f, 0.25f, 0.08f, UI_LEVEL_UP))))
			return E_FAIL;



		if (FAILED(Create_MainUI(L"Prototype_Main_UI", L"Component_UI_PlayerState_Shaman_State_Base", UI_INFO(0.135f, 0.087f, 0.255f, 0.14f, UI_NORMAL))))
			return E_FAIL;

		//if (FAILED(Create_MainUI(L"Prototype_Main_UI", L"Component_UI_PlayerState_HitEffect", UI_INFO(0.145f, 0.12f, 0.26f, 0.18f, UI_HIT_EFFECT))))
		//	return E_FAIL;

		if (FAILED(Create_MainUI(L"Prototype_Main_UI", L"Component_UI_PlayerState_HP_Gage", UI_INFO(0.145f, 0.0833f, 0.12f, 0.018f, UI_HP_GAUAGE))))
			return E_FAIL;

		if (FAILED(Create_MainUI(L"Prototype_Main_UI", L"Component_UI_PlayerState_MP_Gage", UI_INFO(0.132f, 0.104f, 0.105f, 0.015f, UI_MP_GAUAGE))))
			return E_FAIL;

		if (FAILED(Create_MainUI(L"Prototype_Main_UI", L"Component_UI_PlayerState_Class_Mark", UI_INFO(0.086f, 0.087f, 0.03f, 0.05f, UI_NORMAL))))
			return E_FAIL;

		if (FAILED(Create_MainUI(L"Prototype_Main_UI", L"Component_UI_PlayerState_Class_W_0", UI_INFO(0.086f, 0.087f, 0.03f, 0.05f, UI_NORMAL))))
			return E_FAIL;



		

		/*if (FAILED(Create_MainUI(L"Prototype_Main_UI", L"Component_UI_BossGage", UI_INFO(0.5f, 0.12f, 0.37f, 0.06f, UI_NORMAL))))
			return E_FAIL;*/

	}


	return NOERROR;
}

HRESULT UIMgr::ComboUI_Create()
{
	if (FAILED(Create_ComboUI(L"Prototype_Combo_UI", L"Component_UI_Numbering_Combo", UI_INFO(0.78f, 0.3f, 0.05f, 0.05f, UI_COMBO_1), &mComboCount)))
		return E_FAIL;

	if (FAILED(Create_ComboUI(L"Prototype_Combo_UI", L"Component_UI_Numbering_Combo", UI_INFO(0.78f, 0.3f, 0.05f, 0.05f, UI_COMBO_10), &mComboCount)))
		return E_FAIL;

	if (FAILED(Create_ComboUI(L"Prototype_Combo_UI", L"Component_UI_Numbering_Combo", UI_INFO(0.78f, 0.3f, 0.05f, 0.05f, UI_COMBO_100), &mComboCount)))
		return E_FAIL;

	if (FAILED(Create_ComboUI(L"Prototype_Combo_UI", L"Component_UI_Combo_Char", UI_INFO(0.78f, 0.3f, 0.05f, 0.05f, UI_COMBO_CHAR), &mComboCount)))
		return E_FAIL;

	//if (FAILED(Create_ComboUI(L"Prototype_Combo_UI", L"Component_UI_Combo_Effect", UI_INFO(0.85f, 0.3f, 0.05f, 0.05f, UI_COMBO_EFFECT))))
	//	return E_FAIL;

	return NOERROR;
}

HRESULT UIMgr::BossUI_Create()
{
	if (FAILED(Create_BossUI(L"Prototype_Boss_UI", L"Component_UI_BossBar", UI_INFO(0.5f, 0.12f, 0.37f, 0.06f, UI_NORMAL))))
		return E_FAIL;

	if (FAILED(Create_BossUI(L"Prototype_Boss_UI", L"Component_UI_BossGage", UI_INFO(0.497f, 0.116f, 0.35f, 0.038f, UI_BOSS_RED_GAUAGE))))
		return E_FAIL;

	if (FAILED(Create_BossUI(L"Prototype_Boss_UI", L"Component_UI_BossGage", UI_INFO(0.497f, 0.116f, 0.35f, 0.038f, UI_BOSS_YELLOW_GAUAGE))))
		return E_FAIL;

	if (FAILED(Create_BossUI(L"Prototype_Boss_UI", L"Component_UI_BossGage", UI_INFO(0.497f, 0.116f, 0.35f, 0.038f, UI_BOSS_GREEN_GAUAGE))))
		return E_FAIL;

	if (FAILED(Create_BossUI(L"Prototype_Boss_UI", L"Component_UI_BossTex", UI_INFO(0.675f, 0.115f, 0.06f, 0.065f, UI_NORMAL))))
		return E_FAIL;

	if (FAILED(Create_BossUI(L"Prototype_Boss_UI", L"Component_UI_BossBar2", UI_INFO(0.5f, 0.115f, 0.37f, 0.06f, UI_NORMAL))))
		return E_FAIL;

	if (FAILED(Create_BossUI(L"Prototype_Boss_UI", L"Component_UI_BossName", UI_INFO(0.5f, 0.05f, 0.2f, 0.06f, UI_NORMAL))))
		return E_FAIL;

	return NOERROR;
}

HRESULT UIMgr::NumberingUI_Create(const D3DXMATRIX& pEnemyWorldMatrix, const float damageValue)
{
	{
		if (damageValue <= 52000)
		{

			if (FAILED(Create_NumberingUI(pEnemyWorldMatrix, damageValue, UI_INFO(0.5f, 0.5f, 0.04f, 0.04f, UI_NUMBERING_DAMAGE_1))))
				return E_FAIL;

			if (FAILED(Create_NumberingUI(pEnemyWorldMatrix, damageValue, UI_INFO(0.5f, 0.5f, 0.04f, 0.04f, UI_NUMBERING_DAMAGE_10))))
				return E_FAIL;

			if (FAILED(Create_NumberingUI(pEnemyWorldMatrix, damageValue, UI_INFO(0.5f, 0.5f, 0.04f, 0.04f, UI_NUMBERING_DAMAGE_100))))
				return E_FAIL;

			if (FAILED(Create_NumberingUI(pEnemyWorldMatrix, damageValue, UI_INFO(0.5f, 0.5f, 0.04f, 0.04f, UI_NUMBERING_DAMAGE_1000))))
				return E_FAIL;

			if (FAILED(Create_NumberingUI(pEnemyWorldMatrix, damageValue, UI_INFO(0.5f, 0.5f, 0.04f, 0.04f, UI_NUMBERING_DAMAGE_10000))))
				return E_FAIL;

			
		}
		else
		{
			if (FAILED(Create_NumberingUI(pEnemyWorldMatrix, damageValue, UI_INFO(0.5f, 0.5f, 0.04f, 0.04f, UI_NUMBERING_DAMAGE_CRITICAL))))
				return E_FAIL;
		}


	}

	return NOERROR;
}

HRESULT UIMgr::LogoUI_Create()
{
	if (FAILED(Create_LogoUI(L"Prototype_Logo_UI", L"Component_Logo_Ice_Loading_Base", UI_INFO(0.5f, 0.5f, 1.f, 1.f, UI_NORMAL), true)))
		return E_FAIL;

	if (FAILED(Create_LogoUI(L"Prototype_Logo_UI", L"Component_Logo_Loading_Bar", UI_INFO(0.5f, 0.92f, 0.5f, 0.04f, UI_LOADING_BAR), true)))
		return E_FAIL;

	if (FAILED(Create_LogoUI(L"Prototype_Logo_UI", L"Component_Logo_Loading_Gage", UI_INFO(0.5f, 0.919f, 0.476f, 0.037f, UI_LOADING_GAGE), true)))
		return E_FAIL;

	if (FAILED(Create_LogoUI(L"Prototype_Logo_UI", L"Component_Logo_Loading_Char", UI_INFO(0.5f, 0.88f, 0.1f, 0.037f, UI_LOADING_CHAR), true)))
		return E_FAIL;

	//if (FAILED(Create_LogoUI(L"Prototype_Logo_UI", L"Component_Logo_Loading_Effect", UI_INFO(0.5f, 0.92f, 0.01f, 0.05f, UI_LOADING_EFFECT), true)))
	//	return E_FAIL;

	return NOERROR;
}

HRESULT UIMgr::Create_MainUI(const TCHAR* pUITag, const TCHAR * pTextureTag, const UI_INFO & UI_Info)
{
	UI* pUIPrototype = (UI*)Engine::ObjectMgr::GetInstance()->FindPrototype(SCENE_UI, pUITag);
	if (pUIPrototype == nullptr)
	{
		_MSG_BOX("UIPrototype Find Failed!");
		return E_FAIL;
	}

	mUISpace.emplace_back((UI*)pUIPrototype->MainUI_Clone(pTextureTag, UI_Info));

	return NOERROR;
}

HRESULT UIMgr::Create_BossUI(const TCHAR * pUITag, const TCHAR * pTextureTag, const UI_INFO & UI_Info)
{
	UI* pUIPrototype = (UI*)Engine::ObjectMgr::GetInstance()->FindPrototype(SCENE_UI, pUITag);
	if (pUIPrototype == nullptr)
	{
		_MSG_BOX("UIPrototype Find Failed!");
		return E_FAIL;
	}

	mUISpace.emplace_back((UI*)pUIPrototype->BossUI_Clone(pTextureTag, UI_Info));

	return NOERROR;
}

HRESULT UIMgr::Create_ComboUI(const TCHAR * pUITag, const TCHAR * pTextureTag, const UI_INFO & UI_Info, size_t* pComboCount)
{
	UI* pUIPrototype = (UI*)Engine::ObjectMgr::GetInstance()->FindPrototype(SCENE_UI, pUITag);
	if (pUIPrototype == nullptr)
	{
		_MSG_BOX("UIPrototype Find Failed!");
		return E_FAIL;
	}

	mUISpace.emplace_back((UI*)pUIPrototype->ComboUI_Clone(pTextureTag, UI_Info, pComboCount));

	return NOERROR;
}

HRESULT UIMgr::Create_NumberingUI(const D3DXMATRIX& pEnemyWorldMatrix, const float damageValue, const UI_INFO& UI_Info)
{
	UI* pUIPrototype = (UI*)Engine::ObjectMgr::GetInstance()->FindPrototype(SCENE_UI, L"Prototype_Numbering_UI");
	if (pUIPrototype == nullptr)
	{
		_MSG_BOX("UIPrototype Find Failed!");
		return E_FAIL;
	}

	mNumberingSpace.emplace_back((UI*)pUIPrototype->NumberingUI_Clone(L"Component_UI_Numbering_Damage", pEnemyWorldMatrix, damageValue, UI_Info));

	return NOERROR;
}

HRESULT UIMgr::Create_LogoUI(const TCHAR * pUITag, const TCHAR * pTextureTag, const UI_INFO & UI_Info, const bool bNotObject)
{
	UI* pUIPrototype = (UI*)Engine::ObjectMgr::GetInstance()->FindPrototype(SCENE_LOGO, pUITag);
	if (pUIPrototype == nullptr)
	{
		_MSG_BOX("UIPrototype Find Failed!");
		return E_FAIL;
	}

	mUISpace.emplace_back((UI*)pUIPrototype->LogoUI_Clone(pTextureTag, UI_Info, bNotObject));

	return NOERROR;
}

DWORD UIMgr::Free()
{
	for (UI* pUI : mUISpace)
	{
		Engine::Safe_Release(pUI);
	}
	mUISpace.clear();

	for (UI* pUI : mNumberingSpace)
	{
		Engine::Safe_Release(pUI);
	}
	mNumberingSpace.clear();

	return 0;
}
