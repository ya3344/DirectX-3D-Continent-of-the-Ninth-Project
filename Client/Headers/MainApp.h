#ifndef MAINAPP_H__
#define MAINAPP_H__

#include "Base.h"
#include "Include.h"

namespace Engine
{
	class GraphicDevice;
	class Management;
	class Renderer;
}

_BEGIN(Client)


class MainApp final : public Engine::Base
{
private:
	explicit MainApp();
	virtual ~MainApp() = default;

public:
	HRESULT Init();
	UPDATE Update(const float timeDelta);
	void Render();

private:
	HRESULT DefaultSetting(WIN_MODE mode, const WORD backSizeX, const WORD backSizeY);
	HRESULT Ready_ComponentStatic();
	HRESULT SetUp_StartScene(SCENE sceneType);

public:
	static MainApp* Create();
	void Pre_ReleaseSingleton();
	void Late_ReleaseSingleton();
	void ReleaseSingleton();
	virtual DWORD Free() override;

private:
	Engine::GraphicDevice* m_pGraphicDevice = nullptr;
	Engine::Management* m_pManagement = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
	UtilityMgr* m_pUtilityMgr = nullptr;
};

_END

#endif // MAINAPP_H__

