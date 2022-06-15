#ifndef DECAL_BOX_H__
#define DECAL_BOX_H__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class Shader;
	class Texture;
	class CubeTex;
	class TargetManager;
}

_BEGIN(Client)


class DecalBox : public Engine::GameObject
{
protected:
	explicit DecalBox(LPDIRECT3DDEVICE9 pDevice);
	explicit DecalBox(const DecalBox& rhs);
	virtual ~DecalBox() = default;


public:
	HRESULT ReadyPrototype();
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) = 0;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() = 0;
	virtual void SetDecalInfo(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale, const size_t textureNum, const size_t decalType, const bool bDecal, const float alpha = 0.f) = 0;

public:
	void SetTargetMatrix(const D3DXMATRIX* pTargetMatrix);
	void SetDecalPos(const D3DXVECTOR3& vPos);
	
	void SetDecalState(const bool bDecal) { m_bDecal = bDecal; }
	void SetColor(const D3DXVECTOR4& vColor) { m_vColor = vColor; }
	void SetTextureNum(const size_t textureNum) { mTextureNum = textureNum; }
	void SetScale(const D3DXVECTOR3& vScale);

	


protected:
	HRESULT ReadyComponent(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);


public:
	virtual DWORD Free() override;

protected:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Shader* m_pShader = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::Texture* m_pTexture = nullptr;
	Engine::CubeTex* m_pCubeBuffer = nullptr;

protected:
	LPD3DXEFFECT m_pEffect = nullptr;

protected:
	size_t mTextureNum = 0;
	bool m_bDecal = false;
	size_t mDecalType = 0;
	float mAlpha = 0.f;
	float mRotY = 0.f;
	float mDecalTime = 0.f;

protected:
	const D3DXMATRIX* m_pMatTarget = nullptr;
	D3DXVECTOR4 m_vColor = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	const D3DXMATRIX* m_pMatWorld = nullptr;

protected:
	Engine::TargetManager* m_pTargetMgr = nullptr;
	D3DXVECTOR3 m_vScale = D3DXVECTOR3(1.f, 1.f, 1.f);
};

_END

#endif