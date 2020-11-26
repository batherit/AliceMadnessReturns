#include "ManualCol.h"

USING(Engine)
Engine::CManualCol::CManualCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

Engine::CManualCol::CManualCol(const CManualCol& rhs)
	: CVIBuffer(rhs)
{
}

Engine::CManualCol::~CManualCol(void)
{

}

HRESULT Engine::CManualCol::Ready_Buffer(void)
{
	// 속성 설정
	m_dwFVF = FVF_COL;
	m_dwTriCnt = 0;
	m_dwVtxCnt = 0;
	m_dwVtxSize = sizeof(VTXCOL);

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	m_vecVertices.reserve(600);	// 삼각형 200개 정도

	return S_OK;
}

void CManualCol::Render_Buffer(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	CVIBuffer::Render_Buffer();
	//m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	//m_pGraphicDev->SetFVF(m_dwFVF);
	//m_pGraphicDev->SetIndices(m_pIB);
	//m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
}

_bool CManualCol::IsValidTriangleIndex(_int _iTriangleIndex)
{
	return !(_iTriangleIndex < 0 || _iTriangleIndex >= static_cast<_int>(GetVertices().size()) / 3);
}

_bool CManualCol::IsValidVertexIndex(_int _iVertexIndex)
{
	return !(_iVertexIndex < 0 || _iVertexIndex >= 3);
}

_bool CManualCol::IsValidIndex(_int _iTriangleIndex, _int _iVertexIndex)
{
	return IsValidTriangleIndex(_iTriangleIndex) && IsValidVertexIndex(_iVertexIndex);
}

void CManualCol::PushTriangleVertices(_vec3 _vTriPos1, _vec3 _vTriPos2, _vec3 _vTriPos3)
{
	// 삼각형 추가
	m_vecVertices.push_back(_vTriPos1);
	m_vecVertices.push_back(_vTriPos2);
	m_vecVertices.push_back(_vTriPos3);

	m_dwTriCnt = m_vecVertices.size() / 3;	// 정점 3개로 삼각형 하나를 만들 수 있으므로.
	m_dwVtxCnt = m_vecVertices.size();
	//m_dwFVF = FVF_COL;
	//m_dwVtxSize = sizeof(VTXCOL);

	// 기존 버퍼 파괴 (인덱스 버퍼는 사용하지 않을 것이다.)
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	// 그릴 삼각형이 없다면 종료.
	if (m_dwTriCnt == 0)
		return;

	// 새 버퍼 생성
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), );

	VTXCOL*		pVertex = NULL;
	INDEX16*	pIndex = nullptr;
	
	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);
	// 1인자 : 어디서부터 잠글 것인가
	// 2인자 : 숫자가 0이면 전체 영역을 잠근다.
	// 3인자 : 인자값을 통해서 버텍스 버퍼 내 정점 중 첫번째 주소를 얻어온다.
	// 4인자 : 잠그는 형태를 묻는 인자, 정적 버퍼인 경우 

	// 시스템 메모리에 둘 정점 버퍼 채우기
	_int iVecSize = m_vecVertices.size();
	for (_int i = 0; i < iVecSize; ++i) {
		pVertex[i].vPos = m_vecVertices[i];
		pVertex[i].dwColor = D3DCOLOR_ARGB(150, 255, 0, 0);//D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	}
	m_pVB->Unlock();

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	iVecSize /= 3;
	for (_int i = 0; i < iVecSize; ++i) {
		pIndex[i]._0 = i * 3;
		pIndex[i]._1 = i * 3 + 1;
		pIndex[i]._2 = i * 3 + 2;
	}
	m_pIB->Unlock();
}

void CManualCol::PopTriangleVertices(_int _iTriangleIndex)
{
	if (!IsValidTriangleIndex(_iTriangleIndex))
		return;

	// 삼각형은 세 정점으로 이루어져 있으므로 정점을 제거하기 위해 for문을 3번 진행한다.
	for (_int i = 0; i < 3; ++i) {
		// 정점 제거
		m_vecVertices.erase(m_vecVertices.begin() + 3 * _iTriangleIndex);
	}

	m_dwTriCnt = m_vecVertices.size() / 3;	// 정점 3개로 삼각형 하나를 만들 수 있으므로.
	m_dwVtxCnt = m_vecVertices.size();

	// 기존 버퍼 파괴 (인덱스 버퍼는 사용하지 않을 것이다.)
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	// 그릴 삼각형이 없다면 종료.
	if (m_dwTriCnt == 0)
		return;

	// 새 버퍼 생성
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), );

	VTXCOL*		pVertex = NULL;
	INDEX16*	pIndex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);
	// 1인자 : 어디서부터 잠글 것인가
	// 2인자 : 숫자가 0이면 전체 영역을 잠근다.
	// 3인자 : 인자값을 통해서 버텍스 버퍼 내 정점 중 첫번째 주소를 얻어온다.
	// 4인자 : 잠그는 형태를 묻는 인자, 정적 버퍼인 경우 

	// 시스템 메모리에 둘 정점 버퍼 채우기
	_int iVecSize = m_vecVertices.size();
	for (_int i = 0; i < iVecSize; ++i) {
		pVertex[i].vPos = m_vecVertices[i];
		pVertex[i].dwColor = D3DCOLOR_ARGB(150, 255, 0, 0);//D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	}
	m_pVB->Unlock();

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	iVecSize /= 3;
	for (_int i = 0; i < iVecSize; ++i) {
		pIndex[i]._0 = i * 3;
		pIndex[i]._1 = i * 3 + 1;
		pIndex[i]._2 = i * 3 + 2;
	}
	m_pIB->Unlock();
}

void CManualCol::SetTriangleColor(_int _iTriangleIndex, D3DXCOLOR _colTriangleColor)
{
	if (!IsValidTriangleIndex(_iTriangleIndex))
		return;

	VTXCOL*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);

	// 시스템 메모리에 둘 정점 버퍼 채우기
	for (_int i = 0; i < 3; ++i) {
		pVertex[3 * _iTriangleIndex + i].dwColor = _colTriangleColor;
	}

	m_pVB->Unlock();
}

void CManualCol::SetTriangleVertexPosition(_int _iTriangleIndex, _int _iVertexIndex, const _vec3 _vNewPosition)
{
	if (!IsValidIndex(_iTriangleIndex, _iVertexIndex))
		return;

	VTXCOL* pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);

	pVertex[3 * _iTriangleIndex + _iVertexIndex].vPos = _vNewPosition;
	m_vecVertices[3 * _iTriangleIndex + _iVertexIndex] = _vNewPosition;

	m_pVB->Unlock();
}

_vec3 CManualCol::GetTriangleVertexPosition(_int _iTriangleIndex, _int _iVertexIndex) const
{
	return m_vecVertices[3 * _iTriangleIndex + _iVertexIndex];
}

Engine::CManualCol* Engine::CManualCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManualCol*	pInstance = new CManualCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))		// 버퍼 생성하고 정점 정보를 세팅한다.
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CManualCol::Clone(void)
{
	return new CManualCol(*this);
}

void Engine::CManualCol::Free(void)
{
	m_vecVertices.clear();
	m_vecVertices.shrink_to_fit();
	CVIBuffer::Free();
}
