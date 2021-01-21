texture		g_NormalTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

vector		g_vLightDir;	// ���� �����̽� ������ �ִ� ����
vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse = (vector)1.f;
vector		g_vMtrlAmbient = (vector)1.f;

float		g_fPower = 10.f;
vector		g_vCamPos;

vector		g_vLightPos;
float		g_fRange;


texture		g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

matrix		g_matViewInv;
matrix		g_matProjInv;


struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vShade : COLOR0;
	vector			vSpecular : COLOR1;
};

PS_OUT PS_DIRECTIONAL(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vNormal = tex2D(NormalSampler, In.vTexUV);
	// �븻 ���� �ؽ��� uv(0 ~ 1)���� ����

	// ���� �����̽� ������ ����ȭ�� �̷��� �븻 ���ͷ� ����
	// �ؽ���uv -> ���� �����̽� ������ ((0 -> -1) , (1 -> 1))
	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	// ����Ʈ�� Ȯ�� ��������� ���� ���� ���⸦ ����
	Out.vShade = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	// �ݻ� ���͸� ����
	vector		vReflect = normalize(reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal));

	vector		vDepth = tex2D(DepthSampler, In.vTexUV);

	float		fViewZ = vDepth.y * 1000.f;	// ī�޶� �����󿡼��� z����

	vector vPosition; // ���� ������ ��ġ���� �Ѵ�.

	// �ؽ��� UV�� �켱 ���� ��ǥ�� ��ȯ
	// �ؽ��� UV : 0,0 ~ 1,1
	// ���� ��ǥ : -1,1 ~ 1,-1 

	// texture U�� �ü� �ִ� ������ ������ 0~1, ���� X�� ��ȯ�� ������ ������ -1~1
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	// texture V�� �ü� �ִ� ������ ������ (0)~(1), ���� X�� ��ȯ�� ������ ������ (1)~(-1)
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;		// vDepth.x = ���� ���� z/ī�޶� ���� z(��������) �̹Ƿ� fViewZ�� ���ϸ� �ᱹ ��������z�� ���� ��.
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector		vLook = normalize(g_vCamPos - vPosition);

	Out.vSpecular = pow(saturate(dot(vLook, vReflect)), g_fPower);

	return Out;
}

PS_OUT PS_POINT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vNormal = tex2D(NormalSampler, In.vTexUV);
	vector		vDepth = tex2D(DepthSampler, In.vTexUV);
	float		fViewZ = vDepth.y * 1000.f;

	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	vector vPosition;

	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector vLightDir = vPosition - g_vLightPos;
	float  fDistance = length(vLightDir);

	float	fAtt = saturate((g_fRange - fDistance) / g_fRange);

	Out.vShade = (saturate(dot(normalize(vLightDir) * -1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
	Out.vShade.a = 1.f;

	vector		vReflect = normalize(reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal));

	vector		vLook = normalize(g_vCamPos - vPosition);

	Out.vSpecular = pow(saturate(dot(vLook, vReflect)), g_fPower) * fAtt;

	return Out;
}

technique Default_Device
{
	// ����� ĸ��ȭ
	pass Direction
	{
		zwriteenable = false;

		alphablendenable = true;
		srcblend = one;
		destblend = one;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_DIRECTIONAL();
	}

	pass Point
	{
		zwriteenable = false;

		alphablendenable = true;
		srcblend = one;
		destblend = one;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_POINT();
	}
};