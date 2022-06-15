#include "stdafx.h"
#include "Logo.h"
#include "BackLogo.h"
#include "Stage.h"
#include "Management.h"

_USING(Client)

Logo::Logo(LPDIRECT3DDEVICE9 pDevice) : Scene(pDevice)
{
}

HRESULT Logo::Init()
{
	// ���� ���� �ʿ��� ������ü���� ��������.
	if (FAILED(Ready_GameObjectPrototype()))
		return E_FAIL;

	// �ΰ� ������ �ʿ��� ��ü�� ������Ʈ���� ��������.
	if (FAILED(Ready_LayerBackGround(L"Layer_BackGround")))
		return E_FAIL;

	return NOERROR;
}

UPDATE Logo::Update(const float timeDelta)
{
	return Scene::Update(timeDelta);
}

UPDATE Logo::LastUpdate(const float timeDelta)
{
	Scene::LastUpdate(timeDelta);
	
	if (g_bLoading && g_fLoading >= 1.f)
	{
		if (GetKeyState(VK_RETURN) & 0x8000)
		{
			Scene* pNewScene = Stage::Create(m_pDevice);
			if (pNewScene == nullptr)
				return UPDATE::FAIL;

			if (FAILED(Engine::Management::GetInstance()->SetUp_CurrentScene(pNewScene)))
				return UPDATE::FAIL;

			Engine::Safe_Release(pNewScene);

			return UPDATE::NORMAL;
		}
	}

	return UPDATE::NORMAL;
}

void Logo::Render(void)
{
}

HRESULT Logo::Ready_GameObjectPrototype()
{
	Engine::GameObject* pGameObject = nullptr;

	// For.Prototype_GameObject_BackLogo
	pGameObject = BackLogo::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_BackLogo", SCENE::SCENE_LOGO, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Logo::ReadyComponent()
{
	return E_NOTIMPL;
}

HRESULT Logo::Ready_LayerBackGround(const TCHAR * pLayerTag)
{
	// �߰��Ѵ�. (�߰��ҷ����ߴ� ���̾ �ֳľ���, ������ ����)
	if (FAILED(Add_GameObjectToLayer(SCENE::SCENE_LOGO, L"Prototype_GameObject_BackLogo", SCENE::SCENE_LOGO, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

Logo * Logo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Logo* pInstance = new Logo(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Logo::Free()
{
	Engine::Scene::ClearGameObject(SCENE_LOGO);
	Engine::Scene::Free();
	return 0;
}
