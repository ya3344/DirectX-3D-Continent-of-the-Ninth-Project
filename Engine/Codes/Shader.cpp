#include "Shader.h"

_USING(Engine)

Shader::Shader(LPDIRECT3DDEVICE9 pDevice)
	: Component(pDevice)
{

}

Shader::Shader(const Shader & rhs)
	: Component(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_pErrBuffer(rhs.m_pErrBuffer)
{
	m_pEffect->AddRef();

	if (m_pErrBuffer != nullptr)
		m_pErrBuffer->AddRef();
}

HRESULT Shader::Init(const TCHAR* pFilePath)
{
	// fx인 파일안에 작성된 hlsl이라는 언어를 번역한다.
	// 셰이더 파일을 대표하는 객체를 생성한다.
	if (FAILED(D3DXCreateEffectFromFile(m_pDevice, pFilePath, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_pEffect, &m_pErrBuffer)))
	{
		MessageBoxA(0, (char*)m_pErrBuffer->GetBufferPointer(), "Shader Compiled Error", MB_OK);
		return E_FAIL;
	}

	return NOERROR;
}

Shader * Shader::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR * pFilePath)
{
	Shader*	pInstance = new Shader(pDevice);

	if (FAILED(pInstance->Init(pFilePath)))
	{
		_MSG_BOX("Shader Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Component * Shader::Clone(void)
{
	return new Shader(*this);
}

DWORD Shader::Free(void)
{
	Safe_Release(m_pErrBuffer);
	Safe_Release(m_pEffect);

	Component::Free();

	return DWORD();
}
