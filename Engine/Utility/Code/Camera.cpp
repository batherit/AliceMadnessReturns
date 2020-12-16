#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

Engine::CCamera::~CCamera(void)
{

}

HRESULT Engine::CCamera::Ready_Object(void)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

Engine::_int Engine::CCamera::Update_Object(const _float& fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}

void Engine::CCamera::SetProjectionMatrix(const _float & _fFOV, const _float & _fAspect, const _float & _fNearZ, const _float & _fFarZ)
{
	m_fFovY = _fFOV;
	m_fAspect = _fAspect;
	m_fNear = _fNearZ;
	m_fFar = _fFarZ;

	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, // 행렬 결과
		_fFOV,		// fovY
		_fAspect,	// 종횡비
		_fNearZ,	// 절두체의 near 평면의 z값
		_fFarZ); // 절두체의 far 평면의 z값

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CCamera * CCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCamera*	pInstance = new CCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCamera::Free(void)
{
	CGameObject::Free();
}

