#include "stdafx.h"
#include "Effect.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "TargetManager.h"

_USING(Client)

Effect::Effect(LPDIRECT3DDEVICE9 pDevice)
	:GameObject(pDevice)
{

}

Effect::Effect(const Effect & rhs)
	:GameObject(rhs)
	,m_pTexture(rhs.m_pTexture)
	,mEffect_InitInfo(rhs.mEffect_InitInfo)
{
	m_pTargetMgr = Engine::TargetManager::GetInstance();
	m_pSoundMgr = SoundMgr::GetInstance();
	m_pTexture->AddRef();
}


HRESULT Effect::ReadyPrototype(const EFFECT_INIT_INFO& effect_InitInfo)
{
	Engine::Component*	pComponent = nullptr;

	mEffect_InitInfo = effect_InitInfo;

	// For.Com_Texture
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_EFFECT, mEffect_InitInfo.pTextureName);
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Engine::GameObject::AddComponent(mEffect_InitInfo.pTextureName, pComponent)))
		return E_FAIL;

	float cellPixelWidth = (float)mEffect_InitInfo.imageSizeX / mEffect_InitInfo.imageNumX; // 스프라이트 하나의 넓이
	mEffect_InitInfo.UV_PercenTageX = cellPixelWidth / mEffect_InitInfo.imageSizeX; // 스프라이트 하나의 퍼센트

	float cellPixelHeight = (float)mEffect_InitInfo.imageSizeY / mEffect_InitInfo.imageNumY; // 스프라이트 하나의 넓이
	mEffect_InitInfo.UV_PercentageY = cellPixelHeight / mEffect_InitInfo.imageSizeY; // 스프라이트 하나의 퍼센트

	return NOERROR;
}


HRESULT Effect::EffectInit(const EFFECT_INFO& effectInfo)
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	mEffectInfo = effectInfo;

	m_pPlayerTransform = (Engine::Transform*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Transform", L"Prototype_GameObject_Player");

	if (m_pPlayerTransform == nullptr)
	{
		_MSG_BOX(" PlayerTransform Setting Failed!");
		return E_FAIL;
	}
	m_pPlayerPos = m_pPlayerTransform->GetInfo(Engine::Transform::INFO_POSITION);
	
	switch (mEffectInfo.effectType)
	{
	case EFFECT_HOLDING_BULLET:
		m_vColor = D3DXVECTOR4(0.f, 0.8f, 0.8f, 0.f);
		break;
	case EFFECT_SNIPER_BULLET:
		m_vColor = D3DXVECTOR4(0.4f, 0.1f, 0.9f, 0.f);
		break;
	case EFFECT_EXPLOSION:
		m_vPos = *mEffectInfo.pPos;
		m_vPos.y += 15.f;
		break;
	case EFFECT_SNIPER_EXPLOSION:
		m_vPos = *mEffectInfo.pPos;
		m_vPos.y += 2.5f;
		break;
	case EFFECT_PLAYER_MOVE:
		mEffectInfo.pPos = m_pPlayerPos;
		break;
	case EFFECT_ARROW_EXPLOSION:
		m_vPos = *mEffectInfo.pPos;
		break;
	case EFFECT_BLOOD:
		m_vColor = D3DXVECTOR4(1.f, 0.1f, 0.1f, 0.f);
		m_vPos = *mEffectInfo.pPos;
		break;
	case EFFECT_BLUE_BLOOD:
		m_vColor = D3DXVECTOR4(0.1f, 1.0f, 0.1f, 0.f);
		m_vPos = *mEffectInfo.pPos;
		break;
	case EFFECT_PURPLE_BLOOD:
		m_vColor = D3DXVECTOR4(0.5f, 0.1f, 1.0f, 0.f);
		m_vPos = *mEffectInfo.pPos;
		break;
	default:
		break;
	}

	m_pTransform->SetPos(m_vPos);
	m_pTransform->SetScale(mEffectInfo.vScale);


	return NOERROR;
}

UPDATE Effect::Update(const float timeDelta)
{
	switch (mEffectInfo.effectType)
	{
	case EFFECT_HOLDING_BULLET:
	case EFFECT_SNIPER_BULLET:
		m_pTransform->SetPos(*mEffectInfo.pPos);
		break;
	case EFFECT_PLAYER_MOVE:
		m_vPos = *mEffectInfo.pPos;
		m_vPos.y += 5.f;
		//m_vPos.z -= 5.f;
		m_pTransform->SetPos(m_vPos);
		break;
	default:
		break;
	}

	ComputeViewZ(m_pTransform->GetInfo(Engine::Transform::INFO_POSITION));
	AtlasUpdate(timeDelta);

	return GameObject::Update(timeDelta);
}

UPDATE Effect::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		UPDATE::FAIL;

	if (FAILED(SetUp_BillBoard()))
		UPDATE::FAIL;


	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_ALPHA, this)))
		return UPDATE::FAIL;


	return GameObject::LastUpdate(timeDelta);;
}

void Effect::Render()
{
	if (m_pRectTex == nullptr || m_pShader == nullptr)
		return;

	if (FAILED(SetUp_ConstantTable(m_pEffect)))
		return;

	m_pEffect->Begin(nullptr, 0);
	m_pEffect->BeginPass(0);

	m_pRectTex->Render();

	m_pEffect->EndPass();
	m_pEffect->End();
}

HRESULT Effect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransform->SetUp_OnShader(pEffect, "g_matWorld");

	m_pTexture->SetUp_OnShader(pEffect, "g_DiffuseTex", size_t(mFrame));
	m_pTargetMgr->SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTex");

	D3DXMATRIX	matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetInt("g_iFrameX", mEffect_InitInfo.frameX);
	pEffect->SetInt("g_iFrameY", mEffect_InitInfo.frameY);
	pEffect->SetFloat("g_UVPercentage_X", mEffect_InitInfo.UV_PercenTageX);
	pEffect->SetFloat("g_UVPercentage_Y", mEffect_InitInfo.UV_PercentageY);
	pEffect->SetVector("g_vColor", &m_vColor);

	return NOERROR;
}

HRESULT Effect::ReadyComponent(void)
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
	pComponent = m_pRectTex = (Engine::RectTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_RectTex");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_RectTex", pComponent)))
		return E_FAIL;

	// For.Com_Shader
	pComponent = m_pShader = (Engine::Shader*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SHADER, L"Component_Shader_Effect");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Shader_Effect", pComponent)))
		return E_FAIL;

	m_pEffect = m_pShader->Get_EffectHandle();
	if (m_pEffect == nullptr)
		return E_FAIL;

	return NOERROR;
}

HRESULT Effect::SetUp_BillBoard(void)
{
	D3DXMATRIX matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	D3DXVECTOR3	vScale = m_pTransform->GetScale();

	m_pTransform->SetInfo(Engine::Transform::INFO_RIGHT, &(*(D3DXVECTOR3*)&matView.m[0][0] * vScale.x));
	m_pTransform->SetInfo(Engine::Transform::INFO_LOOK, &(*(D3DXVECTOR3*)&matView.m[2][0] * vScale.z));

	return NOERROR;
}

void Effect::AtlasUpdate(const float timeDelta)
{
	bool bTempDie = false;

	mTimeCheckX += mEffect_InitInfo.frameSpeed * timeDelta;

	if (mTimeCheckX >= 1.f)
	{
		++mEffect_InitInfo.frameX;
		mTimeCheckX = 0.f;
	}
	//X < Y 일 경우에 Y가 1장돌때 X는 x장 돌아야함 
	if (mEffect_InitInfo.frameX >= (int)mEffect_InitInfo.imageNumX)
	{
		mEffect_InitInfo.frameX = 0;
		++mEffect_InitInfo.frameY;
		++mEffectCount;
	}
		
	if (mEffect_InitInfo.frameY >= (int)mEffect_InitInfo.imageNumY)
	{
		mEffect_InitInfo.frameY = 0;
		bTempDie = true;
		
	}

	switch (mEffectInfo.effectType)
	{
	case EFFECT_HOLDING_BULLET:
		if (mEffectInfo.pDie != nullptr)
		{
			if (*mEffectInfo.pDie == true)
			{
				m_pSoundMgr->PlaySound(L"WitchSpinningMove.wav", CHANNEL::CHANNEL_EFFECT);
				m_bDie = true;
			}
		}
		break;
	case EFFECT_SNIPER_BULLET:
		if (mEffectInfo.pDie != nullptr)
		{
			if (*mEffectInfo.pDie == true)
			{
				m_pSoundMgr->PlaySound(L"WitchDarkPollingWind1.wav", CHANNEL::CHANNEL_EFFECT);
				m_bDie = true;
			}
		}
		break;
	case EFFECT_EXPLOSION:
	case EFFECT_ARROW_EXPLOSION:
	case EFFECT_SNIPER_EXPLOSION:
	case EFFECT_BLOOD:
	case EFFECT_PURPLE_BLOOD:
	case EFFECT_BLUE_BLOOD:
		m_bDie = bTempDie;
		break;
	case EFFECT_PLAYER_MOVE:
		if (mEffectCount == 6)
		{
			m_bDie = true;
		}
		break;
	default:
		break;
	}
	
}

Effect * Effect::Create(LPDIRECT3DDEVICE9 pDevice, const EFFECT_INIT_INFO& effect_InitInfo)
{
	Effect*	pInstance = new Effect(pDevice);

	if (FAILED(pInstance->ReadyPrototype(effect_InitInfo)))
	{
		_MSG_BOX("Effect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Effect::EffectClone(const EFFECT_INFO& effectInfo)
{
	Effect* pInstance = new Effect(*this);

	if (FAILED(pInstance->EffectInit(effectInfo)))
	{
		_MSG_BOX("Effect Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Effect::Clone()
{
	return nullptr;
}

DWORD Effect::Free(void)
{
	Engine::Safe_Release(m_pTexture);
	
	if (m_bClone)
	{
		Engine::Safe_Release(m_pShader);
		Engine::Safe_Release(m_pTransform);
		Engine::Safe_Release(m_pRenderer);
		Engine::Safe_Release(m_pRectTex);
		
	}

	GameObject::Free();

	return DWORD();
}


HRESULT Effect::Init()
{
	return NOERROR;
}