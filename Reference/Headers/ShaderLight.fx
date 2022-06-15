vector g_vLightDir;
vector g_vLightPos;
float g_fRange;

vector g_vLightDiffuse;
vector g_vLightAmbient;
vector g_vLightSpecular;

vector g_vMtrlDiffuse = vector(1.0f, 1.0f, 1.0f, 1.0f);
vector g_vMtrlAmbient = vector(1.0f, 1.0f, 1.0f, 1.0f);
vector g_vMtrlSpecular = vector(1.0f, 1.0f, 1.0f, 1.0f);
float g_fPower = 15.f;

vector g_vCamPosition;
matrix g_matViewInv;
matrix g_matProjInv;

texture	g_NormalTex;
sampler NormalSampler = sampler_state
{
	texture = g_NormalTex;
};

texture g_DepthTex;
sampler DepthSampler = sampler_state
{
	texture = g_DepthTex;
};

texture g_MeshSpecularTex;
sampler SpecularSampler = sampler_state
{
	texture = g_MeshSpecularTex;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct PS_IN
{
	vector vPosition : POSITION;
	float2 vTexUV : TEXCOORD;
};

struct PS_OUT
{
	vector vShade : COLOR0;
	vector vSpecular : COLOR1;
};


PS_OUT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vNormalInfo = tex2D(NormalSampler, In.vTexUV);
	vector vDepthInfo = tex2D(DepthSampler, In.vTexUV);
	float vViewZ = vDepthInfo.x * 1000.f;

	// 0 -> -1, 1 -> 1
	vector vNormal = vector((vNormalInfo.xyz * 2.f) - 1.f, 0.f);
	vector vLightDir = normalize(g_vLightDir);
	vector vShade = saturate(dot(vLightDir * -1.f, vNormal));
	vShade = (g_vLightDiffuse * g_vMtrlDiffuse) * vShade;

	vector vResultColor = vShade + (g_vLightAmbient * g_vMtrlAmbient);

	Out.vShade = saturate(vResultColor);
	Out.vShade.a = 1.f;

	// ¹Ý»çº¤ÅÍ, ½Ã¼±º¤ÅÍ.
	if (vShade.b > 0)
	{
		vector vReflect = reflect(vLightDir, vNormal);
		vReflect = normalize(vReflect);

		vector vProjPos;
		vProjPos.x = (In.vTexUV.x * 2.f - 1.f) * vViewZ;
		vProjPos.y = (In.vTexUV.y * -2.f + 1.f) * vViewZ;
		vProjPos.z = (vDepthInfo.g) * vViewZ;
		vProjPos.w = vViewZ;

		vector vViewPos = mul(vProjPos, g_matProjInv);
		vector vWorldPos = mul(vViewPos, g_matViewInv);
		vector vLook = normalize(vWorldPos - g_vCamPosition);

		vector vSpecular = saturate(dot(vReflect, vLook * -1.f));
		vSpecular = pow(vSpecular, g_fPower);

		vector vSpecularIntensity = tex2D(SpecularSampler, In.vTexUV);
		if (vSpecularIntensity.r > 0)
		{
			vSpecular = vSpecular * vSpecularIntensity;
		}


		Out.vSpecular = vSpecular * g_vLightDiffuse;
		Out.vSpecular.a = 1.f;
	}




	return Out;
}

PS_OUT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;




	vector vNormalInfo = tex2D(NormalSampler, In.vTexUV);
	vector vDepthInfo = tex2D(DepthSampler, In.vTexUV);
	float vViewZ = vDepthInfo.x * 1000.f;

	vector vProjPos;
	vProjPos.x = (In.vTexUV.x *  2.f - 1.f) * vViewZ;
	vProjPos.y = (In.vTexUV.y * -2.f + 1.f) * vViewZ;
	vProjPos.z = (vDepthInfo.g) * vViewZ;
	vProjPos.w = vViewZ;

	vector vViewPos = mul(vProjPos, g_matProjInv);
	vector vWorldPos = mul(vViewPos, g_matViewInv);
	vector vNormal = vector(vNormalInfo.xyz * 2.f - 1.f, 0.f);
	vector vLightDir = vWorldPos - g_vLightPos;

	float fDistance = length(vLightDir);
	float fRange = max((g_fRange - fDistance) / g_fRange, 0.f);

	float att = g_fRange - fDistance / g_fRange;
	clip(att <= 0.f ? -1 : 1);

	vector vShade = saturate(dot(vLightDir * -1.f, vNormal));
	vShade = (g_vLightDiffuse)* vShade;
	vector vResultColor = vShade + (g_vLightAmbient * g_vLightDiffuse);



	Out.vShade = saturate(vResultColor) * fRange;
	Out.vShade.a = 1.f;

	if (vShade.b > 0)
	{
		vector vReflect = reflect(vLightDir, vNormal);
		vector vLook = normalize(vWorldPos - g_vCamPosition);

		vector vSpecular = saturate(dot(vReflect, vLook * -1.f));
		vSpecular = pow(vSpecular, g_fPower) * fRange;

		/*vector vSpecularIntensity = tex2D(SpecularSampler, In.vTexUV);
		vSpecular = vSpecular * vector(vSpecularIntensity.bgr, 1.f);*/

		Out.vSpecular = vSpecular * g_vLightDiffuse;
		Out.vSpecular.a = 1.f;
	}

	return Out;
}

technique Default_Device
{
	pass Light_Diretional
	{
		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		ZWriteEnable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_DIRECTIONAL();
	}

	pass Light_Point
	{
		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		ZWriteEnable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_POINT();
	}

}