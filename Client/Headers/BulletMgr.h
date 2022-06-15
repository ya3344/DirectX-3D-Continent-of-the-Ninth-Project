#ifndef BULLET_MGR_H__
#define BULLET_MGR_H__

#include "Base.h"
#include "Include.h"

namespace Engine
{
	class GameObject;
}

_BEGIN(Client)

class Bullet;
class EffectMgr;

class BulletMgr final : public::Engine::Base
{
public:
	_DECLARE_SINGLETON(BulletMgr)

private:
	explicit BulletMgr();
	virtual ~BulletMgr() = default;

public:
	HRESULT Ready_BulletPrototype(LPDIRECT3DDEVICE9 pDevice);
	HRESULT Ready_ArrowPrototype(LPDIRECT3DDEVICE9 pDevice);
	HRESULT Ready_BossPrototype(LPDIRECT3DDEVICE9 pDevice);
	UPDATE Update(const float timeDelta);
	UPDATE LastUpdate(const float timeDelta);

public:
	void AddBullet(const TCHAR* pBulletTag, size_t bulletType, const D3DXVECTOR3& vBulletDir = D3DXVECTOR3(0.f, 0.f, 0.f), const D3DXVECTOR3& vBulletPos = D3DXVECTOR3(0.f, 0.f, 0.f));
	void IceMisile(const TCHAR* pBulletTag, size_t bulletType, const size_t misileNum, const D3DXVECTOR3& vBulletDir = D3DXVECTOR3(0.f, 0.f, 0.f), const D3DXVECTOR3& vBulletPos = D3DXVECTOR3(0.f, 0.f, 0.f));

private:
	void EffectCreate(const Bullet* pBullet);

public:
	virtual DWORD Free() override;

private:
	list<Bullet*> m_pBulletSpace;
	EffectMgr* m_pEffectMgr = nullptr;


private:
	UPDATE mUpdate = UPDATE::FAIL;

private: // Time Related Varialbe 
	float mTimeDelta = 0.f;

};

_END
#endif

