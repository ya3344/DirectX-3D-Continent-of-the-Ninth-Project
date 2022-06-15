#ifndef LOGO_UI_H__
#define LOGO_UI_H__

#include "UI.h"

_BEGIN(Client)

class LogoUI final : public UI
{
private:
	explicit LogoUI(LPDIRECT3DDEVICE9 pDevice);
	explicit LogoUI(const LogoUI& rhs);
	virtual ~LogoUI() = default;

public:
	void LogoUI_Init();

public:
	virtual UPDATE Update(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect) override;

public:
	static LogoUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject * LogoUI_Clone(const TCHAR* pTextureTag, const UI_INFO& UI_Info, const bool bNotObject) override;
	virtual DWORD Free(void) override;

private:
	void GauageUpdate(float gauageState);

private:
	float mLoadingPercent = 0.f;
	bool m_bChar = false;
	float mCharAlpha = 0.f;
};

_END


#endif
