#include "NaviMesh.h"
#include "Physics.h"
USING(Engine)

Engine::CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	//, m_iIndex(0)
{
	
}

Engine::CNaviMesh::CNaviMesh(const CNaviMesh& rhs)
	: CMesh(rhs)
	, m_vecCell(rhs.m_vecCell)
	//, m_iIndex(rhs.m_iIndex)
{
	for (auto& iter : m_vecCell)
		Safe_AddRef(iter);
}

Engine::CNaviMesh::~CNaviMesh(void)
{

}

_int CNaviMesh::GetNaviIndexByPos(const _vec3 & _vCurrentPos, const _vec3 & _vTargetPos) const
{
	_int iCellSize = m_vecCell.size();

	for (_int i = 0; i < iCellSize; ++i) {
		if (m_vecCell[i]->IsCollided(_vCurrentPos, _vTargetPos)) {
			return i;
		}
	}

	return -1;
}

_int CNaviMesh::GetNaviIndexByPos(const _vec3 & _vCurrentPos) const
{
	_int iCellSize = m_vecCell.size();

	for (_int i = 0; i < iCellSize; ++i) {
		if (m_vecCell[i]->IsPosInCell(_vCurrentPos)) {
			return i;
		}
	}

	return -1;
}

HRESULT Engine::CNaviMesh::Ready_NaviMeshes(void)
{
	//m_vecCell.reserve(4);

	//CCell*		pCell = nullptr;

	//// 0번 
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 1번 
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 2번 
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 4.f), &_vec3(2.f, 0.f, 2.f), &_vec3(0.f, 0.f, 2.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 3번 
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(2.f, 0.f, 2.f), &_vec3(4.f, 0.f, 0.f), &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);

	return S_OK;
}

void CNaviMesh::Render_NaviMeshes(void)
{
	for (auto& iter : m_vecCell)
		iter->Render_Cell();

	for (auto& iter : m_vecSlidingCell)
		iter->Render_Cell();
}

_vec3 CNaviMesh::Move_OnNaviMesh(_int& _iCellIndex, const _vec3* pCurrentPos, const _vec3 * pTargetPos, CPhysics* _pPhysics)
{
	if (*pCurrentPos == *pTargetPos)
		return *pTargetPos;

	_vec3 vEndPos = *pTargetPos;
	_vec3 vHitPos;

	switch (m_vecCell[_iCellIndex]->GetTagIndex())
	{
	case CCell::TYPE_NORMAL:
		if (CCell::INSIDE == m_vecCell[_iCellIndex]->CompareCell(&vEndPos, &_iCellIndex)) {
			if (m_vecCell[_iCellIndex]->IsCollided(*pCurrentPos, *pTargetPos, &vHitPos)) {
				vEndPos.y = vHitPos.y;
				return vEndPos;
			}
		}
		break;
	case CCell::TYPE_SLIDE:
		if (CCell::INSIDE == m_vecCell[_iCellIndex]->CompareCell(&vEndPos, &_iCellIndex)) {
			//if (m_vecCell[m_iIndex]->IsCollided(*pCurrentPos, *pTargetPos, &vHitPos)) {
			vEndPos.y = m_vecCell[_iCellIndex]->GetHeight(vEndPos);

			//if (_pPhysics) {
			//	_vec3 vTemp = _vec3((m_vecCell[m_iIndex]->GetCenterPoint().x - pCurrentPos->x) * 0.05f /*+ vVel.x * 0.02f*/, 0.f, (m_vecCell[m_iIndex]->GetCenterPoint().z - pCurrentPos->z)  * 0.05f /*+ vVel.z* 0.02f*/);
			//	_pPhysics->AddVelocityXZ(_vec2(vTemp.x, vTemp.z));
			//}

			return vEndPos;
			//}
			//else
			//	int a = 10;
		}
		else {
			_vec3 vCorrectedPos;
			vCorrectedPos = m_vecCell[_iCellIndex]->GetPosInCell(vEndPos);
			vCorrectedPos.y = m_vecCell[_iCellIndex]->GetHeight(vCorrectedPos);
			if (_pPhysics) {
				_vec3 vTemp;
				vTemp = GetReflectionVector(_pPhysics->GetVelocity() ,*D3DXVec3Normalize(&vTemp, &_vec3(vCorrectedPos.x - vEndPos.x, 0.f, vCorrectedPos.z - vEndPos.z)), 1.1f);
				//_vec3 vVel = _pPhysics->GetVelocity();
				//vTemp = _vec3((m_vecCell[m_iIndex]->GetCenterPoint().x - pCurrentPos->x) /*+ vVel.x * 0.02f*/, 0.f, (m_vecCell[m_iIndex]->GetCenterPoint().z - pCurrentPos->z) /*+ vVel.z* 0.02f*/);
				_pPhysics->SetVelocityXZ(_vec2(0.f, 0.f));
				_pPhysics->AddVelocityXZ(_vec2(vTemp.x, vTemp.z));
			}
			return vCorrectedPos + (m_vecCell[_iCellIndex]->GetCenterPoint() - vCorrectedPos) * 0.004f;
		}
		break;
	case CCell::TYPE_SLIDE_EXIT:
		break;
	default:
		break;
	}


	return *pTargetPos;
}

_vec3 CNaviMesh::MoveOnNaviMesh_Adhesion(_int & _iCellIndex, const _vec3 * pCurrentPos, const _vec3 * pTargetPos)
{
	if (*pCurrentPos == *pTargetPos)
		return *pTargetPos;

	_vec3 vEndPos = *pTargetPos;
	_vec3 vHitPos;

	/*if (_iCellIndex == -1 || !m_vecCell[_iCellIndex]->IsPosInCell(*pCurrentPos)) {
		_iCellIndex = GetNaviIndexByPos(*pCurrentPos);
	}

	if (_iCellIndex == -1)
		return *pCurrentPos;*/

	if (CCell::INSIDE == m_vecCell[_iCellIndex]->CompareCell(&vEndPos, &_iCellIndex)) {
		_float fHeight = m_vecCell[_iCellIndex]->GetHeight(vEndPos);
		vEndPos.y = fHeight;
		return vEndPos;
	}
	else {
		vEndPos = m_vecCell[_iCellIndex]->GetPosInCell(vEndPos);
		_float fHeight = m_vecCell[_iCellIndex]->GetHeight(vEndPos);
		vEndPos.y = fHeight;
		return vEndPos;
	}

	return *pTargetPos;
}

_vec3 CNaviMesh::GetSlidedPos(const _vec3 & _vPos)
{
	_vec3 vSlidedPos = _vPos;
	_bool bIsForward = false;
	_vec3 vHitPos;
	for (auto& rSlidingCell : m_vecSlidingCell) {
		if (Engine::IsSphereAndTriangleCollided(
			*rSlidingCell->Get_Point(CCell::POINT_A),
			*rSlidingCell->Get_Point(CCell::POINT_B),
			*rSlidingCell->Get_Point(CCell::POINT_C),
			vSlidedPos, 1.f, &bIsForward, &vHitPos)) {

			if (!bIsForward) {
				// 뒷편에 있었다면 앞으로 밀어낸다.
				vSlidedPos.x = vHitPos.x;
				vSlidedPos.z = vHitPos.z;
			}
		}
	}

	return vSlidedPos;
}

//_bool CNaviMesh::LoadNaviMeshFromFile(const _tchar* _pFilePath)
//{
//	if (!_pFilePath)
//		return false;
//
//	HANDLE hFile = CreateFile(_pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
//
//	if (INVALID_HANDLE_VALUE == hFile)
//		return false;
//
//	_int iVerticesSize = 0;
//	ReadFile(hFile, &iVerticesSize, sizeof(_int), nullptr, nullptr);
//	
//	m_vecCell.reserve(iVerticesSize + 10);
//	m_vecCell.clear();
//
//	_vec3 vPos[3];
//	_int iTriangleSize = iVerticesSize / 3;
//	for (_int i = 0; i < iTriangleSize; ++i) {
//		for (_int j = 0; j < 3; ++j) {
//			ReadFile(hFile, &vPos[j], sizeof(_vec3), nullptr, nullptr);
//		}
//		AddCell(vPos[0], vPos[1], vPos[2]);
//	}
//
//	CloseHandle(hFile);
//
//	return true;
//}

HRESULT CNaviMesh::AddCell(const _vec3 & _vV1, const _vec3 & _vV2, const _vec3 & _vV3, const _int& _iTagIndex)
{
	CCell*		pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vV1, &_vV2, &_vV3, _iTagIndex);
	NULL_CHECK_RETURN(pCell, E_FAIL);

	if (_iTagIndex == CCell::TYPE_NORMAL || _iTagIndex == CCell::TYPE_SLIDE || _iTagIndex == CCell::TYPE_SLIDE_EXIT)
		m_vecCell.push_back(pCell);
	else if(_iTagIndex == CCell::TYPE_WALL)
		m_vecSlidingCell.push_back(pCell);
	return S_OK;
}

HRESULT Engine::CNaviMesh::Link_Cell(void)
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		for (_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if(i == j)
				continue;

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]->Get_Point(CCell::POINT_B),	
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B),
													m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}

		}
	}
	
	return S_OK;
}

Engine::CNaviMesh* Engine::CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMesh*		pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_NaviMeshes()))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* Engine::CNaviMesh::Clone(void)
{
	return new CNaviMesh(*this);
}

void Engine::CNaviMesh::Free(void)
{

	for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());
	m_vecCell.clear();
	for_each(m_vecSlidingCell.begin(), m_vecSlidingCell.end(), CDeleteObj());
	m_vecSlidingCell.clear();

	CMesh::Free();
}

