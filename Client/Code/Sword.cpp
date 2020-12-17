#include "pch.h"
#include "Sword.h"
#include "AliceW.h"
#include "Export_Function.h"

CSword::CSword(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CSword::~CSword(void)
{

}

HRESULT Client::CSword::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;
	
	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Mesh_Sword"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	//pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(L"Proto_Transform"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	//m_pTransformCom = AddComponent<Engine::CTransform>();

	// Renderer
	//pComponent = m_pRendererCom = Engine::Get_Renderer();
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//Safe_AddRef(pComponent);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom = AddComponent<Engine::CMeshRenderer>();
	m_pRendererCom->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMeshCom);

	//// Calculator
	//pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone(L"Proto_Calculator"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	//// Collider 
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_NumVtx(), m_pMeshCom->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}


CSword* CSword::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSword*	pInstance = new CSword(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CSword::Free(void)
{
	Engine::CGameObject::Free();
}


HRESULT CSword::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->RotateByAxis(D3DXToRadian(90.f), WORLD_X_AXIS);

	return S_OK;
}
_int CSword::Update_Object(const _float& fTimeDelta)
{
	//CPlayer* pPlayer = dynamic_cast<CPlayer*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());

	//if (nullptr == m_pParentBoneMatrix)
	//{
	//	Engine::CDynamicMesh*	pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pPlayer->GetComponent(Engine::ID_STATIC, L"Com_Mesh"));
	//		//dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Mesh", Engine::ID_STATIC));
	//	NULL_CHECK_RETURN(pPlayerMeshCom, 0);

	//	const Engine::D3DXFRAME_DERIVED* pFrame = pPlayerMeshCom->Get_FrameByName("R_Hand");

	//	m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

	//	//Engine::CTransform*		pPlayerTransCom = pPlayer->GetComponent<Engine::CTransform>();
	//	//NULL_CHECK_RETURN(pPlayerTransCom, 0);
	//	//m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();
	//}
	//m_ParentWorldMatrix = pPlayer->GetTransform()->GetObjectMatrix();
	//Engine::CGameObject::Update_Object(fTimeDelta);
		
	//m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));
	

	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");

	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pRendererCom->Update(fTimeDelta);

	return 0;
}
void CSword::Render_Object(void)
{
	//m_pTransformCom->Set_Transform(m_pGraphicDev);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &(GetTransform()->GetObjectMatrix() * *m_pParentBoneMatrix * m_ParentWorldMatrix));

	//m_pMeshCom->Render_Meshes();
	m_pRendererCom->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pRendererCom->Render();

	//_matrix matWorld;
	//	m_pTransformCom->Get_WorldMatrix(&matWorld);
		//m_pTransformCom->Get_NRotWorldMatrix(&matWorld);

	//m_pColliderCom->Render_Collider(Engine::COLLTYPE(m_bColl), &matWorld);


}


_bool CSword::Collision_ToObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	//Engine::CCollider*	pPlayerColliderCom = dynamic_cast<Engine::CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", Engine::ID_STATIC));
	//NULL_CHECK_RETURN(pPlayerColliderCom, false);
	//
	///*return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(),
	//										pPlayerColliderCom->Get_Max(),
	//										pPlayerColliderCom->Get_CollMatrix(),
	//										m_pColliderCom->Get_Min(), 
	//										m_pColliderCom->Get_Max(),
	//										m_pColliderCom->Get_CollMatrix());*/

	//return m_pCalculatorCom->Collision_OBB(pPlayerColliderCom->Get_Min(),
	//	pPlayerColliderCom->Get_Max(),
	//	pPlayerColliderCom->Get_CollMatrix(),
	//	m_pColliderCom->Get_Min(),
	//	m_pColliderCom->Get_Max(),
	//	m_pColliderCom->Get_CollMatrix());
	return false;
}
