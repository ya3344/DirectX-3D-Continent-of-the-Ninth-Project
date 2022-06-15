#include "stdafx.h"
#include "BackLogo.h"
#include "ComponentMgr.h"
#include <process.h>

_USING(Client);

BackLogo::BackLogo(LPDIRECT3DDEVICE9 pDevice)
	: GameObject(pDevice)
{

}

BackLogo::BackLogo(const BackLogo & rhs)
	: GameObject(rhs)
	, mComponentName_Space(rhs.mComponentName_Space)
	, m_pUIMgr(rhs.m_pUIMgr)
{
	
}

HRESULT BackLogo::Init()
{
	// 이 객체에 필요한 컴포넌트를 추가하자.
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	return NOERROR;
}


UPDATE BackLogo::Update(const float timeDelta)
{
	m_pUIMgr->Update(timeDelta);
	return UPDATE();
}

UPDATE BackLogo::LastUpdate(const float timeDelta)
{
	m_pUIMgr->LastUpdate(timeDelta);

	return UPDATE();
}

void BackLogo::Render()
{

}

HRESULT BackLogo::ReadyPrototype()
{
	// 실제 원형 객체를 생성하기 위한 작업을 수행한다.
	// ex> 파일입출력을 통한 데이터 셋
	m_pUIMgr = UIMgr::GetInstance();

	Engine::Component* pComponent = nullptr;

	{
		// For.Com_UI_Shader
		pComponent = Engine::Shader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_UI.fx");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SHADER, L"Component_Shader_UI", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_LoadingScene
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/LoadingScene/Ice_Loading_Base.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_Logo_Ice_Loading_Base", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Loading_Bar
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/LoadingScene/Loading_Bar.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_Logo_Loading_Bar", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Loading_Gage
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/LoadingScene/Loading_Gage.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_Logo_Loading_Gage", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Loading_Char
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/LoadingScene/Loading_Char.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_Logo_Loading_Char", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Loading_Effect
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/LoadingScene/Loading_Effect.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_Logo_Loading_Effect", pComponent)))
			return E_FAIL;
	}

	if (FAILED(m_pUIMgr->Ready_UIPrototype(m_pDevice)))
		return E_FAIL;

	m_pUIMgr->LogoUI_Create();

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, this, 0, nullptr);

	return NOERROR;
}



HRESULT BackLogo::ReadyComponent()
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Transform
	pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Transform", pComponent)))
		return E_FAIL;

	// For.Com_Renderer
	pComponent = m_pRenderer = (Engine::Renderer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Renderer");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Renderer", pComponent)))
		return E_FAIL;

	// For.Com_RetTexture
	pComponent = m_pRectTex = (Engine::RectTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_RectTex");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_RectTex", pComponent)))
		return E_FAIL;


	return NOERROR;
}

HRESULT BackLogo::PlayerLoad()
{
	Engine::Component* pComponent = nullptr;

	{
		// For.Component_Mesh_Player	
		pComponent = Engine::DynamicMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/DynamicMesh/Player_Mistic/", L"Mistic.x");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_PLAYER, L"Component_Mesh_Player", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Mesh_MisticWeapon
		pComponent = Engine::StaticMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/StaticMesh/MisticWeapon/", L"MisticWeapon.x");
		if (nullptr == pComponent)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_PLAYER, L"Component_Mesh_MisticWeapon", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Bullet
		pComponent = Engine::StaticMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/StaticMesh/FireCircle/", L"FireCircle.X");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_BULLET, L"Component_FireCritical", pComponent)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT BackLogo::DynamicObject_Load()
{
	Engine::Component* pComponent = nullptr;

	{
		// For.Component_Mesh_Gate	
		pComponent = Engine::DynamicMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/DynamicMesh/Gate/", L"Gate_A.x");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_DYNAMIC_MESH, L"Component_Mesh_Gate", pComponent)))
			return E_FAIL;
	}
	
	{
		// For.Component_Mesh_IceTwister
		pComponent = Engine::DynamicMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/DynamicMesh/IceTwister/", L"IceTwister.x");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_DYNAMIC_MESH, L"Component_Mesh_IceTwister", pComponent)))
			return E_FAIL;
	}


	{
		// For.Component_Mesh_SnowBall	
		pComponent = Engine::DynamicMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/DynamicMesh/SnowBall/", L"SnowBall.x");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_DYNAMIC_MESH, L"Component_Mesh_SnowBall", pComponent)))
			return E_FAIL;
	}


	return NOERROR;
}

HRESULT BackLogo::StaticObject_Load()
{
	Engine::Component* pComponent = nullptr;

	{
		// For.Component_Mesh_Cristal
		pComponent = Engine::StaticMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/StaticMesh/Object/Cristal_1/", L"Cristal_1.x");
		if (nullptr == pComponent)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC_MESH, L"Component_Mesh_Cristal_1", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Mesh_Cristal2
		pComponent = Engine::StaticMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/StaticMesh/Object/Cristal_2/", L"Cristal_2.x");
		if (nullptr == pComponent)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC_MESH, L"Component_Mesh_Cristal_2", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Mesh_Ice_Rock_1
		pComponent = Engine::StaticMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/StaticMesh/Object/Ice_Rock_1/", L"Ice_Rock_1.x");
		if (nullptr == pComponent)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC_MESH, L"Component_Mesh_Ice_Rock_1", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Mesh_Lamp_Bone_A
		pComponent = Engine::StaticMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/StaticMesh/Object/Lamp_Bone_A/", L"Lamp_Bone_A.x");
		if (nullptr == pComponent)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC_MESH, L"Component_Mesh_Lamp_Bone_A", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Mesh_Lamp_Bone_B
		pComponent = Engine::StaticMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/StaticMesh/Object/Lamp_Bone_B/", L"Lamp_Bone_B.x");
		if (nullptr == pComponent)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC_MESH, L"Component_Mesh_Lamp_Bone_B", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Mesh_Lamp_Bone_C
		pComponent = Engine::StaticMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/StaticMesh/Object/Lamp_Bone_C/", L"Lamp_Bone_C.x");
		if (nullptr == pComponent)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC_MESH, L"Component_Mesh_Lamp_Bone_C", pComponent)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT BackLogo::EnemyLoad()
{
	Engine::Component* pComponent = nullptr;

	{
		// For.Component_Mesh_Babegazi_Warrior
		pComponent = Engine::DynamicMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/DynamicMesh/Babegazi_Warrior/", L"Babegazi_Warrior.x");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SOULVALLEY_ENEMY, L"Component_Mesh_Babegazi_Warrior", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Mesh_Babegazi_Axe
		pComponent = Engine::DynamicMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/DynamicMesh/Babegazi_Axe/", L"Babegazi_Axe.x");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SOULVALLEY_ENEMY, L"Component_Mesh_Babegazi_Axe", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Mesh_Babegazi_Bow
		pComponent = Engine::DynamicMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/DynamicMesh/Babegazi_Bow/", L"Babegazi_Bow.x");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SOULVALLEY_ENEMY, L"Component_Mesh_Babegazi_Bow", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Mesh_Knole_Commander
		pComponent = Engine::DynamicMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/DynamicMesh/Knole_Commander/", L"Knole_Commander.x");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SOULVALLEY_ENEMY, L"Component_Mesh_Knole_Commander", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Mesh_Knole_Warrior
		pComponent = Engine::DynamicMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/DynamicMesh/Knole_Warrior/", L"Knole_Warrior.x");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SOULVALLEY_ENEMY, L"Component_Mesh_Knole_Warrior", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Mesh_HanumanBoss
		pComponent = Engine::DynamicMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/DynamicMesh/HanumanBoss/", L"Hanuman.x");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SOULVALLEY_ENEMY, L"Component_Mesh_HumanBoss", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Arrow
		pComponent = Engine::StaticMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/StaticMesh/Arrow/", L"Arrow.X");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_BULLET, L"Component_Arrow", pComponent)))
			return E_FAIL;
	}

	{
		//For.Component_Ice_Missile_1
		pComponent = Engine::StaticMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/StaticMesh/Ice_Missile_1/", L"Ice_Missile_1.X");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_BULLET, L"Component_Ice_Missile_1", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Ice_Missile_2
		pComponent = Engine::StaticMesh::Create(m_pDevice, L"../Bin/Resources/Meshes/StaticMesh/Ice_Missile_2/", L"Ice_Missile_2.X");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_BULLET, L"Component_Ice_Missile_2", pComponent)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT BackLogo::NumberIngUI_Load()
{
	Engine::Component* pComponent = nullptr;

	{
		// For.Component_Dam_Numbering
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/Numbering/Dam_Num%d.png", 11);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_Numbering_Damage", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Combo_Numbering
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/Numbering/Combo_Num%d.png", 10);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_Numbering_Combo", pComponent)))
			return E_FAIL;

		// For.Component_Combo_Char
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/Numbering/Combo_Char.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_Combo_Char", pComponent)))
			return E_FAIL;

		// For.Component_Combo_Effect
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/Numbering/Combo_Effect.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_Combo_Effect", pComponent)))
			return E_FAIL;
	}

	return NOERROR;
}


HRESULT BackLogo::MainUI_Load()
{
	Engine::Component* pComponent = nullptr;

	{
		// For.Component_UI_Main_Base
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/Main/Main_Base.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_Main_Base", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_UI_Exp_Gage
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/Main/Exp_Gage.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_Main_Exp_Gage", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_LevelUp
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/PlayerState/LevelUp.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_PlayerState_LevelUp", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_LevelUpBack
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/PlayerState/LevelUp_Back.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_PlayerState_LevelUp_Back", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Shaman_State_Base
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/PlayerState/Shaman_State_Base.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_PlayerState_Shaman_State_Base", pComponent)))
			return E_FAIL;
	}


	{
		// For.Component_HP_Gage
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/PlayerState/HP_Gage.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_PlayerState_HP_Gage", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_MP_Gage
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/PlayerState/MP_Gage.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_PlayerState_MP_Gage", pComponent)))
			return E_FAIL;
	}


	{
		// For.Component_Class_Mark
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/PlayerState/Class_Mark.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_PlayerState_Class_Mark", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Class_W_0
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/PlayerState/Class_W_0.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_PlayerState_Class_W_0", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_HitEffect
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/PlayerState/HitDir.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_PlayerState_HitEffect", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_BossBar
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/Boss/BossBar.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_BossBar", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_BossBar2
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/Boss/BossBar2.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_BossBar2", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_BossGage
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/Boss/BossGage.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_BossGage", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_BossTex
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/Boss/BossTex.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_BossTex", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_BossName
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/UI/Boss/BossName.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_UI, L"Component_UI_BossName", pComponent)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT BackLogo::TrailLoad()
{
	Engine::Component* pComponent = nullptr;

	// For.Component_Shader_Trail
	pComponent = Engine::Shader::Create(m_pDevice, L"../Bin/ShaderFiles/ShaderTrail.fx");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SHADER, L"Component_Shader_Trail", pComponent)))
		return E_FAIL;


	// For.Component_Trail
	pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/Textures/Trail/Trail%d.dds", 32);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_TRAIL, L"Component_Trail", pComponent)))
		return E_FAIL;


	return NOERROR;
}


HRESULT BackLogo::DecalLoad()
{
	Engine::Component* pComponent = nullptr;

	// For.Component_Shader_Decal
	pComponent = Engine::Shader::Create(m_pDevice, L"../Bin/ShaderFiles/ShaderDecal.fx");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SHADER, L"Component_Shader_Decal", pComponent)))
		return E_FAIL;


	// For.Component_Combo_Decal
	pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/Textures/Decal/Decal%d.dds", 17);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_EFFECT, L"Component_Decal", pComponent)))
		return E_FAIL;

	return NOERROR;
}

HRESULT BackLogo::EffectLoad()
{
	Engine::Component* pComponent = nullptr;

	// For.Component_Shader_Effect
	pComponent = Engine::Shader::Create(m_pDevice, L"../Bin/ShaderFiles/ShaderEffect.fx");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SHADER, L"Component_Shader_Effect", pComponent)))
		return E_FAIL;

	// For.Component_Shader_Particle
	pComponent = Engine::Shader::Create(m_pDevice, L"../Bin/ShaderFiles/ShaderParticle.fx");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SHADER, L"Component_Shader_Particle", pComponent)))
		return E_FAIL;


	// For.Component_Fire
	pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/Sprite/Fire.dds");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_EFFECT, L"Component_Effect_Fire", pComponent)))
		return E_FAIL;

	// For.Component_BlueFire
	pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/Sprite/BlueFire.dds");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_EFFECT, L"Component_Effect_BlueFire", pComponent)))
		return E_FAIL;

	// For.Component_Explosion
	pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/Sprite/Explosion.dds");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_EFFECT, L"Component_Effect_Explosion", pComponent)))
		return E_FAIL;

	// For.Component_Bullet
	pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/Sprite/Bullet.dds");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_EFFECT, L"Component_Effect_Bullet", pComponent)))
		return E_FAIL;

	// For.Component_SniperBullet
	pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/Sprite/SniperBullet.dds");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_EFFECT, L"Component_Effect_SniperBullet", pComponent)))
		return E_FAIL;

	// For.Component_PlayerMove
	pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/Sprite/PlayerMove.dds");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_EFFECT, L"Component_Effect_PlayerMove", pComponent)))
		return E_FAIL;

	// For.Component_Blood
	pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/Sprite/Blood.dds");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_EFFECT, L"Component_Effect_Blood", pComponent)))
		return E_FAIL;

	// For.Component_PurpleBlood
	pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/Sprite/PurpleBlood.dds");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_EFFECT, L"Component_Effect_PurpleBlood", pComponent)))
		return E_FAIL;

	// For.Component_BlueBlood
	pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/Sprite/BlueBlood.dds");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_EFFECT, L"Component_Effect_BlueBlood", pComponent)))
		return E_FAIL;

	// For.Component_Snow
	pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::TEXTURE_TYPE::GENERAL, L"../Bin/Resources/Particle/Snow.png");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_EFFECT, L"Component_Paritcle_Snow", pComponent)))
		return E_FAIL;


	return NOERROR;
}

unsigned int BackLogo::ThreadFunc(void * pArg)
{
	Engine::Component* pComponent = nullptr;
	BackLogo* pBackLogo = (BackLogo*)pArg;

	
	{
		// For.Component_Shader_Mesh
		pComponent = Engine::Shader::Create(pBackLogo->m_pDevice, L"../Bin/ShaderFiles/ShaderDeferred.fx");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SHADER, L"Component_Shader_Mesh", pComponent)))
			return E_FAIL;
		g_fLoading = 0.05f;
	}


	{
		// For.Component_Shader_Sky
		pComponent = Engine::Shader::Create(pBackLogo->m_pDevice, L"../Bin/ShaderFiles/ShaderSky.fx");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SHADER, L"Component_Shader_Sky", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Mesh_SoulValley
		TCHAR componentNameBuf[255] = L"";
		TCHAR nameBuf[255] = L"";

		for (size_t i = 0; i <= SOUL_VALLEY_MAX; i++)
		{
			wsprintf(nameBuf, L"SoulValley_%d.X", i);
			wsprintf(componentNameBuf, L"Component_Mesh_Terrain%d", i);
			pBackLogo->mComponentName_Space.emplace_back(componentNameBuf);

			pComponent = Engine::StaticMesh::Create(pBackLogo->m_pDevice, L"../Bin/Resources/Meshes/StaticMesh/Terrain/SoulValley/", nameBuf);
			if (nullptr == pComponent)
				return E_FAIL;

			if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_SOULVALLEY, pBackLogo->mComponentName_Space[i].c_str(), pComponent)))
				return E_FAIL;
		}
		g_fLoading = 0.2f;
	}

#ifdef PLAYER_OPERATION
	if (FAILED(pBackLogo->PlayerLoad()))
		return E_FAIL;
	g_fLoading = 0.3f;
#endif

#ifdef STATIC_OBJECT_OPERATION
	if (FAILED(pBackLogo->StaticObject_Load()))
		return E_FAIL;
	g_fLoading = 0.4f;
#endif


#ifdef ENEMY_OPERATION
	if (FAILED(pBackLogo->EnemyLoad()))
		return E_FAIL;
	g_fLoading = 0.6f;
#endif

#ifdef DYNAMIC_OBJECT_OPERATION
	if (FAILED(pBackLogo->DynamicObject_Load()))
		return E_FAIL;
	g_fLoading = 0.7f;
#endif




#ifdef MAIN_UI_OPERATION
	if (FAILED(pBackLogo->MainUI_Load()))
		return E_FAIL;
#endif

	if (FAILED(pBackLogo->NumberIngUI_Load()))
		return E_FAIL;
	g_fLoading = 0.8f;

	if (FAILED(pBackLogo->TrailLoad()))
		return E_FAIL;

	if (FAILED(pBackLogo->DecalLoad()))
		return E_FAIL;
	g_fLoading = 0.9f;

	if (FAILED(pBackLogo->EffectLoad()))
		return E_FAIL;

	g_fLoading = 1.f;
	g_bLoading = true;

	return 0;
}



BackLogo * BackLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	BackLogo* pInstance = new BackLogo(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Back_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * BackLogo::Clone()
{
	BackLogo* pInstance = new BackLogo(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Back_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD BackLogo::Free()
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pRectTex);
	Engine::Safe_Release(m_pShader);
	Engine::Safe_Release(m_pTexture);

	Engine::GameObject::Free();

	return 0;
}
