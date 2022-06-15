#include "stdafx.h"
#include "Obstacle.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Layer.h"
#include "PlayerAnim.h"
#include "CamearFPS.h"


_USING(Client)

Obstacle::Obstacle(LPDIRECT3DDEVICE9 pDevice)
	: Engine::GameObject(pDevice)
	, m_pUtilityMgr(UtilityMgr::GetInstance())
{
	m_pUtilityMgr->AddRef();
}

Obstacle::Obstacle(const Obstacle & rhs)
	: Engine::GameObject(rhs)
	, m_pTransform(rhs.m_pTransform)
	, m_pMeshObject(rhs.m_pMeshObject)
	, mType(rhs.mType)
	, m_pUtilityMgr(UtilityMgr::GetInstance())
	, m_matTrans(rhs.m_matTrans)
{
	m_pUtilityMgr->AddRef();
	m_pTransform->AddRef();
	m_pMeshObject->AddRef();
}

HRESULT Obstacle::ReadyPrototype(const size_t type, const D3DXVECTOR3& vPos)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Transform
	pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Transform", pComponent)))
		return E_FAIL;

	mType = type;

	switch (mType)
	{
	case OBSTACLE_GATE:
		m_pMeshTag = L"Component_Mesh_Gate";
		m_pTransform->SetPos(vPos);
		m_pTransform->SetScale(D3DXVECTOR3(1.f, 2.f, 1.6f));
		m_pTransform->SetRotation(D3DXVECTOR3(0.f, 270.f, 0.f));
		break;
	case OBSTACLE_ICETWISTER1:
	case OBSTACLE_ICETWISTER2:
		m_pMeshTag = L"Component_Mesh_IceTwister";
		D3DXMatrixTranslation(&m_matTrans, vPos.x, vPos.y, vPos.z);
		m_pTransform->SetScale(D3DXVECTOR3(3.f, 3.f, 3.f));
		m_pTransform->UpdateTransform();
		break;
	case OBSTACLE_SNOWBALL1:
	case OBSTACLE_SNOWBALL2:
		m_pMeshTag = L"Component_Mesh_SnowBall";
		m_pTransform->SetScale(D3DXVECTOR3(0.3f, 0.3f, 0.3f));
		m_pTransform->SetRotation(D3DXVECTOR3(0.f, 270.f, 0.f));
	//	m_pTransform->SetRotation(D3DXVECTOR3(0.f, -65.f, 0.f));
		break;
	default:
		break;
	}

	pComponent = m_pMeshObject = (Engine::DynamicMesh*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_DYNAMIC_MESH, m_pMeshTag);
	if (pComponent == nullptr)
		return E_FAIL;
	// For.Com_Mesh
	if (FAILED(GameObject::AddComponent(m_pMeshTag, pComponent)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Obstacle::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	m_pPos = m_pTransform->GetInfo(Engine::Transform::INFO_POSITION);
	m_pPlaneY = m_pNavigation->GetPlaneY();
	*m_pPlaneY = *m_pPos;
	m_pTransform->SetPlaneY(m_pPlaneY); // 주소전달
	
	{
		m_pPlayerTransform = (Engine::Transform*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Transform", L"Prototype_GameObject_Player");
		m_pPlayerPos = m_pPlayerTransform->GetInfo(Engine::Transform::INFO_POSITION);

		m_pPlayerCollider = (Engine::Collider*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Collider_Box", L"Prototype_GameObject_Player");

		//Component_Player_Anim
		m_pPlayerAnim = (PlayerAnim*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Anim_Player", L"Prototype_GameObject_Player");
		m_pPlayerAnimState = m_pPlayerAnim->GetAnimState();

		//Component_Player_Navigation
		m_pPlayerNavigation = (Engine::Navigation*)Engine::ObjectMgr::GetInstance()->GetComponent(SCENE_PLAYER, L"Layer_Player", L"Component_Navigation", L"Prototype_GameObject_Player");
		m_pPlayerIndex = m_pPlayerNavigation->Get_CurrentIndex();
	}


	switch (mType)
	{
	case OBSTACLE_GATE:
		mAnimState = "Closed";
		mAnimSpeed = 0.5f;
		m_pAnimCtrl_Info = m_pMeshObject->Set_AnimaByName(mAnimState.c_str());
		break;
	case OBSTACLE_ICETWISTER1:
		mTwisterRot = -160.f;
		mTwister_RevolutionValue = 5.5f;
		break;
	case OBSTACLE_ICETWISTER2:
		mTwisterRot = 160.f;
		mTwister_RevolutionValue = -5.5f;
		break;
	case OBSTACLE_SNOWBALL1:
		mAnimState = "Roll";
		mAnimSpeed = 3.f;
		mSnowBallSpeed = 35.f;
		m_pAnimCtrl_Info = m_pMeshObject->Set_AnimaByName(mAnimState.c_str());
		// 173 ~ 194
		m_pNavigation->Setup_CurrentIndex(194);
		m_pTransform->SetPos(m_pNavigation->GetCenterPos());
		SnowBall_PathLoad(L"../Bin/Data/PathFiles/SnowBallPath.dat");
		break;
	case OBSTACLE_SNOWBALL2:
		mAnimState = "Roll";
		mAnimSpeed = 6.f;
		mSnowBallSpeed = 100.f;
		m_pAnimCtrl_Info = m_pMeshObject->Set_AnimaByName(mAnimState.c_str());
		// 173 ~ 194
		m_pNavigation->Setup_CurrentIndex(194);
		m_pTransform->SetPos(m_pNavigation->GetCenterPos());
		SnowBall_PathLoad(L"../Bin/Data/PathFiles/SnowBallPath2.dat");
		break;
	default:
		break;
	}

	
	return NOERROR;
}

UPDATE Obstacle::Update(const float timeDelta)
{
	switch (mType)
	{
	case OBSTACLE_GATE:
		break;
	case OBSTACLE_ICETWISTER1:
	case OBSTACLE_ICETWISTER2:
		if (m_bTwister)
		{
			TwisterUpdate(timeDelta);
		
		}
		break;
	case OBSTACLE_SNOWBALL1:
	case OBSTACLE_SNOWBALL2:
		if (m_bSnowBall)
		{
			SnowBallUpdate(timeDelta);
		}
	default:
		break;
	}

	AnimUpdate(timeDelta);
	AnimStateChange(timeDelta);

	return Engine::GameObject::Update(timeDelta);
}

UPDATE Obstacle::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr || m_pCenterPos == nullptr)
		return UPDATE::FAIL;

	if (m_pFrustum->FrustumColling(m_pCenterPos, mRadius))
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
			return UPDATE::FAIL;
	}

	return GameObject::LastUpdate(timeDelta);
}

void Obstacle::Render()
{
	if (m_pMeshObject == nullptr || m_pTransform == nullptr || m_pShader == nullptr)
		return;

	// 뷰랑 투영도 다 가지고 있는 상태.
	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);


	switch (mType)
	{
	case OBSTACLE_GATE:
		if (m_bGate)
		{
			m_pMeshObject->Render(pEffect);
		}
		break;
	case OBSTACLE_ICETWISTER1:
	case OBSTACLE_ICETWISTER2:
		if (m_bTwister)
		{
			m_pMeshObject->Render(pEffect);
		}
		break;
	case OBSTACLE_SNOWBALL1:
	case OBSTACLE_SNOWBALL2:
		if (m_bSnowBall)
		{
			m_pMeshObject->Render(pEffect);
		}
		break;
	default:
		break;
	}

	pEffect->EndPass();
	pEffect->End();

	if (g_bColliderDraw)
	{
		switch (mType)
		{
		case OBSTACLE_GATE:
			if (m_bGate)
			{
				m_pColliderBox->Render();
			}
			break;
		case OBSTACLE_ICETWISTER1:
		case OBSTACLE_ICETWISTER2:
			if (m_bTwister)
			{
				m_pColliderSphere->Render();
			}
		case OBSTACLE_SNOWBALL1:
		case OBSTACLE_SNOWBALL2:
			if (m_bSnowBall)
			{
				m_pColliderSphere->Render();
			}
			break;
		default:
			break;
		}
	}

}



HRESULT Obstacle::ReadyComponent(void)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Shader
	pComponent = m_pShader = (Engine::Shader*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_SHADER, L"Component_Shader_Mesh");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Shader_Mesh", pComponent)))
		return E_FAIL;


	// For.Com_Renderer
	pComponent = m_pRenderer = (Engine::Renderer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Renderer");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Renderer", pComponent)))
		return E_FAIL;

	// For.Com_Collider_Box
	pComponent = m_pColliderBox = (Engine::Collider*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Collider_Box");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Engine::GameObject::AddComponent(L"Component_Collider_Box", pComponent)))
		return E_FAIL;

	// For.Com_Collider_Sphere
	pComponent = m_pColliderSphere = (Engine::Collider*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Collider_Sphere");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(Engine::GameObject::AddComponent(L"Component_Collider_Sphere", pComponent)))
		return E_FAIL;

	// For.Com_Navigation
	pComponent = m_pNavigation = (Engine::Navigation*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Navigation");
	if (m_pNavigation == nullptr)
		return E_FAIL;
	if (FAILED(Engine::GameObject::AddComponent(L"Component_Navigation", pComponent)))
		return E_FAIL;

	switch (mType)
	{
	case OBSTACLE_GATE:
		if (FAILED(m_pColliderBox->SetUp_Collider(D3DXVECTOR3(20.f, 3.f, -20.f), D3DXVECTOR3(30.f, 40.f, 20.f), m_pTransform->Get_WorldMatrix()
			, &m_vCenterPos, &mRadius)))
			return E_FAIL;

		D3DXVec3TransformCoord(&m_vCenterPos, &m_vCenterPos, m_pTransform->Get_WorldMatrix());
		m_pCenterPos = &m_vCenterPos;
		break;
	case OBSTACLE_ICETWISTER1:
	case OBSTACLE_ICETWISTER2:
		if (FAILED(m_pColliderSphere->SetUp_Collider(D3DXVECTOR3(-1.f, 2.f, 4.f), D3DXVECTOR3(1.0f, 4.f, 6.f), m_pTransform->Get_FinalMatrix()
			, &m_vCenterPos, &mRadius)))
			return E_FAIL;
		
		m_pCenterPos = &m_vFinal_CenterPos;
		break;
	case OBSTACLE_SNOWBALL1:
	case OBSTACLE_SNOWBALL2:
		if (FAILED(m_pColliderSphere->SetUp_Collider(D3DXVECTOR3(-10.f, 9.f, -10.f), D3DXVECTOR3(13.f, 32.f, 13.f), m_pTransform->Get_WorldMatrix()
			, &m_vCenterPos, &mRadius)))
			return E_FAIL;
		
		m_pTransform->SetUpdate_CenterPos(true);
		m_pTransform->Set_CenterPos(m_vCenterPos);
		m_pCenterPos = m_pTransform->Get_CenterPos();


		
		break;
	default:
		break;
	
	}
	
	// For.Com_Frustum
	pComponent = m_pFrustum = (Engine::Frustum*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Frustum");
	if (pComponent == nullptr)
		return E_FAIL;
	if (FAILED(GameObject::AddComponent(L"Component_Frustum", pComponent)))
		return E_FAIL;

	m_pCamerFPS = (CameraFPS*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_STAGE, L"Layer_Camera", L"Prototype_GameObject_CameraFPS");


	return NOERROR;
}

HRESULT Obstacle::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	switch (mType)
	{
	case OBSTACLE_GATE:
	case OBSTACLE_SNOWBALL1:
	case OBSTACLE_SNOWBALL2:
		m_pTransform->SetUp_OnShader(pEffect, "g_matWorld", false);
		break;
	case OBSTACLE_ICETWISTER1:
	case OBSTACLE_ICETWISTER2:
		m_pTransform->SetUp_OnShader(pEffect, "g_matWorld", true);
		break;
	default:
		break;
	}

	D3DXMATRIX matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	return NOERROR;
}

void Obstacle::AnimUpdate(const float timeDelta)
{
	D3DXVECTOR3 vPos = *m_pPos;
	D3DXVECTOR3 vPlayerPos = *m_pPlayerPos;
	vPos.y = 0.f;
	vPlayerPos.y = 0.f;

	switch (mType)
	{
	case OBSTACLE_GATE:
		if (mAnimState == "Closed")
		{
			m_bGate = true;
			if (*m_pPlayerIndex >= 78 && *m_pPlayerIndex <= 80)
			{
				mAnimState = "Open";
				SoundMgr::GetInstance()->PlaySound(L"Gate_Open.wav", CHANNEL_EFFECT);
			}
		}
		else if (mAnimState == "Open")
		{
			mAnimTime += mAnimSpeed *timeDelta;
			m_pCamerFPS->SetCameraMode(CAMERA_GATE);

			if (FAILED(m_pMeshObject->PlayAnimation(mAnimSpeed * timeDelta)))
			{
				_MSG_BOX("Obtacle Anim FAILED!");
			}
		}
		else
		{
			if (*m_pPlayerIndex >= 227 && *m_pPlayerIndex <= 229)
			{
				
				m_bGate = false;
			}
		}
		break;
	case OBSTACLE_SNOWBALL1:
	case OBSTACLE_SNOWBALL2:
		if (*m_pPlayerIndex >= 174 && *m_pPlayerIndex <= 176)
		{
			m_bSnowBall = true;
			m_pCamerFPS->SetCameraMode(CAMERA_SNOW_BALL);
		}
		else if (*m_pPlayerIndex >= 194 && *m_pPlayerIndex <= 196)
		{
			m_bSnowBall = false;
			m_pCamerFPS->SetCameraMode(CAMERA_NORMAL);
		}

		if (m_bSnowBall)
		{
			if (mAnimState == "Roll")
			{
				mAnimTime += mAnimSpeed *timeDelta;

				if (FAILED(m_pMeshObject->PlayAnimation(mAnimSpeed * timeDelta)))
				{
					_MSG_BOX("Obtacle Anim FAILED!");
				}
			}
		}
		break;
	case OBSTACLE_ICETWISTER1:
	case OBSTACLE_ICETWISTER2:
		if (*m_pPlayerIndex >= 122 && *m_pPlayerIndex <= 124)
		{
			m_bTwister = true;
		}
		else if (*m_pPlayerIndex >= 105 && *m_pPlayerIndex <= 107)
		{
			m_bTwister = false;
		}
	default:
		break;
	}
}

void Obstacle::AnimStateChange(const float timeDelta)
{
	switch (mType)
	{
	case OBSTACLE_GATE:
		if (mAnimTime >= ((m_pAnimCtrl_Info->animPeriod)))
		{
			mAnimTime = 0.0;

			if (mAnimState == "Open")
			{
				m_pCamerFPS->SetCameraMode(CAMERA_NORMAL);
				mAnimState = "Opened";
				SoundMgr::GetInstance()->PlaySound(L"Gate_Hit.wav", CHANNEL_EFFECT);
			}
		}

		if (mPre_AnimState != mAnimState)
		{
			m_pAnimCtrl_Info = m_pMeshObject->Set_AnimaByName(mAnimState.c_str());
			mPre_AnimState = mAnimState;
			mAnimTime = 0.0;
		}
		break;
	case OBSTACLE_SNOWBALL1:
	case OBSTACLE_SNOWBALL2:
		mAnimTime = 0.0;
		break;
	default:
		break;
	}
}

void Obstacle::TwisterUpdate(const float timeDelta)
{
	m_pTransform->SetRotation(D3DXVECTOR3(0.f, mTwisterRot*timeDelta, 0.f));
	m_pTransform->SetPos(D3DXVECTOR3(0.f, 0.f, 10.f));

	mTwisterRevolution += mTwister_RevolutionValue * timeDelta;

	D3DXMatrixRotationY(&m_matRot, mTwisterRevolution);
	m_matParent = m_matRot * m_matTrans;

	m_pTransform->SetUp_ParentMatrix(&m_matParent);
	m_pTransform->UpdateTransform();

	D3DXVec3TransformCoord(m_pCenterPos, &m_vCenterPos, m_pTransform->Get_FinalMatrix());

	if (m_pColliderSphere->Collision_Bullet(m_pPlayerCollider, 4.5f))
	{
		if (*m_pPlayerAnimState != "RisingAttack")
		{
			m_pPlayerAnim->Set_DamagedState(false);
			m_pPlayerAnim->SetJumpInfo(JUMP_INFO(2.5f, 90.f, 75.f, JUMP_BACK));
			*m_pPlayerAnimState = "DownA";
		}
	}
}

void Obstacle::SnowBallUpdate(const float timeDelta)
{
	
	float distance = 0.f;

	if (m_pColliderSphere->Collision_Bullet(m_pPlayerCollider, 0.5f))
	{
		if (*m_pPlayerAnimState != "RisingAttack")
		{
			m_pPlayerAnim->Set_DamagedState(false);
			m_pPlayerAnim->SetJumpInfo(JUMP_INFO(2.5f, 90.f, 75.f, JUMP_BACK));
			*m_pPlayerAnimState = "DownA";
		}
	}

	
	if (mPathIndex < mPathSize)
	{

		if (mPath_PreIndex != mPathIndex)
		{
			m_vSnowDir = m_pUtilityMgr->CalTargetDir(*m_pPos, mSnowBallSpace[mPathIndex]);
			mPath_PreIndex = mPathIndex;
		}

		if (m_pNavigation->is_Move(*m_pPos, m_vSnowDir, &m_vLineNormal))
		{
			m_pTransform->SnowBallStep(m_vSnowDir);
		}
		//m_vSnowDir = m_pUtilityMgr->CalTargetDir(*m_pPos, mSnowBallSpace[mPathIndex]);
		distance = m_pUtilityMgr->Cal_TargetDistance(*m_pPos, mSnowBallSpace[mPathIndex]);
		/*cout << "스노우볼 거리" << distance << "인덱스:" << mPathIndex << endl;
		cout << "스노울 방향벡터" << m_vSnowDir.x << " " << m_vSnowDir.y << " " << m_vSnowDir.z << endl;*/
		m_vDir = m_vSnowDir * mSnowBallSpeed * timeDelta;
		//m_vDir
		//m_pTransform->DirStep(m_vDir, true);
	}


	if (distance <= 15.f)
	{
		++mPathIndex;

		if (mPathIndex >= mPathSize)
		{
			mPathIndex = 0;
			m_pNavigation->Setup_CurrentIndex(194);
			m_pTransform->SetPos(m_pNavigation->GetCenterPos());
		}
	}
}

HRESULT Obstacle::SnowBall_PathLoad(const TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	DWORD byte = 0;
	D3DXVECTOR3 vPathPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	mSnowBallSpace.clear();

	ReadFile(hFile, &mPathSize, sizeof(size_t), &byte, nullptr);
	mSnowBallSpace.reserve(mPathSize);

	for (size_t i = 0; i < (size_t)mPathSize; i++)
	{
		ReadFile(hFile, &vPathPos, sizeof(D3DXVECTOR3), &byte, nullptr);
		mSnowBallSpace.emplace_back(vPathPos);
	}

	return NOERROR;
}

Obstacle * Obstacle::Create(LPDIRECT3DDEVICE9 pDevice, const size_t type, const D3DXVECTOR3& vPos)
{
	Obstacle* pInstance = new Obstacle(pDevice);

	if (FAILED(pInstance->ReadyPrototype(type, vPos)))
	{
		_MSG_BOX("Obstacle Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Obstacle::Clone()
{
	Obstacle* pInstance = new Obstacle(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Obstacle Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Obstacle::Free(void)
{
	Engine::Safe_Release(m_pMeshObject);
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pUtilityMgr);

	if (m_bClone)
	{
		Engine::Safe_Release(m_pRenderer);
		Engine::Safe_Release(m_pShader);
		Engine::Safe_Release(m_pFrustum);
		Engine::Safe_Release(m_pColliderBox);
		Engine::Safe_Release(m_pColliderSphere);
		Engine::Safe_Release(m_pNavigation);
	}

	GameObject::Free();

	return DWORD();
}
