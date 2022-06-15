#ifndef SCENE_H__
#define SCENE_H__

#include "Base.h"

_BEGIN(Engine)

class GameObject;
class ObjectMgr;

class _ENGINE_DLL Scene : public Base
{
protected:
	explicit Scene(LPDIRECT3DDEVICE9 pDevice);
	virtual ~Scene() = default;

public:
	virtual HRESULT Init();
	virtual UPDATE Update(const float timeDelta);
	virtual UPDATE LastUpdate(const float timeDelta);
	virtual void Render();

protected:
	HRESULT Add_GameObjectPrototype(const TCHAR* pProtoTag, const size_t sceneIdx, GameObject* pGameObject);
	HRESULT Add_GameObjectToLayer(const size_t sceneIdx_Proto, const TCHAR* pProtoTag, const size_t sceneIdx, const TCHAR* pLayerTag);
	void ClearGameObject(const size_t sceneIdx);

public:
	virtual DWORD Free() override;

private:
	// ��ü���� ������ ���������ϴ� ������.
	ObjectMgr* m_pObjectMgr = nullptr;

protected:
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
};

_END

#endif

