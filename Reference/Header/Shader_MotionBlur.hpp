matrix			g_matInvProjView;		// ���� �ؽ��ķκ��� ���� ��ǥ�� ���ϱ� ����

matrix			g_matPrevViewProj;	// ���� ���� ��ǥ -> ��ũ�� ���� -> UV ���� -> ������

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
	//vector		vNormal : COLOR1;	// �븻�̶�� ����Ÿ��(�ؽ���)�� ����ϱ� ���� ����
	//vector		vDepth : COLOR2;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// �ȼ��� ���� ���� ��ǥ�� ��´�.
	vector vDepth = tex2D(DepthSampler, In.vTexUV);
	float fViewZ = vDepth.y * 1000.f;

	vector vPosition;
	// texture U�� �ü� �ִ� ������ ������ 0~1, ���� X�� ��ȯ�� ������ ������ -1~1
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	// texture V�� �ü� �ִ� ������ ������ (0)~(1), ���� X�� ��ȯ�� ������ ������ (1)~(-1)
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;		// vDepth.x = ���� ���� z/ī�޶� ���� z(��������) �̹Ƿ� fViewZ�� ���ϸ� �ᱹ ��������z�� ���� ��.
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matInvProjView);
	vPosition.w = 1.f;

	// ���� ��ũ������ ���� ��ǥ�� ��´�.
	vector vPrevUV = mul(vPosition, g_matPrevViewProj);
	vPrevUV.xyz /= vPrevUV.w;
	vPrevUV.x = vPrevUV.x * 0.5f + 0.5f;
	vPrevUV.y = -vPrevUV.y * 0.5f + 0.5f;

	// �� ����(���� ��ũ�� ���� ��ǥ(UVȭ) - UV��ǥ)�� ��´�.
	float2 vBlur = vPrevUV.xy - In.vTexUV;
	vBlur.y *= -1.f;

	// ���� �����Ѵ�.
	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� ������ �Լ�, ��ȯ Ÿ���� vector Ÿ��
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
	// ����� ĸ��ȭ
	pass Default
	{
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};