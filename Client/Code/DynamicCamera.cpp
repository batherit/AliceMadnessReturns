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
	AddComponent<Engine::CTransform>();
	m_pTransform = GetComponent<Engine::CTransform>();
	m_pTransform->SetPos(30.f, 80.f, -80.f);
	m_pTransform->RotateByRight(D3DXToRadian(45.f));

	m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetSpeed(100.f, 110.f);

	SetProjectionMatrix(D3DXToRadian(45.f), (_float)WINCX / WINCY, 0.1f, 1000.f);

	return S_OK;
}

int CDynamicCamera::Update_Object(const _float & fTimeDelta)
{	
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
		POINT ptCurrentCursor = Engine::CDirectInputMgr::GetInstance()->GetCurrentMousePos();

		if (Engine::IsPointInClient(g_hWnd, ptCurrentCursor)) {
			SetCapture(g_hWnd);
		}
	}

	if (GetCapture() == g_hWnd && Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(Engine::DIM_LB)) {
		// 화면 내에 순환하는 클라이언트 영역 좌표 얻기
		POINT ptToCurrent = Engine::CDirectInputMgr::GetInstance()->GetDeltaMousePos();
		_float fRotAngleByY = ptToCurrent.x * 2.f * D3DX_PI / WINCX;
		_float fRotAngleByRight = ptToCurrent.y * 2.f * D3DX_PI / WINCY;

		m_pTransform->RotateByRight(fRotAngleByRight);
		m_pTransform->RotateByAxis(fRotAngleByY, WORLD_Y_AXIS);

		//POINT ptCurrentCursor = Engine::CDirectInputMgr::GetInstance()->GetCurrentMousePos();
		//ptCurrentCursor = Engine::GetToroidClientPoint(g_hWnd, ptCurrentCursor);

		////// 순환된 클라이언트 좌표를 스크린 좌표로 세팅하여 커서 좌표로 둔다.
		//ClientToScreen(g_hWnd, &ptCurrentCursor);
		//SetCursorPos(ptCurrentCursor.x, ptCurrentCursor.y);
	}

	if (GetCapture() == g_hWnd && Engine::CDirectInputMgr::GetInstance()->IsKeyUp(Engine::DIM_LB)) {
		ReleaseCapture();
	}

	_vec3 vDir{ 0.f, 0.f, 0.f };

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(L"KEY_W")) {
		vDir += m_pTransform->GetLook();
	}

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(L"KEY_S")) {
		vDir -= m_pTransform->GetLook();
	}

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(L"KEY_A")) {
		vDir -= m_pTransform->GetRight();
	}

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(L"KEY_D")) {
		vDir += m_pTransform->GetRight();
	}

	m_pPhysics->SetDirection(vDir);
	m_pPhysics->MoveByDelta(fTimeDelta);

	// 뷰 스페이스 변환 행렬 생성 함수(즉, 카메라 월드 행렬의 역 행렬을 만들어주는 함수)
	m_vEye = m_pTransform->GetPos();		// 카메라 위치
	m_vAt = m_vEye + m_pTransform->GetLook() * 10.f ;	// 카메라 위치에서 바로 아래를 본다.
	m_vUp = m_pTransform->GetUp();						// 카메라 Up축

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
