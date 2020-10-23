#ifndef TerrainMap_h__
#define TerrainMap_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CTerrainMap : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CTerrainMap(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CTerrainMap(const  CTerrainMap& rhs);
	virtual ~CTerrainMap(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	_float GetHeight(_vec3 _vPos) const {
		return m_pTerrain->GetHeight(_vPos);
	}

public:
	static CTerrainMap*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	Engine::CRenderer* m_pRenderer = nullptr;
	Engine::CTerrain* m_pTerrain = nullptr;
};

END

#endif // !TerrainMap_h__
