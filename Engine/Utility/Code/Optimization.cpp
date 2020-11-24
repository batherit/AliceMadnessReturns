#include "Optimization.h"

USING(Engine)

Engine::COptimization::COptimization(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pFrustum(nullptr)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::COptimization::COptimization(const COptimization& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pFrustum(rhs.m_pFrustum)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::COptimization::~COptimization(void)
{

}

HRESULT Engine::COptimization::Ready_Optimization(void)
{
	m_pFrustum = CFrustum::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pFrustum, E_FAIL);

	
	return S_OK;
}

Engine::_bool Engine::COptimization::Is_InFrustumForObject(const _vec3* pWorldPos, const _float& fRadius)
{
	return m_pFrustum->Is_InFrustumForObject(pWorldPos, fRadius);
}

Engine::COptimization* Engine::COptimization::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	COptimization*		pInstance = new COptimization(pGraphicDev);

	if (FAILED(pInstance->Ready_Optimization()))
		Safe_Release(pInstance);
	
	return pInstance;
}

Engine::CComponent* Engine::COptimization::Clone(void)
{
	return new COptimization(*this);
}

void Engine::COptimization::Free(void)
{
	if (false == m_bClone)
		Safe_Release(m_pFrustum);

	Safe_Release(m_pGraphicDev);
}

HRESULT COptimization::Ready(void)
{
	return E_NOTIMPL;
}

int COptimization::Update(const _float & _fDeltaTime)
{
	return 0;
}

