#ifndef Terrain_h__
#define Terrain_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CTerrain : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CTerrain(const  CTerrain& rhs);
	virtual ~CTerrain(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	_float GetHeight(_vec3 _vPos) const {
		return m_pTerrain->GetHeight(_vPos);
	}
	void CreateTerrain(_uint _iNumOfVerticesW = 2, _uint _iNumOfVerticesD = 2, _float _fWidth = 100.f, _float _fDepth = 100.f, _float _fU = 1.f, _float _fV = 1.f, const _tchar* _szHeightMapFileName = nullptr);
	void SetHeightMapFileName(const _tchar* _szHeightMapFileName);
	void SetVtxNum(_uint _iNumOfVerticesW, _uint _iNumOfVerticesH);
	void SetSize(_float _fWidth, _float _fDepth);
	void SetUV(_float _fU, _float _fV);
	void SetTextureTag(const _tchar* _pTextureTag);

public:
	static CTerrain*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	virtual _bool SaveInfo(HANDLE& _hfOut);	// API
	virtual _bool LoadInfo(HANDLE& _hfIn);

private:
	Engine::CPolygonRenderer* m_pRenderer = nullptr;
	Engine::CTerrainTex* m_pTerrain = nullptr;
	Engine::CTexture*	m_pTexture = nullptr;
	_tchar m_szTextureTag[MAX_PATH] = L"";
	_tchar m_szHeightMapFileName[MAX_PATH] = L"";
};

END

#endif // !TerrainMap_h__
