#include "pch.h"
#include "PlayScene.h"
#include "Terrain.h"
#include "Alice.h"
#include "Monster.h"
#include "Stone.h"
#include "Sword.h"
#include "SphereRenderer.h"
#include "SkyBox.h"
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "Map.h"

CPlayScene::CPlayScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CPlayScene::CPlayScene(const CPlayScene & rhs)
	:
	CScene(rhs)
{
}

CPlayScene::~CPlayScene(void)
{
}

//LRESULT CPlayScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	
//	return LRESULT();
//}

void CPlayScene::ResetScene(void)
{
	
}

HRESULT CPlayScene::Ready(void)
{
	FAILED_CHECK_RETURN(Ready_Resource(Engine::RESOURCE_END), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

int CPlayScene::Update(const _float& fTimeDelta)
{
	// TODO : �׸� �����̴� �ڵ带 �ۼ��մϴ�.
	//_float fHeight = m_pTerrain->GetHeight(m_pPlayer->GetComponent<Engine::CTransform>()->GetPos());
	//m_pPlayer->GetComponent<Engine::CTransform>()->SetPosY(fHeight);

	//if (Engine::CDirectInputMgr::GetInstance()->GetDeltaMouseDegree().z != 0) {
	//	_int iW = m_pTerrain->GetComponent<Engine::CTerrainTex>()->GetNumOfVerticesW();
	//	_int iH = m_pTerrain->GetComponent<Engine::CTerrainTex>()->GetNumOfVerticesH();
	//	if (Engine::CDirectInputMgr::GetInstance()->GetDeltaMouseDegree().z > 0) {
	//		m_pTerrain->CreateTerrain(iW + 1, iH + 1, 129.f, 129.f, 1.f, 1.f, L"../Bin/Resource/Texture/Terrain/Height2.bmp");
	//	}
	//	else {
	//		if(iW > 10 && iH > 10)
	//			m_pTerrain->CreateTerrain(iW - 1, iH - 1, 129.f, 129.f, 1.f, 1.f, L"../Bin/Resource/Texture/Terrain/Height2.bmp");
	//	}
	//}

	//if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(Engine::DIM_LB)) {
	//	// ��ŷ�� �ϱ� ���� �⺻ ������ ����.
	//	auto& pVertices = m_pTerrain->GetComponent<Engine::CTerrainTex>()->GetVertices();
	//	auto stPickingRayInfo = Engine::GetPickingRayInfo(m_pGraphicDev, Engine::GetClientCursorPoint(g_hWnd));
	//	_float fU, fV, fDist;
	//	_vec3 vV1, vV2, vV3;
	//	
	//	// ��ŷ �˻縦 �����Ѵ�.
	//	for (auto& pIndex : m_pTerrain->GetComponent<Engine::CTerrainTex>()->GetIndexes()) {
	//		vV1 = pVertices[pIndex._0];
	//		vV2 = pVertices[pIndex._1];
	//		vV3 = pVertices[pIndex._2];
	//		if (D3DXIntersectTri(&vV1, &vV2, &vV3, &stPickingRayInfo.vRayPos, &stPickingRayInfo.vRayDir, &fU, &fV, &fDist)) {
	//			// ��ŷ�� �����ߴٸ�, ��ŷ ��Ʈ ������ �÷��̾� �̵� �������� ����
	//			//m_pPlayer->SetTartgetPos(stPickingRayInfo.vRayPos + stPickingRayInfo.vRayDir * fDist);
	//			m_pPlayer->SetTartgetPos(Engine::GetHitPos(vV1, vV2, vV3, fU, fV));
	//			break;
	//		}
	//	}
	//}	

	return CScene::Update(fTimeDelta);
}

void CPlayScene::Render(void)
{
	Engine::Get_Renderer()->Render_GameObject();

	Engine::Render_Font(L"Font_Jinji", L"Text Test", &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	//CScene::Render();
}

CPlayScene * CPlayScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayScene*	pInstance = new CPlayScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

HRESULT CPlayScene::Ready_LightInfo()
{
	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayScene::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.emplace(pLayerTag, pLayer);

	// �� ����
	CMap* pMap = CMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pMap, E_FAIL);
	pMap->LoadMap(
		pLayer,
		L"../../Resource/Terrain/Terrain.trr",
		L"../../Resource/Navi/NaviTest.navi",
		L"../../Resource/Map/Map.map"
	);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pMap), E_FAIL);

	// �÷��̾� ����
	m_pPlayer = CAlice::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", m_pPlayer), E_FAIL);
	m_pPlayer->GetTransform()->Translate(_vec3(45.f, 0.f, -10.f));
	
	// ī�޶� ����
	m_pCamera = CDynamicCamera::Create(m_pGraphicDev);		// ���� ī�޶�
	//m_pCamera = CStaticCamera::Create(m_pGraphicDev);			// ���� ī�޶�
	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	//m_pCamera->SetParent(m_pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", m_pCamera), E_FAIL);

	// ��ī�� �ڽ� ����
	m_pSkyBox = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pSkyBox, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", m_pSkyBox), E_FAIL);

	return S_OK;
}

HRESULT CPlayScene::Ready_Resource(Engine::RESOURCETYPE eType)
{
	return S_OK;
}

void CPlayScene::Free(void)
{
	//Client::Safe_Release(m_pPlayer);
	//Client::Safe_Release(m_pMonster);
	//Client::Safe_Release(m_pSkyBox);
	//Client::Safe_Release(m_pCamera);
	//Client::Safe_Release(m_pTerrain);
	CScene::Free();
}
