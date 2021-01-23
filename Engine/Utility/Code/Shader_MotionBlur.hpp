matrix			g_matInvProjView;		// 깊이 텍스쳐로부터 월드 좌표를 구하기 위함

matrix			g_matPrevViewProj;	// 이전 월드 좌표 -> 스크린 공간 -> UV 공간 -> 블러벡터

texture			g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

texture		g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};


struct	PS_IN
{
	//vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	//vector		vProjPos : TEXCOORD1;
};

struct	PS_OUT
{
	vector		vColor : COLOR0;
	//vector		vNormal : COLOR1;	// 노말이라는 렌더타겟(텍스쳐)에 출력하기 위한 색상
	//vector		vDepth : COLOR2;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// 픽셀의 현재 월드 좌표를 얻는다.
	vector vDepth = tex2D(DepthSampler, In.vTexUV);
	float fViewZ = vDepth.y * 1000.f;

	vector vPosition;
	// texture U로 올수 있는 숫자의 범위는 0~1, 투영 X로 변환된 숫자의 범위는 -1~1
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	// texture V로 올수 있는 숫자의 범위는 (0)~(1), 투영 X로 변환된 숫자의 범위는 (1)~(-1)
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;		// vDepth.x = 투영 공간 z/카메라 공간 z(원근적용) 이므로 fViewZ을 곱하면 결국 투영공간z를 구한 것.
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matInvProjView);
	vPosition.w = 1.f;

	// 이전 스크린공간 상의 좌표를 얻는다.
	vector vPrevUV = mul(vPosition, g_matPrevViewProj);
	vPrevUV.xyz /= vPrevUV.w;
	vPrevUV.x = vPrevUV.x * 0.5f + 0.5f;
	vPrevUV.y = -vPrevUV.y * 0.5f + 0.5f;

	// 블러 벡터(이전 스크린 공간 좌표(UV화) - UV좌표)를 얻는다.
	float2 vBlur = vPrevUV.xy - In.vTexUV;
	vBlur.y *= -1.f;

	// 블러를 수행한다.
	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 얻어오는 함수, 반환 타입이 vector 타입
	Out.vColor.a = 1.f;

	float2 vOffset;
	for (int i = 1; i < 20; ++i) {
		// get offset in range [-0.5, 0.5]:
		vOffset = vBlur * (float(i) / (20.f - 1.f) - 0.5f);

		// sample & add to result:
		Out.vColor += tex2D(BaseSampler, In.vTexUV + vOffset);
	}

	Out.vColor /= 20.f;
	//Out.vColor = vector(vDepth.x * fViewZ * 0.01f, vDepth.x * fViewZ * 0.01f, vDepth.x * fViewZ * 0.01f, 1.f);

	return Out;

}


technique Default_Device
{
	// 기능의 캡슐화
	pass Default
	{
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};