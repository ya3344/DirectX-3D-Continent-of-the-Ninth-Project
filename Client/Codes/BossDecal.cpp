#include "stdafx.h"
#include "BossDecal.h"
#include "ComponentMgr.h"
#include "TargetManager.h"

_USING(Client)


BossDecal::BossDecal(LPDIRECT3DDEVICE9 pDevice)
	:DecalBox(pDevice)
{
}

BossDecal::BossDecal(const BossDecal & rhs)
	:DecalBox(rhs)
{
}

UPDATE BossDecal::Update(const float timeDelta)
{
	if (m_bDecal == false)
		return UPDATE::NORMAL;


	switch (mDecalType)
	{
	case DECAL_BOSS_ATTACK2:
		mAlpha += timeDelta * 0.5f;
		if (mAlpha >= 1.0f)
		{
			mAlpha = 1.f;
			m_bDecal = false;
		}
		break;
	case DECAL_CRAZY_FOOT:
		m_vScale.x += timeDelta * 180.f;
		m_vScale.z += timeDelta * 180.f;
		m_pTransform->SetScale(m_vScale);

		if (m_vScale.x >= 160.f)
		{
			m_bDecal = false;
		}
		break;
	case DECAL_BOSS_COMBO:
		mRotY += timeDelta * 80.f;
		m_pTransform->SetRotation(D3DXVECTOR3(0.f, mRotY, 0.f));
		mDecalTime += timeDelta * 10.f;
		if (mDecalTime >= 6.0f)
		{
			mDecalTime = 0.f;
			m_bDecal = false;
		}
		break;
	default:
		break;
	}

	return Engine::GameObject::Update(timeDelta);
}

void BossDecal::Render()
{
	if (m_pTransform == nullptr || m_pShader == nullptr)
		return;

	if (FAILED(SetUp_ConstantTable(m_pEffect)))
		return;

	m_pEffect->Begin(nullptr, 0);

	m_pEffect->BeginPass(0);

	m_pCubeBuffer->Render();

	m_pEffect->EndPass();

	m_pEffect->End();
}

void BossDecal::SetDecalInfo(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vScale, const size_t textureNum, const size_t decalType, const bool bDecal, const float alpha)
{
	m_pTransform->SetPos(vPos);
	m_pTransform->SetScale(vScale);
	mTextureNum = textureNum;
	mDecalType = decalType;
	m_bDecal = bDecal;
	mAlpha = alpha;
	m_vScale = vScale;
	mRotY = 0.f;
	mDecalTime = 0.f;
}

BossDecal * BossDecal::Create(LPDIRECT3DDEVICE9 pDevice)
{
	BossDecal* pInstance = new BossDecal(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("BossDecal Created Failed!!!");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * BossDecal::Clone()
{
	BossDecal* pInstance = new BossDecal(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("BossDecal Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD BossDecal::Free()
{
	DecalBox::Free();

	return 0;
}
