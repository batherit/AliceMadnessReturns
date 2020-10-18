#include "pch.h"
#include "CPlayScene.h"
#include "CLogoScene.h"
#include "CLogoObject.h"
#include "CMonster.h"

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

void CPlayScene::ResetScene(void)
{

}

HRESULT CPlayScene::Ready(void)
{
	// 3) 텍스처 사각형
	//FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_Logo", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Logo.jpg"), E_FAIL);

	m_pLogoObject = CLogoObject::Create(m_pGraphicDev);
	m_pMonster = CMonster::Create(m_pGraphicDev, m_pLogoObject);

	return S_OK;
}

int CPlayScene::Update(const _float& fTimeDelta)
{
	// TODO : 네모를 움직이는 코드를 작성합니다.
	m_pLogoObject->Update_Object(fTimeDelta);
	m_pMonster->Update_Object(fTimeDelta);

	/*if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_RBUTTON)) {
		Engine::CManagement::GetInstance()->SetNextScene(CLogoScene::Create(m_pGraphicDev));
	}*/
	/*if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_A)) {
		m_vPos.x -= 100.f * fTimeDelta;
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_D)) {
		m_vPos.x += 100.f * fTimeDelta;
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_S)) {
		m_vPos.y -= 100.f * fTimeDelta;
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_W)) {
		m_vPos.y += 100.f * fTimeDelta;
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_Q)) {
		m_vRot.z += 3.f * fTimeDelta;
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_E)) {
		m_vRot.z -= 3.f * fTimeDelta;
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_R)) {
		m_vScale.x -= 10.f * fTimeDelta;
		m_vScale.y -= 10.f * fTimeDelta;
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_F)) {
		m_vScale.x += 10.f * fTimeDelta;
		m_vScale.y += 10.f * fTimeDelta;
	}*/

	return 0;
}

void CPlayScene::Render(void)
{
	// 오브젝트 월드 변환/세팅
	/*_matrix matWorld, matScale, matRot, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationYawPitchRoll(&matRot, m_vRot.y, m_vRot.x, m_vRot.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	matWorld = matScale * matRot * matTrans;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);*/

	// 투영 변환/세팅
	//_matrix matView;
	//D3DXMatrixOrthoLH(&matView, WINCX, WINCY, 0.f, 1000.f);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matView);

	m_pLogoObject->Render_Object();
	m_pMonster->Render_Object();
	//Engine::Render_Texture(Engine::RESOURCE_STATIC, L"Texture_Logo", 0);
	//Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"Buffer_RcTex");
}

CPlayScene * CPlayScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayScene*	pInstance = new CPlayScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CPlayScene::Free(void)
{
	Client::Safe_Release(m_pLogoObject);
	Client::Safe_Release(m_pMonster);
	CScene::Free();
}
