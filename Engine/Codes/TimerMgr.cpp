#include "TimerMgr.h"
#include "Timer.h"

_USING(Engine)

_IMPLEMENT_SINGLETON(TimerMgr)

TimerMgr::TimerMgr()
{
}

HRESULT TimerMgr::AddTimer(const TCHAR * pTimerTag)
{
	Timer* pTimer = FindTimer(pTimerTag);

	if (pTimer != nullptr)
		return E_FAIL;

	pTimer = Timer::Create();
	if (nullptr == pTimer)
		return E_FAIL;

	mTimerSpace.emplace(pTimerTag, pTimer);

	return NOERROR;
}

const float TimerMgr::Get_TimeDelta(const TCHAR * pTimerTag)
{
	Timer*	pTimer = FindTimer(pTimerTag);
	
	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}

void TimerMgr::ShowFPS(HWND hWnd, float * timeFPS)
{
//#ifdef _DEBUG
	++mTimeCount;

	if (*timeFPS >= 1.f)
	{
		wsprintf(mFPSBuf, L"FPS : %d", mTimeCount);

		mTimeCount = 0;
		*timeFPS = 0.f;
	}
	SetWindowText(hWnd, mFPSBuf);
//#endif
}

bool TimerMgr::RangeUpdate(const float range, const float timeDelta)
{
	//mTime += GetTimeDelta();
	mTime += timeDelta;

	if (range <= mTime)
	{
		mTime = 0.f;
		return true;
	}

	return false;
}

Timer * TimerMgr::FindTimer(const TCHAR * pTimerTag)
{
	auto iterFind = find_if(mTimerSpace.begin(), mTimerSpace.end(), CFinder_Tag(pTimerTag));

	if (iterFind == mTimerSpace.end())
		return nullptr;

	return iterFind->second;
}

DWORD TimerMgr::Free(void)
{
	DWORD refCnt = 0;

	for (PAIR_TIMER pair : mTimerSpace)
	{
		if (refCnt = Safe_Release(pair.second))
		{
			_MSG_BOX("Timer Release Failed");
		}
			
	}

	mTimerSpace.clear();

	return 0;
}


