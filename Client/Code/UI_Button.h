#pragma once
#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
template<typename T>
class CUI_Button : public Engine::CGameObject
{
public:
	enum E_BUTTON_STATE { 
		BUTTON_STATE_CLICKED,		// 클릭직후
		BUTTON_STATE_PRESSED,		// 누르고 있는 동안
		BUTTON_STATE_RELEASED,		// 누른걸 뗀 직후
		BUTTON_STATE_HOVERED,		// 누르지 않은 상태로 버튼 위에 커서 존재
		BUTTON_STATE_UNHOVERED,		// 드래그 되지 않은 상태에서 버튼 밖에 위치
		BUTTON_STATE_END };

public:
	explicit CUI_Button(LPDIRECT3DDEVICE9 pGraphicDev, T* _pOwner, LONG _iPosX, LONG _iPosY, LONG _iWidth, LONG _iHeight);
	explicit CUI_Button(const  CUI_Button& rhs);
	virtual ~CUI_Button();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	//virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;
	// CObj을(를) 통해 상속됨
	virtual void Render_Object(void) override;

public:
	static CUI_Button*	Create(LPDIRECT3DDEVICE9 pGraphicDev, T* _pOwner, LONG _iPosX, LONG _iPosY, LONG _iWidth, LONG _iHeight);
	virtual void		Free(void);

public:
	void SetEvent(E_BUTTON_STATE _eButtonState, void (T::*_pEvent) (void*), void* _pArgu);
	void ReleaseEvent(E_BUTTON_STATE _eButtonState);

private:
	T* m_pOwner = nullptr;
	void(T::*m_pEvent[BUTTON_STATE_END]) (void*);
	void* m_pArgu[BUTTON_STATE_END];
	_bool m_bIsClicked = false;
	RECT m_rtButtonRange;
};

template<typename T>
inline CUI_Button<T>::CUI_Button(LPDIRECT3DDEVICE9 pGraphicDev, T* _pOwner, LONG _iPosX, LONG _iPosY, LONG _iWidth, LONG _iHeight)
	:
	CGameObject(pGraphicDev),
	m_pOwner(_pOwner)
{
	ZeroMemory(m_pEvent, sizeof(m_pEvent));
	ZeroMemory(m_pArgu, sizeof(m_pArgu));

	m_rtButtonRange.right = _iPosX + (_iWidth >> 1);
	m_rtButtonRange.top = _iPosY - (_iHeight >> 1);
	m_rtButtonRange.left = _iPosX - (_iWidth >> 1);
	m_rtButtonRange.bottom = _iPosY + (_iHeight >> 1);
}

template<typename T>
inline CUI_Button<T>::CUI_Button(const CUI_Button & rhs)
{
}

template<typename T>
inline CUI_Button<T>::~CUI_Button()
{
}

template<typename T>
inline HRESULT CUI_Button<T>::Ready_Object(void)
{
	return S_OK;
}

template<typename T>
inline _int CUI_Button<T>::Update_Object(const _float & _fDeltaTime)
{
	POINT pt = Engine::GetClientCursorPoint(g_hWnd);

	if (Engine::IsPointInRect(m_rtButtonRange, pt))
	{
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
			// Clicked
			if (m_pOwner && m_pEvent[BUTTON_STATE_CLICKED])
				(m_pOwner->*m_pEvent[BUTTON_STATE_CLICKED])(m_pArgu[BUTTON_STATE_CLICKED]);
			m_bIsClicked = true;
			return 0;
		}
		else {
			if (!m_bIsClicked) {
				// Hovered
				if (m_pOwner && m_pEvent[BUTTON_STATE_HOVERED])
					(m_pOwner->*m_pEvent[BUTTON_STATE_HOVERED])(m_pArgu[BUTTON_STATE_HOVERED]);
				return 0;
			}
		}
	}
	else if (!m_bIsClicked) {
		// Unhovered
		if (m_pOwner && m_pEvent[BUTTON_STATE_UNHOVERED])
			(m_pOwner->*m_pEvent[BUTTON_STATE_UNHOVERED])(m_pArgu[BUTTON_STATE_UNHOVERED]);
		return 0;
	}

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(Engine::DIM_LB)) {
		if (m_bIsClicked) {
			// Pressed
			if (m_pOwner && m_pEvent[BUTTON_STATE_PRESSED])
				(m_pOwner->*m_pEvent[BUTTON_STATE_PRESSED])(m_pArgu[BUTTON_STATE_PRESSED]);
			return 0;
		}
	}
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyUp(Engine::DIM_LB)) {
		if (m_bIsClicked) {
			// Released 
			if (m_pOwner && m_pEvent[BUTTON_STATE_RELEASED])
				(m_pOwner->*m_pEvent[BUTTON_STATE_RELEASED])(m_pArgu[BUTTON_STATE_RELEASED]);
			m_bIsClicked = false;
			return 0;
		}
	}

	return 0;
}

template<typename T>
inline void CUI_Button<T>::Render_Object(void)
{
}

//template<typename T>
//void CUI_Button<T>::Render(HDC & _hdc, CCamera * _pCamera)
//{
//	CObj::Render(_hdc);
//	TextOut(_hdc, GetLeft() + 5, GetTop() + 5, m_szButtonName, lstrlen(m_szButtonName));
//}

template<typename T>
inline CUI_Button<T> * CUI_Button<T>::Create(LPDIRECT3DDEVICE9 pGraphicDev, T* _pOwner, LONG _iPosX, LONG _iPosY, LONG _iWidth, LONG _iHeight)
{
	CUI_Button<T>*	pInstance = new  CUI_Button<T>(pGraphicDev, _pOwner, _iPosX, _iPosY, _iWidth, _iHeight);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

template<typename T>
inline void CUI_Button<T>::Free(void)
{
	for (int i = 0; i < BUTTON_STATE_END; i++) {
		ReleaseEvent(static_cast<E_BUTTON_STATE>(i));
	}
	CGameObject::Free();
}

template<typename T>
inline void CUI_Button<T>::SetEvent(E_BUTTON_STATE _eButtonState, void(T::* _pEvent)(void *), void * _pArgu)
{
	ReleaseEvent(_eButtonState);
	m_pEvent[_eButtonState] = _pEvent;
	m_pArgu[_eButtonState] = _pArgu;
}

template<typename T>
inline void CUI_Button<T>::ReleaseEvent(E_BUTTON_STATE _eButtonState)
{
	Engine::Safe_Delete(m_pArgu[_eButtonState]);
	m_pEvent[_eButtonState] = nullptr;
}

END