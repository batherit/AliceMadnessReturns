#ifndef Player_h__
#define Player_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CPlayer : public Engine::CGameObject
{
private: // ������, �Ҹ���
	explicit  CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CPlayer(const  CPlayer& rhs);
	virtual ~CPlayer(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CPlayer*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	Engine::CMoveComponent* m_pMoveComponent = nullptr;
};

END

#endif // !LogoObject_h__
