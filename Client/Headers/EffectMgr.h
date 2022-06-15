#ifndef EFFECT_MGR_H__
#define EFFECT_MGR_H__

#include "Base.h"
#include "Include.h"


namespace Engine
{
	class GameObject;
}

_BEGIN(Client)

class Effect;

class EffectMgr final : public::Engine::Base
{
public:
	_DECLARE_SINGLETON(EffectMgr)

private:
	explicit EffectMgr();
	virtual ~EffectMgr() = default;

public:
	HRESULT Ready_EffectPrototype(LPDIRECT3DDEVICE9 pDevice);
	UPDATE Update(const float timeDelta);
	UPDATE LastUpdate(const float timeDelta);

private:
	void EffectDelete();

public:
	HRESULT CreateEffect(const TCHAR* pEffectTag, const EFFECT_INFO& effectInfo);

public:
	virtual DWORD Free() override;

private:
	list<Effect*> mEffectSpace;

};

_END
#endif

