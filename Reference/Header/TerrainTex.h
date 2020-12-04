#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
protected:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex(void);

public:
	virtual HRESULT	Ready_Buffer(void) override;
	virtual void	Render_Buffer(void) override;
	static const _tchar* GetComponentTag() { return L"TerrainTex"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

	HRESULT SetTerrainInfo(_uint _iNumOfVerticesW = 2, _uint _iNumOfVerticesH = 2, _float _fWidth = 100.f, _float _fHeight = 100.f, _float _fU = 1.f, _float _fV = 1.f, const _tchar* _szHeightMapFileName = nullptr);
	_float GetHeight(const _vec3& _vPos);
	vector<_vec3>& GetVertices() { return m_vecVertices; }
	vector<INDEX16>& GetIndexes() { return m_vecIndexes; }

public:
	static CTerrainTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void) override;

public:
	inline _uint GetNumOfVerticesW() const { return m_iNumOfVerticesW; }
	inline _uint GetNumOfVerticesH() const { return m_iNumOfVerticesH; }
	inline _float GetWidth() const { return m_fWidth; }
	inline _float GetHeight() const { return m_fHeight; }
	inline _float GetU() const { return m_fU; }
	inline _float GetV() const { return m_fV; }

private:
	_bool LoadHeightMap(const _tchar* _szHeightMapFileName);

private:
	virtual void Free(void);

private:
	_ubyte* m_pHeightMapData = nullptr;
	BITMAPINFOHEADER m_stBitmapInfoHeader;
	_uint m_iBitmapSizeW;
	_uint m_iBitmapSizeH;
	_uint m_iNumOfVerticesW = 2;
	_uint m_iNumOfVerticesH = 2;
	_float m_fWidth = 100.f;
	_float m_fHeight = 100.f;
	_float m_fU = 1.f;
	_float m_fV = 1.f;
	vector<_vec3> m_vecVertices;
	vector<INDEX16> m_vecIndexes;
};
END

#endif // TriCol_h__