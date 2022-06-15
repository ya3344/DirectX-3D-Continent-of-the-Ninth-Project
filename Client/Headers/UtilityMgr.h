#ifndef UTILITY_MGR_H__
#define UTILITY_MGR_H__

#include "Base.h"
#include "Include.h"

namespace Engine
{
	class KeyMgr;
}
_BEGIN(Client)

class UtilityMgr final : public Engine::Base
{
public:
	_DECLARE_SINGLETON(UtilityMgr)

private:
	explicit UtilityMgr();
	virtual ~UtilityMgr() = default;

private:
	enum OBJ_MOTION : size_t
	{
		LEFT = 0,
		LD,
		LU,
		UP,
		RIGHT,
		RU,
		RD,
		DOWN,
		MOTION_END
	};

public:
	void KeyCheck();
	bool FindString(const char* pAnimTag, const char* pFindTag);

public:
	const D3DXVECTOR3& RandomDir(const D3DXVECTOR3& vPos, const float timeDelta, float distance = 1.f);
	float Cal_TargetDistance(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vTarget, bool bYPos = false);
	const D3DXVECTOR3& CalTargetDir(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vTarget, bool bYPos = false);
	const float CalDirDegree(const D3DXVECTOR3& vSrcPos, const D3DXVECTOR3& vDestPos);
	const bool Cal_DirCross(const D3DXVECTOR3& vSrcDir, const D3DXVECTOR3& vDestDir);
	void Cal_SlidingVector(const D3DXVECTOR3& vDir, D3DXVECTOR3& vLineNormal, D3DXVECTOR3* pSlide, float* pSlideSpeed);

public:
	float inline Min(float a, float b) { return a > b ? a : b; }

public:
	void StringReplace(string& animTag, const char* pSrcKey, const char* pReplaceKey);

public:
	virtual DWORD Free() override;

private:
	Engine::KeyMgr* m_pKeyMgr = nullptr;

private:
	size_t mMoveState = MOTION_END;
	D3DXVECTOR3 m_vRandomPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vRandomDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vTargetDir = D3DXVECTOR3(0.f, 0.f, 0.f);

private:
	D3DXVECTOR3 m_vSrcDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vDestDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vCross = D3DXVECTOR3(0.f, 0.f, 0.f);

};
_END
#endif

