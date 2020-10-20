#include "pch.h"
#include "DynamicCamera.h"


CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CCamera(pGraphicDev)
{
}

CDynamicCamera::CDynamicCamera(const CDynamicCamera & rhs)
	:
	CCamera(rhs)
{
}

CDynamicCamera::~CDynamicCamera(void)
{
}


HRESULT CDynamicCamera::Ready_Object(void)
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

int CDynamicCamera::Update_Object(const _float & fTimeDelta)
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
		_float fRotAngleByY = vToCurrent.x * 2.f * D3DX_PI / WINCX;
		_float fRotAngleByRight = vToCurrent.y * 2.f * D3DX_PI / WINCY;

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
	m_vEye = m_pMoveComponent->GetPos();		// 카메라 위치
	m_vAt = m_vEye + m_pMoveComponent->GetLook() * 10.f ;	// 카메라 위치에서 바로 아래를 본다.
	m_vUp = m_pMoveComponent->GetUp();						// 카메라 Up축

	return Engine::CCamera::Update_Object(fTimeDelta);
}

CDynamicCamera * CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamicCamera*	pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CDynamicCamera::Free(void)
{
	CCamera::Free();
}
