#ifndef DYNAMIC_MESH_H__
#define DYNAMIC_MESH_H__

#include "Mesh.h"

_BEGIN(Engine)

class HierarchyLoader;
class AnimCtrl;

class _ENGINE_DLL DynamicMesh final : public Mesh
{
private:
	explicit DynamicMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit DynamicMesh(const DynamicMesh& rhs);
	virtual ~DynamicMesh() = default;

public:
	HRESULT Init(const TCHAR* pFilePath, const TCHAR* pFileName);
	void Render(LPD3DXEFFECT pEffect);

public:
	D3DXMATRIX* Get_BoneMatrix(const char* pBoneName) const;
	D3DXVECTOR3* Get_BonePos(const char* pBoneName) const;

public:
	// 뼈들의 CombinedTransformationMatrix에 값을 채운다.
	HRESULT Recursive_CombinedTransformMatrix(D3DXFRAME* pFrame, const D3DXMATRIX* pParentMatrix);
	HRESULT Recursive_SetMeshContainer(D3DXFRAME* pFrame);
	HRESULT SetUp_CombinedTransformMatrixPointer(D3DXMESHCONTAINER_DERIVED* pMeshContainer);

public: // AnimCtrl Related Function
	ANIM_CTRL_INFO* Set_AnimaByName(const char* pAnimName);
	HRESULT PlayAnimation(const float timeDelta);

public:
	static DynamicMesh* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath, const TCHAR* pFileName);
	virtual Component* Clone();
	virtual DWORD Free();

private:
	HierarchyLoader* m_pLoader = nullptr;
	D3DXFRAME* m_pRootFrame = nullptr;
	list<D3DXMESHCONTAINER_DERIVED*> mMeshContainer_Space;
	AnimCtrl* m_pAnimCtrl = nullptr;

};

_END
#endif

