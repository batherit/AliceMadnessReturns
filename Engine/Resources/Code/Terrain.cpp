#include "Terrain.h"

USING(Engine)
Engine::CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTerrain::CTerrain(const CTerrain& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CTerrain::~CTerrain(void)
{

}

HRESULT Engine::CTerrain::Ready_Buffer(void)
{
	// default
	return SetTerrainInfo(_vec3(0.f, 0.f, 0.f), 3, 3, 1.f, 1.f);
}

void CTerrain::Render_Buffer(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXTEX));
	m_pGraphicDev->SetFVF(FVF_TEX);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CTerrain::SetTerrainInfo(_vec3 _vStartPos, _uint _iNumOfVerticesW, _uint _iNumOfVerticesH, _float _fWidth, _float _fHeight, const _tchar* _szHeightMapFileName)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	// ���̸� �ε��ϱ�
	LoadHeightMap(_szHeightMapFileName);

	m_dwFVF = FVF_TEX;
	m_dwTriCnt = (_iNumOfVerticesW - 1) * (_iNumOfVerticesH - 1) * 2;	
	m_dwVtxCnt = _iNumOfVerticesW * _iNumOfVerticesH;		//4;
	m_dwVtxSize = sizeof(VTXTEX);

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	// �ε��� ���� ����
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * m_dwTriCnt,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL),
		E_FAIL);

	VTXTEX*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);
	// 1���� : ��𼭺��� ��� ���ΰ�
	// 2���� : ���ڰ� 0�̸� ��ü ������ ��ٴ�.
	// 3���� : ���ڰ��� ���ؼ� ���ؽ� ���� �� ���� �� ù��° �ּҸ� ���´�.
	// 4���� : ��״� ���¸� ���� ����, ���� ������ ��� 0

	_float fCU = 1.f / (_iNumOfVerticesW - 1);
	_float fCV = 1.f / (_iNumOfVerticesH - 1);
	for (_uint i = 0; i < _iNumOfVerticesH; ++i) {
		for (_uint j = 0; j < _iNumOfVerticesW; ++j) {
			pVertex[i * _iNumOfVerticesW + j].vPos = _vStartPos 
				+ _vec3(j * _fWidth, (m_pHeightMapData ? (m_pHeightMapData[4 * (_iNumOfVerticesW * i + j)] / 40.f) : 0.f), (i * _fHeight));
			pVertex[i * _iNumOfVerticesW + j].vTexUV = _vec2(fCU * j, 1.f - fCV * i);
		}
	}


	m_pVB->Unlock();

	vector<INDEX16> indices;

	for (_uint i = 1; i < _iNumOfVerticesH; ++i) {
		for (_uint j = 1; j < _iNumOfVerticesW; ++j) {
			/*indices.emplace_back(INDEX16{ 
				static_cast<_ushort>(0 + i * _iNumOfVerticesW + j), 
				static_cast<_ushort>(1 + i * _iNumOfVerticesW + j),
				static_cast<_ushort>(_iNumOfVerticesW + 1 + i * _iNumOfVerticesW + j) });
			indices.emplace_back(INDEX16{ 
				static_cast<_ushort>(0 + i * _iNumOfVerticesW + j),
				static_cast<_ushort>(_iNumOfVerticesW + 1 + i * _iNumOfVerticesW + j),
				static_cast<_ushort>(_iNumOfVerticesW + i * _iNumOfVerticesW + j) });*/

			indices.emplace_back(INDEX16{
				static_cast<_ushort>(0 + i * _iNumOfVerticesW + j),
				static_cast<_ushort>(1 + i * _iNumOfVerticesW + j),
				static_cast<_ushort>(0 + (i - 1) * _iNumOfVerticesW + j)});
			indices.emplace_back(INDEX16{
				static_cast<_ushort>(1 + (i - 1) * _iNumOfVerticesW + j),
				static_cast<_ushort>(0 + (i - 1) * _iNumOfVerticesW + j),
				static_cast<_ushort>(0 + i * _iNumOfVerticesW + j) });
		}
	}

	VOID* pIndices;
	m_pIB->Lock(0, sizeof(INDEX16) * indices.size(), (void**)&pIndices, 0);
	memcpy(pIndices, indices.data(), sizeof(INDEX16) * indices.size());
	m_pIB->Unlock();

	return S_OK;
}


Engine::CTerrain* Engine::CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))		// ���� �����ϰ� ���� ������ �����Ѵ�.
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTerrain::Clone(void)
{
	return new CTerrain(*this);
}

_bool CTerrain::LoadHeightMap(const _tchar * _szHeightMapFileName)
{
	if (!_szHeightMapFileName)
		return false;

	// ���̳ʸ� ���� ���̸� ���� ����
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, _szHeightMapFileName, L"rb");
	
	if (!err) {
		// ���� ����� �б�
		BITMAPFILEHEADER bmpFileHeader;
		if (fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp) != 1) {
			if (fp) fclose(fp);
			return false;
		}

		// ��Ʈ�� ���� ��� �б�
		BITMAPINFOHEADER bmpInfoHeader;
		if (fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp) != 1) {
			if (fp) fclose(fp);
			return false;
		}

		// ��Ʈ�� ũ�⸦ �����ϱ�
		LONG iImageWidth = bmpInfoHeader.biWidth;
		LONG iImageHeight = bmpInfoHeader.biHeight;

		// ��Ʈ�� �̹��� �������� ũ�⸦ ����ϱ�
		_int iImageSize = 4 * iImageWidth * iImageHeight;

		// ��Ʈ�� �̹��� �����͸� ���� �޸� �Ҵ��ϱ�
		if (m_pHeightMapData) {
			delete m_pHeightMapData;
			m_pHeightMapData = nullptr;
		}
		
		m_pHeightMapData = new _ubyte[iImageSize];

		// ��Ʈ�� �������� ���� �κ����� �̵��ϱ�
		fseek(fp, bmpFileHeader.bfOffBits, SEEK_SET);

		// ��Ʈ�� �̹��� �ε��ϱ�
		if (fread(m_pHeightMapData, 1, iImageSize, fp) != iImageSize) {
			if (m_pHeightMapData) {
				delete m_pHeightMapData;
				m_pHeightMapData = nullptr;
			}
			if (fp) 
				fclose(fp);
			return false;
		}
	}

	// ���� �ݱ�
	if (fp) fclose(fp);
	
	return true;
}

void Engine::CTerrain::Free(void)
{
	Safe_Delete(m_pHeightMapData);
	CVIBuffer::Free();
}
