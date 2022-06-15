#ifndef UI_H__
#define UI_H__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class Shader;
	class Texture;
	class RectTex;
	class Navigation;
}


_BEGIN(Client)

class Player;

class UI : public Engine::GameObject
{
protected:
	explicit UI(LPDIRECT3DDEVICE9 pDevice);
	explicit UI(const UI& rhs);
	virtual ~UI() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) = 0;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() = 0;
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect) = 0;
	virtual HRESULT ReadyPrototype() override;

public:
	HRESULT InitUI(const TCHAR* pTextureTag, const UI_INFO& UI_Info, const bool bGet_NotObject = false);
	bool GetNotObject() const { return m_bNotObject; }

private:
	HRESULT ReadyComponent();
	

public:
	virtual DWORD Free(void) override;
	// GameObject을(를) 통해 상속됨
	virtual GameObject * MainUI_Clone(const TCHAR* pTextureTag, const UI_INFO& UI_Info);
	virtual GameObject * LogoUI_Clone(const TCHAR* pTextureTag, const UI_INFO& UI_Info, const bool bNot_Object);
	virtual GameObject * BossUI_Clone(const TCHAR* pTextureTag, const UI_INFO& UI_Info);
	virtual GameObject * ComboUI_Clone(const TCHAR* pTextureTag, const UI_INFO& UI_Info, size_t* pComboCount);
	virtual GameObject * NumberingUI_Clone(const TCHAR * pTextureTag, const D3DXMATRIX& pEnemyWorldMatrix, const float damageValue, const UI_INFO& UI_Info);
	virtual GameObject * Clone() override;


protected:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::Texture* m_pTexture = nullptr;
	Engine::RectTex* m_pRectTex = nullptr;
	Engine::Navigation* m_pPlayerNavigation = nullptr;
	SoundMgr* m_pSoundMgr = nullptr;

protected:
	Player* m_pPlayer = nullptr;
	const size_t* m_pPlayerIndex = nullptr;
	PLAYER_INFO* m_pPlayerInfo = nullptr;
	const float* m_pBossHP = nullptr;
	float mBossOriginalHP = 0.f;
	const size_t* m_pBossCount = nullptr;

protected:
	UI_INFO mUIInfo;
	const TCHAR* m_pTextureTag = nullptr;
	LPD3DXEFFECT m_pEffect = nullptr;
	bool m_bNotObject = false;

protected:
	D3DXMATRIX m_matProj;
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	float mAlphaValue = 0.f;
	float mAlphaTestValue = 0.f;
	size_t mTextureNum = 0;
	D3DXVECTOR4 m_vColor = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
	size_t mShaderIndex = 0;
};

_END

#endif

