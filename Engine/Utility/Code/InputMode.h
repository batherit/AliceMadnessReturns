#ifndef InputMode_h__
#define InputMode_h__


#include "Engine_Define.h"
#include "Base.h"
//#include "InputModeMgr.h"

BEGIN(Engine)
class CInputModeMgr;
class ENGINE_DLL CInputMode abstract : public CBase
{
protected:
	explicit CInputMode(CInputModeMgr* _pInputModeMgr);	// 일반 생성자
	virtual ~CInputMode();

public:
	virtual _int ProcessInput(const _float& _fDeltaTime) = 0;

public:
	virtual void Free(void) override;

protected:
	CInputModeMgr* m_pInputModeMgr = nullptr;
};

END

#endif