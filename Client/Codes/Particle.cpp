#include "stdafx.h"
#include "..\Headers\Particle.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "TargetManager.h"


_USING(Client)


Particle::Particle(LPDIRECT3DDEVICE9 pDevice)
	:GameObject(pDevice)
{

}

Particle::Particle(const Particle & rhs)
	:GameObject(rhs)
	,m_pPlayerPos(rhs.m_pPlayerPos)
	,m_pTargetMgr(rhs.m_pTargetMgr)
{

}

HRESULT Particle::ReadyPrototype()
{
	m_pTargetMgr = Engine::TargetManager::GetInstance();
	m_pPlayerTransform = (Engine::Transform*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Transform", L"Prototype_GameObject_Player");

	if (m_pPlayerTransform == nullptr)
	{
		_MSG_BOX(" PlayerTransform Setting Failed!");
		return E_FAIL;
	}
	m_pPlayerPos = m_pPlayerTransform->GetInfo(Engine::Transform::INFO_POSITION);

	return NOERROR;
}


HRESULT Particle::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	mt19937 mtRand((size_t)(rand()));
	uniform_real_distribution<float> XRange(-40.f, 40.f);
	uniform_real_distribution<float> YRange(0.f, 20.f);
	uniform_real_distribution<float> ZRange(-20.f, 45.f);

	m_vRandomPos.x = XRange(mtRand);
	m_vRandomPos.y = YRange(mtRand);
	m_vRandomPos.z = ZRange(mtRand);

	m_vPos = *m_pPlayerPos + m_vRandomPos;
	m_vPos.y += 10.f;
	m_pTransform->SetPos(m_vPos);


	if (m_vRandomPos.y >= 0.f && m_vRandomPos.y < 5.f)
	{
		m_vColor = D3DXVECTOR4(0.1f, 1.f, 1.f, 0.f);
	}
	else if (m_vRandomPos.y >= 5.f && m_vRandomPos.y < 10.f)
	{
		m_vColor = D3DXVECTOR4(0.1f, 0.1f, 1.f, 0.f);
	}
	else if (m_vRandomPos.y >= 10.f && m_vRandomPos.y < 15.f)
	{
		m_vColor = D3DXVECTOR4(0.5f, 0.1f, 1.f, 0.f);
	}
	else if (m_vRandomPos.y >= 15.f && m_vRandomPos.y < 20.f)
	{
		m_vColor = D3DXVECTOR4(0.1f, 0.1f, 0.5f, 0.f);
	}



	return NOERROR;
}

UPDATE Particle::Update(const float timeDelta)
{
	mt19937 mtRand((size_t)(rand()));
	
	

	mX_TimeValue += timeDelta * 10.f;
	mZ_TimeValue += timeDelta * 10.f;
	if (mX_TimeValue >= 15.f)
	{
		uniform_real_distribution<float> XDrop(-0.15f, 0.15f);
		mX_TimeValue = 0.f;
		mX_Value = XDrop(mtRand);
	}

	if (mZ_TimeValue >= 7.f)
	{
		uniform_real_distribution<float> ZDrop(-0.05f, 0.05f);
		mZ_TimeValue = 0.f;
		mZ_Value = ZDrop(mtRand);
	}

	m_vPos.x += mX_Value;
	m_vPos.z += mZ_Value;

	m_vPos.y -= timeDelta * 8.f;

	if (m_vPos.y <= m_pPlayerPos->y - 3.f)
	{
		uniform_real_distribution<float> XRange(-40.f, 40.f);
		uniform_real_distribution<float> YRange(0.f, 20.f);
		uniform_real_distribution<float> ZRange(-20.f, 45.f);

		m_vRandomPos.x = XRange(mtRand);
		m_vRandomPos.y = YRange(mtRand);
		m_vRandomPos.z = ZRange(mtRand);

		m_vPos = *m_pPlayerPos + m_vRandomPos; 
		m_vPos.y += 10.f;
		m_pTransform->SetPos(m_vPos);
	}

	m_pTransform->SetPos(m_vPos);

	ComputeViewZ(m_pTransform->GetInfo(Engine::Transform::INFO_POSITION));

	return GameObject::Update(timeDelta);
}

UPDATE Particle::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		UPDATE::FAIL;

	if (FAILED(SetUp_BillBoard()))
		UPDATE::FAIL;

	
	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_ALPHA, this)))
		return UPDATE::FAIL;


	return GameObject::LastUpdate(timeDelta);;
}

void Particle::Render()
{
	if (m_pParticleBuffer == nullptr || m_pShader == nullptr)
		return;

	if (FAILED(SetUp_ConstantTable(m_pEffect)))
		return;

	m_pEffect->Begin(nullptr, 0);
	m_pEffect->BeginPass(0);

	m_pParticleBuffer->RenderBuffer();

	m_pEffect->EndPass();
	m_pEffect->End();
}



HRESULT Particle::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransform->SetUp_OnShader(pEffect, "g_matWorld");

	m_pTexture->SetUp_OnShader(pEffect, "g_DiffuseTex", size_t(mFrame));
	m_pTargetMgr->SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTex");

	D3DXMATRIX	matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &m_vColor);

	return NOERROR;
}

HRESULT Particle::ReadyComponent(void)
{
	Engine::Component*	pComponent = nullptr;

	// For.Com_Transform
	pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Transform", pComponent)))
		return E_FAIL;

	// For.Com_Renderer
	pComponent = m_pRenderer = (Engine::Renderer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Renderer");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Renderer", pComponent)))
		return E_FAIL;

	// For.Com_Buffer
	pComponent = m_pParticleBuffer = (Engine::ParticleBuffer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_ParticleBuffer");
	if (pComponent == nullptr)
		return E_FAIL;

	//m_pParticleBuffer->SetParticleBuffer(true);
	if (FAILED(Engine::GameObject::AddComponent(L"Component_ParticleBuffer", pComponent)))
		return E_FAIL;

	// For.Com_Shader_Particle
	pComponent = m_pShader = (Engine::Shader*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SHADER, L"Component_Shader_Particle");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Shader_Particle", pComponent)))
		return E_FAIL;

	// For.Com_Texture
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_EFFECT, L"Component_Paritcle_Snow");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Engine::GameObject::AddComponent(L"Component_Paritcle_Snow", pComponent)))
		return E_FAIL;

	m_pEffect = m_pShader->Get_EffectHandle();
	if (m_pEffect == nullptr)
		return E_FAIL;

	return NOERROR;
}

HRESULT Particle::SetUp_BillBoard(void)
{
	D3DXMATRIX matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	D3DXVECTOR3	vScale = m_pTransform->GetScale();

	m_pTransform->SetInfo(Engine::Transform::INFO_RIGHT, &(*(D3DXVECTOR3*)&matView.m[0][0] * vScale.x));
	m_pTransform->SetInfo(Engine::Transform::INFO_LOOK, &(*(D3DXVECTOR3*)&matView.m[2][0] * vScale.z));

	return NOERROR;
}

Particle * Particle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Particle*	pInstance = new Particle(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Particle Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Particle::Clone()
{
	Particle* pInstance = new Particle(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Particle Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Particle::Free(void)
{
	if (m_bClone)
	{
		Engine::Safe_Release(m_pShader);
		Engine::Safe_Release(m_pTransform);
		Engine::Safe_Release(m_pRenderer);
		Engine::Safe_Release(m_pParticleBuffer);
		Engine::Safe_Release(m_pTexture);
	}

	GameObject::Free();

	return DWORD();
}
