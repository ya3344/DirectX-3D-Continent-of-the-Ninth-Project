#ifndef BOSS_DECAL_H__
#define BOSS_DECAL_H__

#include "GameObject.h"
#include "Include.h"
#include "DecalBox.h"


_BEGIN(Client)


class BossDecal final : public DecalBox
{
private:
	explicit BossDecal(LPDIRECT3DDEVICE9 pDevice);
	explicit BossDecal(const BossDecal& rhs);
	virtual ~BossDecal() = default;


public:
	virtual UPDATE Update(const float timeDelta) override;
	virtual void Render() override;

public:
	void SetDecalInfo(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale, const size_t textureNum, const size_t decalType, const bool bDecal, const float alpha = 0.f) override;

public:
	static BossDecal* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject * Clone() override;
	virtual DWORD Free() override;
};

_END

#endif