#include "Line.h"

USING(Engine)

Engine::CLine::CLine(void)
{

}

Engine::CLine::~CLine(void)
{

}

HRESULT Engine::CLine::Ready_Line(const _vec2* pPointA, const _vec2* pPointB)
{
	m_vPoint[POINT_START] = *pPointA;
	m_vPoint[POINT_FINISH] = *pPointB;

	m_vDirection = m_vPoint[POINT_FINISH] - m_vPoint[POINT_START];

	m_vNormal = _vec2(m_vDirection.y * -1.f, m_vDirection.x);
	D3DXVec2Normalize(&m_vNormal, &m_vNormal);

	m_fGradient = (m_vPoint[POINT_FINISH].y - m_vPoint[POINT_START].y) / (m_vPoint[POINT_FINISH].x - m_vPoint[POINT_START].x);
	// 절편 구하기(b = y - ax)
	m_fIntercept = m_vPoint[POINT_START].y - m_fGradient * m_vPoint[POINT_START].x;

	return S_OK;
}

Engine::CLine::COMPARE Engine::CLine::Compare(const _vec2* pPos)
{
	_vec2		vDest = *pPos - m_vPoint[POINT_START];

	_float		fDot = D3DXVec2Dot(D3DXVec2Normalize(&vDest, &vDest), &m_vNormal);

	if (0.f <= fDot)
		return COMPARE_LEFT;
	else
		return COMPARE_RIGHT;
}

Engine::CLine* Engine::CLine::Create(const _vec2* pPointA, const _vec2* pPointB)
{
	CLine*	pInstance = new CLine;

	if (FAILED(pInstance->Ready_Line(pPointA, pPointB)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLine::Free(void)
{

}

