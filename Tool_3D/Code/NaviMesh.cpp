#include "pch.h"
#include "NaviMesh.h"


CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CNaviMesh::CNaviMesh(const CNaviMesh & rhs)
	:
	CGameObject(rhs)
{
}

CNaviMesh::~CNaviMesh(void)
{
}


HRESULT CNaviMesh::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();
	m_pManualCol = AddComponent<Engine::CManualCol>();
	return S_OK;
}

int CNaviMesh::Update_Object(const _float & fTimeDelta)
{
	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CNaviMesh::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	m_pGraphicDev->SetTexture(0, NULL);
	m_pManualCol->Render_Buffer();
}

CNaviMesh * CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CNaviMesh::Free(void)
{
	CGameObject::Free();
}

_bool CNaviMesh::IsValidTriangleIndex(_int _iTriangleIndex)
{
	return m_pManualCol->IsValidTriangleIndex(_iTriangleIndex);
}

_bool CNaviMesh::IsValidVertexIndex(_int _iVertexIndex)
{
	return m_pManualCol->IsValidVertexIndex(_iVertexIndex);
}

_bool CNaviMesh::IsValidIndex(_int _iTriangleIndex, _int _iVertexIndex)
{
	return m_pManualCol->IsValidIndex(_iTriangleIndex, _iVertexIndex);
}

void CNaviMesh::PushTriangleVertices(_vec3 _vTriPos1, _vec3 _vTriPos2, _vec3 _vTriPos3)
{
	m_pManualCol->PushTriangleVertices(_vTriPos1, _vTriPos2, _vTriPos3);
}

void CNaviMesh::PopTriangleVertices(_int _iTriangleIndex)
{
	m_pManualCol->PopTriangleVertices(_iTriangleIndex);

	if (-1 == m_iMarkedTriangleIndex)
		return;

	if (_iTriangleIndex < m_iMarkedTriangleIndex) {
		--m_iMarkedTriangleIndex;
	}
	else if (_iTriangleIndex == m_iMarkedTriangleIndex) {
		m_iMarkedTriangleIndex = -1;
	}
}

void CNaviMesh::MarkTriangle(_int _iTriangleIndex)
{
	if (!IsValidTriangleIndex(_iTriangleIndex))
		return;

	// 기존에 마크된 트라이앵글이 있다면, 해제한다.
	if (-1 != m_iMarkedTriangleIndex)
		ReleaseMarkedTriangle();

	// 해당 인덱스의 트라이앵글을 마크한다.
	m_iMarkedTriangleIndex = _iTriangleIndex;
	m_pManualCol->SetTriangleColor(_iTriangleIndex, D3DCOLOR_ARGB(150, 255, 255, 50));
}

void CNaviMesh::ReleaseMarkedTriangle()
{
	if (-1 == m_iMarkedTriangleIndex)
		return;

	// 마크된 삼각형을 해제한다.
	m_pManualCol->SetTriangleColor(m_iMarkedTriangleIndex);
	m_iMarkedTriangleIndex = -1;
}

void CNaviMesh::SetTriangleVertexPosition(_int _iTriangleIndex, _int _iVertexIndex, const _vec3 & _vNewPosition)
{
	m_pManualCol->SetTriangleVertexPosition(_iTriangleIndex, _iVertexIndex, _vNewPosition);
}

_vec3 CNaviMesh::GetTriangleVertexPosition(_int _iTriangleIndex, _int _iVertexIndex) const
{
	return _vec3();
}

vector<_vec3>& CNaviMesh::GetNaviVertices()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pManualCol->GetVertices();
}
