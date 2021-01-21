texture		g_NormalTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

vector		g_vLightDir;	// 월드 스페이스 공간에 있는 방향
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
	// 노말 값이 텍스쳐 uv(0 ~ 1)값인 상태

	// 월드 스페이스 공간의 정규화가 이뤄진 노말 벡터로 변경
	// 텍스쳐uv -> 월드 스페이스 값으로 ((0 -> -1) , (1 -> 1))
	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	// 램버트의 확산 조명공식을 위한 빛의 세기를 구함
	Out.vShade = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	// 반사 벡터를 구함
	vector		vReflect = normalize(reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal));

	vector		vDepth = tex2D(DepthSampler, In.vTexUV);

	float		fViewZ = vDepth.y * 1000.f;	// 카메라 공간상에서의 z깊이

	vector vPosition; // 월드 영역에 위치여야 한다.

	// 텍스쳐 UV를 우선 투영 좌표로 변환
	// 텍스쳐 UV : 0,0 ~ 1,1
	// 투영 좌표 : -1,1 ~ 1,-1 

	// texture U로 올수 있는 숫자의 범위는 0~1, 투영 X로 변환된 숫자의 범위는 -1~1
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	// texture V로 올수 있는 숫자의 범위는 (0)~(1), 투영 X로 변환된 숫자의 범위는 (1)~(-1)
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;		// vDepth.x = 투영 공간 z/카메라 공간 z(원근적용) 이므로 fViewZ을 곱하면 결국 투영공간z를 구한 것.
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
	// 기능의 캡슐화
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