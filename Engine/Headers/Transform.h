#ifndef TRANSFORM_H__
#define TRANSFORM_H__

#include "Component.h"

_BEGIN(Engine)
class VIBuffer;

typedef struct tagMatrixInfo
{
	tagMatrixInfo()
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matWorld_NotRot);
		D3DXMatrixIdentity(&matParnet);
		D3DXMatrixIdentity(&matFinal);
		D3DXMatrixIdentity(&matFinal_NotRot);
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matRotation);
		D3DXMatrixIdentity(&matTrans);
	}

	D3DXMATRIX matWorld;
	D3DXMATRIX matWorld_NotRot;
	D3DXMATRIX matParnet;
	D3DXMATRIX matFinal;
	D3DXMATRIX matFinal_NotRot;
	D3DXMATRIX matScale;
	D3DXMATRIX matRotation;
	D3DXMATRIX matTrans;

}MATRIX_INFO;

class _ENGINE_DLL Transform final : public Component
{
public:
	enum INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POSITION };

private:
	explicit Transform(LPDIRECT3DDEVICE9 pDevice);
	explicit Transform(const Transform& rhs);
	virtual ~Transform() = default;

public:
	HRESULT Init();
	UPDATE UpdateTransform();
	UPDATE Cal_NotRotMatrix();
	UPDATE Cal_FinalNotRot_Matrix();

public:
	HRESULT SetUp_World();
	HRESULT SetUp_ParentMatrix(const D3DXMATRIX* pParentMatrix);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, bool bComplexMesh = false);
	void SetPlaneY(D3DXVECTOR3* pPlaneY) { m_pPlaneY = pPlaneY; }
	void Set_WorldMatrix(const D3DXMATRIX& matWorld) { mMatrixInfo.matWorld = matWorld; }
	void Set_CenterPos(const D3DXVECTOR3& vCenterPos) { m_vCenterPos = vCenterPos; }
	void SetUpdate_CenterPos(const bool bUpdate_CenterPos) { m_bUpdate_CenterPos = bUpdate_CenterPos; }

public:
	D3DXVECTOR3* GetInfo(INFO info);
	const D3DXMATRIX* Get_WorldMatrix() const { return &mMatrixInfo.matWorld; }
	const D3DXMATRIX* Get_WorldMatrix_NotRot() const { return &mMatrixInfo.matWorld_NotRot; }
	const D3DXMATRIX* Get_FinalMatrix() const { return &mMatrixInfo.matFinal; }
	const D3DXMATRIX* Get_FinalMatrix_NotRot() const { return &mMatrixInfo.matFinal_NotRot; }
	D3DXVECTOR3 GetScale();
	D3DXVECTOR3* Get_CenterPos() { return &m_vCenter_FinalPos; }

public:
	void SetScale(const D3DXVECTOR3& vScale);
	void SetRotation(const D3DXVECTOR3& vRot);
	void Set_FixRotation(const D3DXVECTOR3& vRot);
	void SetInfo(INFO info, const D3DXVECTOR3* pVector);

public:
	int* Get_AIModePointer() { return &mAIMode;  }

public:
	void SetPos(const D3DXVECTOR3& pVector);
	void TargetStep(D3DXVECTOR3 vTargetPos, const float speedPerSec);
	void GoStraight(const float speedPerSec);
	void Arrow_GoStraight(const float speedPerSec);
	void RightStep(const float speedPerSec);
	void LeftStep(const float speedPerSec);
	void CalJump(D3DXVECTOR3* pJumpDir, const size_t jumpState, const float velocity, const float speedPerSec, const float sinDegree = 90.f, const float cosDegree =45.f , const D3DXVECTOR3* pEnemyDir = nullptr);
	void DirStep(const D3DXVECTOR3& vDir, const bool bNotPlaneY = false);
	void SnowBallStep(const D3DXVECTOR3& vDir);
	void Gravity(const float speedPerSec);

public:
	HRESULT ComputeY_OnBuffer(Engine::VIBuffer* pBuffer);

public:
	void Combine_WorldMatrix(const D3DXMATRIX& matrix);

private:
	void Combine_WorldMatrix();

public:
	static Transform* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Component* Clone() override;
	virtual DWORD Free() override;

private:
	// 이 객체를 가지고 있는 객체의 상태를 가진다.
	MATRIX_INFO mMatrixInfo;
	D3DXQUATERNION mQuatRot;
	D3DXVECTOR3 m_vRot = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3* m_pPlaneY = nullptr;
	int mAIMode = 0;
	D3DXVECTOR3 m_vCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vCenter_FinalPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	bool m_bUpdate_CenterPos = false;

};

_END


#endif

