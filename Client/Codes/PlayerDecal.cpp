#include "stdafx.h"
#include "PlayerDecal.h"
#include "ComponentMgr.h"


_USING(Client)



PlayerDecal::PlayerDecal(LPDIRECT3DDEVICE9 pDevice)
	:DecalBox(pDevice)
{
}

PlayerDecal::PlayerDecal(const PlayerDecal & rhs)
	: DecalBox(rhs)
{
}

UPDATE PlayerDecal::Update(const float timeDelta)
{
	if (m_bDecal == false)
		return UPDATE::NORMAL;


	switch (mDecalType)
	{
	case DECAL_PALYER_GAIL:
		mRotY += timeDelta * 100.f;
		m_pTransform->SetRotation(D3DXVECTOR3(0.f, mRotY, 0.f));
		m_vColor = D3DXVECTOR4(0.3f, 0.f, 0.7f, 0.f);
		mDecalTime += timeDelta * 10.f;
		if (mDecalTime >= 2.f)
		{
			mDecalTime = 0.f;
			mRotY = 0.f;
			m_bDecal = false;
		}
		break;
	case DECAL_PALYER_ATTSHEL:
		mRotY = timeDelta * 30.f;
		if (mAlpha <= 0.7f)
		{
			m_pTransform->SetRotation(D3DXVECTOR3(0.f, mRotY, 0.f));
		}
		m_vColor = D3DXVECTOR4(0.f, 0.f, 0.f, -1.f);
		mAlpha += timeDelta * 0.3f;
		if (mAlpha >= 0.95f)
		{
			mRotY = 0.f;
			mAlpha = 0.f;
			m_bDecal = false;
		}
		break;
	case DECAL_PALYER_RISING_ATTACK:
		mRotY += timeDelta * 100.f;
		m_pTransform->SetRotation(D3DXVECTOR3(0.f, mRotY, 0.f));
		m_vColor = D3DXVECTOR4(0.8f, 0.1f, 0.1f, 0.9f);
		mDecalTime += timeDelta * 10.f;
		if (mDecalTime >= 1.5f)
		{
			mDecalTime = 0.f;
			mRotY = 0.f;
			m_bDecal = false;
		}
		break;
	case DECAL_PLAYER_SPIN_CRUSH_ATTACK:
		mRotY = timeDelta * 70.f;
		m_pTransform->SetRotation(D3DXVECTOR3(0.f, mRotY, 0.f));
		m_vColor = D3DXVECTOR4(0.f, 0.1f, 0.7f, 0.9f);
		mDecalTime += timeDelta * 10.f;
		
		if (mDecalTime >= 1.5f)
		{
			mDecalTime = 0.f;
			mRotY = 0.f;
			m_bDecal = false;
		}
		break;
	case DECAL_PLAYER_UPPERKICK:
		mRotY = timeDelta * 30.f;
		if (mAlpha <= 0.7f)
		{
			m_pTransform->SetRotation(D3DXVECTOR3(0.f, mRotY, 0.f));
		}
		m_vColor = D3DXVECTOR4(0.2f, 0.8f, 0.8f, 0.4f);
		mDecalTime += timeDelta * 10.f;
		mAlpha += timeDelta * 0.5f;
		if (mDecalTime >= 1.5f)
		{
			mDecalTime = 0.f;
			mRotY = 0.f;
			m_bDecal = false;
			mAlpha = 0.f;
		}
		break;
	case DECAL_PLAYER_STUN_ATTACK:
		mRotY += timeDelta * 100.f;
		m_pTransform->SetRotation(D3DXVECTOR3(0.f, mRotY, 0.f));
		//m_vColor = D3DXVECTOR4(0.f, 0.f, 0.f, 0.9f);
		mDecalTime += timeDelta * 10.f;
		if (mDecalTime >= 1.5f)
		{
			mDecalTime = 0.f;
			mRotY = 0.f;
			m_bDecal = false;
		}
		break;
	case DECAL_PALYER_HOLDING_SHOT:
		mRotY = timeDelta * 30.f;
		if (mAlpha <= 0.7f)
		{
			m_pTransform->SetRotation(D3DXVECTOR3(0.f, mRotY, 0.f));
		}
		m_vColor = D3DXVECTOR4(0.5, 0.f, 1.f, -1.f);
		mAlpha += timeDelta * 0.45f;
		if (mAlpha >= 0.95f)
		{
			mRotY = 0.f;
			mAlpha = 0.f;
			m_bDecal = false;
		}
		break;
	default:
		break;
	}

	return Engine::GameObject::Update(timeDelta);
}

void PlayerDecal::Render()
{
	if (m_pTransform == nullptr || m_pShader == nullptr)
		return;

	if (FAILED(SetUp_ConstantTable(m_pEffect)))
		return;

	m_pEffect->Begin(nullptr, 0);

	m_pEffect->BeginPass(1);

	m_pCubeBuffer->Render();

	m_pEffect->EndPass();

	m_pEffect->End();
}

void PlayerDecal::SetDecalInfo(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vScale, const size_t textureNum, const size_t decalType, const bool bDecal, const float alpha )
{
	m_pTransform->SetPos(vPos);
	m_pTransform->SetScale(vScale);
	mTextureNum = textureNum;
	mDecalType = decalType;
	m_bDecal = bDecal;
	if (alpha > -1.f)
	{
		mAlpha = alpha;
	}
	m_vScale = vScale;
	//mRotY = 0.f;
	mDecalTime = 0.f;
	m_vColor = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
}

PlayerDecal * PlayerDecal::Create(LPDIRECT3DDEVICE9 pDevice)
{
	PlayerDecal* pInstance = new PlayerDecal(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("PlayerDecal Created Failed!!!");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * PlayerDecal::Clone()
{
	PlayerDecal* pInstance = new PlayerDecal(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("PlayerDecal Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD PlayerDecal::Free()
{
	DecalBox::Free();

	return 0;
}
