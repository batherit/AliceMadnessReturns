#include "pch.h"
#include "Terrain.h"
#include "Shader.h"


CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	:
	CGameObject(rhs)
{
}

CTerrain::~CTerrain(void)
{
}


HRESULT CTerrain::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CPolygonRenderer>();

	m_pTerrain = AddComponent<Engine::CTerrainTex>();

	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pTerrain, nullptr);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", m_pShader);

	return S_OK;
}

int CTerrain::Update_Object(const _float & fTimeDelta)
{
	//m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pRenderer->Update(fTimeDelta);

	return 0;
}

void CTerrain::Render_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	//Engine::Render_Texture(Engine::RESOURCE_STAGE, m_szTextureTag, 0);
	//m_pTerrain->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	LPD3DXEFFECT	 pEffect = m_pShader->Get_EffectHandle();
	//const D3DLIGHT9*		pLightInfo = Engine::Get_Light(0);

	//pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	//m_vDir = Engine::GetRotatedVector(WORLD_Z_AXIS, D3DXToRadian(30.f) * 0.1f, m_vDir);
	//pEffect->SetVector("g_vLightDir", &_vec4(m_vDir, 0.f));
	//pEffect->SetVector("g_LightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	//pEffect->SetVector("g_LightAmbient", (_vec4*)&pLightInfo->Ambient);

	/*D3DMATERIAL9			tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 0.f;

	pEffect->SetVector("g_MtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_MtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);*/

	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pRenderer->Render(pEffect);

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	//LPD3DXEFFECT	 pEffect = m_pShader->Get_EffectHandle();
	//NULL_CHECK(pEffect);
	//Engine::Safe_AddRef(pEffect);

	//_uint	iMaxPass = 0;

	//pEffect->Begin(&iMaxPass, 0);	// 현재 쉐이더 파일이 갖고 있는 최대 패스의 개수를 리턴, 사용하는 방식
	//pEffect->BeginPass(0);

	//FAILED_CHECK_RETURN(SetUpConstantTable(pEffect), );

	//m_pBuffer->Render_Buffer();

	//pEffect->EndPass();
	//pEffect->End();

	//Engine::Safe_Release(pEffect);
}

void CTerrain::CreateTerrain(_uint _iNumOfVerticesW, _uint _iNumOfVerticesD, _float _fWidth, _float _fDepth, _float _fU, _float _fV, const _tchar * _szHeightMapFileName)
{
	m_pTerrain->SetTerrainInfo(_iNumOfVerticesW, _iNumOfVerticesD, _fWidth, _fDepth, _fU, _fV, _szHeightMapFileName);
}

void CTerrain::SetHeightMapFileName(const _tchar * _szHeightMapFileName)
{
	if (!_szHeightMapFileName)
		return;
	lstrcpy(m_szHeightMapFileName, _szHeightMapFileName);
	m_pTerrain->SetTerrainInfo(
		m_pTerrain->GetNumOfVerticesW(),
		m_pTerrain->GetNumOfVerticesH(),
		m_pTerrain->GetWidth(),
		m_pTerrain->GetHeight(),
		m_pTerrain->GetU(),
		m_pTerrain->GetV(),
		_szHeightMapFileName);
}

void CTerrain::SetVtxNum(_uint _iNumOfVerticesW, _uint _iNumOfVerticesD)
{
	if (_iNumOfVerticesW < 2)
		_iNumOfVerticesW = 2;
	if (_iNumOfVerticesD < 2)
		_iNumOfVerticesD = 2;
	m_pTerrain->SetTerrainInfo(
		_iNumOfVerticesW,
		_iNumOfVerticesD,
		m_pTerrain->GetWidth(),
		m_pTerrain->GetHeight(),
		m_pTerrain->GetU(),
		m_pTerrain->GetV(),
		m_szHeightMapFileName);
}

void CTerrain::SetSize(_float _fWidth, _float _fDepth)
{
	m_pTerrain->SetTerrainInfo(
		m_pTerrain->GetNumOfVerticesW(),
		m_pTerrain->GetNumOfVerticesH(),
		_fWidth,
		_fDepth,
		m_pTerrain->GetU(),
		m_pTerrain->GetV(),
		m_szHeightMapFileName);
}

void CTerrain::SetUV(_float _fU, _float _fV)
{
	m_pTerrain->SetTerrainInfo(
		m_pTerrain->GetNumOfVerticesW(),
		m_pTerrain->GetNumOfVerticesH(),
		m_pTerrain->GetWidth(),
		m_pTerrain->GetHeight(),
		_fU,
		_fV,
		m_szHeightMapFileName);
}

void CTerrain::SetTextureTag(const _tchar * _pTextureTag)
{
	if (!_pTextureTag) {
		lstrcpy(m_szTextureTag, L"");
		m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pTerrain, nullptr);
		return;
	}
	lstrcpy(m_szTextureTag, _pTextureTag);
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pTerrain, static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STAGE, m_szTextureTag)));
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{
	CGameObject::Free();
}

_bool CTerrain::SaveInfo(HANDLE & _hfOut)
{
	DWORD dwByte = 0;

	// 1) 높이맵 태그
	_int iStrLen = lstrlen(m_szHeightMapFileName);
	WriteFile(_hfOut, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	WriteFile(_hfOut, &m_szHeightMapFileName, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);

	// 2) 텍스쳐 태그
	iStrLen = lstrlen(m_szTextureTag);
	WriteFile(_hfOut, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	WriteFile(_hfOut, &m_szTextureTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);

	// 3) 트랜스폼 정보 세팅
	_vec3 vPos = GetTransform()->GetPos();
	_vec3 vAngle = GetTransform()->GetAngle();
	_vec3 vScale = GetTransform()->GetScale();
	WriteFile(_hfOut, &vPos, sizeof(vPos), &dwByte, nullptr);
	WriteFile(_hfOut, &vAngle, sizeof(vAngle), &dwByte, nullptr);
	WriteFile(_hfOut, &vScale, sizeof(vScale), &dwByte, nullptr);

	// 4) 터레인 사이즈
	_float fWidth = m_pTerrain->GetWidth(); 
	_float fHeight = m_pTerrain->GetHeight();
	WriteFile(_hfOut, &fWidth, sizeof(fWidth), &dwByte, nullptr);
	WriteFile(_hfOut, &fHeight, sizeof(fHeight), &dwByte, nullptr);

	// 5) 터레인 정점 수
	_uint uiVtxNumW = m_pTerrain->GetNumOfVerticesW();
	_uint uiVtxNumD = m_pTerrain->GetNumOfVerticesH();
	WriteFile(_hfOut, &uiVtxNumW, sizeof(uiVtxNumW), &dwByte, nullptr);
	WriteFile(_hfOut, &uiVtxNumD, sizeof(uiVtxNumD), &dwByte, nullptr);

	// 6) 터레인 UV
	_float fU = m_pTerrain->GetU();
	_float fV = m_pTerrain->GetV();
	WriteFile(_hfOut, &fU, sizeof(fU), &dwByte, nullptr);
	WriteFile(_hfOut, &fV, sizeof(fV), &dwByte, nullptr);

	return true;
}

_bool CTerrain::LoadInfo(HANDLE & _hfIn)
{
	DWORD dwByte = 0;

	// 1) 높이맵 태그
	_int iStrLen = 0;
	ReadFile(_hfIn, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	ReadFile(_hfIn, &m_szHeightMapFileName, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);

	// 2) 텍스쳐 태그
	iStrLen = 0;
	ReadFile(_hfIn, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	ReadFile(_hfIn, &m_szTextureTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);

	// 3) 트랜스폼 정보 세팅
	_vec3 vPos, vAngle, vScale;
	ReadFile(_hfIn, &vPos, sizeof(vPos), &dwByte, nullptr);
	ReadFile(_hfIn, &vAngle, sizeof(vAngle), &dwByte, nullptr);
	ReadFile(_hfIn, &vScale, sizeof(vScale), &dwByte, nullptr);
	GetTransform()->SetPos(vPos);
	GetTransform()->SetAngle(vAngle);
	GetTransform()->SetScale(vScale);

	// 4) 터레인 사이즈
	_float fWidth, fHeight;
	ReadFile(_hfIn, &fWidth, sizeof(fWidth), &dwByte, nullptr);
	ReadFile(_hfIn, &fHeight, sizeof(fHeight), &dwByte, nullptr);

	// 5) 터레인 정점 수
	_uint uiVtxNumW, uiVtxNumD;
	ReadFile(_hfIn, &uiVtxNumW, sizeof(uiVtxNumW), &dwByte, nullptr);
	ReadFile(_hfIn, &uiVtxNumD, sizeof(uiVtxNumD), &dwByte, nullptr);

	// 6) 터레인 UV
	_float fU, fV;
	ReadFile(_hfIn, &fU, sizeof(fU), &dwByte, nullptr);
	ReadFile(_hfIn, &fV, sizeof(fV), &dwByte, nullptr);

	CreateTerrain(uiVtxNumW, uiVtxNumD, fWidth, fHeight, fU, fV, m_szHeightMapFileName);
	SetTextureTag(m_szTextureTag);

	return true;
}
