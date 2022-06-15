#ifndef MATERIAL_H__
#define MATERIAL_H__

#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL Material : public Component
{
private:
	explicit Material(LPDIRECT3DDEVICE9 pDevice);
	explicit Material(const Material& rhs);
	virtual ~Material() = default;

public:
	HRESULT Init(const D3DMATERIAL9* pMaterial);

public:
	const D3DMATERIAL9* Get_MaterialInfo() const { return &mMtrlInfo; }

public:
	static Material* Create(LPDIRECT3DDEVICE9 pDevice, const D3DMATERIAL9* pMaterial);
	virtual Component* Clone();
	virtual DWORD Free();

private:
	D3DMATERIAL9 mMtrlInfo;

};

_END

#endif

