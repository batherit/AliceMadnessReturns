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

	// 높이맵 로드하기
	LoadHeightMap(_szHeightMapFileName);

	m_dwFVF = FVF_TEX;
	m_dwTriCnt = (_iNumOfVerticesW - 1) * (_iNumOfVerticesH - 1) * 2;	
	m_dwVtxCnt = _iNumOfVerticesW * _iNumOfVerticesH;		//4;
	m_dwVtxSize = sizeof(VTXTEX);

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	// 인덱스 버퍼 생성
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * m_dwTriCnt,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL),
		E_FAIL);

	VTXTEX*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);
	// 1인자 : 어디서부터 잠글 것인가
	// 2인자 : 숫자가 0이면 전체 영역을 잠근다.
	// 3인자 : 인자값을 통해서 버텍스 버퍼 내 정점 중 첫번째 주소를 얻어온다.
	// 4인자 : 잠그는 형태를 묻는 인자, 정적 버퍼인 경우 0

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

	if (FAILED(pInstance->Ready_Buffer()))		// 버퍼 생성하고 정점 정보를 세팅한다.
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

	// 바이너리 모드로 높이맵 파일 열기
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, _szHeightMapFileName, L"rb");
	
	if (!err) {
		// 파일 헤더를 읽기
		BITMAPFILEHEADER bmpFileHeader;
		if (fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp) != 1) {
			if (fp) fclose(fp);
			return false;
		}

		// 비트맵 정보 헤더 읽기
		BITMAPINFOHEADER bmpInfoHeader;
		if (fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp) != 1) {
			if (fp) fclose(fp);
			return false;
		}

		// 비트맵 크기를 저장하기
		LONG iImageWidth = bmpInfoHeader.biWidth;
		LONG iImageHeight = bmpInfoHeader.biHeight;

		// 비트맵 이미지 데이터의 크기를 계산하기
		_int iImageSize = 4 * iImageWidth * iImageHeight;

		// 비트맵 이미지 데이터를 위한 메모리 할당하기
		if (m_pHeightMapData) {
			delete m_pHeightMapData;
			m_pHeightMapData = nullptr;
		}
		
		m_pHeightMapData = new _ubyte[iImageSize];

		// 비트맵 데이터의 시작 부분으로 이동하기
		fseek(fp, bmpFileHeader.bfOffBits, SEEK_SET);

		// 비트맵 이미지 로드하기
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

	// 파일 닫기
	if (fp) fclose(fp);
	
	return true;
}

void Engine::CTerrain::Free(void)
{
	Safe_Delete(m_pHeightMapData);
	CVIBuffer::Free();
}
