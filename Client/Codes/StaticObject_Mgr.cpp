#include "stdafx.h"
#include "StaticObject_Mgr.h"
#include "StaticObject.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"

_USING(Client)

_IMPLEMENT_SINGLETON(StaticObject_Mgr)


StaticObject_Mgr::StaticObject_Mgr()
{
}


HRESULT StaticObject_Mgr::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR * pFilePath, const TCHAR* pLayerTag)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	DWORD byte;

	size_t objectSize;
	STATIC_OBJECT_INFO objectInfo;
	Engine::GameObject* pGameObject = nullptr;

	ReadFile(hFile, &objectSize, sizeof(size_t), &byte, nullptr);
	mProtoName_Space.reserve(objectSize);

	for (size_t i = 0; i < objectSize; i++)
	{
		ReadFile(hFile, &objectInfo, sizeof(STATIC_OBJECT_INFO), &byte, nullptr);

		switch (objectInfo.objectType)
		{
		case OBJECT_CRISTAL:
			wsprintf(mProtoName_Buf, L"Prototype_Cristal_1_%d", i);
			break;
		case OBJECT_CRISTAL2:
			wsprintf(mProtoName_Buf, L"Prototype_Cristal_2_%d", i);
			break;
		case OBEJCT_ICE_ROCK:
			wsprintf(mProtoName_Buf, L"Prototype_Ice_Rock_1_%d", i);
			break;
		case OBJECT_LAMP_BONE_A:;
			wsprintf(mProtoName_Buf, L"Prototype_Lamp_Bone_A_%d", i);
			break;
		case OBJECT_LAMP_BONE_B:
			wsprintf(mProtoName_Buf, L"Prototype_Lamp_Bone_B_%d", i);
			break;
		case OBJECT_LAMP_BONE_C:
			wsprintf(mProtoName_Buf, L"Prototype_Lamp_Bone_C_%d", i);
			break;
		default:
			assert(false && "Create");
			break;
		}

		mProtoName_Space.emplace_back(mProtoName_Buf);

		pGameObject = StaticObject::Create(pDevice, objectInfo);
		if (pGameObject == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(mProtoName_Space[i].c_str(), SCENE_STATIC_MESH, pGameObject)))
			return E_FAIL;

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_STATIC_MESH, mProtoName_Space[i].c_str(), SCENE_STATIC_MESH, pLayerTag)))
			return E_FAIL;
	}

	return NOERROR;
}

DWORD StaticObject_Mgr::Free()
{
	/*for (StaticObject* pStaticObject : mStaticObject_Space)
	{
		Safe_Delete(pStaticObject);
	}

	mStaticObject_Space.clear();*/
	mProtoName_Space.clear();

	return 0;
}
