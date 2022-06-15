#ifndef UI_MGR_H__
#define UI_MGR_H__

#include "Base.h"
#include "Include.h"

namespace Engine
{
	class GameObject;
}

_BEGIN(Client)

class UI;

class UIMgr final : public::Engine::Base
{
public:
	_DECLARE_SINGLETON(UIMgr)

private:
	explicit UIMgr();
	virtual ~UIMgr() = default;

public:
	HRESULT Ready_UIPrototype(LPDIRECT3DDEVICE9 pDevice);
	UPDATE Update(const float timeDelta);
	UPDATE LastUpdate(const float timeDelta);

public:
	void AddComboCout() { ++mComboCount; }

public:
	void NumberIngDelete();
	void LogoDelte();

public:
	HRESULT MainUI_Create();
	HRESULT ComboUI_Create();
	HRESULT BossUI_Create();
	HRESULT NumberingUI_Create(const D3DXMATRIX& pEnemyWorldMatrix, const float damageValue);
	HRESULT LogoUI_Create();

public:
	HRESULT Create_MainUI(const TCHAR* pUITag, const TCHAR* pTextureTag, const UI_INFO& UI_Info);
	HRESULT Create_BossUI(const TCHAR* pUITag, const TCHAR* pTextureTag, const UI_INFO& UI_Info);
	HRESULT Create_LogoUI(const TCHAR* pUITag, const TCHAR* pTextureTag, const UI_INFO& UI_Info, const bool bNotObject);
	HRESULT Create_ComboUI(const TCHAR* pUITag, const TCHAR* pTextureTag, const UI_INFO& UI_Info, size_t* pComboCount);
	HRESULT Create_NumberingUI(const D3DXMATRIX& pEnemyWorldMatrix, const float damageValue, const UI_INFO& UI_Info);

public:
	virtual DWORD Free() override;

private:
	list<UI*> mUISpace;
	list<UI*> mNumberingSpace;

private:
	size_t mComboCount = 0;
};

_END
#endif

