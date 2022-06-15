#include "stdafx.h"
#include "ComboUI.h"
#include "RectTex.h"
#include "Texture.h"

_USING(Client)

ComboUI::ComboUI(LPDIRECT3DDEVICE9 pDevice)
	:UI(pDevice)
{
}

ComboUI::ComboUI(const ComboUI & rhs)
	:UI(rhs)
{
}

void ComboUI::ComboUI_Init(const TCHAR * pTextureTag, const UI_INFO & UI_Info, size_t* pComboCount)
{
	m_bCombo = false;
	mWidthValue = 0.73f;
	m_pComboCount = pComboCount;


	
	//switch (mUIInfo.type)
	//{
	//case UI_COMBO_1:
	//	mOffset = mUIInfo.width * (mWidthValue*2.f);
	//	mTextureNum = 0;
	//	break;
	//case UI_COMBO_10:
	//	mOffset = mUIInfo.width * mWidthValue;
	//	mTextureNum = 1;
	//	break;
	//case UI_COMBO_100:
	//	mOffset = 0;
	//	mTextureNum = 2;
	//	break;
	//case UI_COMBO_CHAR:
	//	mUIInfo.width = (WIN_SIZEX * 0.13f);
	//	mUIInfo.height = (WIN_SIZEY * 0.05f);
	//	mOffset = (mUIInfo.width * (mWidthValue*1.9f));
	//	break;
	//case UI_COMBO_EFFECT:
	//	break;
	//default:
	//	break;
	//}

	//m_matWorld._11 = mUIInfo.width;
	//m_matWorld._22 = mUIInfo.height;
	//m_matWorld._41 = mUIInfo.X - (WIN_SIZEX * 0.5f) + mOffset;
	//m_matWorld._42 = -mUIInfo.Y + (WIN_SIZEY * 0.5f);
}

UPDATE ComboUI::Update(const float timeDelta)
{
	mComboCount = *m_pComboCount;
	mComboDuration += 10 * timeDelta;

	if (mComboCount != mPreComboCount && mComboCount > 0)
	{
		m_bCombo = true;
		mPreComboCount = mComboCount;
		mComboTime = 0.f;
		mComboDuration = 0.f;
	}
	if (mComboDuration >= 50.f)
	{
		*m_pComboCount = 0;
		//cout << "Combo: " << mComboDuration << endl;
	}

	if (m_bCombo == false)
	{
		mComboTime = 0.f;
		return UPDATE::NORMAL;
	}

	mComboTime += 10 * timeDelta;
	
	
	if (mComboCount < 10)
	{
		m_vColor = D3DXVECTOR4(1.3f, 1.3f, 1.3f, 0.f);
	}
	else if (mComboCount < 30)
	{
		m_vColor = D3DXVECTOR4(1.5f, 1.1f, 0.7f, 0.f);
	}
	else if (mComboCount < 50)
	{
		m_vColor = D3DXVECTOR4(1.3f, 0.7f, 1.3f, 0.f);
	}
	else if (mComboCount < 70)
	{
		m_vColor = D3DXVECTOR4(1.5f, 0.2f, 1.5f, 0.f);
	}
	else
	{
		m_vColor = D3DXVECTOR4(1.3f, 0.4f, 0.4f, 0.f);
	}


	

	if (mComboTime <= 0.7f)
	{
		switch (mUIInfo.type)
		{
		case UI_COMBO_1:
		case UI_COMBO_10:
		case UI_COMBO_100:
			mUIInfo.width = (WIN_SIZEX * 0.05f) * (1.1f + (mComboCount * 0.01f));
			mUIInfo.height = (WIN_SIZEY * 0.05f) * (1.1f + (mComboCount * 0.01f));
			break;
		case UI_COMBO_CHAR:
			mUIInfo.width = (WIN_SIZEX * 0.13f) *(1.1f + (mComboCount * 0.01f));
			mUIInfo.height = (WIN_SIZEY * 0.05f) * (1.1f + (mComboCount * 0.01f));
			break;
		case UI_COMBO_EFFECT:
			break;
		default:
			break;
		}
		mUIInfo.X = WIN_SIZEX * (0.68f - (mComboCount * 0.0015f));
	}
	else if(mComboTime > 0.7f && mComboTime <= 5.f)
	{
		switch (mUIInfo.type)
		{
		case UI_COMBO_1:
		case UI_COMBO_10:
		case UI_COMBO_100:
			mUIInfo.width = (WIN_SIZEX * 0.05f);
			mUIInfo.height = (WIN_SIZEY * 0.05f);
			break;
		case UI_COMBO_CHAR:
			mUIInfo.width = (WIN_SIZEX * 0.13f);
			mUIInfo.height = (WIN_SIZEY * 0.05f);
			break;
		case UI_COMBO_EFFECT:
			break;
		default:
			break;
		}

		mUIInfo.X = WIN_SIZEX * 0.73f;
	}
	else
	{
		m_bCombo = false;
		mComboTime = 0.f;
	}

	PositionSetting();

	return UPDATE::NORMAL;
}

void ComboUI::Render()
{
	if (m_pTransform == nullptr || m_pShader == nullptr)
		return;

	if (m_bCombo == false)
		return;

	if (FAILED(SetUp_ConstantTable(m_pEffect)))
		return;

	m_pEffect->Begin(nullptr, 0);
	m_pEffect->BeginPass(0);

	m_pRectTex->Render();

	m_pEffect->EndPass();
	m_pEffect->End();
}

HRESULT ComboUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void ComboUI::PositionSetting()
{
	size_t tempComboCount = mComboCount;

	while (tempComboCount != 0)
	{
		mComboInfo.positionNum_100 = tempComboCount / 100;
		tempComboCount %= 100;

		mComboInfo.positionNum_10 = tempComboCount / 10;
		tempComboCount %= 10;

		mComboInfo.positionNum_1 = tempComboCount;
		tempComboCount = 0;
	}

	switch (mUIInfo.type)
	{
	case UI_COMBO_1:
		mOffset = mUIInfo.width * (mWidthValue*2.f);
		mTextureNum = mComboInfo.positionNum_1;
		break;
	case UI_COMBO_10:
		mOffset = mUIInfo.width * mWidthValue;
		mTextureNum = mComboInfo.positionNum_10;
		if (mComboCount < 10)
		{
			m_bCombo = false;
		}
		break;
	case UI_COMBO_100:
		mOffset = 0.f;
		mTextureNum = mComboInfo.positionNum_100;
		if (mComboCount < 100)
		{
			m_bCombo = false;
		}
		break;
	case UI_COMBO_CHAR:
		mOffset = (mUIInfo.width * (mWidthValue*1.7f));
		break;
	case UI_COMBO_EFFECT:
		break;
	default:
		break;
	}

	m_matWorld._11 = mUIInfo.width;
	m_matWorld._22 = mUIInfo.height;
	m_matWorld._41 = mUIInfo.X - (WIN_SIZEX * 0.5f) + mOffset;
	m_matWorld._42 = -mUIInfo.Y + (WIN_SIZEY * 0.5f);
}

ComboUI * ComboUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	ComboUI* pInstance = new ComboUI(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("ComboUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * ComboUI::ComboUI_Clone(const TCHAR * pTextureTag, const UI_INFO & UI_Info, size_t* pComboCount)
{
	ComboUI* pInstance = new ComboUI(*this);

	if (FAILED(pInstance->InitUI(pTextureTag, UI_Info)))
	{
		_MSG_BOX("ComboUI Clone Failed");
		Engine::Safe_Release(pInstance);
	}

	pInstance->ComboUI_Init(pTextureTag, UI_Info, pComboCount);

	return pInstance;
}

DWORD ComboUI::Free(void)
{
	UI::Free();

	return DWORD();
}
