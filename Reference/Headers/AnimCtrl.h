#ifndef ANIM_CTRL_H__
#define ANIM_CTRL_H__

#include "Base.h"

_BEGIN(Engine)

class AnimCtrl final : public Base
{
private:
	explicit AnimCtrl(LPDIRECT3DDEVICE9 pDevice);
	explicit AnimCtrl(const AnimCtrl& rhs);
	virtual ~AnimCtrl() = default;

public:
	HRESULT Init(LPD3DXANIMATIONCONTROLLER pAnimationCtrl);
	HRESULT Set_AnimByName(const char* pAnimName);
	HRESULT PlayAnim(const float timeDelta);

public:
	ANIM_CTRL_INFO* Get_AnimCtrlInfo() { return &mAnimCtrl_Info; }

public:
	static AnimCtrl* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXANIMATIONCONTROLLER pAnimationCtrl);
	AnimCtrl* Clone();
	virtual DWORD Free();

private:
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;
	LPD3DXANIMATIONCONTROLLER m_pAnimationCtrl = nullptr;
	LPD3DXANIMATIONSET m_pAnimSet = nullptr;
	D3DXTRACK_DESC mTrackDesc;
	ANIM_CTRL_INFO mAnimCtrl_Info;

private:
	double mTimeAcc = 0.0;
	double mDuration = 0.25;

private:
	DWORD mMaxNum_AnimSet = 0;
	DWORD mCurrentTrack = 0;
	DWORD mNewTrack = 1;
	size_t mCurrent_AnimIdx = 0;
};

_END

#endif
