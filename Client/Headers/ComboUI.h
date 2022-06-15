#ifndef COMBO_UI_H__
#define COMBO_UI_H__

#include "UI.h"

_BEGIN(Client)

typedef struct tagComboInfo
{
	size_t positionNum_1 = 0;
	size_t positionNum_10 = 0;
	size_t positionNum_100 = 0;

}COMBO_INFO;


class ComboUI final : public UI
{
private:
	explicit ComboUI(LPDIRECT3DDEVICE9 pDevice);
	explicit ComboUI(const ComboUI& rhs);
	virtual ~ComboUI() = default;

public:
	void ComboUI_Init(const TCHAR* pTextureTag, const UI_INFO& UI_Info, size_t* pComboCount);

public:
	virtual UPDATE Update(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect) override;
	
private:
	void PositionSetting();

public:
	static ComboUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject * ComboUI_Clone(const TCHAR* pTextureTag, const UI_INFO& UI_Info, size_t* pComboCount) override;
	virtual DWORD Free(void) override;



private:
	bool m_bCombo = false;
	float mOffset = 0.f;
	
	float mWidthValue = 0.f;
	COMBO_INFO mComboInfo;

private:
	size_t* m_pComboCount = nullptr;
	size_t mComboCount = 0;
	size_t mPreComboCount = 0;
	float mComboTime = 0.f;
	float mComboDuration = 0.f;
	float mSizeValue = 0.f;
};

_END


#endif
