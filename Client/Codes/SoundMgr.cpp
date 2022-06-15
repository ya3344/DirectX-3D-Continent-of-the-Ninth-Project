#include "stdafx.h"
#include "SoundMgr.h"


_USING(Client)
_IMPLEMENT_SINGLETON(SoundMgr);

SoundMgr::SoundMgr()
{
	Init();
}


HRESULT SoundMgr::Init()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, CHANNEL_END, FMOD_INIT_NORMAL, nullptr);

	LoadSoundFile();

	return NOERROR;
}




HRESULT SoundMgr::Update()
{
	FMOD_System_Update(m_pSystem);

	return NOERROR;
}

HRESULT SoundMgr::LoadSoundFile()
{
	_finddata_t fd;

	long handle = 0;
	handle = _findfirst("../Sound/*.*", &fd);

	if (-1 == handle)
	{
		_MSG_BOX("Not Found Sounds")
			return  FALSE;
	}

	int result = 0;

	while (-1 != result)
	{
		char szFullPath[128] = "../Sound/";
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT res = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE
			, nullptr, &pSound);

		if (FMOD_OK == res)
		{
			int iLength = strlen(fd.name);
			TCHAR* pSoundKey = new TCHAR[iLength + 1];

			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength + 1, pSoundKey, iLength + 1);
			mSoundSpace.insert({ pSoundKey, pSound });
		}

		result = _findnext(handle, &fd);
	}

	_findclose(handle);
	FMOD_System_Update(m_pSystem);

	return NOERROR;
}

void SoundMgr::PlaySound(const TCHAR * pSoundKey, CHANNEL id)
{
	auto iter_find = find_if(mSoundSpace.begin(), mSoundSpace.end(),
		[&pSoundKey](auto& MyPair)
	{
		return !lstrcmp(pSoundKey, MyPair.first);
	});

	if (mSoundSpace.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE
		, iter_find->second, FALSE, &(m_pChannelArr[id]));
}

void SoundMgr::PlayBGM(const TCHAR * pSoundKey, CHANNEL id)
{
	auto iter_find = find_if(mSoundSpace.begin(), mSoundSpace.end(),
		[&pSoundKey](auto& MyPair)
	{
		return !lstrcmp(pSoundKey, MyPair.first);
	});

	if (mSoundSpace.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE
		, iter_find->second, FALSE, &(m_pChannelArr[id]));

	FMOD_Channel_SetMode(m_pChannelArr[id], FMOD_LOOP_NORMAL);
}

void SoundMgr::StopSound(CHANNEL id)
{
	FMOD_Channel_Stop(m_pChannelArr[id]);
}

void SoundMgr::StopAll()
{
	for (size_t i = 0; i < (size_t)CHANNEL::CHANNEL_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void SoundMgr::SetVolume(CHANNEL id, float fVal)
{
	FMOD_Channel_SetVolume(m_pChannelArr[id], fVal);
}

int SoundMgr::IsPlayCheck(CHANNEL id)
{
	FMOD_Channel_IsPlaying(m_pChannelArr[id], &m_bChannalPlayCheck);
	return m_bChannalPlayCheck;
}


DWORD SoundMgr::Free()
{
	for (auto& MyPair : mSoundSpace)
	{
		delete[] MyPair.first;
		FMOD_Sound_Release(MyPair.second);
	}

	mSoundSpace.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
	return 0;
}
