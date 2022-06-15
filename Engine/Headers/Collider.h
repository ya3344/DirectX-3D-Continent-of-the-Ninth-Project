#ifndef COLLIDER_H__
#define COLLIDER_H__

#include "Component.h"

_BEGIN(Engine)

class StaticMesh;
class DynamicMesh;

// 1. �������ؿ� ������ 1¥�� ť�긦 ���� �����Ѵ�.(������ü)
// 2. ������Ʈ��ü�μ� �������vü �߰��Ǵ� ���� Ư�� ������� ���ַ� �����ȴ�.

typedef struct tagOBBInfo
{
	D3DXVECTOR3 vPoint[8]; // ť���� ����������
	D3DXVECTOR3 vCenter;
	D3DXVECTOR3 vProjAxis[3]; // ť���� �߽ɿ��� �� ���� �߾��� �ٶ󺸴� ���� ����
	D3DXVECTOR3	vAlignAxis[3]; // ť���� �� �鿡 ������ ���� ����

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
	enum TYPE { TYPE_BOX, TYPE_SPHERE }; // �ڽ����� �������� ���� �Ǵ�.

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
	// ���� �ڽ��� ���� �����ϱ����� MinVector�� MaxVector�� �־���, ���� ����� �־���.
	// �޽��� �������� ��������� �翬�� �޽÷κ��� MinvEctor, MaxVector ���ͼ� �־��ָ� ��.
	// ���� ���彺���̽��� �����ϰ� ����� ���� �ƴϴ�.
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
