#include "pch.h"
#include "InputMode_Map.h"
#include "InputMode_Navi.h"

CInputMode_Map::CInputMode_Map(Engine::CInputModeMgr * _pInputModeMgr)
	:
	CInputMode(_pInputModeMgr)
{
}

CInputMode_Map::~CInputMode_Map(void)
{
}

_int CInputMode_Map::ProcessInput(const _float & _fDeltaTime)
{
	auto ptCurrentCursor = Engine::GetClientCursorPoint(g_hWnd);
	if (!Engine::IsPointInClient(g_hWnd, ptCurrentCursor))
		return -1;

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
		// TODO : �ͷ��θ�忡���� LButton ó��
		//m_pInputModeMgr->SetNextInputMode(new CInputMode_Navi(m_pInputModeMgr));
	}
	return 0;
}
