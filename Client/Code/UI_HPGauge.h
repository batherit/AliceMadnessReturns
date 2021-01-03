#ifndef UI_HPGauge_h__
#define UI_HPGauge_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CUI_Image;
class CUI_HPGauge : public Engine::CGameObject
{
#define OFFSET_X 30
#define OFFSET_Y 30
private: // 생성자, 소멸자
	explicit  CUI_HPGauge(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_HPGauge(const  CUI_HPGauge& rhs);
	virtual ~CUI_HPGauge(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_HPGauge*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetPlayer(Engine::CGameObject* _pPlayer);

private:
	Engine::CGameObject* m_pPlayer = nullptr;
	_float m_fCurHP = 0.f;
	CUI_Image* m_pBackImage = nullptr;
	vector<CUI_Image*> m_vecPiece;
};

END

#endif // !LogoObject_h__