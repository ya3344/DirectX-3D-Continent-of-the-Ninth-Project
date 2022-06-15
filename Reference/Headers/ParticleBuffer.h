#ifndef PARTICLE_BUFFER_H__
#define PARTICLE_BUFFER_H__

#include "VIBuffer.h"

_BEGIN(Engine)

class _ENGINE_DLL ParticleBuffer final : public VIBuffer
{
private:
	explicit ParticleBuffer(LPDIRECT3DDEVICE9 pDevice);
	explicit ParticleBuffer(const ParticleBuffer& rhs);
	virtual ~ParticleBuffer() = default;

public:
	HRESULT InitBuffer();
	void RenderBuffer();

public:
	D3DXVECTOR3 m_vInitPos = D3DXVECTOR3(0.f, 0.f, 0.f);

public:
	static ParticleBuffer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Component* Clone() override;
	virtual DWORD Free() override;
};


_END
#endif // PARTICLE_BUFFER_H__

