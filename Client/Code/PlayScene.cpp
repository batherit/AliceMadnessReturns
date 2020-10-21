#include "pch.h"
#include "PlayScene.h"
#include "TerrainMap.h"
#include "Player.h"
#include "DynamicCamera.h"
#include "StaticCamera.h"

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

LRESULT CPlayScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	
	return LRESULT();
}

void CPlayScene::ResetScene(void)
{
	
}

HRESULT CPlayScene::Ready(void)
{
	FAILED_CHECK_RETURN(Ready_Resource(Engine::RESOURCE_END), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

int CPlayScene::Update(const _float& fTimeDelta)
{
	// TODO : 네모를 움직이는 코드를 작성합니다.
	_float fHeight = m_pTerrain->GetHeight(m_pPlayer->GetComponent<Engine::CMoveComponent>()->GetPos());
	m_pPlayer->GetComponent<Engine::CMoveComponent>()->SetY(fHeight);

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_LBUTTON")) {
		_vec3 vCameraPos = m_pCamera->GetComponent<Engine::CMoveComponent>()->GetPos();
		_vec3 vRay = Engine::GetRayVector(m_pGraphicDev, Engine::GetClientCursorPoint(g_hWnd));
		_float fU, fV, fDist;
		auto& pVertices = m_pTerrain->GetComponent<Engine::CTerrain>()->GetVertices();
		for (auto& pIndex : m_pTerrain->GetComponent<Engine::CTerrain>()->GetIndexes()) {
			if (D3DXIntersectTri(&pVertices[pIndex._0], &pVertices[pIndex._1], &pVertices[pIndex._2], &vCameraPos, &vRay, &fU, &fV, &fDist)) {
				m_pPlayer->SetTartgetPos(vCameraPos + vRay * fDist);
			}
		}
	}	

	return CScene::Update(fTimeDelta);
}

void CPlayScene::Render(void)
{
	CScene::Render();
}

CPlayScene * CPlayScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayScene*	pInstance = new CPlayScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

HRESULT CPlayScene::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 플레이어 생성
	m_pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", m_pPlayer), E_FAIL);

	// 카메라 생성
	//m_pCamera = CDynamicCamera::Create(m_pGraphicDev);		// 동적 카메라
	m_pCamera = CStaticCamera::Create(m_pGraphicDev);			// 정적 카메라
	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	m_pCamera->SetParent(m_pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", m_pCamera), E_FAIL);

	// 지형 생성
	m_pTerrain = CTerrainMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pTerrain, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", m_pTerrain), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CPlayScene::Ready_Resource(Engine::RESOURCETYPE eType)
{
	return S_OK;
}

void CPlayScene::Free(void)
{
	Client::Safe_Release(m_pPlayer);
	Client::Safe_Release(m_pCamera);
	Client::Safe_Release(m_pTerrain);
	CScene::Free();
}
