#ifndef LAYER_H__
#define LAYER_H__

#include "Base.h"

_BEGIN(Engine)

class GameObject;
class Component;

class  _ENGINE_DLL  Layer final : public Base
{
private:
	explicit Layer();
	virtual ~Layer() = default;

public:
	HRESULT Init();
	UPDATE Update(const float timeDelta);
	UPDATE LastUpdate(const float timeDelta);

public:
	// 복합 컴포넌트 구조에서 따로 호출.(Player, Boss)
	void GameObjectRender();

public:
	Component* GetComponent(const TCHAR* pComponentTag, const TCHAR * pObjectName = nullptr);
	GameObject* GetGameObject(const TCHAR* pObjectName = nullptr);
	HRESULT Add_GameObject(GameObject* pGameObject);

private:
	list<GameObject*> mGameObject_Space;

public:
	static Layer* Create();
	virtual DWORD Free() override;

};

_END

#endif
