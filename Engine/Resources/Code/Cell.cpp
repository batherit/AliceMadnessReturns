#include "Cell.h"	

USING(Engine)

Engine::CCell::CCell(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pD3DXLine(nullptr)
	, m_pGraphicDev(pGraphicDev)
{

	Safe_AddRef(m_pGraphicDev);

	ZeroMemory(m_pLine, sizeof(CLine*) * LINE_END);
	ZeroMemory(m_pNeighbor, sizeof(CCell*) * NEIGHBOR_END);
}

Engine::CCell::~CCell(void)
{

}

_vec3 CCell::GetPosInCell(const _vec3 & _vPos)
{
	// 현재 네비메쉬는 xz평면에만 유효하다. (1206)
	_vec2 vPos = _vec2(_vPos.x, _vPos.z);
	_float fLength = 0.f;
	for (int i = 0; i < CCell::LINE_END; ++i) {
		vPos = m_pLine[i]->GetPosInLine(vPos);
	}

	/*_vec3 vPointAvg = (m_vPoint[POINT_A] + m_vPoint[POINT_B] + m_vPoint[POINT_C]) / 3.f;
	_vec2 vToPointAvgDir = _vec2(vPointAvg.x, vPointAvg.z) - vPos;
	D3DXVec2Normalize(&vToPointAvgDir, &vToPointAvgDir);
	vPos = vPos + vToPointAvgDir * 0.1f;*/
	
	return _vec3(vPos.x, _vPos.y, vPos.y);
}

_float CCell::GetHeight(const _vec3 & _vPos)
{
	return GetPointProjectedOntoTriangle(m_vPoint[POINT_A], m_vPoint[POINT_B], m_vPoint[POINT_C], _vPos).y;
}

_bool CCell::IsPosInCell(const _vec3 & _vPos)
{
	_float fDotToLine = 0.f;
	_float fDotToPosition = 0.f;
	_vec2 vPos = _vec2(_vPos.x, _vPos.z);
	_float fLength = 0.f;
	for (int i = 0; i < CCell::LINE_END; ++i) {
		// 포지션과 원점간 내적을 구한다. (n.p)
		fDotToPosition = D3DXVec2Dot(&m_pLine[i]->GetNormal(), &vPos);
		// 라인과 원점간 내적을 구한다. (-d)
		fDotToLine = D3DXVec2Dot(&m_pLine[i]->GetNormal(), &m_pLine[i]->GetPoint(CLine::POINT_START));

		if (fDotToPosition - fDotToLine > 0.f) {
			return false;
		}
	}

	return true;
}

_bool CCell::IsCollided(const _vec3& _vFromPos, const _vec3& _vToPos, _vec3* _pHitPos)
{
	if (_vFromPos == _vToPos)
		return false;

	_float fU, fV, fDist;

	_vec3 vDir = _vToPos - _vFromPos;

	// 위로 상승하고 있는 상황에 대해서는 고려하지 않겠다.
	if (vDir.y > 0.f)
		return false;

	if (D3DXIntersectTri(&m_vPoint[POINT_A], &m_vPoint[POINT_B], &m_vPoint[POINT_C], &_vToPos, &WORLD_Y_AXIS, &fU, &fV, &fDist)) {
		if (fDist > -2.f * vDir.y + 1.f) {	// 이거 좀 고칠 필요가 있엉ㅅㅇ
			// 충돌 허용 두께(y축에서의 이동 격차 + 일반 충돌 허용 두께)를 넘어간 거리에 대해서는 허용x
			return false;
		}

		// 충돌했다면, 충돌 지점을 찾는다.
		_vec3 vHitPos= Engine::GetHitPos(m_vPoint[POINT_A], m_vPoint[POINT_B], m_vPoint[POINT_C], fU, fV);

		if (_pHitPos)
			*_pHitPos = vHitPos;

		return true;
	}

	return false;
}

HRESULT Engine::CCell::Ready_Cell(const _int& iIndex,
								const _vec3* pPointA, 
								const _vec3* pPointB, 
								const _vec3* pPointC,
								const _int& _iTagIndex)
{
	m_iIndex = iIndex;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	m_vCenterPoint = (*pPointA + *pPointB + *pPointC) / 3.f;
	m_iTagIndex = _iTagIndex;

	if (_iTagIndex == CCell::TYPE_WALL) {
		// 크기를 약간 키운다.
		m_vPoint[POINT_A] = m_vPoint[POINT_A] + (m_vPoint[POINT_A] - m_vCenterPoint) * 0.1f;
		m_vPoint[POINT_B] = m_vPoint[POINT_B] + (m_vPoint[POINT_B] - m_vCenterPoint) * 0.1f;
		m_vPoint[POINT_C] = m_vPoint[POINT_C] + (m_vPoint[POINT_C] - m_vCenterPoint) * 0.1f;
	}

	m_pLine[LINE_AB] = CLine::Create(&_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z),
									 &_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z));

	m_pLine[LINE_BC] = CLine::Create(&_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z),
									 &_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z));

	m_pLine[LINE_CA] = CLine::Create(&_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z),
									 &_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z));

	m_vNormal = GetNormalFromPoints(*pPointA, *pPointB, *pPointC);

	if (m_vNormal.y != 1.f) {
		// 일반적으로 여길 들어올 것이다. 슬라이딩 벡터는 보통 옆으로 영향을 주므로,
		m_vNormalXZ = m_vNormal;
		m_vNormalXZ.y = 0.f;
		D3DXVec3Normalize(&m_vNormalXZ, &m_vNormalXZ);

		_vec3 vPoint = m_vCenterPoint - WORLD_Y_AXIS;
		vPoint = Engine::GetPointProjectedOntoTriangle(
			m_vPoint[POINT_A],
			m_vPoint[POINT_B],
			m_vPoint[POINT_C],
			vPoint
		);

		D3DXVec3Normalize(&m_vSliding, &(vPoint - m_vCenterPoint));
	}
	else {
		m_vSliding = Engine::GetRotatedVector(
			WORLD_Y_AXIS,
			Engine::GetNumberBetweenMinMax(0.f, D3DX_PI * 2.f),
			WORLD_X_AXIS);
		m_vNormalXZ = m_vSliding;
	}

#ifdef _DEBUG
	if (FAILED(D3DXCreateLine(m_pGraphicDev, &m_pD3DXLine)))
		return S_OK;
#endif

	return S_OK;
}

void Engine::CCell::Render_Cell(void)
{
	_vec3		vPoint[4];


	vPoint[0] = m_vPoint[POINT_A];
	vPoint[1] = m_vPoint[POINT_B];
	vPoint[2] = m_vPoint[POINT_C];
	vPoint[3] = m_vPoint[POINT_A];


	_matrix			matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
		
		if (vPoint[i].z <= 0.1f)
			vPoint[i].z = 0.1f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}
	
	m_pD3DXLine->SetWidth(3.f);	// 라인의 굵기 결정

	m_pGraphicDev->EndScene();
	m_pGraphicDev->BeginScene();

	m_pD3DXLine->Begin();
	
	_matrix matTemp;

	m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	m_pD3DXLine->End();


}

Engine::CCell* Engine::CCell::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _int& iIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, const _int& _iTagIndex)
{
	CCell*	pInstance = new CCell(pGraphicDev);

	if (FAILED(pInstance->Ready_Cell(iIndex, pPointA, pPointB, pPointC, _iTagIndex)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCell::Free(void)
{
	for (_int i = 0; i < LINE_END; ++i)
	{
		Safe_Release(m_pLine[i]);
	}
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pD3DXLine);
}

Engine::_bool Engine::CCell::Compare_Point(const _vec3* pFirstPoint, const _vec3* pSecondPoint, CCell* pCell)
{
	if (*pFirstPoint == m_vPoint[POINT_A])
	{
		if (*pSecondPoint == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}

		if (*pSecondPoint == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	if (*pFirstPoint == m_vPoint[POINT_B])
	{
		if (*pSecondPoint == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}

		if (*pSecondPoint == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
	}

	if (*pFirstPoint == m_vPoint[POINT_C])
	{
		if (*pSecondPoint == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}

		if (*pSecondPoint == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	return false;
}

Engine::CCell::MOVING Engine::CCell::CompareCell(const _vec3 * pEndPos, _int * pCellIndex)
{
	for (_int i = 0; i < LINE_END; ++i)
	{
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if (nullptr == m_pNeighbor[i])
				return CCell::OUTSIDE;
			else
			{
				*pCellIndex = *m_pNeighbor[i]->Get_Index();
				return CCell::INSIDE;
			}
		}
	}
	
	return CCell::INSIDE;
}

