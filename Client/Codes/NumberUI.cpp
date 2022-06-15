#include "stdafx.h"
#include "NumberUI.h"
#include "RectTex.h"
#include "Texture.h"

_USING(Client)

NumberUI::NumberUI(LPDIRECT3DDEVICE9 pDevice)
	:UI(pDevice)
{
}

NumberUI::NumberUI(const NumberUI & rhs)
	: UI(rhs)
{
	
}

void NumberUI::NumberUI_Init(const D3DXMATRIX& pEnemyWorldMatrix, int damageValue)
{
	// 50000이상이면 크리티컬로 간주.
	mDamagedValue = damageValue;
	m_matEnemyWorld = pEnemyWorldMatrix;

	//(D3DXVECTOR3*)&mMatrixInfo.matWorld.m[INFO_LOOK][0]
	 /*D3DXVECTOR3 vEnemyDir = *(D3DXVECTOR3*)&m_matEnemyWorld.m[2][0];
	 D3DXVec3Normalize(&vEnemyDir, &vEnemyDir);
	 *(D3DXVECTOR3*)&m_matEnemyWorld.m[3][0] -= vEnemyDir  * 3.f;*/

	if (mUIInfo.type == UI_NUMBERING_DAMAGE_CRITICAL)
	{
		mTextureNum = 10;
		WorldToViewPort();
		m_matWorld._42 -= (WIN_SIZEY * 0.1f);
		m_bNumbering = true;
		return;
	}


	while (damageValue != 0)
	{
		mNumberInfo.positionNum_10000 = damageValue / 10000;
		damageValue %= 10000;

		mNumberInfo.positionNum_1000 = damageValue / 1000;
		damageValue %= 1000;

		mNumberInfo.positionNum_100 = damageValue / 100;
		damageValue %= 100;

		mNumberInfo.positionNum_10 = damageValue / 10;
		damageValue %= 10;

		mNumberInfo.positionNum_1 = damageValue;
		damageValue = 0;
	}

	m_bNumbering = true;
	WorldToViewPort();

	mWidthValue = 0.75f;

	switch (mUIInfo.type)
	{
	case UI_NUMBERING_DAMAGE_1:
		mTextureNum = mNumberInfo.positionNum_1;
		mOffset = mUIInfo.width * (mWidthValue * 2.f);
		break;
	case UI_NUMBERING_DAMAGE_10:
		mTextureNum = mNumberInfo.positionNum_10;
		mOffset = mUIInfo.width * mWidthValue;
		break;
	case UI_NUMBERING_DAMAGE_100:
		mTextureNum = mNumberInfo.positionNum_100;
		mOffset = 0.f;
		break;
	case UI_NUMBERING_DAMAGE_1000:
		mTextureNum = mNumberInfo.positionNum_1000;
		mOffset = -mUIInfo.width * mWidthValue;
		break;
	case UI_NUMBERING_DAMAGE_10000:
		mTextureNum = mNumberInfo.positionNum_10000;
		if (mTextureNum == 0)
		{
			m_bNumbering = false;
		}
		mOffset = -mUIInfo.width * (mWidthValue * 2.f);
		break;
	default:
		break;
	}

	m_matWorld._11 = mUIInfo.width;
	m_matWorld._22 = mUIInfo.height;
	m_matWorld._41 = mUIInfo.X - (WIN_SIZEX * 0.5f) + mOffset;
	m_matWorld._42 = -mUIInfo.Y + (WIN_SIZEY * 0.5f); // 0.55
	mTimeDelta = 0.f;
}


UPDATE NumberUI::Update(const float timeDelta)
{
	if (m_bNumbering == false)
		return UPDATE::NORMAL;

	if (mUIInfo.type != UI_NUMBERING_DAMAGE_CRITICAL)
	{
		NumberingUpdate(timeDelta);
	}
	else if(mUIInfo.type == UI_NUMBERING_DAMAGE_CRITICAL)
	{
		CriticalUpdate(timeDelta);
	}

	//NumberingUpdate(timeDelta);
	return UPDATE::NORMAL;
}

void NumberUI::Render()
{
	if (m_pTransform == nullptr || m_pShader == nullptr)
		return;

	if (m_bNumbering == false)
		return;

	if (FAILED(SetUp_ConstantTable(m_pEffect)))
		return;

	m_pEffect->Begin(nullptr, 0);
	m_pEffect->BeginPass(0);

	m_pRectTex->Render();

	m_pEffect->EndPass();
	m_pEffect->End();
}

void NumberUI::NumberingUpdate(float timeDelta)
{
	mTimeDelta += timeDelta * 10.f;

	if (mDamagedValue > 0.f && mDamagedValue <= 14000.f)
	{
		m_vColor = D3DXVECTOR4(1.3f, 1.3f, 1.3f, 0.f);
		mSize_AddValue = 0.07f;
		mHeightSpeed = 450.f;
	}
	else if (mDamagedValue > 14000.f && mDamagedValue <= 28000.f)
	{
		m_vColor = D3DXVECTOR4(1.5f, 1.5f, 0.2f, 0.f);
		mSize_AddValue = 0.08f;
		mHeightSpeed = 400.f;
	}
	else if (mDamagedValue > 28000.f && mDamagedValue <= 42000.f)
	{
		m_vColor = D3DXVECTOR4(1.5f, 0.2f, 1.5f, 0.f);
		mSize_AddValue = 0.09f;
		mHeightSpeed = 350.f; // 350.f
	}
	else if (mDamagedValue > 42000.f && mDamagedValue <= 52000)
	{
		m_vColor = D3DXVECTOR4(1.5f, 0.2f, 0.2f, 0.f);
		mSize_AddValue = 0.09f; // 0.1f
		mHeightSpeed = 350.f; // 300.f
	}
	else
	{
		m_vColor = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
		mSize_AddValue = 0.15f;
	}


	if (mTimeDelta <= 0.5f)
	{
		mUIInfo.width = WIN_SIZEX * mSize_AddValue;
		mUIInfo.height = WIN_SIZEY * mSize_AddValue;
		mAlphaValue -= 2.f * timeDelta;
	}
	else if (mTimeDelta > 0.5f && mTimeDelta <= 4.f)
	{
		mUIInfo.width -= timeDelta * (mSize_AddValue * 1000.f);
		mUIInfo.height -= timeDelta * (mSize_AddValue * 1000.f);
		mAlphaValue = 1.f;

	}
	else
	{
		mUIInfo.width -= timeDelta * (mSize_AddValue * 400.f);
		mUIInfo.height -= timeDelta * (mSize_AddValue * 400.f);
		mAlphaValue -= 1.3f * timeDelta;
	}

	switch (mUIInfo.type)
	{
	case UI_NUMBERING_DAMAGE_1:
		mOffset = mUIInfo.width * (mWidthValue * 2.f);
		break;
	case UI_NUMBERING_DAMAGE_10:
		mOffset = mUIInfo.width * mWidthValue;
		break;
	case UI_NUMBERING_DAMAGE_100:
		mOffset = 0.f;
		break;
	case UI_NUMBERING_DAMAGE_1000:
		mOffset = -mUIInfo.width * mWidthValue;
		break;
	case UI_NUMBERING_DAMAGE_10000:
		mOffset = -mUIInfo.width * (mWidthValue * 2.f);
		break;
	default:
		break;
	}

	m_matWorld._11 = mUIInfo.width;
	m_matWorld._22 = mUIInfo.height;
	m_matWorld._41 = mUIInfo.X - (WIN_SIZEX * 0.5f) + mOffset;
	m_matWorld._42 += mHeightSpeed * timeDelta;

	if (m_matWorld._42 >= 400.f)
	{
		m_bNumbering = false;
	}
}

void NumberUI::CriticalUpdate(float timeDelta)
{
	mTimeDelta += timeDelta * 10.f;

	if (mTimeDelta <= 0.5f)
	{
		mUIInfo.width = WIN_SIZEX * 0.3f;
		mUIInfo.height = WIN_SIZEY * 0.15f;
		mAlphaValue -= 2.f * timeDelta;
	}
	else if (mTimeDelta > 0.5f && mTimeDelta <= 4.f)
	{
		mUIInfo.width -= timeDelta * (150.f);
		mUIInfo.height -= timeDelta * (100.f);
		mAlphaValue = 1.f;
	}
	else
	{
		mUIInfo.width -= timeDelta * (45.f);
		mUIInfo.height -= timeDelta * (25.f);
		mAlphaValue -= 1.3f * timeDelta;
	}

	m_matWorld._11 = mUIInfo.width;
	m_matWorld._22 = mUIInfo.height;
	m_matWorld._42 += 350.f * timeDelta;

	if (m_matWorld._42 >= 400.f)
	{
		m_bNumbering = false;
	}
}



HRESULT NumberUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void NumberUI::WorldToViewPort()
{
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	D3DXMATRIX matWVP;
	D3DXVECTOR4 vWorldToScreen(0, 0, 0, 1);

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	matWVP = m_matEnemyWorld * matView * matProj;

	D3DXVec4Transform(&vWorldToScreen, &vWorldToScreen, &matWVP);
	
	float worldToScreenX = vWorldToScreen.x / vWorldToScreen.w;
	float worldToScreenY = vWorldToScreen.y / vWorldToScreen.w;
	float worldToScreenZ = vWorldToScreen.z / vWorldToScreen.w;

	// -1 ~ 1 구간을 0 ~ 1 구간으로 변환
	worldToScreenX = (worldToScreenX + 1) * 0.5f;
	worldToScreenY = (worldToScreenY + 1) * 0.5f;
	worldToScreenY = 1.0f - worldToScreenY;

	worldToScreenX *= WIN_SIZEX;
	worldToScreenY *= WIN_SIZEY;

	mUIInfo.X = worldToScreenX;
	mUIInfo.Y = worldToScreenY;
}




NumberUI * NumberUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	NumberUI* pInstance = new NumberUI(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("NumberUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * NumberUI::NumberingUI_Clone(const TCHAR * pTextureTag, const D3DXMATRIX& pEnemyWorldMatrix, const float damageValue, const UI_INFO & UI_Info)
{
	NumberUI* pInstance = new NumberUI(*this);

	if (FAILED(pInstance->InitUI(pTextureTag, UI_Info)))
	{
		_MSG_BOX("NumberUI Clone Failed");
		Engine::Safe_Release(pInstance);
	}

	pInstance->NumberUI_Init(pEnemyWorldMatrix, (int)damageValue);

	return pInstance;
}

DWORD NumberUI::Free(void)
{
	UI::Free();

	return DWORD();
}

