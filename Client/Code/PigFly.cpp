#include "pch.h"
#include "PigFly.h"
#include "Map.h"
#include "StateMgr.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "Trigger.h"
#include "Route.h"
#include "UI_SpeechBubble.h"
#include "PopDomino.h"
#include "CrushingFist.h"

CPigFly::CPigFly(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CPigFly::CPigFly(const CPigFly & rhs)
	:
	CGameObject(rhs)
{
}

CPigFly::~CPigFly(void)
{
}

HRESULT CPigFly::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"PigFly"));
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), m_pMesh);
	m_pMesh->Set_AnimationSet(ANIM::NoseSwich_Fly_Relax);

	// Load Colliders
	LoadColliders(L"PigFly.col");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	// Physics
	m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetSpeed(ALICE_RUN_SPEED);

	// Route
	m_pRoute = AddComponent<CRoute>();

	// SpeechBubble
	m_pSpeechBubble = CUI_SpeechBubble::Create(m_pGraphicDev);
	AddChild(m_pSpeechBubble);

	return S_OK;
}

int CPigFly::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	switch (m_eStep) {
	case STEP_INIT:
		GetTransform()->SetPos(m_pRoute->GetStartPosition());
		m_eStep = STEP_RELAX;
		break;
	case STEP_ACTION:
		if (m_pMesh->GetAnimationProgress() >= 0.99f) {
			// 돼지코 카메라 컨트롤러로 세팅.
			m_pMesh->Set_AnimationSet(ANIM::NoseSwich_Fly_Idle01);
			switch (m_eEventType) {
			case TYPE_DOMINO: {
				ShoutOut(WINCX / 6.f, WINCY - WINCY / 6.f, L" 당신은 행운의 돼지코를 만났습니다. \n생겨나라 미노미노를 외치면 도미노가 보이게 됩니다.");
				break;
			}
			case TYPE_FIST: {
				ShoutOut(WINCX / 6.f, WINCY - WINCY / 6.f, L" 당신은 행운의 돼지코를 만났습니다. \n멈춰라 주먹주먹을 외치면 쇠주먹이 멈추게 됩니다.");
				break;
			}
			}
			m_eStep = STEP_TALKING;
		}
		break;
	case STEP_TALKING:
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_F)) {
			switch (m_eEventType) {
			case TYPE_DOMINO:
				ShoutOut(WINCX / 6.f, WINCY - WINCY / 6.f, L" 생겨나라 미노미노!!");
				break;
			case TYPE_FIST:
				ShoutOut(WINCX / 6.f, WINCY - WINCY / 6.f, L" 멈춰라 주먹주먹!!");
				break;
			}
			m_eStep = STEP_PROCEE_EVENT;
			break;
		}
		break;
	case STEP_PROCEE_EVENT:
		if (m_pRoute->IsNextPositionExisted()) {
			_vec3 vPos = GetTransform()->GetPos();
			_vec3 vPostPos = m_pRoute->GetNextPosition(_fDeltaTime, ALICE_RUN_SPEED);
			if (vPos != vPostPos) {
				_vec3 vNewLook;
				D3DXVec3Normalize(&vNewLook, &(vPostPos - vPos));
				GetTransform()->ResetRightUpLook(&(vPos + vNewLook), &WORLD_Y_AXIS);
				
			}
			GetTransform()->SetPos(vPostPos);
		}
		else {
			// 이벤트 처리
			ProcessEvent();
			SetValid(false);
			return 1;
		}
		break;
	}

	// 부모 먼저 렌더러에 들어가야 올바르게 자식도 transform 됨.
	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);
	m_pMesh->Play_Animation(_fDeltaTime);

	return 0;
}

void CPigFly::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	m_pMesh->UpdateAnimation();
	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	if (!m_bIsVisible)
		return;

	m_pRenderer->Render(m_pShader->Get_EffectHandle());
}

_bool CPigFly::LoadColliders(const _tchar* _pFileName)
{
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Colliders\\");

	lstrcat(szDataPath, _pFileName);

	HANDLE hFile = CreateFileW(szDataPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	LoadCollidersInfo(hFile);

	CloseHandle(hFile);

	return true;
}

void CPigFly::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (!m_bIsOn) {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"PlayerAttack") == 0) {
			m_eStep = STEP_ATTACKED;
			m_iTriggerGauge += 1;
			switch (m_iTriggerGauge / 3) {
			case 0: //0~2
				m_pMesh->Set_AnimationSet(ANIM::NoseSwich_Fly_Charge_1);
				break;
			case 1:	//3~5
				m_pMesh->Set_AnimationSet(ANIM::NoseSwich_Fly_Charge_2);
				break;
			case 2: // 6~8
				m_pMesh->Set_AnimationSet(ANIM::NoseSwich_Fly_Charge_3);
				break;
			case 3: // 9~11
				m_pMesh->Set_AnimationSet(ANIM::NoseSwich_Fly_Charge_4);
				break;
			case 4: // 12~15
				m_pMesh->Set_AnimationSet(ANIM::NoseSwich_Fly_Charge_5);
				break;
			default:
				m_bIsOn = true;
				m_pMesh->Set_AnimationSet(ANIM::NoseSwich_Fly_Action);
				m_eStep = STEP_ACTION;
				break;
			}
		}
	}
}

void CPigFly::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
}

CPigFly * CPigFly::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPigFly*	pInstance = new CPigFly(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CPigFly::Free(void)
{
	CGameObject::Free();
}

_bool CPigFly::LoadRoutePosList(const _tchar * _pRouteFilePath)
{
	FILE* fpIn;
	errno_t err = 0;

	TCHAR tmp[256] = L"";
	lstrcpy(tmp, _pRouteFilePath);

	_int len = 256;
	char ctemp[256];

	WideCharToMultiByte(CP_ACP, 0, tmp, len, ctemp, len, NULL, NULL);

	err = fopen_s(&fpIn, ctemp, "rt");
	if (!err) {
		vector<_vec3> vecPosList;
		vecPosList.reserve(300);

		_int iSize = 0;
		fscanf_s(fpIn, "%d", &iSize);

		_float fX, fY, fZ;
		for (_int i = 0; i < iSize; ++i) {
			fscanf_s(fpIn, "%f %f %f", &fX, &fY, &fZ);
			vecPosList.emplace_back(_vec3(fX, fY, fZ));
		}
		m_pRoute->SetPosList(vecPosList);

		GetTransform()->SetPos(m_pRoute->GetStartPosition());
	}
	else abort();

	if (fpIn)
		fclose(fpIn);

	return true;
}

void CPigFly::ShoutOut(_float _fOffsetX, _float _fOffsetY, const wstring & _wstrText)
{
	m_pSpeechBubble->SetText(_fOffsetX, _fOffsetY, _wstrText);
}

_bool CPigFly::ProcessEvent()
{
	switch (m_eEventType) {
	case TYPE_DOMINO: {
		// TODO : 도미노를 찾아 보이게 한다. 
		auto& rMapObjects = Engine::GetLayer(L"Environment")->GetLayerList(L"MapObjects");
		CPopDomino* pPopDomino = nullptr;
		for (auto& rObj : rMapObjects) {
			pPopDomino = dynamic_cast<CPopDomino*>(rObj);
			if (pPopDomino) {
				pPopDomino->EventOn();
			}
		}
	}
		break;
	case TYPE_FIST: {
		// TODO : 쇠주먹을 찾아 제자리에 멈추도록 한다.
		auto& rMapObjects = Engine::GetLayer(L"Environment")->GetLayerList(L"MapObjects");
		CCrushingFist* pFist = nullptr;
		for (auto& rObj : rMapObjects) {
			pFist = dynamic_cast<CCrushingFist*>(rObj);
			if (pFist) {
				pFist->EventOn();
			}
		}
	}
		
		break;
	}
	return true;
}

//_bool CPigFly::IsMoving(const _float & _fDeltaTime, _vec3 * _pDir)
//{
//	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
//		return false;
//
//	_matrix matView;
//	GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);
//	D3DXMatrixInverse(&matView, nullptr, &matView);
//
//	_vec3 vCamRight = _vec3(matView._11, matView._12, matView._13);
//	vCamRight.y = 0.f;
//	D3DXVec3Normalize(&vCamRight, &vCamRight);
//	_vec3 vCamLook = _vec3(matView._31, matView._32, matView._33);
//	vCamLook.y = 0.f;
//	D3DXVec3Normalize(&vCamLook, &vCamLook);
//	_vec3 vDir = _vec3(0.f, 0.f, 0.f);
//
//	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_A)) {
//		vDir -= vCamRight;
//	}
//	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_D)) {
//		vDir += vCamRight;
//	}
//	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_W)) {
//		vDir += vCamLook;
//	}
//	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_S)) {
//		vDir -= vCamLook;
//	}
//
//	if (D3DXVec3LengthSq(&vDir) != 0.f) {
//		if (_pDir) {
//			*_pDir = *D3DXVec3Normalize(&vDir, &vDir);
//		}
//		return true;
//	}
//
//	return false;
//}