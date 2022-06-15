#ifndef TRAIL_BUFFER_H__
#define TRAIL_BUFFER_H__

#include "VIBuffer.h"

_BEGIN(Engine)

class _ENGINE_DLL TrailBuffer final : public VIBuffer
{
private:
	explicit TrailBuffer(LPDIRECT3DDEVICE9 pDevice);
	explicit TrailBuffer(const TrailBuffer& rhs);
	virtual ~TrailBuffer() = default;

public:
	HRESULT InitBuffer(const size_t vtxNum );

public:
	static TrailBuffer* Create(const size_t vtxNum, LPDIRECT3DDEVICE9 pDevice);
	virtual Component* Clone() override;
	virtual DWORD Free() override;
};


_END
#endif // RECT_TEX_H__

