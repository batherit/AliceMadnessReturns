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
	void CreateTerrain(_uint _iNumOfVerticesW, _uint _iNumOfVerticesH, _float _fWidth, _float _fHeight, const _tchar* _szHeightMapFileName);

public:
	static CTerrain*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	Engine::CPolygonRenderer* m_pRenderer = nullptr;
	Engine::CTerrainTex* m_pTerrain = nullptr;
	Engine::CTexture*	m_pTexture = nullptr;
};

END

#endif // !TerrainMap_h__
