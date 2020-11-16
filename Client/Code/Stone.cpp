#include "pch.h"
#include "Stone.h"
#include "Export_Function.h"

CStone::CStone(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_vDir(0.f, 0.f, 0.f)
{

}

CStone::~CStone(void)
{

}
//
//Client::_vec3 Client::CStone::PickUp_OnTerrain(void)
//{
//	Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Environment", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
//	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3(0.f, 0.f, 0.f));
//
//	Engine::CTransform*		pTerrainTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Environment", L"Terrain", L"Com_Transform", Engine::ID_DYNAMIC));
//	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3(0.f, 0.f, 0.f));
//
//	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
//}

HRESULT Client::CStone::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;
	
	// Mesh
	pComponent = m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Mesh_Stone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransform = AddComponent<Engine::CTransform>();

	// Renderer
	pComponent = m_pRenderer = AddComponent<Engine::CRenderer>();

	// Calculator
	/*pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);*/

	return S_OK;
}

void Client::CStone::Key_Input(const _float& fTimeDelta)
{

}

CStone* CStone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStone*	pInstance = new CStone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CStone::Free(void)
{
	Engine::CGameObject::Free();
}


HRESULT Client::CStone::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}
_int CStone::Update_Object(const _float & fTimeDelta)
{
	Key_Input(fTimeDelta);
	
	//CGameObject::Update_Object(fTimeDelta);
	//Engine::CGameObject::Update_Object(fTimeDelta);
	
	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}
//_int Client::CStone::Update_Object(const _float& fTimeDelta)
//{
//	//SetUp_OnTerrain();
//	Key_Input(fTimeDelta);
//
//	Engine::CGameObject::Update_Object(fTimeDelta);
//
//	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
//
//	return 0;
//}
void Client::CStone::Render_Object(void)
{
	//m_pTransformCom->Set_Transform(m_pGraphicDev);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());

	m_pMesh->Render_Meshes();

}
//void Client::CStone::SetUp_OnTerrain(void)
//{
//	_vec3	vPosition;
//	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPosition);
//
//	Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Environment", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
//	NULL_CHECK(pTerrainBufferCom);
//
//	_float fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPosition, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ, VTXITV);
//
//	m_pTransformCom->Move_Pos(vPosition.x, fHeight, vPosition.z);
//}