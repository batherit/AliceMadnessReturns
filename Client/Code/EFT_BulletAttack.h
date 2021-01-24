#ifndef EFT_BulletAttack_h__
#define EFT_BulletAttack_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CPlateEffect;
class CEFT_BulletAttack : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CEFT_BulletAttack(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CEFT_BulletAttack(const  CEFT_BulletAttack& rhs);
	virtual ~CEFT_BulletAttack(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CEFT_BulletAttack*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetInfo(const _vec3& _vPos);

private:
	CPlateEffect* m_pAttackEffect = nullptr;
	CPlateEffect* m_pLightEffect = nullptr;
};

END

#endif // !LogoObject_h__