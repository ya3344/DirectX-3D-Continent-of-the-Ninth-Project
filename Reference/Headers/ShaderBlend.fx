
texture g_DiffuseTex;
texture	g_ShadeTex;
texture	g_SpecularTex;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTex;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


sampler ShadeSampler = sampler_state
{
	texture = g_ShadeTex;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTex;
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
	vector vColor : COLOR0;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDiffuseInfo = tex2D(DiffuseSampler, In.vTexUV);
	vector vShadeInfo = tex2D(ShadeSampler, In.vTexUV);
	vector vSpecularInfo = tex2D(SpecularSampler, In.vTexUV);

	Out.vColor = (vDiffuseInfo * vShadeInfo) + vSpecularInfo;

	return Out;	
}

technique Default_Device
{
	pass Blend
	{
		AlphaTestEnable = true;
		AlphaRef = 0x00;
		AlphaFunc = Greater;

		ZWriteEnable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}

}