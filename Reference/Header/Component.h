#ifndef Component_h__
#define Component_h__

#include "Engine_Define.h"
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent();	// 老馆 积己磊
	explicit CComponent(const CComponent& rhs);			// 汗荤 积己磊
	virtual ~CComponent();

public:
	virtual HRESULT Ready(void) = 0;
	virtual int Update(const _float& _fDeltaTime) = 0;
	//virtual void Render(void) {};
	void SetOwner(CGameObject* _pOwner) { if (m_pOwner) return; m_pOwner = _pOwner; }

public:
	virtual CComponent* Clone(void) = 0;

protected:
	virtual void Free(void) = 0;
	CGameObject* m_pOwner = nullptr;
	_bool		m_bClone;
};

END

#endif