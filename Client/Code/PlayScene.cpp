#include "pch.h"
#include "PlayScene.h"
#include "TerrainMap.h"
#include "Player.h"
#include "Monster.h"
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

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

int CPlayScene::Update(const _float& fTimeDelta)
{
	// TODO : 네모를 움직이는 코드를 작성합니다.
	_float fHeight = m_pTerrain->GetHeight(m_pPlayer->GetComponent<Engine::CTransform>()->GetPos());
	m_pPlayer->GetComponent<Engine::CTransform>()->SetPosY(fHeight);

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_LBUTTON")) {
		// 픽킹을 하기 위한 기본 변수들 세팅.
		auto& pVertices = m_pTerrain->GetComponent<Engine::CTerrain>()->GetVertices();
		auto stPickingRayInfo = Engine::GetPickingRayInfo(m_pGraphicDev, Engine::GetClientCursorPoint(g_hWnd));
		_float fU, fV, fDist;
		_vec3 vV1, vV2, vV3;
		
		// 픽킹 검사를 진행한다.
		for (auto& pIndex : m_pTerrain->GetComponent<Engine::CTerrain>()->GetIndexes()) {
			vV1 = pVertices[pIndex._0];
			vV2 = pVertices[pIndex._1];
			vV3 = pVertices[pIndex._2];
			if (D3DXIntersectTri(&vV1, &vV2, &vV3, &stPickingRayInfo.vRayPos, &stPickingRayInfo.vRayDir, &fU, &fV, &fDist)) {
				// 픽킹이 성공했다면, 픽킹 히트 지점을 플레이어 이동 지점으로 세팅
				//m_pPlayer->SetTartgetPos(stPickingRayInfo.vRayPos + stPickingRayInfo.vRayDir * fDist);
				m_pPlayer->SetTartgetPos(Engine::GetHitPos(vV1, vV2, vV3, fU, fV));
				break;
			}
		}
	}	

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

HRESULT CPlayScene::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 플레이어 생성
	m_pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", m_pPlayer), E_FAIL);

	// 카메라 생성
	m_pCamera = CDynamicCamera::Create(m_pGraphicDev);		// 동적 카메라
	//m_pCamera = CStaticCamera::Create(m_pGraphicDev);			// 정적 카메라
	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	//m_pCamera->SetParent(m_pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", m_pCamera), E_FAIL);

	// 지형 생성
	m_pTerrain = CTerrainMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pTerrain, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", m_pTerrain), E_FAIL);

	// 몬스터 생성
	m_pMonster = CMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pMonster, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", m_pMonster), E_FAIL);
	
	// 몬스터에 카메라를 세팅함.
	m_pMonster->SetCameraForBillboard(m_pCamera);

	// 몬스터를 임의의 위치에 둠.
	m_pMonster->GetComponent<Engine::CTransform>()->SetPos(
		Engine::GetNumberMinBetweenMax(10.f, 20.f),
		0.f,
		Engine::GetNumberMinBetweenMax(10.f, 20.f)
	);
	_float fHeight = m_pTerrain->GetHeight(m_pMonster->GetComponent<Engine::CTransform>()->GetPos());
	m_pMonster->GetComponent<Engine::CTransform>()->SetPosY(fHeight);

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
	Client::Safe_Release(m_pMonster);
	Client::Safe_Release(m_pCamera);
	Client::Safe_Release(m_pTerrain);
	CScene::Free();
}
