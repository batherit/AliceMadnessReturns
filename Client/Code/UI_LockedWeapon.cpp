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

	CUI_Button<CUI_LockedWeapon>* m_pUnlockButton = CUI_Button<CUI_LockedWeapon>::Create(m_pGraphicDev, this, 0, 100, m_pToothImage->GetWidth(), m_pToothImage->GetHeight());
	m_pUnlockButton->SetEvent(CUI_Button<CUI_LockedWeapon>::BUTTON_STATE_CLICKED, &CUI_LockedWeapon::ClickUnlockButton, nullptr);
	AddChild(m_pUnlockButton);
	
	return S_OK;
}

int CUI_LockedWeapon::Update_Object(const _float & _fDeltaTime)
{
	m_pRenderer->Add_RenderGroup(Engine::RENDER_UI, this);
	CGameObject::Update_Object(_fDeltaTime);
	return 0;
}

void CUI_LockedWeapon::Render_Object(void)
{
	if (!m_pToothImage->IsActivated())
		return;
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

void CUI_LockedWeapon::SetPos(LONG _iPosX, LONG _iPosY)
{
}

void CUI_LockedWeapon::SetLocked(_bool _bIsLocked)
{
	m_bIsLocked = _bIsLocked; 
	m_pLockImage->SetActivated(_bIsLocked);
	m_pToothImage->SetActivated(_bIsLocked);
}

void CUI_LockedWeapon::ClickUnlockButton(void *)
{
	abort();
}

//_bool CUI_LockedWeapon::PressUnlockButton()
//{
//	if (!m_pAliceW || m_pAliceW->GetToothNum() < m_iToothNum || !m_bIsLocked)
//		return false;
//
//
//	return _bool();
//}
