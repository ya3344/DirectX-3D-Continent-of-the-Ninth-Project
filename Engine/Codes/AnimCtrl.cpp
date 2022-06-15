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
	//cout << "��ť" << endl;
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
		
	// �ִϸ��̼� ��Ʈ�ǽð�
	mAnimCtrl_Info.animPeriod = m_pAnimSet->GetPeriod();
	mAnimCtrl_Info.pDuration = &mDuration;
	mAnimCtrl_Info.pPosition = &mTrackDesc.Position;

	mNewTrack = (mCurrentTrack == 0 ? 1 : 0);

	m_pAnimationCtrl->SetTrackAnimationSet(mNewTrack, m_pAnimSet);
	Safe_Release(m_pAnimSet);

	
	// �̺�Ʈ�� �����߿� ������ ��ҵǱ� ������ �̺�Ʈ�� �� �����Ѵ�. 
	// Ʈ���� �ִ� Ű������ ��� ���ش�.
	m_pAnimationCtrl->UnkeyAllTrackEvents(mCurrentTrack);
	m_pAnimationCtrl->UnkeyAllTrackEvents(mNewTrack);

	// ���� ������� Ʈ���� �����.
	// Ʈ���� ��ȿ�ϰ� �����Ѵ�. �� �޼���� Ʈ���� ������ ��ȿ �Ǵ� ������Ѵ�.
	// ����Ʈ���� ��������ʴ� Ű���� ����ð����κ��� ������ ������ �ð��� �ִ´�.
	m_pAnimationCtrl->KeyTrackEnable(mCurrentTrack, FALSE, mTimeAcc + mDuration);
	// Ʈ���� �ӵ��� �����Ѵ�. �̰��� Ʈ���� ����ӵ��� �����Ѵ�.  
	// ����Ʈ���� �ӵ��� ����ð��η� 0���� ���� ���������Ѵ�.
	m_pAnimationCtrl->KeyTrackSpeed(mCurrentTrack, 0.f, mTimeAcc, mDuration, D3DXTRANSITION_LINEAR);
	// Ʈ���� �߷����� �����Ѵ�. �߷����� 2���� Ʈ���� ���ÿ� ����� ��쿡 �¼��μ� ����Ѵ�.
	// ����Ʈ���� ����ġ�� ����ð��η� 0���� ���� ���������Ѵ�.
	m_pAnimationCtrl->KeyTrackWeight(mCurrentTrack, 0.f, mTimeAcc, mDuration, D3DXTRANSITION_LINEAR);

	//���ο� Ʈ���� ����Ѵ�.
	// �ִϸ��̼� �ͼ��� Ʈ���� ��ȿ �Ǵ� ��ȿ�� �Ѵ�.
	m_pAnimationCtrl->SetTrackEnable(mNewTrack, TRUE);
	m_pAnimationCtrl->KeyTrackSpeed(mNewTrack, 1.f, mTimeAcc, mDuration, D3DXTRANSITION_LINEAR);
	// ���ο�Ʈ���� �ӵ��� ����ð��η� 1�� ���� ���������Ѵ�.
	m_pAnimationCtrl->KeyTrackWeight(mNewTrack, 1.f, mTimeAcc, mDuration, D3DXTRANSITION_LINEAR);
	// ���ο�Ʈ���� �ӵ��� ����ð��η� 1�� ���� ���������Ѵ�.

	m_pAnimationCtrl->ResetTime();
	mTimeAcc = 0.0;

	m_pAnimationCtrl->SetTrackPosition(mNewTrack, 0.0);

	mCurrentTrack = mNewTrack;

	// ���ư����ִ� �ð��� �����´�.
	m_pAnimationCtrl->GetTrackDesc(mCurrentTrack, &mTrackDesc);
	mTrackDesc.Position = 0.0;

	return NOERROR;
}

HRESULT AnimCtrl::PlayAnim(const float timeDelta)
{
	if (m_pAnimationCtrl == nullptr)
		return E_FAIL;

	// Ȱ��ȭ�Ǿ��ִ� Ʈ���� ����ض�.
	// ���ڷ� �Ѱ��� �ð��� �����Ͽ� �ð������� ���� ��ȭ������� ��� �������־��.
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
