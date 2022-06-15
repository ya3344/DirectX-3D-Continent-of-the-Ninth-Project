#ifndef STATIC_OBJECT_MGR_H__
#define STATIC_OBJECT_MGR_H__

#include "Base.h"
#include "Include.h"

namespace Engine
{
	class GameObject;
}

_BEGIN(Client)

class StaicObject;

class StaticObject_Mgr final : public::Engine::Base
{
public:
	_DECLARE_SINGLETON(StaticObject_Mgr)

private:
	explicit StaticObject_Mgr();
	virtual ~StaticObject_Mgr() = default;

public:
	HRESULT Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath, const TCHAR* pLayerTag);


public:
	virtual DWORD Free() override;

private:
	//vector<StaicObject*> mStaticObject_Space;
	vector<wstring> mProtoName_Space;
	//TCHAR mComponentName_Buf[MAX_PATH] = L"";
	TCHAR mProtoName_Buf[MAX_PATH] = L"";
};

_END
#endif

