#ifndef Component_h__
#define Component_h__

#include "Engine_Define.h"
#include "Base.h"

class CGameObject;

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent();	// �Ϲ� ������
	explicit CComponent(const CComponent& rhs);			// ���� ������
	virtual ~CComponent();

public:
	virtual HRESULT Ready(void) = 0;
	virtual int Update(const _float& _fDeltaTime) = 0;
	virtual void Render(void) {};
	//virtual const _tchar* GetComponentTag() const = 0;
	//virtual void Release(void) = 0;

public:
	virtual CComponent* Clone(void) = 0;
protected:
	virtual void Free(void) = 0;
};

END

#endif