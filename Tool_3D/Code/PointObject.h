#ifndef PointObject_h__
#define PointObject_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CPointObject : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CPointObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CPointObject(const  CPointObject& rhs);
	virtual	~CPointObject(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CPointObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);

public:
	void SetPosition(const _vec3& _vPos);
	_vec3 GetPosition() const;
	void Translate(const _vec3& _vDeltaMove) const;

	_bool IsPickedByRay(const Engine::PICKINGRAYINFO& _stRayInfo);

private:
	Engine::CRenderer* m_pRenderer = nullptr;
};

END

#endif // !TerrainMap_h__