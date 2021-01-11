#include "pch.h"
#include "Route.h"

BEGIN(Client)

CRoute::CRoute() :
	CComponent() {

}

CRoute::CRoute(const CRoute& Route) :
	CComponent(Route) {

}

CRoute::~CRoute() {

}

HRESULT CRoute::Ready() {

	return S_OK;
}

_int CRoute::Update(const _float& fTimeDelta) {
	return 0;
}

Engine::CComponent* CRoute::Create() {
	CRoute* pInstance = new CRoute;

	if (FAILED(pInstance->Ready()))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* CRoute::Clone(void) {
	return new CRoute(*this);
}

void CRoute::SetPosList(const vector<_vec3>& _vecPosList)
{
	if (_vecPosList.empty()) abort();

	m_vecPosList.clear();
	m_vecPosList = _vecPosList;
	m_vCurrentPos = m_vecPosList[0];
}

//_bool CRoute::UpdateNextPosition(const _float& _fDeltaTime)
//{
//	if (!IsNextPositionExisted()) return false;
//	_int iTo = m_iFrom + 1;
//	m_fElapsedSec = Clamp(m_fElapsedSec + _fDeltaTime, 0.f, m_veCRouteInfos[iTo].fSecToReach);
//	return true;
//}

//_vec3 CRoute::GetNextPosition(const _float& _fDeltaTime)
//{
//	// IsNextPositionExisted가 true를 반환한 이후에 이 함수를 호출해야 한다.
//	_int iTo = m_iFrom + 1;
//	m_fElapsedSec = Engine::Clamp(m_fElapsedSec + _fDeltaTime, 0.f, m_vecPosList[iTo].fSecToReach);
//
//	_float fSecToReach = m_veCRouteInfos[iTo].fSecToReach;
//	_float fT = Clamp(m_fElapsedSec / fSecToReach, 0.f, 1.f);
//
//	_vec3 vNextPos = m_veCRouteInfos[m_iFrom].vPoint * (1.f - fT) + m_veCRouteInfos[iTo].vPoint * fT;
//
//	if (m_fElapsedSec >= fSecToReach) {
//		m_iFrom++;
//		m_fElapsedSec = 0.f;
//	}
//
//	m_vCurrentPos = vNextPos;
//
//	return m_vCurrentPos;
//}

_vec3 CRoute::GetNextPosition(const _float & _fDeltaTime, const _float & _fSpeed)
{
	// IsNextPositionExisted가 true를 반환한 이후에 이 함수를 호출해야 한다.
	if (m_vecPosList.size() == 1)
		return m_vCurrentPos;

	if (m_iFrom == -1)
		++m_iFrom;

	_int iTo = m_iFrom + 1;
	_vec3 vFrom = m_vecPosList[m_iFrom];
	_vec3 vTo = m_vecPosList[iTo];
	_vec3 vToNextPos = vTo - vFrom;
	D3DXVec3Normalize(&vToNextPos, &vToNextPos);

	_vec3 vNextPos = m_vCurrentPos + vToNextPos * _fSpeed * _fDeltaTime;

	if (D3DXVec3Length(&(vTo - vFrom)) <= D3DXVec3Length(&(vNextPos - vFrom))) {
		++m_iFrom;
		vNextPos = vTo;
		m_fElapsedSec = 0.f;
	}

	m_vCurrentPos = vNextPos;

	return m_vCurrentPos;
}

_vec3 CRoute::GetPrevPosition(const _float & _fDeltaTime, const _float & _fSpeed)
{
	if (m_vecPosList.size() == 1)
		return m_vCurrentPos;

	// m_veCRouteInfo가 2개 이상의 원소를 지님을 보장함.
	if (m_iFrom == m_vecPosList.size() - 1)
		--m_iFrom;

	_int iTo = m_iFrom;
	_vec3 vFrom = m_vecPosList[iTo + 1];
	_vec3 vTo = m_vecPosList[iTo];
	_vec3 vToPrevPos = vTo - vFrom;
	D3DXVec3Normalize(&vToPrevPos, &vToPrevPos);

	_vec3 vPrevPos = m_vCurrentPos + vToPrevPos * _fSpeed * _fDeltaTime;

	if (D3DXVec3Length(&(vTo - vFrom)) <= D3DXVec3Length(&(vPrevPos - vFrom))) {
		--m_iFrom;
		vPrevPos = vTo;
		m_fElapsedSec = 0.f;
	}

	m_vCurrentPos = vPrevPos;

	return m_vCurrentPos;
}

_vec3 CRoute::GetCurrentPosition(void)
{
	return m_vCurrentPos;
}

_bool CRoute::IsNextPositionExisted(void)
{
	return m_iFrom + 1 < m_vecPosList.size();
}

_bool CRoute::IsPrevPositionExisted(void)
{
	return m_iFrom >= 0;
}

void CRoute::Free(void)
{
	m_vecPosList.clear();
	m_vecPosList.shrink_to_fit();
}


END
