matrix			g_matWorld;		// ��� ���̺�
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

// ���ؽ� ���̴�

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

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� ������ �Լ�, ��ȯ Ÿ���� vector Ÿ��
	Out.vColor = vector(Out.vColor.x * g_vEffectColor.x, Out.vColor.y * g_vEffectColor.y, Out.vColor.z * g_vEffectColor.z, saturate(Out.vColor.a - g_fT));
	
	//if(fAlpha != 0.f)
	//	Out.vColor.a = saturate(1.f - g_fT);
	// (-1 ~ 1)���� ���� ������ ���� ���͸� ����ȭ�Ͽ��� ������ xyz���� ���� �� �ִ� ������ �ش�
	// (0 ~ 1) �ؽ��� uv��ǥ�� ��ȯ

	//Out.vColor = vector(1.f, 1.f, 1.f, 1.f);

	return Out;
}


//PS_OUT		PS_ALPHA(PS_IN In)
//{
//	PS_OUT		Out = (PS_OUT)0;
//
//	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� ������ �Լ�, ��ȯ Ÿ���� vector Ÿ��
//
//	return Out;
//}

technique Default_Device
{
	// ����� ĸ��ȭ
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