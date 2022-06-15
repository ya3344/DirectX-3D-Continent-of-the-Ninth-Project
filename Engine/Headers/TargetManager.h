#ifndef TARGET_MANAGER_H__
#define TARGET_MANAGER_H__

#include "Base.h"

_BEGIN(Engine)

class Target;
class _ENGINE_DLL TargetManager final : public Base
{
	_DECLARE_SINGLETON(TargetManager)

private:
	explicit TargetManager();
	virtual ~TargetManager() = default;

public:
	HRESULT AddTarget(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pTargetTag, const size_t width, const size_t height, D3DFORMAT format, D3DXCOLOR color);
	HRESULT AddMRT(const TCHAR* pMRTTag, const TCHAR* pTargetTag);
	HRESULT BeginMRT(const TCHAR* pMRTTag);
	HRESULT EndMRT(const TCHAR* pMRTTag);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const TCHAR* pTargetTag, const char* pConstantName);

public:
	HRESULT Ready_BufferDebug(const TCHAR* pTargetTag, const float startX, const float startY, const float sizeX, const float sizeY);
	void Render_BufferDebug(const TCHAR* pMRTTag);

private:
	Target* FindTarget(const TCHAR* pTargetTag);
	list<Target*>* FindMRT(const TCHAR* pMRTTag);

public:
	virtual DWORD Free() override;

private:
	map<const TCHAR*, Target*> mTargetSpace;
	typedef pair<const TCHAR*, Target*> TARGET_PAIR;

private:
	map<const TCHAR*, list<Target*>>	mMRTSpace;
	typedef pair<const TCHAR*, list<Target*>> MRT_PAIR;


};


_END


#endif