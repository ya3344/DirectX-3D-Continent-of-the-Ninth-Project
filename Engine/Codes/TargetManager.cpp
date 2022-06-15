#include "TargetManager.h"
#include "Target.h"

_USING(Engine)

_IMPLEMENT_SINGLETON(TargetManager)


TargetManager::TargetManager()
{
}



HRESULT TargetManager::AddTarget(LPDIRECT3DDEVICE9 pDevice, const TCHAR * pTargetTag, const size_t width, const size_t height, D3DFORMAT format, D3DXCOLOR color)
{
	Target* pTarget = FindTarget(pTargetTag);

	if (pTarget != nullptr)
		return E_FAIL;

	pTarget = Target::Create(pDevice, width, height, format, color);

	if (pTarget == nullptr)
		return E_FAIL;

	mTargetSpace.emplace(pTargetTag, pTarget);

	return NOERROR;
}

HRESULT TargetManager::AddMRT(const TCHAR * pMRTTag, const TCHAR * pTargetTag)
{
	Target* pTarget = FindTarget(pTargetTag);

	if (pTarget == nullptr)
		return E_FAIL;

	list<Target*>* pMRTList = FindMRT(pMRTTag);

	if (pMRTList == nullptr)
	{
		list<Target*> MRTList;
		MRTList.emplace_back(pTarget);

		mMRTSpace.emplace(pMRTTag, MRTList);
	}
	else
	{
		pMRTList->emplace_back(pTarget);
	}
	

	pTarget->AddRef();

	return NOERROR;
}

HRESULT TargetManager::BeginMRT(const TCHAR * pMRTTag)
{
	list<Target*>* pMRTList = FindMRT(pMRTTag);

	if (pMRTList == nullptr)
		return E_FAIL;

	for (Target* pTarget : *pMRTList)
	{
		if (pTarget != nullptr)
		{
			if (FAILED(pTarget->ClearTarget()))
				return E_FAIL;
		}
	}

	size_t index = 0;

	for (Target* pTarget : *pMRTList)
	{
		if (pTarget != nullptr)
		{
			if (FAILED(pTarget->SetUp_RenderTarget(index++)))
				return E_FAIL;
		}
	}

	return NOERROR;
}

HRESULT TargetManager::EndMRT(const TCHAR * pMRTTag)
{
	list<Target*>* pMRTList = FindMRT(pMRTTag);

	if (pMRTList == nullptr)
		return E_FAIL;

	size_t index = 0;

	for (Target* pTarget : *pMRTList)
	{
		if (nullptr != pTarget)
		{
			if (FAILED(pTarget->Release_RenderTarget(index++)))
				return E_FAIL;
		}
	}

	return NOERROR;
}

HRESULT TargetManager::SetUp_OnShader(LPD3DXEFFECT pEffect, const TCHAR * pTargetTag, const char * pConstantName)
{
	Target* pTarget = FindTarget(pTargetTag);

	if (pTarget == nullptr)
		return E_FAIL;

	pTarget->SetUp_OnShader(pEffect, pConstantName);

	return NOERROR;
}

HRESULT TargetManager::Ready_BufferDebug(const TCHAR * pTargetTag, const float startX, const float startY, const float sizeX, const float sizeY)
{

	Target* pTarget = FindTarget(pTargetTag);

	if (pTarget == nullptr)
		return E_FAIL;

	if (FAILED(pTarget->Ready_BufferDebug(startX, startY, sizeX, sizeY)))
		return E_FAIL;



	return NOERROR;
}

void TargetManager::Render_BufferDebug(const TCHAR * pMRTTag)
{
	list<Target*>* pMRTList = FindMRT(pMRTTag);

	if (nullptr == pMRTList)
		return;

	for (Target* pTarget : *pMRTList)
	{
		if (pTarget != nullptr)
			pTarget->Render_BufferDebug();
	}
}

Target * TargetManager::FindTarget(const TCHAR * pTargetTag)
{
	auto iterFind = find_if(mTargetSpace.begin(), mTargetSpace.end(), CFinder_Tag(pTargetTag));

	if (iterFind == mTargetSpace.end())
		return nullptr;

	return iterFind->second;
}

list<Target*>* TargetManager::FindMRT(const TCHAR * pMRTTag)
{
	auto iterFind = find_if(mMRTSpace.begin(), mMRTSpace.end(), CFinder_Tag(pMRTTag));

	if (iterFind == mMRTSpace.end())
		return nullptr;

	return &iterFind->second;
}

DWORD TargetManager::Free()
{
	for (MRT_PAIR pair : mMRTSpace)
	{
		for (Target* pTarget : pair.second)
		{
			Safe_Release(pTarget);
		}
		pair.second.clear();
	}
	mMRTSpace.clear();

	for (TARGET_PAIR pair : mTargetSpace)
	{
		Safe_Release(pair.second);
	}
		
	mTargetSpace.clear();

	return DWORD();
}
