matrix			g_matWorld;		// 상수 테이블
matrix			g_matView;
float4x4		g_matProj;

//vector			g_vPlayerPos;

//float			g_fDetail;
//float			g_fRange;

texture			g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

//texture			g_BaseTexture1;

//sampler BaseSampler1 = sampler_state
//{
//	texture = g_BaseTexture1;
//
//	minfilter = linear;
//	magfilter = linear;
//
//	addressU = mirror;
//	addressV = mirror;
//};

//texture			g_FilterTexture;
//
//sampler FilterSampler = sampler_state
//{
//	texture = g_FilterTexture;
//
//	minfilter = linear;
//	magfilter = linear;
//};

//texture			g_AuraTexture;
//
//sampler AuraSampler = sampler_state
//{
//	texture = g_AuraTexture;
//
//	minfilter = linear;
//	magfilter = linear;
//};

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
	vector		vAuraPixelPos : TEXCOORD1;
	vector		vProjPos : TEXCOORD2;
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
	Out.vAuraPixelPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	Out.vProjPos = Out.vPosition;


	return Out;
}


struct	PS_IN
{
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vAuraPixelPos : TEXCOORD1;
	vector		vProjPos : TEXCOORD2;
};

struct	PS_OUT
{
	vector		vColor : COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth : COLOR2;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV /** g_fDetail*/);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 얻어오는 함수, 반환 타입이 vector 타입
	//vector vColor1 = tex2D(BaseSampler1, In.vTexUV * g_fDetail);

//	vector vFilter = tex2D(FilterSampler, In.vTexUV);

	//Out.vColor = vColor * vFilter + vColor1 * (1.f - vFilter);
	Out.vColor.a = 1.f;

	//vector		vAuraColor = (vector)0.f;

	//if (g_vPlayerPos.x - g_fRange < In.vAuraPixelPos.x &&
	//	g_vPlayerPos.x + g_fRange > In.vAuraPixelPos.x &&
	//	g_vPlayerPos.z - g_fRange < In.vAuraPixelPos.z &&
	//	g_vPlayerPos.z + g_fRange > In.vAuraPixelPos.z)
	//{
	//	float2 vTexUV;

	//	// 0 ~ 1로 만들기 위한 공식
	//	vTexUV.x = (In.vAuraPixelPos.x - (g_vPlayerPos.x - g_fRange)) / (g_fRange * 2.f);
	//	vTexUV.y = ((g_vPlayerPos.z + g_fRange) - In.vAuraPixelPos.z) / (g_fRange * 2.f);

	//	vAuraColor = tex2D(AuraSampler, vTexUV);
	//}

	//Out.vColor += vAuraColor;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	// vector타입의 멤버 변수 중, Z값과 W값은 사용하지 않을 예정
	// vDepth라는 자료형의 멤버 변수들은 데이터를 저장하는 구조체 용도일 뿐

	// x에는 z나누기가 끝난 z값을 저장(In.vProjPos.w 는 뷰스페이스 상태의 z값)
	// y에는 뷰스페이스 상태의 z값을 텍스쳐 uv 값으로 변환시켜 저장한 상태

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

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