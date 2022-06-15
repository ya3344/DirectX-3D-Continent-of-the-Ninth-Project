#ifndef COLLIDER_H__
#define COLLIDER_H__

#include "Component.h"

_BEGIN(Engine)

class StaticMesh;
class DynamicMesh;

// 1. 원점기준에 사이즈 1짜리 큐브를 먼저 생성한다.(원형객체)
// 2. 컴포넌트객체로서 오브젝틍체 추가되는 순간 특성 사이즈와 위주로 편변ㅇ된다.

typedef struct tagOBBInfo
{
	D3DXVECTOR3 vPoint[8]; // 큐브의 여덟개정점
	D3DXVECTOR3 vCenter;
	D3DXVECTOR3 vProjAxis[3]; // 큐브의 중심에서 각 면의 중앙을 바라보는 벡터 세개
	D3DXVECTOR3	vAlignAxis[3]; // 큐브의 각 면에 평행한 벡터 세개

}OBB_INFO;

typedef struct tagColliderInfo
{
	D3DXVECTOR3 vSourMin = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vSourMax = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vDestMin = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vDestMax = D3DXVECTOR3(0.f, 0.f, 0.f);

}COLLIDER_INFO;

class _ENGINE_DLL Collider : public Component
{
public:
	enum TYPE { TYPE_BOX, TYPE_SPHERE }; // 박스인지 구인지에 대한 판단.

private:
	explicit Collider(LPDIRECT3DDEVICE9 pDevice);
	explicit Collider(const Collider& rhs);
	virtual ~Collider() = default;

public:
	HRESULT Init(TYPE type);

public:
	bool Collision_EX(const Collider* pTargetCollider, D3DXVECTOR3* vPush);
	bool Collision_AABB(const Collider* pTargetCollider, D3DXVECTOR3* pRightDir = nullptr);
	bool Collision_OBB(const Collider* pTargetCollider);
	bool Collision_Sphere(const Collider* pTargetCollider);
	bool Collision_Bullet(const Collider* pTargetCollider, float targetRadius = 0.f);
	void CalAxis(OBB_INFO* pOBB_Info);

public:
	const D3DXVECTOR3& GetCenterPos() const { return m_vCenterPos; }
	const D3DXMATRIX* Get_TargetMatrix() const { return m_pTargetMatrix; }
	const D3DXVECTOR3* Get_TargetPos() const { return (D3DXVECTOR3*)&m_pTargetMatrix->m[3][0]; }
	const MESH_INFO& Get_MeshInfo() const { return mMeshInfo; }

public:
	// 현재 박스나 구를 구성하기위한 MinVector와 MaxVector을 넣어줌, 뼈의 행렬을 넣어줌.
	// 메시의 사이즈대로 띄우고싶으면 당연히 메시로부터 MinvEctor, MaxVector 얻어와서 넣어주면 됨.
	// 절대 월드스페이스에 존재하게 만드는 것이 아니다.
	HRESULT SetUp_Collider(const D3DXVECTOR3& vMin, const D3DXVECTOR3& vMax, const D3DXMATRIX* pTargetMatrix, D3DXVECTOR3* pCenterPos = nullptr, float* pRadius = nullptr);
	void Render();

private:
	HRESULT Init_BoundingBox();
	HRESULT Init_BoundingSphere();

public:
	static Collider* Create(LPDIRECT3DDEVICE9 pDevice, TYPE type);
	virtual Component* Clone() override;
	virtual DWORD Free() override;

private:
	LPD3DXMESH m_pMesh = nullptr;
	LPD3DXBUFFER m_pAdjacency = nullptr;
	TYPE mType;
	const D3DXMATRIX* m_pTargetMatrix;

private:
	D3DXMATRIX m_matScale;
	D3DXMATRIX m_matTrans;

private: // Collision Related Variable
	MESH_INFO mMeshInfo;
	bool m_bCollision = false;
	D3DXVECTOR3 m_vLocalPoint[8];
	COLLIDER_INFO mColliderInfo;

private:
	D3DXVECTOR3 m_vCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vTargetDir = D3DXVECTOR3(0.f, 0.f, 0.f);

private:
	float mRadius = 0.f;
	float mDistance = 0.f;
};

_END

#endif
