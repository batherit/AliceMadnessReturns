#include "TerrainTex.h"

USING(Engine)
Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CTerrainTex::~CTerrainTex(void)
{

}

HRESULT Engine::CTerrainTex::Ready_Buffer(void)
{
	// default
	return SetTerrainInfo(_vec3(0.f, 0.f, 0.f), 3, 3, 1.f, 1.f);
}

void CTerrainTex::Render_Buffer(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXTEX));
	m_pGraphicDev->SetFVF(FVF_TEX);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CTerrainTex::SetTerrainInfo(_vec3 _vStartPos, _uint _iNumOfVerticesW, _uint _iNumOfVerticesH, _float _fWidth, _float _fHeight, const _tchar* _szHeightMapFileName)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	// ���̸� �ε��ϱ�
	LoadHeightMap(_szHeightMapFileName);

	m_vStartPos = _vStartPos;
	m_iNumOfVerticesW = _iNumOfVerticesW;
	m_iNumOfVerticesH = _iNumOfVerticesH;
	m_fWidth = _fWidth;
	m_fHeight = _fHeight;

	m_dwFVF = FVF_TEX;
	m_dwTriCnt = (_iNumOfVerticesW - 1) * (_iNumOfVerticesH - 1) * 2;	
	m_dwVtxCnt = _iNumOfVerticesW * _iNumOfVerticesH;		//4;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	// ���� ����
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);
	// 1���� : ��𼭺��� ��� ���ΰ�
	// 2���� : ���ڰ� 0�̸� ��ü ������ ��ٴ�.
	// 3���� : ���ڰ��� ���ؼ� ���ؽ� ���� �� ���� �� ù��° �ּҸ� ���´�.
	// 4���� : ��״� ���¸� ���� ����, ���� ������ ��� 

	// �ý��� �޸𸮿� �� ���� ����
	m_vecVertices.resize(_iNumOfVerticesW * _iNumOfVerticesH);

	_float fCU = 1.f / (_iNumOfVerticesW - 1);
	_float fCV = 1.f / (_iNumOfVerticesH - 1);
	_float fCX = _fWidth / (_iNumOfVerticesW - 1);
	_float fCY = _fHeight / (_iNumOfVerticesH - 1);
	_int iIndex = 0;
	for (_uint i = 0; i < _iNumOfVerticesH; ++i) {
		for (_uint j = 0; j < _iNumOfVerticesW; ++j) {
			iIndex = _iNumOfVerticesW * i + j;
			m_vecVertices[iIndex] = pVertex[iIndex].vPos = _vStartPos
				+ _vec3(j * fCX, (m_pHeightMapData ? (m_pHeightMapData[m_stBitmapInfoHeader.biBitCount/CHAR_BIT * iIndex] / 10.f) : 0.f), (i * fCY));
			pVertex[iIndex].vTexUV = _vec2(fCU * j, 1.f - fCV * i);
		}
	}

	m_pVB->Unlock();

	vector<INDEX16> indices;
	m_vecIndexes.resize(2 * (_iNumOfVerticesW - 1) * (_iNumOfVerticesH - 1));
	m_vecIndexes.clear();

	INDEX16 Indexes;
	for (_uint i = 1; i < _iNumOfVerticesH; ++i) {
		for (_uint j = 0; j < _iNumOfVerticesW - 1; ++j) {
			Indexes = INDEX16{
				static_cast<_ushort>(0 + i * _iNumOfVerticesW + j),
				static_cast<_ushort>(1 + i * _iNumOfVerticesW + j),
				static_cast<_ushort>(1 + (i - 1) * _iNumOfVerticesW + j) };
			m_vecIndexes.emplace_back(Indexes);
			indices.emplace_back(Indexes);

			Indexes = INDEX16{
				static_cast<_ushort>(1 + (i - 1) * _iNumOfVerticesW + j),
				static_cast<_ushort>(0 + (i - 1) * _iNumOfVerticesW + j),
				static_cast<_ushort>(0 + i * _iNumOfVerticesW + j) };
			m_vecIndexes.emplace_back(Indexes);
			indices.emplace_back(Indexes);
		}
	}

	VOID* pIndices;
	m_pIB->Lock(0, sizeof(INDEX16) * indices.size(), (void**)&pIndices, 0);
	memcpy(pIndices, indices.data(), sizeof(INDEX16) * indices.size());
	m_pIB->Unlock();

	return S_OK;
}

_float Engine::CTerrainTex::GetHeight(const _vec3& _vPos) {
	_vec3 vIndexPos = _vPos - m_vStartPos;

	if (vIndexPos.x < 0.f || vIndexPos.x > m_fWidth) return 0.f;
	if (vIndexPos.z < 0.f || vIndexPos.z > m_fHeight) return 0.f;

	_float fCX = m_fWidth / (m_iNumOfVerticesW - 1);
	_float fCY = m_fHeight / (m_iNumOfVerticesH - 1);

	_float fRow = vIndexPos.z / fCY + 1;
	_float fCol = vIndexPos.x / fCX;
	_int iRow = static_cast<_int>(fRow);
	_int iCol = static_cast<_int>(fCol);

	//// ���� �ε��� ���ϱ�
	//_vec3 vRightTopPos = m_vecVertices[(iRow + 1) * m_iNumOfVerticesW + iCol + 1];
	//_vec3 vLeftBottomPos = m_vecVertices[iRow * m_iNumOfVerticesW + iCol];
	D3DXPLANE plane;
	if (fRow - static_cast<_float>(iRow) > fCol - static_cast<_float>(iCol)) {
		// ���ϴܿ� �� ������.
		D3DXPlaneFromPoints(&plane,
			&m_vecVertices[iRow * m_iNumOfVerticesW + iCol],
			&m_vecVertices[iRow * m_iNumOfVerticesW + iCol + 1],
			&m_vecVertices[(iRow - 1) * m_iNumOfVerticesW + iCol + 1]);
	}
	else {
		// ���ܿ� �� ������.
		D3DXPlaneFromPoints(&plane,
			&m_vecVertices[iRow * m_iNumOfVerticesW + iCol],
			&m_vecVertices[(iRow - 1) * m_iNumOfVerticesW + iCol + 1],
			&m_vecVertices[(iRow - 1) * m_iNumOfVerticesW + iCol]);
	}

	return (plane.a * _vPos.x + plane.c * _vPos.z  + plane.d) / -plane.b;

	// ���� �������ε� ���� �� �ִ�.
}


Engine::CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrainTex*	pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))		// ���� �����ϰ� ���� ������ �����Ѵ�.
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTerrainTex::Clone(void)
{
	return new CTerrainTex(*this);
}

_bool CTerrainTex::LoadHeightMap(const _tchar * _szHeightMapFileName)
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
		//BITMAPINFOHEADER bmpInfoHeader;
		if (fread(&m_stBitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp) != 1) {
			if (fp) fclose(fp);
			return false;
		}

		//// ��Ʈ�� ũ��/�ȼ� ��Ʈ�� �����ϱ�
		//m_stHeightMapInfo.iWidth = bmpInfoHeader.biWidth;
		//m_stHeightMapInfo.iHeight = bmpInfoHeader.biHeight;
		//m_stHeightMapInfo.iBitCount = bmpInfoHeader.

		// ��Ʈ�� �̹��� �������� ũ�⸦ ����ϱ�
		_int iImageSize = (m_stBitmapInfoHeader.biBitCount / CHAR_BIT) * m_stBitmapInfoHeader.biWidth * m_stBitmapInfoHeader.biHeight;

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

void Engine::CTerrainTex::Free(void)
{
	m_vecVertices.clear();
	m_vecVertices.shrink_to_fit();
	m_vecIndexes.clear();
	m_vecIndexes.shrink_to_fit();
	Safe_Delete(m_pHeightMapData);
	CVIBuffer::Free();
}


//VTXTEX*		pVertex = NULL;
	//m_pVB->Lock(0, 0, (void**)&pVertex, NULL);

	//// ���� �ε��� ���ϱ�
	//vRightTopPos = pVertex[(iRow + 1) * m_iNumOfVerticesW + iCol + 1].vPos;
	//vLeftBottomPos = pVertex[iRow * m_iNumOfVerticesW + iCol].vPos;

	//if (D3DXVec3LengthSq(&(vRightTopPos - _vPos)) > D3DXVec3LengthSq(&(vLeftBottomPos - _vPos))) {
	//	// ���ϴܿ� �� ������.
	//	D3DXPlaneFromPoints(&plane,
	//		&pVertex[iRow * m_iNumOfVerticesW + iCol + 1].vPos,
	//		&pVertex[iRow * m_iNumOfVerticesW + iCol].vPos,
	//		&pVertex[(iRow + 1) * m_iNumOfVerticesW + iCol].vPos);
	//}
	//else {
	//	// ���ܿ� �� ������.
	//	D3DXPlaneFromPoints(&plane,
	//		&pVertex[(iRow + 1) * m_iNumOfVerticesW + iCol].vPos,
	//		&pVertex[(iRow + 1) * m_iNumOfVerticesW + iCol + 1].vPos,
	//		&pVertex[iRow * m_iNumOfVerticesW + iCol + 1].vPos);
	//}
	//m_pVB->Unlock();