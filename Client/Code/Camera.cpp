#include "pch.h"
#include "Camera.h"


CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CCamera::CCamera(const CCamera & rhs)
	:
	CGameObject(rhs)
{
}

CCamera::~CCamera(void)
{
}


HRESULT CCamera::Ready_Object(void)
{
	AddComponent<Engine::CMoveComponent>();
	m_pMoveComponent = GetComponent<Engine::CMoveComponent>();
	m_pMoveComponent->SetXYZ(30.f, 80.f, -80.f);
	m_pMoveComponent->RotateByRight(D3DXToRadian(45.f));
	m_pMoveComponent->SetMaxSpeed(110.f);
	m_pMoveComponent->SetSpeed(100.f);

	SetProjectionMatrix(D3DXToRadian(45.f), (_float)WINCX / WINCY, 0.1f, 1000.f);

	return S_OK;
}

int CCamera::Update_Object(const _float & fTimeDelta)
{	
	if (Engine::CKeyMgr::GetInstance()->IsKeyDown(L"KEY_LBUTTON")) {
		POINT ptCurrentCursor = Engine::GetClientCursorPoint(g_hWnd);

		if (Engine::IsPointInClient(g_hWnd, ptCurrentCursor)) {
			SetCapture(g_hWnd);
			Engine::CKeyMgr::GetInstance()->SetOldCursorPoint(ptCurrentCursor);
		}
	}

	if (GetCapture() == g_hWnd && Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_LBUTTON")) {
		POINT ptCurrentCursor = Engine::GetClientCursorPoint(g_hWnd);
		POINT ptOldCursor = Engine::CKeyMgr::GetInstance()->GetOldCursorPoint();
		_vec2 vToCurrent = _vec2(
			static_cast<FLOAT>(ptCurrentCursor.x - ptOldCursor.x), 
			static_cast<FLOAT>(ptCurrentCursor.y - ptOldCursor.y));
		_float fRotAngleByY = vToCurrent.x * D3DX_PI / WINCX;
		_float fRotAngleByRight = vToCurrent.y * D3DX_PI / WINCY;

		m_pMoveComponent->RotateByRight(fRotAngleByRight);
		m_pMoveComponent->RotateByAxis(fRotAngleByY, WORLD_Y_AXIS);

		// 화면 내에 순환하는 클라이언트 영역 좌표 얻기
		ptCurrentCursor = Engine::GetToroidClientPoint(g_hWnd, ptCurrentCursor);
		Engine::CKeyMgr::GetInstance()->SetOldCursorPoint(ptCurrentCursor);

		// 순환된 클라이언트 좌표를 스크린 좌표로 세팅하여 커서 좌표로 둔다.
		ClientToScreen(g_hWnd, &ptCurrentCursor);
		SetCursorPos(ptCurrentCursor.x, ptCurrentCursor.y);
	}

	if (GetCapture() == g_hWnd && Engine::CKeyMgr::GetInstance()->IsKeyUp(L"KEY_LBUTTON")) {
		ReleaseCapture();
	}

	_vec3 vDir{ 0.f, 0.f, 0.f };

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_W")) {
		vDir += m_pMoveComponent->GetLook();
	}

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_S")) {
		vDir -= m_pMoveComponent->GetLook();
	}

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_A")) {
		vDir -= m_pMoveComponent->GetRight();
	}

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_D")) {
		vDir += m_pMoveComponent->GetRight();
	}

	m_pMoveComponent->SetToXYZ(vDir);
	m_pMoveComponent->MoveByDelta(fTimeDelta);

	// 뷰 스페이스 변환 행렬 생성 함수(즉, 카메라 월드 행렬의 역 행렬을 만들어주는 함수)
	_matrix matView;
	_vec3 vEye = m_pMoveComponent->GetPos();		// 카메라 위치
	_vec3 vAt = vEye + m_pMoveComponent->GetLook() ;	// 카메라 위치에서 바로 아래를 본다.
	_vec3 vUp = m_pMoveComponent->GetUp();						// 카메라 Up축

	D3DXMatrixLookAtLH(&matView, // 행렬 결과
		&vEye, // eye(카메라 위치)
		&vAt,	// at(카메라가 바라보는 위치)
		&vUp); // up(카메라와 수직을 이루는 방향)

	// 원근 투영 행렬 생성 함수
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	return 0;
}

void CCamera::SetProjectionMatrix(const _float & _fFOV, const _float & _fAspect, const _float & _fNearZ, const _float & _fFarZ)
{
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
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CCamera::Free(void)
{
	CGameObject::Free();
}
