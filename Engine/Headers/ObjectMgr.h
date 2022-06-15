#ifndef OBJECT_MGR_H__
#define OBJECT_MGR_H__

#include "Base.h"

_BEGIN(Engine);

class GameObject;
class Layer;
class Component;

class _ENGINE_DLL ObjectMgr final : public Base
{
public:
	_DECLARE_SINGLETON(ObjectMgr)

private:
	explicit ObjectMgr();
	virtual ~ObjectMgr() = default;

public:
	HRESULT ReserveContainer(const WORD size);
	HRESULT Add_GameObjectPrototype(const TCHAR* pProtoTag, const size_t sceneIdx, GameObject* pGameObject);
	HRESULT Add_GameObjectToLayer(const size_t sceneIdx_Proto, const TCHAR* pProtoTag, const size_t sceneIdx, const TCHAR* pLayerTag);
	UPDATE Update_GameObject(const float timeDelta);
	UPDATE LastUpdate_GameObject(const float timeDelta);

public:
	// ������Ʈ�� ������������ �Լ�
	Component* GetComponent(const size_t sceneIdx, const TCHAR* pLayerTag, const TCHAR* pComponentTag, const TCHAR * pObjectName=nullptr);
	GameObject* GetGameObject(const size_t sceneIdx, const TCHAR* pProtoTag, const TCHAR* pGameObjectTag);

	GameObject* FindPrototype(const size_t sceneIdx, const TCHAR* pProtoTag);
	Layer* FindLayer(const size_t iSceneIdx, const TCHAR* pLayerTag);


public:
	virtual DWORD Free() override;
	void Clear_GameObject(const size_t sceneIdx);

private:
	WORD mReserveSize = 0;
	bool m_bReserved = false;

private: // ������ü���� �����ϱ� ���� �����̳�.
	unordered_map<const TCHAR*, GameObject*>* m_pProtoTypeSpace = nullptr;

	typedef unordered_map<const TCHAR*, GameObject*> PROTOTYPE_MAP;
	typedef pair<const TCHAR*, GameObject*> PROTOTYPE_PAIR;
	
private: // ���� ����� ���� ��ü���� �����ϱ� ���� �����̳�
	map<const TCHAR*, Layer*>* m_pLayerSpace = nullptr;

	typedef map<const TCHAR*, Layer*> LAYER_MAP;
	typedef pair<const TCHAR*, Layer*> LAYER_PAIR;

};

_END

#endif //OBJECT_MGR_H__

