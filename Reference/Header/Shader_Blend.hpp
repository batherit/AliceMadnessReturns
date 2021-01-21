texture			g_AlbedoTexture;

sampler AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;

	minfilter = linear;
	magfilter = linear;
};

texture			g_ShadeTexture;

sampler ShadeSampler = sampler_state
{
	texture = g_ShadeTexture;

	minfilter = linear;
	magfilter = linear;
};


texture			g_SpecularTexture;

sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;

	minfilter = linear;
	magfilter = linear;
};

texture		g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};


float g_fFogEnd;
float g_fFogStart;
float g_fFogHeightEnd;
float g_fFogHeightStart;
vector g_vFogSphereColor;
float g_fFogSphereDensity;
vector g_vFogHeightColor;
float g_fFogHeightDensity;
matrix		g_matViewInv;
matrix		g_matProjInv;

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);

	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);

	Out.vColor = vAlbedo * vShade /** vSpecular*/;
	
	return Out;
}

PS_OUT PS_SphereFog(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);

	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);

	vector vDepth = tex2D(DepthSampler, In.vTexUV);
	float fViewZ = vDepth.y * 1000.f;
	float fFog = clamp((g_fFogEnd - fViewZ) / (g_fFogEnd - g_fFogStart), 0.f, 1.f);

	Out.vColor = vAlbedo * vShade /** vSpecular*/;
	Out.vColor = fFog * Out.vColor + (1.f - fFog) * vector(g_vFogSphereColor.xyz, g_fFogSphereDensity)/* + vSpecular*/;

	return Out;
}

PS_OUT PS_HeightFog(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);

	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);

	vector vDepth = tex2D(DepthSampler, In.vTexUV);
	float fViewZ = vDepth.y * 1000.f;

	vector vPosition;
	// texture U로 올수 있는 숫자의 범위는 0~1, 투영 X로 변환된 숫자의 범위는 -1~1
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	// texture V로 올수 있는 숫자의 범위는 (0)~(1), 투영 X로 변환된 숫자의 범위는 (1)~(-1)
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;		// vDepth.x = 투영 공간 z/카메라 공간 z(원근적용) 이므로 fViewZ을 곱하면 결국 투영공간z를 구한 것.
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	float fFog = clamp((g_fFogHeightEnd - vPosition.y) / (g_fFogHeightEnd - g_fFogHeightStart), 0.f, 1.f);

	Out.vColor = vAlbedo * vShade;
	Out.vColor = fFog * Out.vColor + (1.f - fFog) * vector(g_vFogHeightColor.xyz, g_fFogHeightDensity)/* + vSpecular*/;


	return Out;
}

PS_OUT PS_AllFog(PS_IN In) {
	PS_OUT		Out = (PS_OUT)0;

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);

	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);

	vector vDepth = tex2D(DepthSampler, In.vTexUV);
	float fViewZ = vDepth.y * 1000.f;

	vector vPosition;
	// texture U로 올수 있는 숫자의 범위는 0~1, 투영 X로 변환된 숫자의 범위는 -1~1
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	// texture V로 올수 있는 숫자의 범위는 (0)~(1), 투영 X로 변환된 숫자의 범위는 (1)~(-1)
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;		// vDepth.x = 투영 공간 z/카메라 공간 z(원근적용) 이므로 fViewZ을 곱하면 결국 투영공간z를 구한 것.
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	float fFogSphere = clamp((g_fFogEnd - fViewZ) / (g_fFogEnd - g_fFogStart), 0.f, 1.f);
	float fFogHeight = clamp((g_fFogHeightEnd - vPosition.y) / (g_fFogHeightEnd - g_fFogHeightStart), 0.f, 1.f);

	Out.vColor = vAlbedo * vShade;
	Out.vColor = fFogSphere * Out.vColor + (1.f - fFogSphere) * vector(g_vFogSphereColor.xyz, g_fFogSphereDensity)/* + vSpecular*/;
	Out.vColor = fFogHeight * Out.vColor + (1.f - fFogHeight) * vector(g_vFogHeightColor.xyz, g_fFogHeightDensity)/* + vSpecular*/;

	return Out;
}

technique Default_Device
{
	// 기능의 캡슐화
	pass Blend_Main
	{
		zwriteenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass Blend_SphereFog
	{
		zwriteenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_SphereFog();
	}

	pass Blend_HeightFog
	{
		zwriteenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_HeightFog();
	}

		pass Blend_All
	{
		zwriteenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_AllFog();
	}
};