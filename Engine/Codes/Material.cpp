#include "Material.h"

_USING(Engine)


Material::Material(LPDIRECT3DDEVICE9 pDevice)
	: Component(pDevice)
{
}

Material::Material(const Material & rhs)
	:Component(rhs)
	,mMtrlInfo(rhs.mMtrlInfo)
{
}

HRESULT Material::Init(const D3DMATERIAL9 * pMaterial)
{
	mMtrlInfo = *pMaterial;

	return NOERROR;
}

Material * Material::Create(LPDIRECT3DDEVICE9 pDevice, const D3DMATERIAL9 * pMaterial)
{
	Material* pInstance = new Material(pDevice);

	if (FAILED(pInstance->Init(pMaterial)))
	{
		_MSG_BOX("Material Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Component * Material::Clone()
{
	return new Material(*this);
}

DWORD Material::Free()
{
	Component::Free();
	return DWORD();
}
