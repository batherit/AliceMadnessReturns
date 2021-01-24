#ifndef EFT_SmokeBomb_h__
#define EFT_SmokeBomb_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CPlateEffect;
class CEFT_SmokeBomb : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CEFT_SmokeBomb(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CEFT_SmokeBomb(const  CEFT_SmokeBomb& rhs);
	virtual ~CEFT_SmokeBomb(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CEFT_SmokeBomb*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetInfo(const _vec3& _vPos, const _vec3& _vLook, _float _fRadius, _float _fOffset, _int _iParticleNum, _float _fParticleSpeed, _float _fResistanceXZ);

private:
	vector<CPlateEffect*> m_vecSmokeParticle;
};

END

#endif // !LogoObject_h__