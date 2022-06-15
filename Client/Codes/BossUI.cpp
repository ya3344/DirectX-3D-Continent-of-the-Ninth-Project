#include "stdafx.h"
#include "BossUI.h"
#include "RectTex.h"
#include "Texture.h"
#include "GraphicDevice.h"
#include "Navigation.h"

_USING(Client)

BossUI::BossUI(LPDIRECT3DDEVICE9 pDevice)
	:UI(pDevice)
{

}

BossUI::BossUI(const BossUI & rhs)
	: UI(rhs)
{

}

void BossUI::BossUI_Init()
{
	mUIInfo.X = m_matWorld._41;
	/*m_pFont = Engine::GraphicDevice::GetInstance()->GetFont();
	m_pSprite = Engine::GraphicDevice::GetInstance()->GetSprite();
	D3DXMatrixIdentity(&mFontMatrix);*/

	switch (mUIInfo.type)
	{
	case UI_BOSS_RED_GAUAGE:
		mAlphaValue = 1.f;
		mAlphaTestValue = 85.f;
		m_vColor = D3DXVECTOR4(1.5f, 0.2f, 0.2f, 0.f);
		mShaderIndex = 1;
		break;
	case UI_BOSS_YELLOW_GAUAGE:
		mAlphaValue = 1.f;
		mAlphaTestValue = 85.f;
		m_vColor = D3DXVECTOR4(1.5f, 1.5f, 0.4f, 0.f);
		mShaderIndex = 1;
		break;
	case UI_BOSS_GREEN_GAUAGE:
		mAlphaValue = 1.f;
		mAlphaTestValue = 85.f;
		m_vColor = D3DXVECTOR4(0.4f, 1.5f, 0.4f, 0.f);
		mShaderIndex = 1;
		break;
	default:
		mAlphaValue = 1.f;
		mAlphaTestValue = 85.f;
		mShaderIndex = 0;
		break;
	}

	//D3DXMatrixTranslation(&mFontMatrix, WIN_SIZEX * 0.5f, WIN_SIZEY * 0.5f, 0.f);
	//m_pSprite->SetTransform(&mFontMatrix);
	//lstrcpy(m_pFontBuf, L"Hanuman");
	mBossHP = 1.f;

}


UPDATE BossUI::Update(const float timeDelta)
{
	if (*m_pPlayerIndex >= 356)
	{
		if (m_bBossUI == false)
		{
			m_pSoundMgr->StopAll();
			m_pSoundMgr->PlayBGM(L"HanumanBGM.ogg", CHANNEL_BGM);
			//m_pSoundMgr->SetVolume()
			m_bBossUI = true;
		}
	}

	if (m_bBossUI == false)
	{
		if (1 != m_pSoundMgr->IsPlayCheck(CHANNEL_BGM))
		{
			m_pSoundMgr->StopAll();
			m_pSoundMgr->PlayBGM(L"Stage1BGM.wav", CHANNEL_BGM);
			m_pSoundMgr->PlayBGM(L"BGM_Act4.mp3", CHANNEL_BGM);
			//m_pSoundMgr->SetVolume()
		}
		UPDATE::NORMAL;
	}
		

	switch (mUIInfo.type)
	{
	case UI_BOSS_GREEN_GAUAGE:
		if (*m_pBossCount == 3)
		{
			if (*m_pBossHP / mBossOriginalHP < mBossHP)
			{
				mBossHP -= timeDelta * 0.1f;
			}
			else
			{
				mBossHP = *m_pBossHP / mBossOriginalHP;
			}
		}
		else if (*m_pBossCount == 2)
		{
			mBossHP = 0.f;
		}
		break;
	case UI_BOSS_YELLOW_GAUAGE:
		if (*m_pBossCount == 2)
		{
			if (*m_pBossHP / mBossOriginalHP < mBossHP)
			{
				mBossHP -= timeDelta * 0.1f;
			}
			else
			{
				mBossHP = *m_pBossHP / mBossOriginalHP;
			}
		}
		else if (*m_pBossCount == 1)
		{
			mBossHP = 0.f;
		}
		break;
	case UI_BOSS_RED_GAUAGE:
		if (*m_pBossCount == 1)
		{
			if (*m_pBossHP / mBossOriginalHP < mBossHP)
			{
				mBossHP -= timeDelta * 0.1f;
			}
			else
			{
				mBossHP = *m_pBossHP / mBossOriginalHP;
			}
		}
		else if (*m_pBossCount == 0)
		{
			mBossHP = 0.f;
		}
		break;
	default:
		break;
	}

	return UPDATE::NORMAL;
}

void BossUI::Render()
{
	if (m_bBossUI == false)
		return;

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

HRESULT BossUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->SetMatrix("g_matWorld", &m_matWorld);
	pEffect->SetMatrix("g_matView", &m_matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);
	pEffect->SetFloat("g_fAlphaValue", mAlphaValue);
	pEffect->SetFloat("g_fAlphaTestValue", mAlphaTestValue);
	pEffect->SetVector("g_vColor", &m_vColor);
	pEffect->SetFloat("g_fBossHP", mBossHP);

	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", mTextureNum);

	return NOERROR;
}

BossUI * BossUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	BossUI* pInstance = new BossUI(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("BossUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * BossUI::BossUI_Clone(const TCHAR * pTextureTag, const UI_INFO & UI_Info)
{
	BossUI* pInstance = new BossUI(*this);

	if (FAILED(pInstance->InitUI(pTextureTag, UI_Info)))
	{
		_MSG_BOX("BossUI Clone Failed");
		Engine::Safe_Release(pInstance);
	}
	pInstance->BossUI_Init();

	return pInstance;
}

DWORD BossUI::Free(void)
{
	UI::Free();
	return DWORD();
}

