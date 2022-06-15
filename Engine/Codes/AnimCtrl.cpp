#include "AnimCtrl.h"

_USING(Engine)

AnimCtrl::AnimCtrl(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

AnimCtrl::AnimCtrl(const AnimCtrl & rhs)
	:m_pDevice(rhs.m_pDevice)
	,mCurrentTrack(0)
	,mNewTrack(1)
	,mTrackDesc(rhs.mTrackDesc)
	,mDuration(rhs.mDuration)
{
	m_pDevice->AddRef();


	if (FAILED(rhs.m_pAnimationCtrl->CloneAnimationController(rhs.m_pAnimationCtrl->GetMaxNumAnimationOutputs(), mMaxNum_AnimSet = rhs.m_pAnimationCtrl->GetMaxNumAnimationSets(),
		rhs.m_pAnimationCtrl->GetMaxNumTracks(), rhs.m_pAnimationCtrl->GetMaxNumEvents(), &m_pAnimationCtrl)))
		return;
}

HRESULT AnimCtrl::Init(LPD3DXANIMATIONCONTROLLER pAnimationCtrl)
{
	m_pAnimationCtrl = pAnimationCtrl;
	m_pAnimationCtrl->AddRef();

	return NOERROR;
}

HRESULT AnimCtrl::Set_AnimByName(const char * pAnimName)
{
	//cout << "뻐큐" << endl;
	if (m_pAnimationCtrl == nullptr)
	{
		_MSG_BOX("m_pAnimationCtrl nullptr FAILED!");
		return E_FAIL;
	}
		
	if (FAILED(m_pAnimationCtrl->GetAnimationSetByName(pAnimName, &m_pAnimSet)))
	{
		//cout << "Animation Failed: " << pAnimName << endl;
		//_MSG_BOX("GetAnimationSetByName return FAILED!");
		return E_FAIL;
	}
		
	// 애니메이션 세트의시간
	mAnimCtrl_Info.animPeriod = m_pAnimSet->GetPeriod();
	mAnimCtrl_Info.pDuration = &mDuration;
	mAnimCtrl_Info.pPosition = &mTrackDesc.Position;

	mNewTrack = (mCurrentTrack == 0 ? 1 : 0);

	m_pAnimationCtrl->SetTrackAnimationSet(mNewTrack, m_pAnimSet);
	Safe_Release(m_pAnimSet);

	
	// 이벤트가 동작중에 보간은 취소되기 때문에 이벤트를 다 해제한다. 
	// 트랙에 있는 키값들을 모두 없앤다.
	m_pAnimationCtrl->UnkeyAllTrackEvents(mCurrentTrack);
	m_pAnimationCtrl->UnkeyAllTrackEvents(mNewTrack);

	// 현재 재생중인 트랙을 멈춘다.
	// 트랙을 유효하게 설정한다. 이 메서드는 트랙의 설정을 유효 또는 무료로한다.
	// 현재트랙을 사용하지않는 키값을 현재시간으로부터 블랜딩이 끝나는 시간에 넣는다.
	m_pAnimationCtrl->KeyTrackEnable(mCurrentTrack, FALSE, mTimeAcc + mDuration);
	// 트랙의 속도를 설정한다. 이것은 트랙의 재생속도를 조정한다.  
	// 현재트랙의 속도를 현재시간부로 0으로 점차 선형보간한다.
	m_pAnimationCtrl->KeyTrackSpeed(mCurrentTrack, 0.f, mTimeAcc, mDuration, D3DXTRANSITION_LINEAR);
	// 트랙의 중량감을 설정한다. 중량감은 2개의 트랙을 동시에 재생할 경우에 승수로서 사용한다.
	// 현재트랙의 가중치를 현재시간부로 0으로 점차 선형보간한다.
	m_pAnimationCtrl->KeyTrackWeight(mCurrentTrack, 0.f, mTimeAcc, mDuration, D3DXTRANSITION_LINEAR);

	//새로운 트랙을 재생한다.
	// 애니매이션 믹서의 트랙을 유효 또는 무효로 한다.
	m_pAnimationCtrl->SetTrackEnable(mNewTrack, TRUE);
	m_pAnimationCtrl->KeyTrackSpeed(mNewTrack, 1.f, mTimeAcc, mDuration, D3DXTRANSITION_LINEAR);
	// 새로운트랙의 속도를 현재시간부로 1로 점차 선형보간한다.
	m_pAnimationCtrl->KeyTrackWeight(mNewTrack, 1.f, mTimeAcc, mDuration, D3DXTRANSITION_LINEAR);
	// 새로운트랙의 속도를 현재시간부로 1로 점차 선형보간한다.

	m_pAnimationCtrl->ResetTime();
	mTimeAcc = 0.0;

	m_pAnimationCtrl->SetTrackPosition(mNewTrack, 0.0);

	mCurrentTrack = mNewTrack;

	// 돌아가고있는 시간을 가져온다.
	m_pAnimationCtrl->GetTrackDesc(mCurrentTrack, &mTrackDesc);
	mTrackDesc.Position = 0.0;

	return NOERROR;
}

HRESULT AnimCtrl::PlayAnim(const float timeDelta)
{
	if (m_pAnimationCtrl == nullptr)
		return E_FAIL;

	// 활성화되어있는 트랙을 재생해라.
	// 인자로 넘겨준 시간을 누적하여 시간에따른 뼈의 변화량행렬을 계속 갱신해주어라.
	m_pAnimationCtrl->AdvanceTime(timeDelta, nullptr);
	mTimeAcc += timeDelta;
	mTrackDesc.Position += timeDelta;

	return NOERROR;
}

AnimCtrl * AnimCtrl::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXANIMATIONCONTROLLER pAnimationCtrl)
{
	AnimCtrl* pInstance = new AnimCtrl(pDevice);

	if (FAILED(pInstance->Init(pAnimationCtrl)))
	{
		_MSG_BOX("AnimCtrl Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

AnimCtrl * AnimCtrl::Clone()
{
	return new AnimCtrl(*this);
}

DWORD AnimCtrl::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pAnimationCtrl);

	//DWORD refCnt = Safe_Release(m_pAnimSet);
	//if (refCnt > 0)
	//{
	//	_MSG_BOX("AnimSet RefCnt!");
	//}
	return DWORD();
}
