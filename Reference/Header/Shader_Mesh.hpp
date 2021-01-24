matrix			g_matWorld;		// 상수 테이블
matrix			g_matView;
float4x4		g_matProj;

texture			g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

struct	VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct	VS_OUT
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
};

// 버텍스 쉐이더

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct	PS_IN
{
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
};

struct	PS_OUT
{
	vector		vColor : COLOR0;
	vector		vNormal : COLOR1;	// 노말이라는 렌더타겟(텍스쳐)에 출력하기 위한 색상
	vector		vDepth : COLOR2;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 얻어오는 함수, 반환 타입이 vector 타입
	Out.vColor.a = 1.f;
	// (-1 ~ 1)값은 월드 상태의 법선 벡터를 정규화하였기 때문에 xyz값이 나올 수 있는 범위에 해당
	// (0 ~ 1) 텍스쳐 uv좌표로 변환
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}


struct	VS_IN_Outline
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct	VS_OUT_Outline
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};


VS_OUT_Outline VS_Outline(VS_IN_Outline In)
{
	VS_OUT_Outline		Out = (VS_OUT_Outline)0;

	matrix matWorld, matWV, matWVP;

	vector vOutPos = In.vNormal * 4.f + In.vPosition;
	matWorld = g_matWorld;
	matWorld._41 = vOutPos.x;
	matWorld._42 = vOutPos.y;
	matWorld._43 = vOutPos.z;

	matWV = mul(matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}


struct	PS_OUT_Outline
{
	vector		vOutline : COLOR0;
};

PS_OUT_Outline		PS_Outline(void)
{
	PS_OUT_Outline		Out = (PS_OUT_Outline)0;

	Out.vOutline = vector(0.f, 0.f, 0.f, 1.f);

	return Out;
}



//PS_OUT		PS_ALPHA(PS_IN In)
//{
//	PS_OUT		Out = (PS_OUT)0;
//
//	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 얻어오는 함수, 반환 타입이 vector 타입
//
//	return Out;
//}

technique Default_Device
{
	// 기능의 캡슐화
	pass Default
	{
		//zwriteenable = true;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass Outline
	{
		vertexshader = compile vs_3_0 VS_Outline();
		pixelshader = compile ps_3_0 PS_Outline();
	}

	//pass	AlphaTest
	//{
	//	alphatestenable = true;
	//	alpharef = 0xc0;
	//	alphafunc = greater;
	//	cullmode = none;

	//	vertexshader = compile vs_3_0 VS_MAIN();
	//	pixelshader = compile ps_3_0 PS_ALPHA();
	//}
};