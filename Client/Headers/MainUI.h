#ifndef MAIN_UI_H__
#define MAIN_UI_H__

#include "UI.h"

_BEGIN(Client)

class MainUI final : public UI
{
private:
	explicit MainUI(LPDIRECT3DDEVICE9 pDevice);
	explicit MainUI(const MainUI& rhs);
	virtual ~MainUI() = default;

public:
	void MainUI_Init();

public:
	virtual UPDATE Update(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect) override;

private:
	void GauageUpdate(float gauageState);

public:
	static MainUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject * MainUI_Clone(const TCHAR* pTextureTag, const UI_INFO& UI_Info) override;
	virtual DWORD Free(void) override;


private:
	float mPrePlayerHP = -1.f;
};

_END


#endif
