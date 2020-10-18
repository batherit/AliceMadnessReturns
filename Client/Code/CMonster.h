#ifndef Monster_h__
#define Monster_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CMonster : public Engine::CGameObject
{
private: // ������, �Ҹ���
	explicit  CMonster(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CGameObject* pTarget);
	explicit  CMonster(const  CMonster& rhs);
	virtual ~CMonster(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CMonster*	Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CGameObject* pTarget);
	virtual void		Free(void);

private:
	Engine::CGameObject* m_pTarget = nullptr;
};

END

#endif // !LogoObject_h__
