matrix			g_matWorld;		// ��� ���̺�
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

// ���ؽ� ���̴�

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
	vector		vNormal : COLOR1;	// �븻�̶�� ����Ÿ��(�ؽ���)�� ����ϱ� ���� ����
	vector		vDepth : COLOR2;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� ������ �Լ�, ��ȯ Ÿ���� vector Ÿ��
	Out.vColor.a = 1.f;
	// (-1 ~ 1)���� ���� ������ ���� ���͸� ����ȭ�Ͽ��� ������ xyz���� ���� �� �ִ� ������ �ش�
	// (0 ~ 1) �ؽ��� uv��ǥ�� ��ȯ
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
//	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� ������ �Լ�, ��ȯ Ÿ���� vector Ÿ��
//
//	return Out;
//}

technique Default_Device
{
	// ����� ĸ��ȭ
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