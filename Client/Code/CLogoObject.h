#ifndef LogoObject_h__
#define LogoObject_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CLogoObject : public Engine::CGameObject
{
private: // ������, �Ҹ���
	explicit  CLogoObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CLogoObject(const  CLogoObject& rhs);
	virtual ~CLogoObject(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CLogoObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

END

#endif // !LogoObject_h__
