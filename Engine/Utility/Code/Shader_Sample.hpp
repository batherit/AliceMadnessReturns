matrix			g_matWorld;		// 상수 테이블
matrix			g_matView;
float4x4		g_matProj;

texture			g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;

	addressU = clamp;
	addressV = clamp;
};

struct	VS_IN 
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
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

	Out.vColor.rb = 1.f;

	return Out;
}

//VS_OUT VS_TEMP(VS_IN In);
//PS_OUT PS_TEMP(PS_IN In);

technique Default_Device
{
	// 기능의 캡슐화
	pass	
	{
		/*alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;*/

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	/*pass	
	{
		vertexshader = compile vs_3_0 VS_TEMP();
		pixelshader = compile ps_3_0 PS_TEMP();
	}*/
};