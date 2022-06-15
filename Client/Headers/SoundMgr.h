#ifndef SOUND_MGR_H__
#define SOUND_MGR_H__

#include "Base.h"
#include "Include.h"

_BEGIN(Client)

class SoundMgr final : public Engine::Base
{
public:
	_DECLARE_SINGLETON(SoundMgr)

private:
	explicit SoundMgr();
	virtual ~SoundMgr() = default;



public:
	HRESULT Init();
	HRESULT Update();

public:
	HRESULT LoadSoundFile();
	void PlaySound(const TCHAR* pSoundKey, CHANNEL id);
	void PlayBGM(const TCHAR* pSoundKey, CHANNEL id);
	void StopSound(CHANNEL id);
	void StopAll();
	void SetVolume(CHANNEL id, float fVal);
	int IsPlayCheck(CHANNEL id);

public:
	virtual DWORD Free() override;

private:
	FMOD_SYSTEM* m_pSystem;
	FMOD_CHANNEL* m_pChannelArr[CHANNEL_END];
	const TCHAR* m_pSoundName = nullptr;
	map<const TCHAR*, FMOD_SOUND*>	mSoundSpace;

private:
	size_t mPreFrame = 0;
	bool mCheckOnly[CHANNEL_END] = { true };
	FMOD_BOOL m_bChannalPlayCheck;


};
_END
#endif

