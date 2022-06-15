#ifndef LIGHT_MANAGER_H__
#define LIGHT_MANAGER_H__

#include "Base.h"

_BEGIN(Engine)

class Light;
class KeyMgr;

class _ENGINE_DLL LightManager final : public Base
{
	_DECLARE_SINGLETON(LightManager)

private:
	explicit LightManager();
	virtual ~LightManager() = default;

public:
	HRESULT AddLight(LPDIRECT3DDEVICE9 pDevice, const LIGHT_INFO& lightInfo);
	void Render(LPD3DXEFFECT pEffect);
	void BackBuffer_Rneder();
	HRESULT CreateLight(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath);
	void ChangeLightColor(const size_t specialIndex, const size_t bossHPCount = 0);

public:
	virtual DWORD Free() override;

private:
	vector<Light*> mLightSpace;
	KeyMgr* m_pKeyMgr = nullptr;
	bool m_bPointLight = true;


};

_END

#endif