#ifndef TerrainMap_h__
#define TerrainMap_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CTerrainMap : public Engine::CGameObject
{
private: // ������, �Ҹ���
	explicit  CTerrainMap(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CTerrainMap(const  CTerrainMap& rhs);
	virtual ~CTerrainMap(void);

	// CGameObject��(��) ���� ��ӵ�
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
