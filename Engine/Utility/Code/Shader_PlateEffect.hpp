matrix			g_matWorld;		// 상수 테이블
matrix			g_matView;
float4x4		g_matProj;

float			g_fT;
vector			g_vEffectColor;

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
	float2		vTexUV : TEXCOORD0;
};

// 버텍스 쉐이더

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}


struct	PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct	PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 얻어오는 함수, 반환 타입이 vector 타입
	Out.vColor = vector(Out.vColor.x * g_vEffectColor.x, Out.vColor.y * g_vEffectColor.y, Out.vColor.z * g_vEffectColor.z, saturate(Out.vColor.a - g_fT));
	
	//if(fAlpha != 0.f)
	//	Out.vColor.a = saturate(1.f - g_fT);
	// (-1 ~ 1)값은 월드 상태의 법선 벡터를 정규화하였기 때문에 xyz값이 나올 수 있는 범위에 해당
	// (0 ~ 1) 텍스쳐 uv좌표로 변환

	//Out.vColor = vector(1.f, 1.f, 1.f, 1.f);

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
	zwriteenable = false;

	alphablendenable = true;
	blendop = add;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	cullmode = none;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN();
	}

//pass	AlphaTest
//{
//	alphatestenable = true;
//	alpharef = 0xc0;
//	alphafunc = greater;
//	cullmode = none;
//
//	vertexshader = compile vs_3_0 VS_MAIN();
//	pixelshader = compile ps_3_0 PS_ALPHA();
//}
};