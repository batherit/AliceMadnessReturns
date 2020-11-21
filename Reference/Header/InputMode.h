#ifndef InputMode_h__
#define InputMode_h__


#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInputMode abstract : public CBase
{
protected:
	explicit CInputMode();	// �Ϲ� ������
	virtual ~CInputMode();

public:
	virtual void OnLButtonDown() {};

public:
	virtual void Free(void) override;
};

END

#endif