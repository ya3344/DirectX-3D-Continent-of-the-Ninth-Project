#ifndef LIGHT_H__
#define LIGHT_H__

#include "Base.h"

_BEGIN(Engine)

class KeyMgr;
class Light final : public Base
{
private:
	explicit Light(LPDIRECT3DDEVICE9 pDevice);
	virtual ~Light() = default;

public:
	HRESULT Init(const LIGHT_INFO& lgihtInfo);
	void Render(LPD3DXEFFECT pEffect);
	void BackBuffer_Render();

public:
	LIGHT_INFO& GetLightInfo() { return mLightInfo; }

private:
	float CalCameraDistance();

public:
	static Light* Create(LPDIRECT3DDEVICE9 pDevice, const LIGHT_INFO& lgihtInfo);
	virtual DWORD Free() override;

private:
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
	LIGHT_INFO mLightInfo;

private: // Light Optimization Related Variable
	enum OPTIMIZATION
	{
		LIGHT_OPTIMIZATION_RANGE = 80,
	};
	D3DXVECTOR3 m_vCamPosition = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vDistance = D3DXVECTOR3(0.f, 0.f, 0.f);
	float mDistance = 0.f;

private:
	KeyMgr* m_pKeyMgr = nullptr;

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9 m_pIB = nullptr;
};

_END

#endif

