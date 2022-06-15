#include "LightManager.h"
#include "Light.h"
#include "KeyMgr.h"

_USING(Engine)

_IMPLEMENT_SINGLETON(LightManager)

LightManager::LightManager()
{
	m_pKeyMgr = KeyMgr::GetInstance();
}

HRESULT LightManager::AddLight(LPDIRECT3DDEVICE9 pDevice, const LIGHT_INFO& lightInfo)
{
	Light* pLight = Light::Create(pDevice, lightInfo);
	if (nullptr == pLight)
		return E_FAIL;

	mLightSpace.emplace_back(pLight);

	return NOERROR;
}

void LightManager::Render(LPD3DXEFFECT pEffect)
{
	if (m_pKeyMgr->KeyDown(DIK_4))
	{
		m_bPointLight = !m_bPointLight;
	}

	for (Light* pLight : mLightSpace)
	{
		if (pLight != nullptr)
		{
			if (pLight->GetLightInfo().Type == D3DLIGHT_POINT)
			{
				if (m_bPointLight == false)
					continue;
			}
			pLight->Render(pEffect);
		}
	}
}

void LightManager::BackBuffer_Rneder()
{
	if (mLightSpace.size() <= 0)
		return;

	auto iterBegin = mLightSpace.begin();

	(*iterBegin)->BackBuffer_Render();
}

HRESULT LightManager::CreateLight(LPDIRECT3DDEVICE9 pDevice, const TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	DWORD byte;

	size_t lightSize = 0;
	D3DXVECTOR3 vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	float range = 0.f;
	float ambient = 0.f;
	D3DCOLORVALUE color;
	size_t specialIndex = 0;

	Free();
	if (FAILED(AddLight(pDevice, LIGHT_INFO(D3DLIGHT_DIRECTIONAL, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 0.5f, D3DXVECTOR3(1.f, -1.f, 1.0f)))))
		return E_FAIL;

	//if (FAILED(AddLight(pDevice, LIGHT_INFO(D3DLIGHT_DIRECTIONAL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 0.2f, D3DXVECTOR3(1.f, -1.f, -1.0f)))))
	//	return E_FAIL;

	ReadFile(hFile, &lightSize, sizeof(size_t), &byte, nullptr);
	mLightSpace.reserve(lightSize);

	for (size_t i = 0; i < lightSize; i++)
	{
		ReadFile(hFile, &vPos, sizeof(D3DXVECTOR3), &byte, nullptr);
		ReadFile(hFile, &range, sizeof(float), &byte, nullptr);
		ReadFile(hFile, &ambient, sizeof(float), &byte, nullptr);
		ReadFile(hFile, &color, sizeof(D3DCOLORVALUE), &byte, nullptr);
		ReadFile(hFile, &specialIndex, sizeof(size_t), &byte, nullptr);

		if (FAILED(AddLight(pDevice, LIGHT_INFO(D3DLIGHT_POINT, color, ambient, vPos, specialIndex, range))))
			return E_FAIL;
	}

	CloseHandle(hFile);

	return NOERROR;
}

void LightManager::ChangeLightColor(const size_t specialIndex, const size_t bossHPCount)
{
	for (Light* pLight : mLightSpace)
	{
		if (pLight->GetLightInfo().Type == D3DLIGHT_DIRECTIONAL)
			continue;

		if (pLight->GetLightInfo().specialIndex == specialIndex)
		{
			if (specialIndex == 4)
			{
				if (bossHPCount == 2)
				{
					pLight->GetLightInfo().Diffuse.r = 1.f;
					pLight->GetLightInfo().Diffuse.g = 1.f;
					pLight->GetLightInfo().Diffuse.b = 0.f;
				}
				else if (bossHPCount == 1)
				{
					pLight->GetLightInfo().Diffuse.r = 1.f;
					pLight->GetLightInfo().Diffuse.g = 0.f;
					pLight->GetLightInfo().Diffuse.b = 0.f;
				}
			}
			else
			{
				pLight->GetLightInfo().Diffuse.r = 0.f;
				pLight->GetLightInfo().Diffuse.g = 1.f;
				pLight->GetLightInfo().Diffuse.b = 0.f;
			}

		}
	}
}

DWORD LightManager::Free()
{
	for (Light* pLight : mLightSpace)
	{
		Safe_Release(pLight);
	}
		
	mLightSpace.clear();

	return DWORD();
}
