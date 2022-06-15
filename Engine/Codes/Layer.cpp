#include "Layer.h"
#include "GameObject.h"

_USING(Engine);

Layer::Layer()
	
{
}

HRESULT Layer::Init()
{
	return NOERROR;
}

UPDATE Layer::Update(const float timeDelta)
{
	UPDATE exitCode = UPDATE::FAIL;

	for (GameObject* pGameObject : mGameObject_Space)
	{
		if (pGameObject == nullptr)
			continue;

		exitCode = pGameObject->Update(timeDelta);

		if (exitCode == UPDATE::FAIL)
			break;
	}

	return exitCode;
}

UPDATE Layer::LastUpdate(const float timeDelta)
{
	UPDATE exitCode = UPDATE::FAIL;

	for (GameObject* pGameObject : mGameObject_Space)
	{
		if (pGameObject == nullptr)
			continue;

		exitCode = pGameObject->LastUpdate(timeDelta);

		if (exitCode == UPDATE::FAIL)
			break;
	}

	return exitCode;
}

void Layer::GameObjectRender()
{
	for (GameObject* pGameObject : mGameObject_Space)
	{
		if (pGameObject == nullptr)
			continue;

		pGameObject->Render();
	}
}

Component * Layer::GetComponent(const TCHAR* pComponentTag, const TCHAR * pObjectName)
{
	if (pObjectName == nullptr)
	{
		auto iterBegin = mGameObject_Space.begin();

		if (iterBegin == mGameObject_Space.end())
		{
			return nullptr;
		}

		return (*iterBegin)->GetComponent(pComponentTag);

	}
	else
	{
		for (GameObject* pGameObject : mGameObject_Space)
		{
			if (lstrcmp(pGameObject->GetName().c_str(), pObjectName) == 0)
			{
				return pGameObject->GetComponent(pComponentTag);
			}
		}
		_MSG_BOX("GameObject Find Failed!!");
	}

	return nullptr;
}

GameObject * Layer::GetGameObject(const TCHAR * pObjectName)
{
	if (pObjectName == nullptr)
	{
		auto iterBegin = mGameObject_Space.begin();

		if (iterBegin == mGameObject_Space.end())
		{
			return nullptr;
		}

		return *iterBegin;
	
	}
	else
	{
		for (GameObject* pGameObject : mGameObject_Space)
		{
			if (lstrcmp(pGameObject->GetName().c_str(),pObjectName) == 0)
			{
				return pGameObject;
			}
		}
	}
	
	return nullptr;
}

HRESULT Layer::Add_GameObject(GameObject * pGameObject)
{
	if (pGameObject == nullptr)
		return E_FAIL;

	mGameObject_Space.emplace_back(pGameObject);

	return NOERROR;
}

Layer * Layer::Create()
{
	Layer* pInstance = new Layer();

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Layer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Layer::Free()
{
	for (GameObject* pGameObject : mGameObject_Space)
	{
		Safe_Release(pGameObject);
	}
		
	mGameObject_Space.clear();

	return DWORD();
}
