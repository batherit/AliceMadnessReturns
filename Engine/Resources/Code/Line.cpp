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

_vec2 CLine::GetPosInLine(const _vec2 & _vPosXZ)
{
	_float fDotToLine = 0.f;
	_float fDotToPosition = 0.f;
	// 포지션과 원점간 내적을 구한다. (n.p)
	fDotToPosition = D3DXVec2Dot(&GetNormal(), &_vPosXZ);
	// 라인과 원점간 내적을 구한다. (-d)
	fDotToLine = D3DXVec2Dot(&GetNormal(), &GetPoint(CLine::POINT_START));

	if (fDotToPosition - fDotToLine > 0.f) {
		// (y =) ax + b => ax -y + b = 0;
		// 점과 직선 사이 거리 공식으로 거리를 구한다.
		_float fLength = fabs(GetGradient() * _vPosXZ.x - _vPosXZ.y + GetIntercept())
			/ sqrtf(GetGradient() * GetGradient() + 1.f/*-1 * -1*/);
		// 법선 벡터(마름모 안쪽)쪽으로 밀어낸다.
		return _vec2(_vPosXZ.x - GetNormal().x * fLength, _vPosXZ.y - GetNormal().y * fLength);
	}

	return _vPosXZ;
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

