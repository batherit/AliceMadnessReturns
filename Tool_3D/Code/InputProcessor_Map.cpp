#include "pch.h"
#include "InputProcessor_Map.h"
#include "InputProcessor_Navi.h"

CInputProcessor_Map::CInputProcessor_Map(Engine::CInputProcessorMgr * _pInputProcessorMgr)
	:
	CInputProcessor(_pInputProcessorMgr)
{
}

CInputProcessor_Map::~CInputProcessor_Map(void)
{
}

_int CInputProcessor_Map::ProcessInput(const _float & _fDeltaTime)
{
	auto ptCurrentCursor = Engine::GetClientCursorPoint(g_hWnd);
	if (!Engine::IsPointInClient(g_hWnd, ptCurrentCursor))
		return -1;

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
		// TODO : �ͷ��θ�忡���� LButton ó��
		//m_pInputProcessorMgr->SetNextInputProcessor(new CInputProcessor_Navi(m_pInputProcessorMgr));
	}
	return 0;
}
