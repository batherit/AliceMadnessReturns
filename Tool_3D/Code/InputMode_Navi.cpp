#include "pch.h"
#include "InputMode_Navi.h"

CInputMode_Navi::CInputMode_Navi(Engine::CInputModeMgr * _pInputModeMgr)
	:
	CInputMode(_pInputModeMgr)
{
}

CInputMode_Navi::~CInputMode_Navi(void)
{
}

_int CInputMode_Navi::ProcessInput(const _float & _fDeltaTime)
{
	auto ptCurrentCursor = Engine::GetClientCursorPoint(g_hWnd);
	if (!Engine::IsPointInClient(g_hWnd, ptCurrentCursor))
		return -1;

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
		// TODO : 네비모드에서의 LButton 처리
		abort();
	}
	return _int();
}
