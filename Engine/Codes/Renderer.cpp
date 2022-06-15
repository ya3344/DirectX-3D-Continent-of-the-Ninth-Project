
#include "Renderer.h"
#include "GameObject.h"
#include "TargetManager.h"
#include "LightManager.h"
#include "Shader.h"
#include "KeyMgr.h"

_USING(Engine)

Renderer::Renderer(LPDIRECT3DDEVICE9 pDevice)
	:Component(pDevice)
	, m_pTargetManager(TargetManager::GetInstance())
	, m_pLightManager(LightManager::GetInstance())
{
	m_pTargetManager->AddRef();
	m_pLightManager->AddRef();
}

HRESULT Renderer::Init()
{
	if (m_pTargetManager == nullptr)
		return E_FAIL;

	// For.Shader_Light
	m_pShaderLight = Shader::Create(m_pDevice, L"../../Reference/Headers/ShaderLight.fx");
	if (m_pShaderLight == nullptr)
		return E_FAIL;

	// For.BackBuffer
	m_pShaderBlend = Shader::Create(m_pDevice, L"../../Reference/Headers/ShaderBlend.fx");
	if (m_pShaderBlend == nullptr)
		return E_FAIL;

	D3DVIEWPORT9 ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pDevice->GetViewport(&ViewPort);

	// For.Target_Diffuse
	if (FAILED(m_pTargetManager->AddTarget(m_pDevice, L"Target_Diffuse", ViewPort.Width, ViewPort.Height, D3DFMT_A8R8G8B8, D3DXCOLOR(0.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Ready_BufferDebug(L"Target_Diffuse", 0.f, 0.f, 200.f, 200.f)))
		return E_FAIL;

	// For.Target_Normal
	if (FAILED(m_pTargetManager->AddTarget(m_pDevice, L"Target_Normal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Ready_BufferDebug(L"Target_Normal", 0.f, 200.f, 200.f, 200.f)))
		return E_FAIL;

	// For.Target_Shade
	if (FAILED(m_pTargetManager->AddTarget(m_pDevice, L"Target_Shade", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Ready_BufferDebug(L"Target_Shade", 200.f, 0.f, 200.f, 200.f)))
		return E_FAIL;

	// For.Target_Depth // D3DFMT_A32B32G32R32F
	if (FAILED(m_pTargetManager->AddTarget(m_pDevice, L"Target_Depth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.0f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Ready_BufferDebug(L"Target_Depth", 0.f, 400.f, 200.f, 200.f)))
		return E_FAIL;

	// For.Target_Specualr
	if (FAILED(m_pTargetManager->AddTarget(m_pDevice, L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Ready_BufferDebug(L"Target_Specular", 200.f, 200.f, 200.f, 200.f)))
		return E_FAIL;

	// For.MRT_Deferred
	if (FAILED(m_pTargetManager->AddMRT(L"MRT_Deferred", L"Target_Diffuse")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->AddMRT(L"MRT_Deferred", L"Target_Normal")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->AddMRT(L"MRT_Deferred", L"Target_Depth")))
		return E_FAIL;

	// For.MRT_LightAcc
	if (FAILED(m_pTargetManager->AddMRT(L"MRT_LightAcc", L"Target_Shade")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->AddMRT(L"MRT_LightAcc", L"Target_Specular")))
		return E_FAIL;

	return NOERROR;
}

void Renderer::RenderGameObject()
{
	RenderPriority();

	RenderDeferred();
	RenderLightAcc();
	RenderBlend();

	RenderAlpha();
	RenderUI();


	if (KeyMgr::GetInstance()->KeyDown(DIK_3))
	{
		m_bDraw = !m_bDraw;
	}
	if (m_bDraw)
	{
		m_pTargetManager->Render_BufferDebug(L"MRT_Deferred");
		m_pTargetManager->Render_BufferDebug(L"MRT_LightAcc");
	}
	

}

HRESULT Renderer::Add_RenderGroup(RENDERGROUP type, GameObject * pGameObject)
{
	if (type >= RENDER_END)
		return E_FAIL;

	mObjectSpace[type].emplace_back(pGameObject);

	pGameObject->AddRef();

	return NOERROR;
}

void Renderer::RenderPriority()
{
	for (GameObject* pGameObject : mObjectSpace[RENDER_PRIORITY])
	{
		if (pGameObject != nullptr)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	mObjectSpace[RENDER_PRIORITY].clear();
}

void Renderer::RenderNonAlpha()
{
	for (GameObject* pGameObject : mObjectSpace[RENDER_NONALPHA])
	{
		if (pGameObject != nullptr)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	mObjectSpace[RENDER_NONALPHA].clear();
}

bool Compare(GameObject* pSour, GameObject* pDest)
{
	return pSour->GetViewZ() > pDest->GetViewZ();
}


void Renderer::RenderAlpha()
{
	mObjectSpace[RENDER_ALPHA].sort(Compare);

	for (GameObject* pGameObject : mObjectSpace[RENDER_ALPHA])
	{
		if (pGameObject != nullptr)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	mObjectSpace[RENDER_ALPHA].clear();
}

void Renderer::RenderUI()
{
	for (GameObject* pGameObject : mObjectSpace[RENDER_UI])
	{
		if (pGameObject != nullptr)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	mObjectSpace[RENDER_UI].clear();
}

void Renderer::RenderDeferred()
{
	if (m_pTargetManager == nullptr)
		return;

	// MRT_Deferred 준비해라.
	m_pTargetManager->BeginMRT(L"MRT_Deferred");

	RenderNonAlpha();

	// MRT_Deferred 복구해라.
	m_pTargetManager->EndMRT(L"MRT_Deferred");

}

void Renderer::RenderLightAcc()
{
	if (m_pTargetManager == nullptr ||
		m_pLightManager == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderLight->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	// MRT_LightAcc 준비해라.
	m_pTargetManager->BeginMRT(L"MRT_LightAcc"); // Shade타겟을 장치에 셋했다.

	if (FAILED(m_pTargetManager->SetUp_OnShader(pEffect, L"Target_Normal", "g_NormalTex")))
		return;

	if (FAILED(m_pTargetManager->SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTex")))
		return;

	pEffect->Begin(nullptr, 0);

	// Shade타겟에 명암을 그려나간다.
	m_pLightManager->Render(pEffect);

	pEffect->End();


	m_pTargetManager->EndMRT(L"MRT_LightAcc");
}

void Renderer::RenderBlend()
{
	if (m_pTargetManager == nullptr ||
		m_pLightManager == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderBlend->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	if (FAILED(m_pTargetManager->SetUp_OnShader(pEffect, L"Target_Shade", "g_ShadeTex")))
		return;
	if (FAILED(m_pTargetManager->SetUp_OnShader(pEffect, L"Target_Diffuse", "g_DiffuseTex")))
		return;
	if (FAILED(m_pTargetManager->SetUp_OnShader(pEffect, L"Target_Specular", "g_SpecularTex")))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pLightManager->BackBuffer_Rneder();

	pEffect->EndPass();
	pEffect->End();
}

void Renderer::ClearRenderGroup()
{
	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (GameObject* pGameObject : mObjectSpace[i])
		{
			Safe_Release(pGameObject);
		}
		mObjectSpace[i].clear();
	}
}

Renderer * Renderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Renderer* pInstance = new Renderer(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Renderer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * Renderer::Clone()
{
	AddRef();
	return this;
}

DWORD Renderer::Free(void)
{
	Safe_Release(m_pShaderLight);
	Safe_Release(m_pShaderBlend);
	Safe_Release(m_pTargetManager);
	Safe_Release(m_pLightManager);

	ClearRenderGroup();
	Component::Free();

	return 0;
}
