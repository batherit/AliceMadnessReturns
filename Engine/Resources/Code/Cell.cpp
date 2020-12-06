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
			// (y =) ax + b => ax -y + b = 0;
			// 점과 직선 사이 거리 공식으로 거리를 구한다.
			_float fLength = fabs(m_pLine[i]->GetGradient() * vPos.x - vPos.y + m_pLine[i]->GetIntercept())
				/ sqrtf(m_pLine[i]->GetGradient() * m_pLine[i]->GetGradient() + 1.f/*-1 * -1*/);
			// 법선 벡터(마름모 안쪽)쪽으로 밀어낸다.
			vPos = _vec2(vPos.x - m_pLine[i]->GetNormal().x * fLength, vPos.y - m_pLine[i]->GetNormal().y * fLength);
		}
	}

	return _vec3(vPos.x, _vPos.y, vPos.y);
}

HRESULT Engine::CCell::Ready_Cell(const _ulong& dwIndex,
								const _vec3* pPointA, 
								const _vec3* pPointB, 
								const _vec3* pPointC)
{
	m_dwIndex = dwIndex;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	m_pLine[LINE_AB] = CLine::Create(&_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z),
									 &_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z));

	m_pLine[LINE_BC] = CLine::Create(&_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z),
									 &_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z));

	m_pLine[LINE_CA] = CLine::Create(&_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z),
									 &_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z));

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

	for (_ulong i = 0; i < 4; ++i)
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

Engine::CCell* Engine::CCell::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	CCell*	pInstance = new CCell(pGraphicDev);

	if (FAILED(pInstance->Ready_Cell(dwIndex, pPointA, pPointB, pPointC)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCell::Free(void)
{
	for (_ulong i = 0; i < LINE_END; ++i)
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

Engine::CCell::MOVING Engine::CCell::CompareCell(const _vec3 * pEndPos, _ulong * pCellIndex)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if (nullptr == m_pNeighbor[i])
				return CCell::STOP;
			else
			{
				*pCellIndex = *m_pNeighbor[i]->Get_Index();
				return CCell::MOVE;
			}
		}
	}
	
	return CCell::MOVE;
}

