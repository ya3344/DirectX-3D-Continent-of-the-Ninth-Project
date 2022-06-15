#include "stdafx.h"
#include "MainUI.h"
#include "RectTex.h"
#include "Texture.h"

_USING(Client)

MainUI::MainUI(LPDIRECT3DDEVICE9 pDevice)
	:UI(pDevice)
{

}

MainUI::MainUI(const MainUI & rhs)
	:UI(rhs)
{

}

void MainUI::MainUI_Init()
{
	mUIInfo.X = m_matWorld._41;

	switch (mUIInfo.type)
	{
	case UI_LEVEL_UP:
		mAlphaValue = 1.f;
		mAlphaTestValue = 255.f;
		mShaderIndex = 0;
		break;
	case UI_LEVEL_UP_BACK: 
		mAlphaValue = 1.f;
		mAlphaTestValue = 255.f;
		mShaderIndex = 0;
		break;
	case UI_HIT_EFFECT:
		mAlphaValue = 1.f;
		mAlphaTestValue = 155.f;
		mShaderIndex = 0;
		break;
	default:
		mAlphaValue = 1.f;
		mAlphaTestValue = 85.f;
		m_pPlayerInfo->HP = 1.f;
		m_pPlayerInfo->MP = 1.f;
		mShaderIndex = 0;
		break;
	}
	
}


UPDATE MainUI::Update(const float timeDelta)
{
	switch (mUIInfo.type)
	{
	case UI_EXP_GAUAGE:
		GauageUpdate(m_pPlayerInfo->EXP);
		break;
	case UI_HP_GAUAGE:
		GauageUpdate(m_pPlayerInfo->HP);
		break;
	case UI_MP_GAUAGE:
		GauageUpdate(m_pPlayerInfo->MP);
		break;
	case UI_HIT_EFFECT:
		if (mPrePlayerHP != m_pPlayerInfo->HP)
		{
			if (mAlphaValue < 0.8f)
			{
				mAlphaValue += timeDelta * 1.f;
			}
			else
			{
				mAlphaValue = 0.f;
				mPrePlayerHP = m_pPlayerInfo->HP;
				/*if (mAlphaValue <= 0.f)
				{
					mAlphaValue = 0.f;
					mPrePlayerHP = m_pPlayerInfo->HP;
				}*/
			}
		}
		break;
	case UI_LEVEL_UP:
		if (m_pPlayerInfo->bLevelUP == true)
		{
			mAlphaTestValue = 0.f;
			mAlphaValue = 1.f;
			m_pPlayerInfo->bLevelUP = false;
		}
		else
		{
			if (mAlphaTestValue < 255.f)
			{
				mAlphaTestValue += 50.f * timeDelta;
				mAlphaValue -= 1.3f * timeDelta;
				if (mAlphaTestValue >= 255.f)
				{
					mAlphaTestValue = 255.f;
				}
			}
		}
		break;
	case UI_LEVEL_UP_BACK:
		if (m_pPlayerInfo->bLevelUpBack == true)
		{
			mAlphaTestValue = 0.f;
			m_pPlayerInfo->bLevelUpBack = false;
		}
		else
		{
			if (mAlphaTestValue < 255.f)
			{
				mAlphaTestValue += 300.f * timeDelta;
				if (mAlphaTestValue >= 255.f)
				{
					mAlphaTestValue = 255.f;
					m_pPlayerInfo->bLevelUP = true;
				}
			}
		}
		break;
	default:
		break;
	}

	return UPDATE::NORMAL;
}

void MainUI::Render()
{
	if (m_pTransform == nullptr || m_pShader == nullptr)
		return;

	if (FAILED(SetUp_ConstantTable(m_pEffect)))
		return;

	if (mUIInfo.type == UI_MAIN_BASE)
	{
		if (g_bMainBase)
			return;
	}

	m_pEffect->Begin(nullptr, 0);
	m_pEffect->BeginPass(mShaderIndex);

	m_pRectTex->Render();

	m_pEffect->EndPass();
	m_pEffect->End();
}

HRESULT MainUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->SetMatrix("g_matWorld", &m_matWorld);
	pEffect->SetMatrix("g_matView", &m_matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);
	pEffect->SetFloat("g_fAlphaValue", mAlphaValue);
	pEffect->SetFloat("g_fAlphaTestValue", mAlphaTestValue);
	pEffect->SetVector("g_vColor", &m_vColor);

	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", mTextureNum);

	return NOERROR;
}

void MainUI::GauageUpdate(float gauageState)
{
	m_matWorld._11 = mUIInfo.width * (gauageState);
	m_matWorld._41 = mUIInfo.X - (mUIInfo.width * ((1 - gauageState) * 0.5f));
}



MainUI * MainUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	MainUI* pInstance = new MainUI(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("MainUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * MainUI::MainUI_Clone(const TCHAR * pTextureTag, const UI_INFO & UI_Info)
{
	MainUI* pInstance = new MainUI(*this);

	if (FAILED(pInstance->InitUI(pTextureTag, UI_Info)))
	{
		_MSG_BOX("MainUI Clone Failed");
		Engine::Safe_Release(pInstance);
	}
	pInstance->MainUI_Init();

	return pInstance;
}

DWORD MainUI::Free(void)
{
	UI::Free();
	return DWORD();
}

