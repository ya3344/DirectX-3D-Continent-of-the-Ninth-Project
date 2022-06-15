#ifndef SHADER_H__
#define SHADER_H__

#include "Component.h"

_BEGIN(Engine)

// hlsl : fx

// VertexShader : ������ ��ȯ, ���������� ����.
// ��ȯ : ����, ��, ����.
// ���������� ���� : 

// PixelShader : �ȼ��� ���� �����Ѵ�.
// �����Ͷ���� ���ؼ� ������ �ȼ��� ������ ���������Ͽ� �ȼ��� ���� �����Ѵ�.

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