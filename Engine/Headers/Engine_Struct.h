#ifndef ENGINE_STRUCT_H__
#define ENGINE_STRUCT_H__

namespace Engine
{
	typedef struct tagMyMeshContainer : public D3DXMESHCONTAINER
	{
		LPD3DXMESH pOriginalMesh = nullptr;

		// 서브셋별 사용되는 텍스쳐를 로드하여 보관하자.
		LPDIRECT3DTEXTURE9*	ppDiffuseTex = nullptr;
		LPDIRECT3DTEXTURE9*	ppSpecularTex = nullptr;
		LPDIRECT3DTEXTURE9*	ppNormalTex = nullptr;

		// 메시컨테이너에 영향을 미치는 뼈의 갯수.
		DWORD numFrames = 0;

		// 최종적으로 피부를 붙이기위한 행렬(오프셋매트릭스 * 컴바인드 매트릭스)들의 포인터.
		D3DXMATRIX* pRenderingMatrices = nullptr;

		// 뼈를 기준으로 얼마나 이동되서 피부가 표현되야하는지?!
		D3DXMATRIX* pOffsetMatrices = nullptr;

		D3DXMATRIX** ppCombined_TransformMatrices = nullptr;

	}D3DXMESHCONTAINER_DERIVED;

	typedef struct tagMyFrame : public D3DXFRAME
	{
		D3DXMATRIX	combined_TransformMatrix;

	}D3DXFRAME_DERIVED;



	typedef struct tagVertexColor
	{
		D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
		DWORD color = 0;
	}VTXCOL;
	constexpr DWORD VTXCOL_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
		D3DXVECTOR2 vTexUV = { 0.f, 0.f };
	}VTXTEX;
	constexpr DWORD VTXTEX_FVF = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVertex_NormalTexture
	{
		D3DXVECTOR3	vPos = { 0.f, 0.f, 0.f };
		D3DXVECTOR3	vNormal = { 0.f, 0.f, 0.f };
		D3DXVECTOR2	vTexUV = { 0.f, 0.f };
	}VTXNORMAL_TEX;
	constexpr DWORD VTXNORMAL_TEX_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertex_ViewPort_Texture
	{
		D3DXVECTOR4	vPosition; // ViewPort영역상의 위치를 설정한다.
		D3DXVECTOR2	vTexUV;
	}VTX_VIEWPORT_TEX;

	typedef struct tagVertexCube
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vTex;
	}VTXCUBE;
	constexpr DWORD	VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagVertex_Point
	{
		D3DXVECTOR3	vPos;
		float PSize;
		D3DXVECTOR2	vTexUV;
	}VTXPOINT;

	typedef struct tagPolygon16
	{
		WORD _1, _2, _3;
	}POLYGON16;

	typedef struct tagPolygon32
	{
		DWORD _1, _2, _3;
	}PLYGON32;
}

// AnimCtri Related Struct
typedef struct tagAnimCtrl
{
	tagAnimCtrl() {}
	explicit tagAnimCtrl(double animPeriod, double* pDuration)
	{
		this->animPeriod = animPeriod;
		this->pDuration = pDuration;
	}

	double animPeriod = 0.0;
	double* pDuration = nullptr;
	double* pPosition = nullptr;

}ANIM_CTRL_INFO;

typedef struct tagMeshInfo
{
	D3DXVECTOR3 vMin = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vMax = D3DXVECTOR3(0.f, 0.f, 0.f);

}MESH_INFO;

typedef struct tagLightInfo : public _D3DLIGHT9
{
	tagLightInfo()
	{
		ZeroMemory(this, sizeof(tagLightInfo));
	}
	explicit tagLightInfo(const D3DLIGHTTYPE& type, const D3DCOLORVALUE& diffuse, const float ambient, const D3DXVECTOR3& vPos, const size_t specialIndex = 0, const float range = 0.f)
	{
		ZeroMemory(this, sizeof(tagLightInfo));

		Type = type;
		Diffuse = diffuse;
		Specular = Diffuse;
		Ambient = D3DXCOLOR(ambient, ambient, ambient, 1.f);
		this->specialIndex = specialIndex;

		if (Type == D3DLIGHT_DIRECTIONAL)
		{
			Direction = vPos;
		}
		else if (Type == D3DLIGHT_POINT)
		{
			Position = vPos;
			Range = range;
		}
	}

	size_t specialIndex = 0;

}LIGHT_INFO;


#endif