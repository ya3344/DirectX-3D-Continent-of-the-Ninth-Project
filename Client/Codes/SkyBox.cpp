
#include "stdafx.h"
#include "SkyBox.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "Camera.h"

_USING(Client)


SkyBox::SkyBox(LPDIRECT3DDEVICE9 pDevice)
	: Engine::GameObject(pDevice)
{
}

SkyBox::SkyBox(const SkyBox & rhs)
	: Engine::GameObject(rhs)
{
}

HRESULT SkyBox::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	Engine::ObjectMgr* pObjectMgr = nullptr;
	m_pCamera = (Engine::Camera*)pObjectMgr->GetInstance()->GetGameObject(SCENE_STAGE, L"Layer_Camera", 0);

	return NOERROR;
}

UPDATE SkyBox::Update(const float timeDelta)
{

	return Engine::GameObject::Update(timeDelta);
}

UPDATE SkyBox::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	m_vTargetPos = m_pCamera->GetViewInfo().vEye;

	m_pTransform->SetPos(m_vTargetPos);

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_PRIORITY, this)))
		return UPDATE::FAIL;

	return Engine::GameObject::LastUpdate(timeDelta);
}

void SkyBox::Render()
{
	if (m_pBuffer == nullptr)
		return;
	
	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", 0);
	m_pTransform->SetUp_OnShader(pEffect, "g_matWorld", false);

	D3DXMATRIX matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBuffer->Render();

	pEffect->EndPass();
	pEffect->End();

}

HRESULT SkyBox::ReadyPrototype()
{
	Engine::Component* pComponent = nullptr;
	Engine::GameObject* pGameObject = nullptr;

	TCHAR namePath[MAX_PATH] = L"";
	static size_t count = 0;

	// For.Component_Texture_SkyBox
	pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::CUBE, L"../Bin/Resources/Textures/SkyBox/burger2.dds");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STAGE, L"Component_Texture_SkyBox", pComponent)))
		return E_FAIL;


	return NOERROR;
}

HRESULT SkyBox::ReadyComponent(void)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Transform
	pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Transform", pComponent)))
		return E_FAIL;

	// For.Com_Renderer
	pComponent = m_pRenderer = (Engine::Renderer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Renderer", pComponent)))
		return E_FAIL;

	// For.Com_Buffer
	pComponent = m_pBuffer = (Engine::CubeTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Buffer_CubeTexture");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Buffer_CubeTexture", pComponent)))
		return E_FAIL;

	// For.Com_Texture
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STAGE, L"Component_Texture_SkyBox");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Texture_SkyBox", pComponent)))
		return E_FAIL;

	// For.Com_Shader
	pComponent = m_pShader = (Engine::Shader*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SHADER, L"Component_Shader_Sky");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Shader_Sky", pComponent)))
		return E_FAIL;

	return NO_ERROR;
}

SkyBox * SkyBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	SkyBox* pInstance = new SkyBox(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("SkyBox Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * SkyBox::Clone()
{
	SkyBox*	pInstance = new SkyBox(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("SkyBox Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD SkyBox::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pBuffer);
	Engine::Safe_Release(m_pTexture);
	Engine::Safe_Release(m_pShader);

	GameObject::Free();

	return DWORD();
}
