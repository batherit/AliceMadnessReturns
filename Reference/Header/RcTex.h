#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex : public CVIBuffer
{
protected:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex(void);

public:
	virtual HRESULT	Ready_Buffer(void);

public:
	static CRcTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void);
};
END

#endif // RcTex_h__
