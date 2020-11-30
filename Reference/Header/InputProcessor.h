#ifndef InputProcessor_h__
#define InputProcessor_h__


#include "Engine_Define.h"
#include "Base.h"
//#include "InputProcessorMgr.h"

BEGIN(Engine)
class CInputProcessorMgr;
class ENGINE_DLL CInputProcessor abstract : public CBase
{
protected:
	explicit CInputProcessor(CInputProcessorMgr* _pInputProcessorMgr);	// 일반 생성자
	virtual ~CInputProcessor();

public:
	virtual _int ProcessInput(const _float& _fDeltaTime) = 0;

public:
	virtual void Free(void) override;

protected:
	CInputProcessorMgr* m_pInputProcessorMgr = nullptr;
};

END

#endif