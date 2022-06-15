
#ifndef TARGET_H__
#define TARGET_H__

#include "Base.h"

_BEGIN(Engine)

class Target final : public Base
{
private:
	explicit Target(LPDIRECT3DDEVICE9 pDevice);
	virtual ~Target() = default;

public:
	HRESULT Init(const size_t width, const size_t height, D3DFORMAT format, D3DXCOLOR color);
	HRESULT ClearTarget();
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName);
	HRESULT SetUp_RenderTarget(const size_t index);
	HRESULT Release_RenderTarget(const size_t index);

public:
	HRESULT Ready_BufferDebug(const float startX, const float startY, const float sizeX, const float sizeY);
	void Render_BufferDebug();

public:
	static Target* Create(LPDIRECT3DDEVICE9 pDevice, const size_t width, const size_t height, D3DFORMAT format, D3DXCOLOR color);
	virtual DWORD Free() override;

private:
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
	LPDIRECT3DTEXTURE9 m_pTargetTexture = nullptr;
	LPDIRECT3DSURFACE9 m_pTargetSurface = nullptr;
	LPDIRECT3DSURFACE9 m_pOldSurface = nullptr;

private:
	D3DXCOLOR mClearColor;

private:
	LPDIRECT3DVERTEXBUFFER9	m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9 m_pIB = nullptr;

};

_END
#endif