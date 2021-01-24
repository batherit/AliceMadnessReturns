#include "pch.h"
#include "EffectMgr.h"

CEffectMgr::CEffectMgr(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CChronoTimer* _pTimer)
	:
	CGameObject(pGraphicDev),
	m_pTimer(_pTimer)
{
}

CEffectMgr::CEffectMgr(const CEffectMgr & rhs)
	:
	CGameObject(rhs)
{
}

CEffectMgr::~CEffectMgr(void)
{
}

HRESULT CEffectMgr::Ready_Object(void)
{
	m_pTimer = Engine::GetTimer(L"Timer_FPS60");

	return S_OK;
}

_int CEffectMgr::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (m_bIsTimeSpeed) {
		if ((m_fKeepTime -= _fDeltaTime / m_pTimer->GetDeltaMultiple()) <= 0.f) {
			m_pTimer->SetDeltaMultiple(1.f);
			m_fKeepTime = 0.f;
			m_bIsTimeSpeed = false;
		}
	}

	return 0;
}

void CEffectMgr::Render_Object(void)
{

}

CEffectMgr * CEffectMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CChronoTimer* _pTimer)
{
	CEffectMgr*	pInstance = new CEffectMgr(pGraphicDev, _pTimer);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CEffectMgr::Free(void)
{
	m_pTimer->SetDeltaMultiple(1.f);

	CGameObject::Free();
}

void CEffectMgr::SetTimeSpeed(_float _fDeltaMultiple, _float _fKeepTime)
{
	if (_fKeepTime <= 0.f)
		return;

	m_pTimer->SetDeltaMultiple(_fDeltaMultiple);
	m_fKeepTime = _fKeepTime;
	m_bIsTimeSpeed = true;
}
