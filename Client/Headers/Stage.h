#ifndef STAGE_H__
#define STAGE_H__

#include "Scene.h"
#include "Include.h"

namespace Engine
{
	class Frustum;
	class KeyMgr;
}
_BEGIN(Client)

class Stage final : public Engine::Scene
{
private:
	explicit Stage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~Stage() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

public:
	HRESULT Ready_GameObjectPrototype();
	HRESULT ReadyComponent();
	HRESULT Ready_LayerCamera(const TCHAR* pLayerTag);
	HRESULT Ready_LayerBackGround(const TCHAR* pLayerTag);
	HRESULT Ready_LayerPlayer(const TCHAR* pLayerTag);
	HRESULT Ready_LayerEnemy(const TCHAR* pLayerTag);
	HRESULT Ready_Layer_SoulValley(const TCHAR* pLayerTag);
	HRESULT Ready_LightInfo();
	HRESULT Ready_LayerEffect(const TCHAR* pLayerTag);
	HRESULT Ready_LayerStaticObject(const TCHAR* pLayerTag);
	HRESULT Ready_LayerDynamicObject(const TCHAR* pLayerTag);
	HRESULT Ready_LayerTrail(const TCHAR* pLayerTag);
	HRESULT Ready_LayerDecal(const TCHAR* pLayerTag);
	HRESULT Ready_LayerParticle(const TCHAR* pLayerTag);

public:
	static Stage* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual DWORD Free();

private:
	Engine::Frustum* m_pFrustum = nullptr;
	vector<wstring> mProtoName_Space;

private:
	BulletMgr* m_pBulletMgr = nullptr;
	UIMgr* m_pUIMgr = nullptr;
	EffectMgr* m_pEffectMgr = nullptr;
	ParticleMgr* m_pParticleMgr = nullptr;
	SoundMgr* m_pSoundMgr = nullptr;
	Engine::LightManager* m_pLightMgr = nullptr;
	Engine::KeyMgr* m_pKeyMgr = nullptr;

};

_END

#endif // STAGE_H__ 

