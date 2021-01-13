#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"
//#include "InputDev.h"
#include "DirectInputMgr.h"
#include "ChronoTimer.h"

BEGIN(Engine)

// GraphicDev
// Get
// Set
// General
inline HRESULT		Ready_GraphicDev(HWND hWnd,	WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev);
inline void			Render_Begin(D3DXCOLOR Color);
inline void			Render_End(void);

// TimerMgr
// Get
// Set
// General
inline _float		Get_TimeDelta(const _tchar* pTimerTag);
inline void			Set_TimeDelta(const _tchar* pTimerTag);
inline CChronoTimer* GetTimer(const _tchar* _pTimerTag);
inline HRESULT		Ready_Timer(const _tchar* pTimerTag);

// FrameMgr
// Get
inline _bool		IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
// Set
// General
inline	HRESULT		Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);

// FontMgr
inline HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
inline void		Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color, const _vec2* pScale = &_vec2(1.f, 1.f));

// InputDev
//inline _byte	Get_DIKeyState(_ubyte byKeyID);
//inline _byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
//inline _long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
//inline	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
//inline	void	Set_InputDev(void);

// Release
inline void			Release_System(void);

#include "Export_System.inl"

END

#endif // Export_System_h__
