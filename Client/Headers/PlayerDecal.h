#ifndef PLAYER_DECAL_H__
#define PLAYER_DECAL_H__

#include "GameObject.h"
#include "Include.h"
#include "DecalBox.h"


_BEGIN(Client)


class PlayerDecal final : public DecalBox
{
private:
	explicit PlayerDecal(LPDIRECT3DDEVICE9 pDevice);
	explicit PlayerDecal(const PlayerDecal& rhs);
	virtual ~PlayerDecal() = default;


public:
	virtual UPDATE Update(const float timeDelta) override;
	virtual void Render() override;

public:
	void SetDecalInfo(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale, const size_t textureNum, const size_t decalType, const bool bDecal, const float alpha = 0.f) override;

public:
	static PlayerDecal* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject * Clone() override;
	virtual DWORD Free() override;
};

_END

#endif