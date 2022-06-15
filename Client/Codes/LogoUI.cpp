#include "stdafx.h"
#include "LogoUI.h"
#include "RectTex.h"
#include "Texture.h"
#include "GraphicDevice.h"
#include "Navigation.h"

_USING(Client)

LogoUI::LogoUI(LPDIRECT3DDEVICE9 pDevice)
	:UI(pDevice)
{

}

LogoUI::LogoUI(const LogoUI & rhs)
	:UI(rhs)
{

}

void LogoUI::LogoUI_Init()
{
	mUIInfo.X = m_matWorld._41;

	switch (mUIInfo.type)
	{
	case UI_LOADING_BAR:
		mAlphaValue = 0.6f;
		mAlphaTestValue = 35.f;
		mShaderIndex = 0;
		break;
	case UI_LOADING_GAGE:
		mAlphaValue = 1.f;
		mAlphaTestValue = 35.f;
		mShaderIndex = 2;
		//m_vColor = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 0.f);
		break;
	default:
		mAlphaValue = 1.f;
		mAlphaTestValue = 105.f;
		mShaderIndex = 0;
		break;
	}

	mLoadingPercent = 1.f;
	mShaderIndex = 0;

	m_bNotObject = true;
}

UPDATE LogoUI::Update(const float timeDelta)
{
	switch (mUIInfo.type)
	{
	case UI_LOADING_GAGE:
		GauageUpdate(g_fLoading);
		break;
	case UI_LOADING_CHAR:
		mAlphaValue += mCharAlpha * timeDelta;

		if (mAlphaValue >= 1.f)
		{
			mCharAlpha = -0.2f;
			mAlphaValue = 1.f;
		}
		else if (mAlphaValue <= 0.5f)
		{
			mCharAlpha = 0.2f;
			mAlphaValue = 0.5f;
		}

		break;
	//case UI_LOADING_EFFECT:
	//	m_matWorld._41 = mUIInfo.X - (WIN_SIZEX * 0.476f * ((1 - /*gauageState*/1) * 0.5f));
	//	break;
	default:
		break;
	}
	return UPDATE::NORMAL;
}

void LogoUI::Render()
{
	if (m_pTransform == nullptr || m_pShader == nullptr)
		return;

	if (FAILED(SetUp_ConstantTable(m_pEffect)))
		return;

	m_pEffect->Begin(nullptr, 0);
	m_pEffect->BeginPass(mShaderIndex);

	m_pRectTex->Render();

	m_pEffect->EndPass();
	m_pEffect->End();
}

HRESULT LogoUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->SetMatrix("g_matWorld", &m_matWorld);
	pEffect->SetMatrix("g_matView", &m_matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);
	pEffect->SetFloat("g_fAlphaValue", mAlphaValue);
	pEffect->SetFloat("g_fAlphaTestValue", mAlphaTestValue);
	pEffect->SetFloat("g_fLoadingPercent", g_fLoading);
	pEffect->SetVector("g_vColor", &m_vColor);
	
	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", mTextureNum);

	return NOERROR;
}

LogoUI * LogoUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	LogoUI* pInstance = new LogoUI(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("LogoUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * LogoUI::LogoUI_Clone(const TCHAR * pTextureTag, const UI_INFO & UI_Info, const bool bNotObject)
{
	LogoUI* pInstance = new LogoUI(*this);

	if (FAILED(pInstance->InitUI(pTextureTag, UI_Info, bNotObject)))
	{
		_MSG_BOX("LogoUI Clone Failed");
		Engine::Safe_Release(pInstance);
	}
	m_bNotObject = bNotObject;
	pInstance->LogoUI_Init();

	return pInstance;
}

DWORD LogoUI::Free(void)
{
	UI::Free();
	return DWORD();
}

void LogoUI::GauageUpdate(float gauageState)
{
	m_matWorld._11 = mUIInfo.width * (gauageState);
	m_matWorld._41 = mUIInfo.X - (mUIInfo.width * ((1 - gauageState) * 0.5f));
}
