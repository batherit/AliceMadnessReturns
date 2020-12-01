#ifndef StaticObject_h__
#define StaticObject_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CStaticObject : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CStaticObject(const  CStaticObject& rhs);
	virtual ~CStaticObject(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CStaticObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _pMeshTag);
	virtual void		Free(void);

private:
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	const _tchar* m_pMeshTag = nullptr;
};

END

#endif // !LogoObject_h__
