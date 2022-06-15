#ifndef STATIC_MESH_H__
#define STATIC_MESH_H__

#include "Mesh.h"

_BEGIN(Engine)

typedef struct tagTexture
{
	// 서브셋별 사용되는 텍스쳐를 로드하여 보관하자.
	LPDIRECT3DTEXTURE9*	ppDiffuseTex = nullptr;
	LPDIRECT3DTEXTURE9*	ppSpecularTex = nullptr;
	LPDIRECT3DTEXTURE9*	ppNormalTex = nullptr;

}SHADER_TEXTURE;

class _ENGINE_DLL StaticMesh final : public Mesh
{
private:
	explicit StaticMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit StaticMesh(const StaticMesh& rhs);
	virtual ~StaticMesh() = default;

public:
	HRESULT Init(const TCHAR* pFilePaht, const TCHAR* pFileName);
	void Render(LPD3DXEFFECT pEffect);

public:
	const MESH_INFO& Get_MeshInfo() const{ return mMeshInfo; }

public:
	static StaticMesh* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath, const TCHAR* pFileName);
	virtual Component* Clone();
	virtual DWORD Free();

private:
	LPD3DXMESH m_pMesh = nullptr;
	LPD3DXBUFFER m_pAdjacency = nullptr;
	LPD3DXBUFFER m_pSubset = nullptr;
	DWORD mNumSubset = 0;
	MESH_INFO mMeshInfo;

private:
	SHADER_TEXTURE mShaderTexture;


};

_END

#endif

