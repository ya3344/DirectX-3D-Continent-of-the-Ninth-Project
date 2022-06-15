#ifndef SHADER_H__
#define SHADER_H__

#include "Component.h"

_BEGIN(Engine)

// hlsl : fx

// VertexShader : 정점의 변환, 정점정보의 수정.
// 변환 : 월드, 뷰, 투영.
// 정점정보의 수정 : 

// PixelShader : 픽셀의 색을 결정한다.
// 래스터라이즈에 의해서 결정된 픽셀의 정보를 바탕으로하여 픽셀의 색을 결정한다.

class _ENGINE_DLL Shader : public Component
{
private:
	explicit Shader(LPDIRECT3DDEVICE9 pDevice);
	explicit Shader(const Shader& rhs);
	virtual ~Shader() = default;

public:
	HRESULT Init(const TCHAR* pFilePath);

public:
	LPD3DXEFFECT Get_EffectHandle(void) const { return m_pEffect; }

public:
	static Shader* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath);
	virtual Component* Clone(void);
	virtual DWORD Free(void);

private:
	LPD3DXEFFECT m_pEffect = nullptr;
	LPD3DXBUFFER m_pErrBuffer = nullptr;
};

_END

#endif