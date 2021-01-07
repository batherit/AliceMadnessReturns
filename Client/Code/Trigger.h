#ifndef Trigger_h__
#define Trigger_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CTrigger : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CTrigger(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CTrigger(const  CTrigger& rhs);
	virtual ~CTrigger(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CTrigger*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

	virtual _bool SaveInfo(HANDLE& _hfOut) override;
	virtual _bool LoadInfo(HANDLE& _hfIn) override;

	void SetTriggerType(TRIGGER::E_TYPE _eTriggerType) { m_eTriggerType = _eTriggerType; }
	TRIGGER::E_TYPE GetTriggerType() const { return m_eTriggerType; }
	void SetSortingOrderIndex(_int _iSortingOrderIndex) { m_iSortingOrderIndex = _iSortingOrderIndex; }
	_int GetSortingOrderIndex() const { return m_iSortingOrderIndex; }

private:
	_int m_iSortingOrderIndex = 0;
	TRIGGER::E_TYPE m_eTriggerType = TRIGGER::TYPE_END;
};

END

#endif // !LogoObject_h__
