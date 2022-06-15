#include "stdafx.h"
#include "MainApp.h"
#include "GraphicDevice.h"
#include "Management.h"
#include "Logo.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "InputDevice.h"
#include "TimerMgr.h"
#include "TargetManager.h"


_USING(Client);

MainApp::MainApp() 
	: m_pGraphicDevice(Engine::GraphicDevice::GetInstance())
	, m_pManagement(Engine::Management::GetInstance())
{
	m_pGraphicDevice->AddRef();
	m_pManagement->AddRef();
}


HRESULT MainApp::Init()
{
	srand((size_t)time(NULL));

	if (FAILED(DefaultSetting(WIN_MODE::MODE_WIN, WIN_SIZEX, WIN_SIZEY)))
		return E_FAIL;

	if (FAILED(Ready_ComponentStatic()))
		return E_FAIL;

	if (FAILED(SetUp_StartScene(SCENE_LOGO)))
		return E_FAIL;

	Engine::KeyMgr::GetInstance()->Init(g_hInst, g_hWnd);
	m_pUtilityMgr = UtilityMgr::GetInstance();
	m_pUtilityMgr->AddRef();

	return S_OK;
}

UPDATE MainApp::Update(const float timeDelta)
{
	if (m_pManagement == nullptr)
		return UPDATE::FAIL;

	Engine::KeyMgr::GetInstance()->UpdateKeyMgr();
	m_pUtilityMgr->KeyCheck();

	return m_pManagement->Update(timeDelta);
}

void MainApp::Render()
{
	if (m_pGraphicDevice == nullptr || m_pRenderer == nullptr)
		return;

	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
		D3DXCOLOR(0.0f, 0.f, 1.f, 1.f), 1.0f, 0);
	m_pDevice->BeginScene();

	// Draw
	m_pManagement->Render();
	m_pRenderer->RenderGameObject();

	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, 0, nullptr);
}

HRESULT MainApp::DefaultSetting(WIN_MODE mode, const WORD backSizeX, const WORD backSizeY)
{
	if (m_pGraphicDevice == nullptr)
	{
		_MSG_BOX("Ready_DefaultSetting Failed");
		return E_FAIL;
	}

	if (FAILED(m_pGraphicDevice->Init(mode, g_hWnd, backSizeX, backSizeY, &m_pDevice)))
	{
		_MSG_BOX("Ready_DefaultSetting Failed");
		return E_FAIL;
	}

	// 유브이 좌표로 저장된 텍스쳐로부터 픽셀을 얻어오는 방식.
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	Engine::InputDevice::GetInstance()->Init(g_hInst, g_hWnd);

	m_pManagement->ReserveContainer(SCENE::SCENE_END);
	Engine::ComponentMgr::GetInstance()->ReserveContainer(SCENE_END);

	return NOERROR;
}

HRESULT MainApp::Ready_ComponentStatic()
{
	Engine::Component* pComponent = nullptr;

	// For.Component_Renderer
	pComponent = m_pRenderer = Engine::Renderer::Create(m_pDevice);
	if (pComponent == nullptr)
		return E_FAIL;
	m_pRenderer->AddRef();

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_Renderer", pComponent)))
		return E_FAIL;


	// For.Component_Transform
	pComponent = Engine::Transform::Create(m_pDevice);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_Transform", pComponent)))
		return E_FAIL;


	// For.Component_Buffer_RectColor
	pComponent = Engine::RectColor::Create(m_pDevice);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_RectColor", pComponent)))
		return E_FAIL;


	// For.Component_Buffer_RectTexture
	pComponent = Engine::RectTex::Create(m_pDevice);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_RectTex", pComponent)))
		return E_FAIL;

	// For.Component_Buffer_ParitcleBuffer
	pComponent = Engine::ParticleBuffer::Create(m_pDevice);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_ParticleBuffer", pComponent)))
		return E_FAIL;


	// For.Component_Buffer_CubeTexture
	pComponent = Engine::CubeTex::Create(m_pDevice);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_Buffer_CubeTexture", pComponent)))
		return E_FAIL;


	// For.Component_Buffer_CubeColor
	pComponent = Engine::CubeColor::Create(m_pDevice);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_Buffer_CubeColor", pComponent)))
		return E_FAIL;

	// For.Component_Buffer_Trail
	pComponent = Engine::TrailBuffer::Create(40, m_pDevice);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_Player_Buffer_Trail", pComponent)))
		return E_FAIL;

	pComponent = Engine::TrailBuffer::Create(20, m_pDevice);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_BossLeft_Buffer_Trail", pComponent)))
		return E_FAIL;

	pComponent = Engine::TrailBuffer::Create(20, m_pDevice);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_BossRight_Buffer_Trail", pComponent)))
		return E_FAIL;




	return NOERROR;
}

HRESULT MainApp::SetUp_StartScene(SCENE sceneType)
{
	if (m_pManagement == nullptr)
		return E_FAIL;

	Engine::Scene* pNewScene = nullptr;

	switch (sceneType)
	{
	case SCENE_LOGO:
		pNewScene = Logo::Create(m_pDevice);
		break;
	case SCENE_STAGE:
		break;
	}

	if (pNewScene == nullptr)
		return E_FAIL;

	// 현재 내가 할당해 놓은 씬을 매니지먼트 객체가 가지고 있게 만든다.
	if (FAILED(m_pManagement->SetUp_CurrentScene(pNewScene)))
		return E_FAIL;

	Safe_Release(pNewScene);

	return NOERROR;
}

MainApp * MainApp::Create()
{
	MainApp* pInstance = new MainApp;

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("MainApp Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void MainApp::Pre_ReleaseSingleton()
{
	DWORD refCnt = 0;
	if (refCnt = BulletMgr::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("BulletMgr Release Failed");
	}

	if (refCnt = EnemyMgr::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("EnemyMgr Release Failed");
	}

	if (refCnt = UIMgr::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("UIMgr Release Failed");
	}

	if (refCnt = EffectMgr::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("EffectMgr Release Failed");
	}

	if (refCnt = ParticleMgr::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("ParticleMgr Release Failed");
	}

	if (refCnt = SoundMgr::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("SoundMgr Release Failed");
	}

}

void MainApp::Late_ReleaseSingleton()
{
	DWORD refCnt = 0;


	if (refCnt = UtilityMgr::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("UtilityMgr Release Failed");
	}

	if (refCnt = Engine::KeyMgr::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("KeyMgr Release Failed");
	}

	if (refCnt = StaticObject_Mgr::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("StaticObject_Mgr Release Failed");
	}


}

void MainApp::ReleaseSingleton()
{
	DWORD refCnt = 0;


	if (refCnt = Engine::TimerMgr::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("TimerMgr Release Failed");
	}
		
	if (refCnt = Engine::Management::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("Management Release Failed");
	}

	if (refCnt = Engine::ObjectMgr::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("ObjectMgr Release Failed");
	}

	if (refCnt = Engine::ComponentMgr::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("CComponent_Manager Release Failed");
	}

	if (refCnt = Engine::InputDevice::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("InputDevice Release Failed");
	}

	if (refCnt = Engine::LightManager::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("LightManager Release Failed");
	}

	if (refCnt = Engine::TargetManager::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("TargetManager Release Failed");
	}

	if (refCnt = Engine::GraphicDevice::GetInstance()->DestroyInstance())
	{
		_MSG_BOX("GraphicDevice Release Failed");
	}



}

DWORD MainApp::Free()
{
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pDevice);
	Engine::Safe_Release(m_pGraphicDevice);
	Engine::Safe_Release(m_pManagement);
	Engine::Safe_Release(m_pUtilityMgr);

	Pre_ReleaseSingleton();
	ReleaseSingleton();
	Late_ReleaseSingleton();

	return 0;
}
