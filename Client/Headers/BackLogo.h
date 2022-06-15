#ifndef BACKLOGO_H__
#define BACKLOGO_H__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class RectTex;
	class Shader;
	class Texture;
}

_BEGIN(Client)

#define PLAYER_OPERATION
#define STATIC_OBJECT_OPERATION
#define DYNAMIC_OBJECT_OPERATION
#define ENEMY_OPERATION
#define MAIN_UI_OPERATION
#define COMBO_UI_OPERATION
//#define TRAIL_NOT_OPERATION

class BackLogo final : public Engine::GameObject
{
private:
	explicit BackLogo(LPDIRECT3DDEVICE9 pDevice);
	explicit BackLogo(const BackLogo& rhs);
	virtual ~BackLogo() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT ReadyPrototype() override;

private:
	HRESULT ReadyComponent();

private:
	HRESULT PlayerLoad();
	HRESULT DynamicObject_Load();
	HRESULT StaticObject_Load();
	HRESULT EnemyLoad();
	HRESULT MainUI_Load();
	HRESULT NumberIngUI_Load();
	HRESULT TrailLoad();
	HRESULT DecalLoad();
	HRESULT EffectLoad();

public:
	static unsigned int _stdcall ThreadFunc(void* pArg);

public:
	static BackLogo* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Engine::GameObject* Clone() override;
	virtual DWORD Free() override;

private:
	vector<wstring> mComponentName_Space;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::RectTex* m_pRectTex = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::Texture* m_pTexture = nullptr;

private:
	UIMgr* m_pUIMgr = nullptr;

private:
	bool m_bNotRender = false;
	HANDLE m_hThread;
	LPD3DXEFFECT m_pEffect = nullptr;
	D3DXMATRIX m_matWorld;
};

_END

#endif //BACKLOGO_H__
