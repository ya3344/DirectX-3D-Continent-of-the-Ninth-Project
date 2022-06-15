#include "stdafx.h"
#include "Stage.h"
#include "ComponentMgr.h"
#include "Terrain.h"
#include "CameraFree.h"
#include "CamearFPS.h"
#include "Player.h"
#include "Enemy.h"
#include "SkyBox.h"
#include "Stone.h"
#include "MisticWeapon.h"
#include "Effect.h"
#include "Obstacle.h"
#include "BackLogo.h"
#include "Trail.h"
#include "BossDecal.h"
#include "PlayerDecal.h"
#include "Particle.h"

_USING(Client)

Stage::Stage(LPDIRECT3DDEVICE9 pDevice)
	: Scene(pDevice)
	, m_pBulletMgr(BulletMgr::GetInstance())
	, m_pLightMgr(Engine::LightManager::GetInstance())
	, m_pKeyMgr(Engine::KeyMgr::GetInstance())
	, m_pUIMgr(UIMgr::GetInstance())
	, m_pEffectMgr(EffectMgr::GetInstance())
	, m_pParticleMgr(ParticleMgr::GetInstance())
	, m_pSoundMgr(SoundMgr::GetInstance())
{
	m_pLightMgr->AddRef();
	//m_pBulletMgr->AddRef();
	m_pKeyMgr->AddRef();
	//m_pUIMgr->AddRef();
	
}

HRESULT Stage::Init()
{
	// 현재 씬에 필요한 원형객체들을 생성하자.
	if (FAILED(Ready_GameObjectPrototype()))
		return E_FAIL;

	if (FAILED(ReadyComponent()))
		return E_FAIL;

	// Stage 씬에서 필요한 객체와 컴포넌트들을 생성하자.
	if (FAILED(Ready_LightInfo()))
		return E_FAIL;


	Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_GameObject_CameraFree", SCENE_STAGE, L"Layer_Camera");

	
	if (FAILED(Ready_LayerDecal(L"Layer_Decal")))
		return E_FAIL;

#ifdef PLAYER_OPERATION
	if (FAILED(Ready_LayerTrail(L"Layer_Trail")))
		return E_FAIL;
	if (FAILED(Ready_LayerPlayer(L"Layer_Player")))
		return E_FAIL;
	if (FAILED(m_pBulletMgr->Ready_BulletPrototype(m_pDevice)))
		return E_FAIL;
	Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_GameObject_CameraFPS", SCENE_STAGE, L"Layer_Camera");
#endif

#ifdef ENEMY_OPERATION
	if (FAILED(Ready_LayerEnemy(L"Layer_Enemy")))
		return E_FAIL;
	if (FAILED(m_pBulletMgr->Ready_ArrowPrototype(m_pDevice)))
		return E_FAIL;
	if (FAILED(m_pBulletMgr->Ready_BossPrototype(m_pDevice)))
		return E_FAIL;
#endif





	/*if (FAILED(Ready_LayerCamera(L"Layer_Camera")))
		return E_FAIL;*/

	if (FAILED(Ready_LayerBackGround(L"Layer_BackGround")))
		return E_FAIL;

	if (FAILED(Ready_Layer_SoulValley(L"Layer_SoulValley")))
		return E_FAIL;


#ifdef STATIC_OBJECT_OPERATION
	if (FAILED(Ready_LayerStaticObject(L"Layer_StaticObject")))
		return E_FAIL;
#endif
	
#ifdef DYNAMIC_OBJECT_OPERATION
	if (FAILED(Ready_LayerDynamicObject(L"Layer_DynamicObject")))
		return E_FAIL;
#endif

	//if (FAILED(m_pUIMgr->Ready_UIPrototype(m_pDevice)))
	//	return E_FAIL;

#ifdef MAIN_UI_OPERATION
	if (FAILED(m_pUIMgr->MainUI_Create()))
		return E_FAIL;
	m_pUIMgr->BossUI_Create();
#endif

#ifdef COMBO_UI_OPERATION
	if (FAILED(m_pUIMgr->ComboUI_Create()))
		return E_FAIL;
#endif
	
	if (FAILED(m_pEffectMgr->Ready_EffectPrototype(m_pDevice)))
		return E_FAIL;

	//Ready_LayerParticle(L"Laye_Particle");

	if (FAILED(m_pParticleMgr->Ready_ParticlePrototype(m_pDevice)))
		return E_FAIL;

	return NOERROR;
}

UPDATE Stage::Update(const float timeDelta)
{

#ifdef _DEBUG
	if (m_pKeyMgr->KeyDown(DIK_5))
	{
		Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_GameObject_CameraFPS", SCENE_STAGE, L"Layer_Camera");
	}
	else if (m_pKeyMgr->KeyDown(DIK_6))
	{
		Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_GameObject_CameraFree", SCENE_STAGE, L"Layer_Camera");
	}
#endif

	m_pSoundMgr->Update();
	//if (1 != m_pSoundMgr->IsPlayCheck(CHANNEL_BGM))
	//{
	//	m_pSoundMgr->PlayBGM(L"Stage1BGM.wav", CHANNEL_BGM);
	//	m_pSoundMgr->PlayBGM(L"BGM_Act4.mp3", CHANNEL_BGM);
	//	//m_pSoundMgr->SetVolume()
	//}

	m_pBulletMgr->Update(timeDelta);
	m_pUIMgr->Update(timeDelta);
	m_pEffectMgr->Update(timeDelta);
	m_pParticleMgr->Update(timeDelta);

	return Scene::Update(timeDelta);
}

UPDATE Stage::LastUpdate(const float timeDelta)
{
	if (m_pFrustum == nullptr)
		return UPDATE::FAIL;

	m_pFrustum->Transform_WorldSpace();
	m_pBulletMgr->LastUpdate(timeDelta);
	m_pUIMgr->LastUpdate(timeDelta);
	m_pEffectMgr->LastUpdate(timeDelta);
	m_pParticleMgr->LastUpdate(timeDelta);

	return Scene::LastUpdate(timeDelta);
}

void Stage::Render()
{
}

HRESULT Stage::Ready_GameObjectPrototype()
{
	Engine::GameObject* pGameObject = nullptr;
	
	// For.Prototype_GameObject_CameraFree
	pGameObject = CameraFree::Create(m_pDevice, D3DXVECTOR3(40.f, 20.f, -15.f), D3DXVECTOR3(40.f, 0.f, 20.f), D3DXVECTOR3(0.f, 1.f, 0.f));
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_CameraFree", SCENE_STAGE, pGameObject)))
		return E_FAIL;

	// For.Prototype_GameObject_CameraFPS
	pGameObject = CameraFPS::Create(m_pDevice, D3DXVECTOR3(40.f, 20.f, -15.f), D3DXVECTOR3(40.f, 0.f, 20.f), D3DXVECTOR3(0.f, 1.f, 0.f));
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_CameraFPS", SCENE_STAGE, pGameObject)))
		return E_FAIL;

	// For.Prototype_GameObject_Player
	pGameObject = Player::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	
	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_Player", SCENE_PLAYER, pGameObject)))
		return E_FAIL;

	// For.Prototype_GameObject_MisticWeapon
	pGameObject = MisticWeapon::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_MisticWeapon", SCENE_PLAYER, pGameObject)))
		return E_FAIL;


	// DH_SkyBoxAdd_180613 For.Prototype_GameObject_SkyBox
	pGameObject = SkyBox::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_SkyBox", SCENE_STAGE, pGameObject)))
		return E_FAIL;


	return NOERROR;
}

HRESULT Stage::ReadyComponent()
{
	Engine::Component* pComponent = nullptr;

	// For.Component_Frustum
	m_pFrustum = Engine::Frustum::Create(m_pDevice);
	if (m_pFrustum == nullptr)
		return E_FAIL;
	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_Frustum", m_pFrustum)))
		return E_FAIL;
	m_pFrustum->AddRef();

	// For.Component_Collider_Box
	pComponent = Engine::Collider::Create(m_pDevice, Engine::Collider::TYPE_BOX);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_Collider_Box", pComponent)))
		return E_FAIL;

	// For.Component_Collider_Sphere
	pComponent = Engine::Collider::Create(m_pDevice, Engine::Collider::TYPE_SPHERE);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_Collider_Sphere", pComponent)))
		return E_FAIL;

	// For.Component_Navigation
	pComponent = Engine::Navigation::Create(m_pDevice, L"../Bin/Data/NaviFiles/Navigation.dat");
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_Navigation", pComponent)))
		return E_FAIL;



	return NOERROR;
}

HRESULT Stage::Ready_LayerCamera(const TCHAR * pLayerTag)
{
	// For.Camera 
	//if (FAILED(Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_GameObject_CameraFree", SCENE_STAGE, pLayerTag)))
	//	return E_FAIL;

	if (FAILED(Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_GameObject_CameraFPS", SCENE_STAGE, pLayerTag)))
		return E_FAIL;


	return NOERROR;
}

HRESULT Stage::Ready_LayerBackGround(const TCHAR * pLayerTag)
{
	// DH_SkyBoxAdd_180613 For.SkyBox
	if (FAILED(Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_GameObject_SkyBox", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	//if (FAILED(Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_GameObject_Stone", SCENE_STAGE, pLayerTag)))
	//	return E_FAIL;


	return NOERROR;
}

HRESULT Stage::Ready_LayerPlayer(const TCHAR * pLayerTag)
{
	// For.Player
	if (FAILED(Add_GameObjectToLayer(SCENE_PLAYER, L"Prototype_GameObject_Player", SCENE_PLAYER, pLayerTag)))
		return E_FAIL;

	// For.MisticWeapon
	if (FAILED(Add_GameObjectToLayer(SCENE_PLAYER, L"Prototype_GameObject_MisticWeapon", SCENE_PLAYER, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Stage::Ready_LayerEnemy(const TCHAR * pLayerTag)
{
	// For.Enemy
	EnemyMgr::GetInstance()->CrateEnemy(m_pDevice, pLayerTag);

	return NOERROR;
}

HRESULT Stage::Ready_Layer_SoulValley(const TCHAR * pLayerTag)
{
	Engine::GameObject* pGameObject = nullptr;
	TCHAR componentName_Buf[MAX_PATH] = L"";
	TCHAR protoName_Buf[MAX_PATH] = L"";

	for (size_t i = 0; i <= SOUL_VALLEY_MAX; i++)
	{
		wsprintf(componentName_Buf, L"Component_Mesh_Terrain%d", i);
		wsprintf(protoName_Buf, L"Prototype_GameObject_Terrain%d", i);
		mProtoName_Space.emplace_back(protoName_Buf);

		pGameObject = Terrain::Create(m_pDevice, componentName_Buf);
		if (pGameObject == nullptr)
			return E_FAIL;

		if (FAILED(Add_GameObjectPrototype(mProtoName_Space[i].c_str(), SCENE_SOULVALLEY, pGameObject)))
			return E_FAIL;

		if (FAILED(Add_GameObjectToLayer(SCENE_SOULVALLEY, mProtoName_Space[i].c_str(), SCENE_SOULVALLEY, pLayerTag)))
			return E_FAIL;
	}
	 
	return NOERROR;
}

HRESULT Stage::Ready_LightInfo()
{
	//m_pLightMgr->CreateLight(m_pDevice, L"../Bin/Data/LightFiles/PointLightEmpty.dat");
	m_pLightMgr->CreateLight(m_pDevice, L"../Bin/Data/LightFiles/PointLight.dat");
	

	return NOERROR;
}

HRESULT Stage::Ready_LayerEffect(const TCHAR * pLayerTag)
{
	for (size_t i = 0; i < 50; ++i)
	{
		// For.Effect
		if (FAILED(Add_GameObjectToLayer(SCENE_TRAIL, L"Prototype_GameObject_Effect", SCENE_EFFECT, pLayerTag)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT Stage::Ready_LayerStaticObject(const TCHAR * pLayerTag)
{
	StaticObject_Mgr::GetInstance()->Create(m_pDevice, L"../Bin/Data/ObjectFiles/Object.dat", pLayerTag);
	return NOERROR;
}

HRESULT Stage::Ready_LayerDynamicObject(const TCHAR * pLayerTag)
{
	Engine::GameObject* pGameObject = nullptr;

	//Gate Load
	pGameObject = Obstacle::Create(m_pDevice, OBSTACLE_GATE, D3DXVECTOR3(27.f, -11.f, 135.f));
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_Gate", SCENE_DYNAMIC_MESH, pGameObject)))
		return E_FAIL;
	if (FAILED(Add_GameObjectToLayer(SCENE_DYNAMIC_MESH, L"Prototype_GameObject_Gate", SCENE_DYNAMIC_MESH, pLayerTag)))
		return E_FAIL;

	//IceTwister
	pGameObject = Obstacle::Create(m_pDevice, OBSTACLE_ICETWISTER1, D3DXVECTOR3(-97.f, 37.f, 379.f));
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_IceTwister", SCENE_DYNAMIC_MESH, pGameObject)))
		return E_FAIL;
	if (FAILED(Add_GameObjectToLayer(SCENE_DYNAMIC_MESH, L"Prototype_GameObject_IceTwister", SCENE_DYNAMIC_MESH, pLayerTag)))
		return E_FAIL;

	//IceTwister2
	pGameObject = Obstacle::Create(m_pDevice, OBSTACLE_ICETWISTER2, D3DXVECTOR3(-125.f, 37.f, 280.f));
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_IceTwister2", SCENE_DYNAMIC_MESH, pGameObject)))
		return E_FAIL;
	if (FAILED(Add_GameObjectToLayer(SCENE_DYNAMIC_MESH, L"Prototype_GameObject_IceTwister2", SCENE_DYNAMIC_MESH, pLayerTag)))
		return E_FAIL;

	//SnowBall1
	//pGameObject = Obstacle::Create(m_pDevice, OBSTACLE_SNOWBALL1, D3DXVECTOR3(-125.f, 37.f, 280.f));
	//if (pGameObject == nullptr)
	//	return E_FAIL;

	//if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_SnowBall1", SCENE_DYNAMIC_MESH, pGameObject)))
	//	return E_FAIL;
	//if (FAILED(Add_GameObjectToLayer(SCENE_DYNAMIC_MESH, L"Prototype_GameObject_SnowBall1", SCENE_DYNAMIC_MESH, pLayerTag)))
	//	return E_FAIL;

	//SnowBall2
	pGameObject = Obstacle::Create(m_pDevice, OBSTACLE_SNOWBALL2, D3DXVECTOR3(-125.f, 37.f, 280.f));
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_SnowBall2", SCENE_DYNAMIC_MESH, pGameObject)))
		return E_FAIL;
	if (FAILED(Add_GameObjectToLayer(SCENE_DYNAMIC_MESH, L"Prototype_GameObject_SnowBall2", SCENE_DYNAMIC_MESH, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Stage::Ready_LayerTrail(const TCHAR * pLayerTag)
{
	Engine::GameObject* pGameObject = nullptr;

	// For.Prototype_WeaponTrail
	pGameObject = Trail::Create(m_pDevice, L"Component_Player_Buffer_Trail");
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_PlayerTrail", SCENE_TRAIL, pGameObject)))
		return E_FAIL;
	if (FAILED(Add_GameObjectToLayer(SCENE_TRAIL, L"Prototype_GameObject_PlayerTrail", SCENE_TRAIL, pLayerTag)))
		return E_FAIL;

	pGameObject = Trail::Create(m_pDevice, L"Component_BossLeft_Buffer_Trail");
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_Boss_LeftTrail", SCENE_TRAIL, pGameObject)))
		return E_FAIL;
	if (FAILED(Add_GameObjectToLayer(SCENE_TRAIL, L"Prototype_GameObject_Boss_LeftTrail", SCENE_TRAIL, pLayerTag)))
		return E_FAIL;

	pGameObject = Trail::Create(m_pDevice, L"Component_BossRight_Buffer_Trail");
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_Boss_RightTrail", SCENE_TRAIL, pGameObject)))
		return E_FAIL;
	if (FAILED(Add_GameObjectToLayer(SCENE_TRAIL, L"Prototype_GameObject_Boss_RightTrail", SCENE_TRAIL, pLayerTag)))
		return E_FAIL;


	return NOERROR;
}

HRESULT Stage::Ready_LayerDecal(const TCHAR * pLayerTag)
{
	Engine::GameObject* pGameObject = nullptr;

	// For.Prototype_Boss_DecalBox
	pGameObject = BossDecal::Create(m_pDevice);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_BossDecal", SCENE_EFFECT, pGameObject)))
		return E_FAIL;
	if (FAILED(Add_GameObjectToLayer(SCENE_EFFECT, L"Prototype_GameObject_BossDecal", SCENE_EFFECT, pLayerTag)))
		return E_FAIL;

	// For.Prototype_Player_DecalBox
	pGameObject = PlayerDecal::Create(m_pDevice);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_PlayerDecal", SCENE_EFFECT, pGameObject)))
		return E_FAIL;
	if (FAILED(Add_GameObjectToLayer(SCENE_EFFECT, L"Prototype_GameObject_PlayerDecal", SCENE_EFFECT, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Stage::Ready_LayerParticle(const TCHAR * pLayerTag)
{
	Engine::GameObject* pGameObject = nullptr;

	// For.Prototype_Particle
	pGameObject = Particle::Create(m_pDevice);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_Particle", SCENE_EFFECT, pGameObject)))
		return E_FAIL;
	if (FAILED(Add_GameObjectToLayer(SCENE_EFFECT, L"Prototype_GameObject_Particle", SCENE_EFFECT, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}


Stage * Stage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Stage* pInstance = new Stage(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("StageScene Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Stage::Free()
{
	Engine::Safe_Release(m_pFrustum);
	Engine::Safe_Release(m_pLightMgr);
	Engine::Safe_Release(m_pKeyMgr);
	//Engine::Safe_Release(m_pUIMgr);
	//Engine::Safe_Release(m_pBulletMgr);
	Scene::ClearGameObject(SCENE_STAGE);
	Scene::Free();

	return DWORD();
}
