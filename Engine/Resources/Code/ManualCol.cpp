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
	// �Ӽ� ����
	m_dwFVF = FVF_TEX;
	m_dwTriCnt = 0;
	m_dwVtxCnt = 0;
	m_dwVtxSize = sizeof(VTXCOL);

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	m_vecVertices.reserve(600);	// �ﰢ�� 200�� ����

	return S_OK;
}

void CManualCol::Render_Buffer(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCOL));
	m_pGraphicDev->SetFVF(FVF_TEX);
	//m_pGraphicDev->SetIndices(m_pIB);
	//m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CManualCol::PushTriangleVertices(_vec3 _vTriPos1, _vec3 _vTriPos2, _vec3 _vTriPos3)
{
	// �ﰢ�� �߰�
	m_vecVertices.push_back(_vTriPos1);
	m_vecVertices.push_back(_vTriPos2);
	m_vecVertices.push_back(_vTriPos3);

	m_dwTriCnt = m_vecVertices.size() / 3;	// ���� 3���� �ﰢ�� �ϳ��� ���� �� �����Ƿ�.
	m_dwVtxCnt = m_vecVertices.size();

	// ���� ���� �ı� (�ε��� ���۴� ������� ���� ���̴�.)
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	// �� ���� ����
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), );

	VTXCOL*		pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);
	// 1���� : ��𼭺��� ��� ���ΰ�
	// 2���� : ���ڰ� 0�̸� ��ü ������ ��ٴ�.
	// 3���� : ���ڰ��� ���ؼ� ���ؽ� ���� �� ���� �� ù��° �ּҸ� ���´�.
	// 4���� : ��״� ���¸� ���� ����, ���� ������ ��� 

	// �ý��� �޸𸮿� �� ���� ���� ä���
	_int iVecSize = m_vecVertices.size();
	for (_int i = 0; i < iVecSize; ++i) {
		pVertex[i].vPos = m_vecVertices[i];
		pVertex[i].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	}

	m_pVB->Unlock();
}

void CManualCol::PopTriangleVertices(_int _iTriangleIndex)
{
	if (_iTriangleIndex <= 0 && _iTriangleIndex > static_cast<_int>(m_vecVertices.size()) / 3)
		return;

	// �ﰢ���� �� �������� �̷���� �����Ƿ� ������ �����ϱ� ���� for���� 3�� �����Ѵ�.
	for (_int i = 0; i < 3; ++i) {
		// ���� ����
		m_vecVertices.erase(m_vecVertices.begin() + 3 * _iTriangleIndex);
	}

	// ���� ���� �ı� (�ε��� ���۴� ������� ���� ���̴�.)
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	// �� ���� ����
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), );

	VTXCOL*		pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);
	// 1���� : ��𼭺��� ��� ���ΰ�
	// 2���� : ���ڰ� 0�̸� ��ü ������ ��ٴ�.
	// 3���� : ���ڰ��� ���ؼ� ���ؽ� ���� �� ���� �� ù��° �ּҸ� ���´�.
	// 4���� : ��״� ���¸� ���� ����, ���� ������ ��� 

	// �ý��� �޸𸮿� �� ���� ���� ä���
	_int iVecSize = m_vecVertices.size();
	for (_int i = 0; i < iVecSize; ++i) {
		pVertex[i].vPos = m_vecVertices[i];
		pVertex[i].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	}

	m_pVB->Unlock();
}

Engine::CManualCol* Engine::CManualCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManualCol*	pInstance = new CManualCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))		// ���� �����ϰ� ���� ������ �����Ѵ�.
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
