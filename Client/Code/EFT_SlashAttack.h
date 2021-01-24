#ifndef EFT_SlashAttack_h__
#define EFT_SlashAttack_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CPlateEffect;
class CEFT_SlashAttack : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CEFT_SlashAttack(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CEFT_SlashAttack(const  CEFT_SlashAttack& rhs);
	virtual ~CEFT_SlashAttack(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CEFT_SlashAttack*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetInfo(const _vec3& _vPos);

private:
	CPlateEffect* m_pSlashEffect = nullptr;
	CPlateEffect* m_pLightEffect = nullptr;
};

END

#endif // !LogoObject_h__