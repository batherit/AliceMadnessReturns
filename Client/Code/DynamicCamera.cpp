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
	m_pTransform->SetXYZ(30.f, 80.f, -80.f);
	m_pTransform->RotateByRight(D3DXToRadian(45.f));
	m_pTransform->SetMaxSpeed(110.f);
	m_pTransform->SetSpeed(100.f);

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

		m_pTransform->RotateByRight(fRotAngleByRight);
		m_pTransform->RotateByAxis(fRotAngleByY, WORLD_Y_AXIS);

		// ȭ�� ���� ��ȯ�ϴ� Ŭ���̾�Ʈ ���� ��ǥ ���
		ptCurrentCursor = Engine::GetToroidClientPoint(g_hWnd, ptCurrentCursor);
		Engine::CKeyMgr::GetInstance()->SetOldCursorPoint(ptCurrentCursor);

		// ��ȯ�� Ŭ���̾�Ʈ ��ǥ�� ��ũ�� ��ǥ�� �����Ͽ� Ŀ�� ��ǥ�� �д�.
		ClientToScreen(g_hWnd, &ptCurrentCursor);
		SetCursorPos(ptCurrentCursor.x, ptCurrentCursor.y);
	}

	if (GetCapture() == g_hWnd && Engine::CKeyMgr::GetInstance()->IsKeyUp(L"KEY_LBUTTON")) {
		ReleaseCapture();
	}

	_vec3 vDir{ 0.f, 0.f, 0.f };

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_W")) {
		vDir += m_pTransform->GetLook();
	}

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_S")) {
		vDir -= m_pTransform->GetLook();
	}

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_A")) {
		vDir -= m_pTransform->GetRight();
	}

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_D")) {
		vDir += m_pTransform->GetRight();
	}

	m_pTransform->SetToXYZ(vDir);
	m_pTransform->MoveByDelta(fTimeDelta);

	// �� �����̽� ��ȯ ��� ���� �Լ�(��, ī�޶� ���� ����� �� ����� ������ִ� �Լ�)
	m_vEye = m_pTransform->GetPos();		// ī�޶� ��ġ
	m_vAt = m_vEye + m_pTransform->GetLook() * 10.f ;	// ī�޶� ��ġ���� �ٷ� �Ʒ��� ����.
	m_vUp = m_pTransform->GetUp();						// ī�޶� Up��

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
