#include "pch.h"
#include "InputProcessor_Terrain.h"
#include "InputProcessor_Navi.h"

CInputProcessor_Terrain::CInputProcessor_Terrain(Engine::CInputProcessorMgr * _pInputProcessorMgr)
	:
	CInputProcessor(_pInputProcessorMgr)
{
}

CInputProcessor_Terrain::~CInputProcessor_Terrain(void)
{
}

_int CInputProcessor_Terrain::ProcessInput(const _float & _fDeltaTime)
{
	auto ptCurrentCursor = Engine::GetClientCursorPoint(g_hWnd);
	if(!Engine::IsPointInClient(g_hWnd, ptCurrentCursor))
		return -1;

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
		// TODO : �ͷ��θ�忡���� LButton ó��
		//m_pInputProcessorMgr->SetNextInputProcessor(new CInputProcessor_Navi(m_pInputProcessorMgr));
	}
	return 0;
}
