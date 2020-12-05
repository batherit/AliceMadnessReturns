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
	m_pManualCol = AddComponent<Engine::CManualCol>();
	m_pRenderer = AddComponent<Engine::CPolygonRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pManualCol);
	return S_OK;
}

int CNaviMesh::Update_Object(const _float & fTimeDelta)
{
	//m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pRenderer->Update(fTimeDelta);
	return 0;
}

void CNaviMesh::Render_Object(void)
{
	// �ﰢ�� �׸���
	DWORD dwCullMode;
	m_pGraphicDev->GetRenderState(D3DRS_CULLMODE, &dwCullMode);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	m_pGraphicDev->SetTexture(0, NULL);
	//m_pManualCol->Render_Buffer();
	m_pRenderer->Render();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, dwCullMode);

	

	// �ﰢ�� ���� �׸���
	_matrix matWorld, matView, matProj;
	matWorld = m_pTransform->GetObjectMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	ID3DXLine *pLine;
	D3DXCreateLine(m_pGraphicDev, &pLine);
	pLine->SetWidth(2.f);
	pLine->SetAntialias(true);
	pLine->Begin();

	auto& vecVertices = GetNaviVertices();
	_int iVerticesSize = vecVertices.size();
	_vec3 vLastLine[2];
	for (_int i = 0; i < iVerticesSize / 3; ++i) {
		pLine->DrawTransform(&vecVertices[i * 3], 2, &(matWorld * matView * matProj), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		pLine->DrawTransform(&vecVertices[i * 3 + 1], 2, &(matWorld * matView * matProj), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

		vLastLine[0] = vecVertices[i * 3 + 2];
		vLastLine[1] = vecVertices[i * 3];
		pLine->DrawTransform(vLastLine, 2, &(matWorld * matView * matProj), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	}
	pLine->End();
	pLine->Release();

	// ���� �� �׸���. (���� �̵��ڿ� �־�� �� �� ����.)
	_matrix matTrans;
	_matrix matScale;
	for (auto& rPos : vecVertices) {
		D3DXMatrixIdentity(&matTrans);
		D3DXMatrixTranslation(&matTrans, rPos.x, rPos.y, rPos.z);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matTrans);
		m_pGraphicDev->SetTexture(0, NULL);
		Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"Buffer_SphereCol");
	}
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

	// ������ ��ũ�� Ʈ���̾ޱ��� �ִٸ�, �����Ѵ�.
	if (-1 != m_iMarkedTriangleIndex)
		ReleaseMarkedTriangle();

	// �ش� �ε����� Ʈ���̾ޱ��� ��ũ�Ѵ�.
	m_iMarkedTriangleIndex = _iTriangleIndex;
	m_pManualCol->SetTriangleColor(_iTriangleIndex, D3DCOLOR_ARGB(150, 255, 255, 50));
}

void CNaviMesh::ReleaseMarkedTriangle()
{
	if (-1 == m_iMarkedTriangleIndex)
		return;

	// ��ũ�� �ﰢ���� �����Ѵ�.
	m_pManualCol->SetTriangleColor(m_iMarkedTriangleIndex);
	m_iMarkedTriangleIndex = -1;
}

void CNaviMesh::SetTriangleVertexPosition(_int _iTriangleIndex, _int _iVertexIndex, const _vec3 & _vNewPosition)
{
	m_pManualCol->SetTriangleVertexPosition(_iTriangleIndex, _iVertexIndex, _vNewPosition);
}

_vec3 CNaviMesh::GetTriangleVertexPosition(_int _iTriangleIndex, _int _iVertexIndex) const
{
	return m_pManualCol->GetTriangleVertexPosition(_iTriangleIndex, _iVertexIndex);
}

vector<_vec3>& CNaviMesh::GetNaviVertices()
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_pManualCol->GetVertices();
}

void CNaviMesh::GenerateNewNaviMesh(vector<_vec3>& _rVertices)
{
	m_pManualCol->GenerateNewNaviMesh(_rVertices);
}

_bool CNaviMesh::SaveInfo(HANDLE & _hfOut)
{
	auto& rVertices = GetNaviVertices();
	_int iVerticesSize = static_cast<_int>(GetNaviVertices().size());
	WriteFile(_hfOut, &iVerticesSize, sizeof(_int), nullptr, nullptr);

	for (auto& rPos : GetNaviVertices()) {
		WriteFile(_hfOut, &rPos, sizeof(rPos), nullptr, nullptr);
	}

	return true;
}

_bool CNaviMesh::LoadInfo(HANDLE & _hfIn)
{
	_int iVerticesSize = 0;
	ReadFile(_hfIn, &iVerticesSize, sizeof(_int), nullptr, nullptr);
	vector<_vec3> vecVertices;
	vecVertices.reserve(iVerticesSize + 10);

	_vec3 vPos;
	for (_int i = 0; i < iVerticesSize; ++i) {
		ReadFile(_hfIn, &vPos, sizeof(vPos), nullptr, nullptr);
		vecVertices.emplace_back(vPos);
	}

	GenerateNewNaviMesh(vecVertices);

	vecVertices.clear();
	vecVertices.shrink_to_fit();

	return true;
}
