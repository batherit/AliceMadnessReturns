#ifndef SphereCol_h__
#define SphereCol_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSphereCol : public CVIBuffer
{
protected:
	explicit CSphereCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphereCol(const CSphereCol& rhs);
	virtual ~CSphereCol(void);

public:
	virtual HRESULT	Ready_Buffer(void) override;
	virtual void	Render_Buffer(void) override;
	static const _tchar* GetComponentTag() { return L"SphereCol"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

public:
	static CSphereCol*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void);

private:
	LPD3DXMESH m_pSphereMesh = nullptr;
	LPD3DXMESH m_pClone = nullptr;
};
END

#endif // TriCol_h__