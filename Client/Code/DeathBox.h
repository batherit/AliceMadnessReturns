#ifndef PopDomino_h__
#define PopDomino_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAliceW;
class CDeathBox : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CDeathBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CDeathBox(const  CDeathBox& rhs);
	virtual ~CDeathBox(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);

public:
	static CDeathBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

END

#endif // !LogoObject_h__