#ifndef RcCol_h__
#define RcCol_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcCol : public CVIBuffer
{
protected:
	explicit CRcCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcCol(const CRcCol& rhs);
	virtual ~CRcCol(void);

public:
	virtual HRESULT	Ready_Buffer(void) override;
	virtual void	Render_Buffer(void) override;

public:
	static CRcCol*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void);
};
END

#endif // TriCol_h__