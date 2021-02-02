#include "pch.h"
#include "CreditScene.h"
#include "StaticCamera.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "TitleAlice.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_FadeInOut.h"
#include "UI_OrthoPlane.h"
#include "UI_Cursor.h"
#include "Attribute.h"
#include "GiantAliceW.h"
#include "TitleScene.h"
//#include "PlayScene.h"
#include "PoolScene.h"
#include "Map.h"
#include "SkyBox.h"

CCreditScene::CCreditScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CCreditScene::CCreditScene(const CCreditScene & rhs)
	:
	CScene(rhs)
{
}

CCreditScene::~CCreditScene(void)
{
}

//LRESULT CCreditScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	
//	return LRESULT();
//}

void CCreditScene::ResetScene(void)
{

}

HRESULT CCreditScene::Ready(void)
{


	return S_OK;
}

int CCreditScene::Update(const _float& fTimeDelta)
{
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_NUMPADENTER)) {
		Engine::CManagement::GetInstance()->SetNextScene(CTitleScene::Create(m_pGraphicDev));
	}

	_vec3 vPos = m_pFin->GetTransform()->GetPos();
	vPos += _vec3((WINCX >> 1) - vPos.x, (WINCY >> 1) - vPos.y, 0.f) * 0.1f;
	m_pFin->SetPos(static_cast<LONG>(vPos.x), static_cast<LONG>(vPos.y));
	m_pFin->SetRenderColor(D3DCOLOR_ARGB(static_cast<_int>(Engine::GetValueByWeight((vPos.y - (WINCY >> 1)) / 40.f, 255.f, 0.f)), 255, 255, 255));

	return CScene::Update(fTimeDelta);
}

void CCreditScene::Render()
{
	Engine::CScene::Render();
	Engine::Render_Font(L"Font_Jinji", L"By. 110C ½ÉÁ¤È¯", &_vec2(WINCX * 0.25f, (WINCY >> 1) + 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), &_vec2(0.7f, 0.7f));
}

void CCreditScene::OnLoaded()
{
	Engine::CCollisionMgr::GetInstance()->ClearGameObjectList();
	Engine::CCollisionMgr::GetInstance()->SetColliderVisible(false);

	FAILED_CHECK_RETURN(Ready_LightInfo(), );
	//	FAILED_CHECK_RETURN(Ready_Resource(Engine::RESOURCE_Credit), );
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), );

	CSoundMgr::Get_Instance()->PlayBGM(L"Title.ogg");
}

void CCreditScene::OnExited()
{
}

CCreditScene * CCreditScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCreditScene*	pInstance = new CCreditScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

//HRESULT CCreditScene::Ready_LightInfo()
//{
//	D3DLIGHT9		tLightInfo;
//	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));
//
//	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
//
//	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
//	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
//	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
//
//	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);
//
//	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
//		return E_FAIL;
//
//	return S_OK;
//}

HRESULT CCreditScene::Ready_LightInfo()
{
	Engine::ClearLights();

	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	tLightInfo.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);

	tLightInfo.Direction = _vec3(-1.f, -1.f, 1.f);

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCreditScene::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.emplace(pLayerTag, pLayer);

	m_pBackground = CUI_Image::Create(m_pGraphicDev);
	m_pBackground->SetTexture(L"UI_Layer");
	m_pBackground->SetRenderColor(D3DCOLOR_ARGB(255, 0, 0, 0));
	m_pBackground->SetOutputArea(RECT{ 0, 0, WINCX, WINCY });
	pLayer->Add_GameObject(m_pBackground);

	m_pFin = CUI_Image::Create(m_pGraphicDev);
	m_pFin->SetTexture(L"UI_Fin");
	m_pFin->SetPos(WINCX >> 1, (WINCY >> 1) + 40);
	pLayer->Add_GameObject(m_pFin);

	return S_OK;
}

void CCreditScene::Free(void)
{
	CScene::Free();
}