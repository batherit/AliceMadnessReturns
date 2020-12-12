#include "pch.h"
#include "StaticObject.h"

CStaticObject::CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CStaticObject::CStaticObject(const CStaticObject & rhs)
	:
	CGameObject(rhs)
{
}

CStaticObject::~CStaticObject(void)
{
}

HRESULT CStaticObject::Ready_Object(void)
{
	// Mesh
	//if(m_pMesh)
	//	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", m_pMesh);

	// Renderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	//m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);
	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Object"));
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", m_pShader);


	return S_OK;
}

int CStaticObject::Update_Object(const _float & _fDeltaTime)
{
	m_pRenderer->Update(_fDeltaTime);

	return 0;
}

void CStaticObject::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	LPD3DXEFFECT	 pEffect = m_pShader->Get_EffectHandle();
	const D3DLIGHT9*		pLightInfo = Engine::Get_Light(0);

	//pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	m_vDir = Engine::GetRotatedVector(WORLD_X_AXIS, D3DXToRadian(30.f) * 0.1f, m_vDir);
	pEffect->SetVector("g_vLightDir", &_vec4(m_vDir, 0.f));
	pEffect->SetVector("g_LightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_LightAmbient", (_vec4*)&pLightInfo->Ambient);

	D3DMATERIAL9			tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 0.f;

	pEffect->SetVector("g_MtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_MtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);

	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pRenderer->Render(pEffect);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

CStaticObject * CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStaticObject*	pInstance = new CStaticObject(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

_bool CStaticObject::SetRenderInfo(const _tchar * _pMeshTag, Engine::RENDERID _eRenderID)
{
	if (!_pMeshTag || _eRenderID >= Engine::RENDER_END)
		return false;

	lstrcpy(m_tcMeshTag, _pMeshTag);
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, m_tcMeshTag));

	if (!m_pMesh)
		return false;

	Engine::CComponent* pComponent = m_mapComponent[Engine::ID_STATIC][L"Com_Mesh"];
	if (pComponent) {
		// 기존 세팅된 메쉬가 있다면 제거한다.
		Engine::Safe_Release(pComponent);
	}

	// 새로운 메시로 세팅한다.
	m_mapComponent[Engine::ID_STATIC][L"Com_Mesh"] = m_pMesh;
	m_pMesh->SetOwner(this);
	m_pRenderer->SetRenderInfo(_eRenderID, m_pMesh);
	return true;
}

void CStaticObject::Free(void)
{
	CGameObject::Free();
}

_bool CStaticObject::SaveInfo(HANDLE & _hfOut)
{
	DWORD dwByte = 0;

	// 1) 메시 태그
	_int iStrLen = lstrlen(m_tcMeshTag);
	WriteFile(_hfOut, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	WriteFile(_hfOut, &m_tcMeshTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);
	
	// 2) 렌더 아이디
	Engine::RENDERID eRenderID = m_pRenderer->GetRenderID();
	WriteFile(_hfOut, &eRenderID, sizeof(eRenderID), &dwByte, nullptr);

	// 3) 트랜스폼 정보 세팅
	_vec3 vPos = GetTransform()->GetPos();
	_vec3 vAngle = GetTransform()->GetAngle();
	_vec3 vScale = GetTransform()->GetScale();
	WriteFile(_hfOut, &vPos, sizeof(vPos), &dwByte, nullptr);
	WriteFile(_hfOut, &vAngle, sizeof(vAngle), &dwByte, nullptr);
	WriteFile(_hfOut, &vScale, sizeof(vScale), &dwByte, nullptr);

	return true;
}

_bool CStaticObject::LoadInfo(HANDLE & _hfIn)
{
	DWORD dwByte = 0;

	// 1) 메시 태그 로드
	_int iStrLen = 0;
	ReadFile(_hfIn, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	ReadFile(_hfIn, &m_tcMeshTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);

	// 2) 렌더 아이디
	Engine::RENDERID eRenderID = Engine::RENDER_END;
	ReadFile(_hfIn, &eRenderID, sizeof(eRenderID), &dwByte, nullptr);
	
	// 3) 트랜스폼 정보 세팅
	_vec3 vPos, vAngle, vScale;
	ReadFile(_hfIn, &vPos, sizeof(vPos), &dwByte, nullptr);
	ReadFile(_hfIn, &vAngle, sizeof(vAngle), &dwByte, nullptr);
	ReadFile(_hfIn, &vScale, sizeof(vScale), &dwByte, nullptr);
	GetTransform()->SetPos(vPos);
	GetTransform()->SetAngle(vAngle);
	GetTransform()->SetScale(vScale);
	
	return SetRenderInfo(m_tcMeshTag, eRenderID);
}
