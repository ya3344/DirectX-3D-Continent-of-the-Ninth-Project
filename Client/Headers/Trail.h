#ifndef TRAIL_H__
#define TRAIL_H__

#include "GameObject.h"
#include "Include.h"
#include "VIBuffer.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class Shader;
	class Texture;
	class TrailBuffer;
}

_BEGIN(Client)


class Trail final : public Engine::GameObject
{
private:
	explicit Trail(LPDIRECT3DDEVICE9 pDevice);
	explicit Trail(const Trail& rhs);
	virtual ~Trail() = default;


public:
	HRESULT ReadyPrototype(const TCHAR* pBufferTag);
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

public:
	void SetTargetMatrix(const D3DXMATRIX& targetMatrix);
	void SetInitPoint(const D3DXVECTOR3& vInitPosA, const D3DXVECTOR3& vInitPosB);
	void SetTrailState(const bool bTrail) { m_bTrail = bTrail; }
	void SetColor(const D3DXVECTOR4& vColor) { m_vColor = vColor; }
	void SetTextureNum(const size_t textureNum) { mTextureNum = textureNum; }

private:
	HRESULT ReadyComponent(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	

public:
	enum TRAIL_VALUE
	{
		TRAIL_POINT_COUNT = 40,
	};

public:
	static Trail* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pBufferTag);
	virtual GameObject * Clone() override;
	virtual DWORD Free() override;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::Texture* m_pTexture = nullptr;

private:
	Engine::TrailBuffer* m_pTrailBuffer = nullptr;
	Engine::VTX_INFO* m_pVtxInfo = nullptr;

private:
	LPD3DXEFFECT m_pEffect = nullptr;

private:
	size_t mTextureNum = 0;
	float mTrail_DeltaTime = 0.f;
	float mTrailTimeGap = 0.015f;
	bool m_bTrail = false;

private:
	D3DXVECTOR3 m_vInitPosA = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vInitPosB = D3DXVECTOR3(0.f, 0.f, 0.f);
	const D3DXMATRIX* m_pMatWorld = nullptr;
	D3DXVECTOR4 m_vColor = D3DXVECTOR3(0.f, 0.f, 0.f);
	const TCHAR* m_pBufferTag = nullptr;
};

_END

#endif