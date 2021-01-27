#include "Trail.h"

USING(Engine)

Engine::CTrail::CTrail(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTrail::CTrail(const CTrail& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CTrail::~CTrail()
{

}

HRESULT Engine::CTrail::Ready_Buffer(const _ulong& dwVtxMax)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = dwVtxMax;
	m_dwFVF = FVF_TEX;
	m_dwTriCnt = dwVtxMax - 2;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	return S_OK;
}

void CTrail::Render_Buffer()
{
	if (!m_pTrailList || m_pTrailList->size() < 4) {
		m_pTrailList = nullptr;
		return;
	}

	//Add_Vertex_CatmullRom(m_pTrailList);
	Clear_Vertex();
	for (auto iter = m_pTrailList->begin(); iter != m_pTrailList->end(); ++iter) {
		Add_Vertex(&iter->first, &iter->second);
	}

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwFVF);
	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwUsingVtxCnt, 0, m_dwUsingTriCnt);
	
	m_pTrailList = nullptr;
}

//void CTrail::Render_Buffer(const list<pair<_vec3, _vec3> >* pTrailList)
//{
//	if (pTrailList->size() < 4)
//		return;
//
//	Add_Vertex_CatmullRom(pTrailList);
//
//	Render_Buffer();
//}

HRESULT CTrail::Add_Vertex(const _vec3 * pTopPos, const _vec3 * pBottomPos)
{
	if (m_dwUsingVtxCnt >= m_dwVtxCnt || m_dwUsingTriCnt >= m_dwTriCnt)
		return S_OK;

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[m_dwUsingVtxCnt].vPos = *pTopPos;
	++m_dwUsingVtxCnt;

	pVertex[m_dwUsingVtxCnt].vPos = *pBottomPos;
	++m_dwUsingVtxCnt;

	if (m_dwUsingVtxCnt <= 2)
	{
		m_pVB->Unlock();
		return S_OK;
	}

	for (_ulong i = 0; i < m_dwUsingVtxCnt; i += 2)
	{
		pVertex[i].vTexUV = _vec2((i / 2) / (_float)(m_dwUsingVtxCnt / 2), 0.f);
		pVertex[i + 1].vTexUV = _vec2(((i + 1) / 2) / (_float)(m_dwUsingVtxCnt / 2), 1.f);
	}

	m_pVB->Unlock();
	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[m_dwUsingTriCnt]._0 = m_dwUsingVtxCnt - 4;
	pIndex[m_dwUsingTriCnt]._1 = m_dwUsingVtxCnt - 2;
	pIndex[m_dwUsingTriCnt]._2 = m_dwUsingVtxCnt - 1;
	++m_dwUsingTriCnt;

	pIndex[m_dwUsingTriCnt]._0 = m_dwUsingVtxCnt - 4;
	pIndex[m_dwUsingTriCnt]._1 = m_dwUsingVtxCnt - 1;
	pIndex[m_dwUsingTriCnt]._2 = m_dwUsingVtxCnt - 3;
	++m_dwUsingTriCnt;

	m_pIB->Unlock();



	return S_OK;
}

HRESULT CTrail::Clear_Vertex()
{
	m_dwUsingTriCnt = 0;
	m_dwUsingVtxCnt = 0;

	return S_OK;
}

HRESULT CTrail::Add_Vertex_CatmullRom(const list<pair<_vec3, _vec3>>* pTrailList)
{
	list<pair<_vec3, _vec3>> ItplList(pTrailList->begin(), pTrailList->end());
	for (_int i = 0; i < 3; ++i)
	{
		auto iter0 = ItplList.begin();
		auto iter1 = ++iter0;
		auto iter2 = ++iter1;
		auto iter3 = ++iter2;
		auto iterEnd = ItplList.end();
		for (; iter3 != iterEnd;)
		{
			_vec3 vItplTop, vItplBottom;
			D3DXVec3CatmullRom(&vItplTop, &iter0->first, &iter1->first, &iter2->first, &iter3->first, 0.5f);
			D3DXVec3CatmullRom(&vItplBottom, &iter0->second, &iter1->second, &iter2->second, &iter3->second, 0.5f);

			ItplList.insert(iter2, make_pair(vItplTop, vItplBottom));

			iter0 = iter1;
			iter1 = iter2;
			iter2 = iter3;
			++iter3;
		}
	}

	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	m_dwUsingVtxCnt = (_ulong)ItplList.size() * 2;
	m_dwUsingTriCnt = m_dwUsingVtxCnt - 2;

	if ((_ulong)ItplList.size() > m_dwVtxCnt)
	{
		wstring wstrMsg = L"The number of vertex is over. There are " + to_wstring(m_dwVtxCnt) + L" reserved vertices.";
		MessageBox(NULL, wstrMsg.c_str(), L"Trail", MB_OK);
	}

	_int i = 0;
	for (auto& Pair : ItplList)
	{
		pVertex[i].vPos = Pair.first;
		pVertex[i].vTexUV = _vec2(((_float)(i / 2)) / (m_dwUsingVtxCnt / 2), 0.f);
		++i;
		pVertex[i].vPos = Pair.second;
		pVertex[i].vTexUV = _vec2(((_float)(i / 2)) / (m_dwUsingVtxCnt / 2), 1.f);
		++i;
	}

	m_pVB->Unlock();

	INDEX16* pIndex;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_uint i = 0; i < m_dwUsingTriCnt; ++i)
	{
		pIndex[i]._0 = i;
		pIndex[i]._1 = i + 2;
		pIndex[i]._2 = i + 3;

		++i;
		pIndex[i]._0 = i - 1;
		pIndex[i]._1 = i + 2;
		pIndex[i]._2 = i;
	}

	m_pIB->Unlock();

	return S_OK;
}

Engine::CTrail* Engine::CTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwVtxMax)
{
	CTrail* pInst = new CTrail(pGraphicDev);

	if (FAILED(pInst->Ready_Buffer(dwVtxMax)))
		Safe_Release(pInst);

	return pInst;
}

CComponent* Engine::CTrail::Clone()
{
	return new CTrail(*this);
}

void Engine::CTrail::Free()
{
	CVIBuffer::Free();
}


