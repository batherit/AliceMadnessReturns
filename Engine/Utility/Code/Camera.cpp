#include "Camera.h"
#include "Transform.h"

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
	if (m_bIsShaking) {
		m_fShakeTickTime += fTimeDelta;
		if (m_fShakeTickTime >= (m_fShakeKeepTime / m_iShakeNum)) {
			m_fShakeElapsedTime += m_fShakeTickTime;
			if (m_fShakeElapsedTime <= m_fShakeKeepTime) {
				m_fShakeTickTime = 0.f;
				_float fT = 1.f - m_fShakeElapsedTime / m_fShakeKeepTime;
				m_vShakeOffset = Engine::GetRotatedVector(GetTransform()->GetLook(), Engine::GetNumberBetweenMinMax(0.f, D3DX_PI * 2.f), GetTransform()->GetRight()) * m_fShakeRadius * fT;
			}
			else {
				m_fShakeKeepTime = 0.f;
				m_vShakeOffset = _vec3(0.f, 0.f, 0.f);
				m_bIsShaking = false;
			}
		}
	}



	D3DXMatrixLookAtLH(&m_matView, &(m_vEye + m_vShakeOffset), &(m_vAt + m_vShakeOffset), &m_vUp);
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

void CCamera::Shake(_float _fShakeKeepTime, _float _fSakeRadius, _int _iShakeNum)
{
	m_bIsShaking = true;
	m_fShakeKeepTime = _fShakeKeepTime;
	m_fShakeRadius = _fSakeRadius;
	m_iShakeNum = _iShakeNum;
	m_fShakeElapsedTime = 0.f;
	m_fShakeTickTime = 0.f;
	_float fT = 1.f - m_fShakeElapsedTime / m_fShakeKeepTime;

	m_vShakeOffset = Engine::GetRotatedVector(GetTransform()->GetLook(), Engine::GetNumberBetweenMinMax(0.f, D3DX_PI * 2.f), GetTransform()->GetRight()) * m_fShakeRadius * fT;

	//m_fShakeOffsetX = cosf(D3DXToRadian(m_fOffsetDegree)) * m_fShakeRadius * fT;
	//m_fShakeOffsetY = sinf(D3DXToRadian(m_fOffsetDegree)) * m_fShakeRadius * fT;
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

