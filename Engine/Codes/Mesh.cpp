#include "Mesh.h"

_USING(Engine)

Mesh::Mesh(LPDIRECT3DDEVICE9 pDevice)
	:Component(pDevice)
{

}

Mesh::Mesh(const Mesh & rhs)
	:Component(rhs)
{
}

HRESULT Mesh::Init()
{
	return NOERROR;
}

DWORD Mesh::Free()
{
	Component::Free();

	return DWORD();
}
