#ifndef MESH_H__
#define MESH_H__

#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL Mesh abstract : public Component
{
protected:
	explicit Mesh(LPDIRECT3DDEVICE9 pDevice);
	const Mesh(const Mesh& rhs);
	virtual ~Mesh() = default;

public:
	virtual HRESULT Init();


public:
	virtual Component* Clone() = 0;
	virtual DWORD Free();
};

_END
#endif

