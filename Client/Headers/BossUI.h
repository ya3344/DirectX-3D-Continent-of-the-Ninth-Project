#ifndef BOSS_UI_H__
#define BOSS_UI_H__

#include "UI.h"

_BEGIN(Client)

class BossUI final : public UI
{
private:
	explicit BossUI(LPDIRECT3DDEVICE9 pDevice);
	explicit BossUI(const BossUI& rhs);
	virtual ~BossUI() = default;

public:
	void BossUI_Init();

public:
	virtual UPDATE Update(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect) override;

public:
	static BossUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject * BossUI_Clone(const TCHAR* pTextureTag, const UI_INFO& UI_Info) override;
	virtual DWORD Free(void) override;

private:
	float mBossHP = 0.f;
	float mPreBossHP = 0.f;

private:
	TCHAR m_pFontBuf[10] = {};
	bool m_bBossUI = false;
	//D3DXMATRIX mFontMatrix;
	//LPD3DXFONT m_pFont = nullptr;
	//LPD3DXSPRITE m_pSprite = nullptr;
};

_END


#endif
