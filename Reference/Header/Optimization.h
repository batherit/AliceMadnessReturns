#ifndef Optimization_h__
#define Optimization_h__

#include "Component.h"
#include "Frustum.h"

BEGIN(Engine)

class ENGINE_DLL COptimization : public CComponent
{
private:
	explicit COptimization(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit COptimization(const COptimization& rhs);
	virtual ~COptimization(void);

public:
	HRESULT				Ready_Optimization(void);
	_bool				Is_InFrustumForObject(const _vec3* pWorldPos, const _float& fRadius);

private:
	CFrustum*			m_pFrustum;
	LPDIRECT3DDEVICE9	m_pGraphicDev;

public:
	static COptimization*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*				Clone(void);
	virtual void					Free(void);

public:
	static const _tchar* GetComponentTag() { return L"Optimization"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Ready(void) override;
	virtual int Update(const _float & _fDeltaTime) override;
};

END
#endif // Optimization_h__
