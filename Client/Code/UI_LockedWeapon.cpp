#include "pch.h"
#include "UI_LockedWeapon.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "AliceW.h"

CUI_LockedWeapon::CUI_LockedWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_LockedWeapon::CUI_LockedWeapon(const CUI_LockedWeapon & rhs)
	:
	CGameObject(rhs)
{
}

CUI_LockedWeapon::~CUI_LockedWeapon(void)
{
}

HRESULT CUI_LockedWeapon::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();

	// WeaponImage 
	m_pWeaponImage = CUI_Image::Create(m_pGraphicDev);
	AddChild(m_pWeaponImage);
	
	// LockImage
	m_bIsLocked = true;
	m_pLockImage = CUI_Image::Create(m_pGraphicDev);
	m_pLockImage->SetTexture(L"UI_WeaponLock_Lock");
	AddChild(m_pLockImage);
	
	m_pToothImage = CUI_Image::Create(m_pGraphicDev);
	m_pToothImage->SetTexture(L"UI_WeaponLock_Tooth");
	m_pToothImage->SetPos(-10, 100);
	AddChild(m_pToothImage);

	m_pUnlockButton = CUI_Button<CUI_LockedWeapon>::Create(m_pGraphicDev, this, 0, 100, m_pToothImage->GetWidth(), m_pToothImage->GetHeight());
	m_pUnlockButton->SetEvent(CUI_Button<CUI_LockedWeapon>::BUTTON_STATE_CLICKED, &CUI_LockedWeapon::ClickUnlockButton, nullptr);
	//m_pUnlockButton->SetEvent(CUI_Button<CUI_LockedWeapon>::BUTTON_STATE_HOVERED, &CUI_LockedWeapon::HoveredOnButton, nullptr);
	//m_pUnlockButton->SetEvent(CUI_Button<CUI_LockedWeapon>::BUTTON_STATE_UNHOVERED, &CUI_LockedWeapon::UnhoveredOnButton, nullptr);
	AddChild(m_pUnlockButton);

// HelpImage
	//m_pHelpImage = CUI_Image::Create(m_pGraphicDev);
	//AddChild(m_pHelpImage);

	m_pHelpButton = CUI_Button<CUI_LockedWeapon>::Create(m_pGraphicDev, this, 0, 0, 100, 100);
	m_pHelpButton->SetEvent(CUI_Button<CUI_LockedWeapon>::BUTTON_STATE_HOVERED, &CUI_LockedWeapon::HoveredOnButton, nullptr);
	m_pHelpButton->SetEvent(CUI_Button<CUI_LockedWeapon>::BUTTON_STATE_UNHOVERED, &CUI_LockedWeapon::UnhoveredOnButton, nullptr);
	AddChild(m_pHelpButton);
	
	
	return S_OK;
}

int CUI_LockedWeapon::Update_Object(const _float & _fDeltaTime)
{
	m_pWeaponImage->Update_Object(_fDeltaTime);
	m_pHelpButton->Update_Object(_fDeltaTime);

	if (m_bIsLocked) {
		m_pRenderer->Add_RenderGroup(Engine::RENDER_UI, this);
		m_pLockImage->Update_Object(_fDeltaTime);
		m_pToothImage->Update_Object(_fDeltaTime);
		m_pUnlockButton->Update_Object(_fDeltaTime);
	}
	return 0;
}

void CUI_LockedWeapon::Render_Object(void)
{
	_vec3 vPos = m_pToothImage->GetTransform()->GetPos();
	_tchar tcBuffer[20];
	wsprintf(tcBuffer, L"%d", m_iToothNum);
	Engine::Render_Font(L"Font_Jinji", tcBuffer, &_vec2(vPos.x - 15.f, vPos.y + 40.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), &_vec2(0.5f, 0.5f));
}

CUI_LockedWeapon * CUI_LockedWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_LockedWeapon*	pInstance = new CUI_LockedWeapon(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_LockedWeapon::Free(void)
{
	CGameObject::Free();
}

void CUI_LockedWeapon::SetPlayer(CAliceW * _pPlayer)
{
	if (!_pPlayer)
		return;

	m_pAliceW = _pPlayer;
}

void CUI_LockedWeapon::SetWeaponTexture(const _tchar * _pTextureTag)
{
	m_pWeaponImage->SetTexture(_pTextureTag);
}

void CUI_LockedWeapon::SetLocked(_bool _bIsLocked)
{
	m_bIsLocked = _bIsLocked; 
}

//void CUI_LockedWeapon::SetHelpImage(const _tchar * _TextureTag, LONG _iPosX, LONG _iPosY)
//{
//	m_pHelpImage->SetTexture(_TextureTag);
//	m_pHelpImage->SetPos(_iPosX, _iPosY);
//}

void CUI_LockedWeapon::ClickUnlockButton(void *)
{
	if (m_iToothNum <= m_pAliceW->GetToothNum()) {
		SetLocked(false);
		m_pAliceW->DecreaseTooth(m_iToothNum);
		CSoundMgr::Get_Instance()->PlaySound(L"UI_WeaponPickup.ogg", CSoundMgr::UI);
	}
}

void CUI_LockedWeapon::HoveredOnButton(void *)
{
	if (!m_bIsOnHelpHovered) {
		if(m_pHelpImage)
			m_pHelpImage->SetVisible(true);
		m_bIsOnHelpHovered = true;
	}
}

void CUI_LockedWeapon::UnhoveredOnButton(void *)
{
	if (m_bIsOnHelpHovered) {
		if (m_pHelpImage)
			m_pHelpImage->SetVisible(false);
		m_bIsOnHelpHovered = false;
	}
}

//_bool CUI_LockedWeapon::PressUnlockButton()
//{
//	if (!m_pAliceW || m_pAliceW->GetToothNum() < m_iToothNum || !m_bIsLocked)
//		return false;
//
//
//	return _bool();
//}
