#ifndef HIERACHY_LOADER
#define HIERACHY_LOADER

#include "Engine_Include.h"

_BEGIN(Engine)

class HierarchyLoader final : public ID3DXAllocateHierarchy
{
private:
	explicit HierarchyLoader(LPDIRECT3DDEVICE9 pDevice);
	virtual ~HierarchyLoader() = default;

public:
	HRESULT Init(const TCHAR* pFilePath);

public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame) override;
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer) override;
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;

private:
	HRESULT AllocateName(char** ppSourName, const char* pDestName);

public:
	DWORD AddRef();
	DWORD Release();

public:
	static HierarchyLoader* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath);
	virtual DWORD Free();

private:
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
	const TCHAR* m_pPath = nullptr;
	DWORD mRefCnt = 0; // ÂüÁ¶°¹¼ö

};

_END

#endif

