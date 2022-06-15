#ifndef NUMBER_UI_H__
#define NUMBER_UI_H__

#include "UI.h"

_BEGIN(Client)

typedef struct tagNumberInfo
{
	int positionNum_1 = 0;
	int positionNum_10 = 0;
	int positionNum_100 = 0;
	int positionNum_1000 = 0;
	int positionNum_10000 = 0;
}NUMBER_INFO;

class NumberUI final : public UI
{
private:
	explicit NumberUI(LPDIRECT3DDEVICE9 pDevice);
	explicit NumberUI(const NumberUI& rhs);
	virtual ~NumberUI() = default;

public:
	void NumberUI_Init(const D3DXMATRIX& pEnemyWorldMatrix, int damageValue);

public:
	virtual UPDATE Update(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect) override;

public:
	bool GetNumberingSate() const { return m_bNumbering;  }

private:
	void WorldToViewPort();

public:
	static NumberUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject * NumberingUI_Clone(const TCHAR * pTextureTag, const D3DXMATRIX& pEnemyWorldMatrix, const float damageValue, const UI_INFO & UI_Info) override;
	virtual DWORD Free(void) override;

private:
	void NumberingUpdate(float timeDelta);
	void CriticalUpdate(float timeDelta);

private:
	int mDamagedValue = 0;
	bool m_bNumbering = false;
	float mOffset = 0.f;
	float mTimeDelta = 0.f;
	float mWidthValue = 0.f;
	float mSize_AddValue = 0.f;
	float mHeightSpeed = 0.f;

private:
	D3DXMATRIX m_matEnemyWorld = {};
	NUMBER_INFO mNumberInfo;
};

_END


#endif
