#include "pch.h"
#include "AliceW.h"
#include "Map.h"
#include "StateMgr.h"
#include "AliceWState_Idle.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "Trigger.h"
#include "Attribute.h"
#include "BunnyBomb.h"
#include "Platform.h"
#include "MovingPlatform.h"
#include "UI_InGame.h"
#include "UI_WeaponLock.h"
#include "UI_LockedWeapon.h"
#include "VorpalBlade.h"
#include "HobbyHorse.h"
#include "UI_BloodScreen.h"

CAliceW::CAliceW(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CAliceW::CAliceW(const CAliceW & rhs)
	:
	CGameObject(rhs)
{
}

CAliceW::~CAliceW(void)
{
}

HRESULT CAliceW::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"AliceW"));
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"AliceW.col");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	// Attribute
	m_pAttribute = AddComponent<CAttribute>();
	m_pAttribute->SetHP(ALICE_MAX_HP, ALICE_MAX_HP);

	// Physics
	m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetSpeed(ALICE_RUN_SPEED);
	m_pPhysics->SetGravity(9.8f * 3.f);

	// StateMgr
	m_pStateMgr = new CStateMgr<CAliceW>(*this);
	m_pStateMgr->SetNextState(new CAliceWState_Idle(*this));

	// Weapon
	// 1) Vorpal Blade
	CStaticObject* pStaticObject = CVorpalBlade::Create(m_pGraphicDev);
	pStaticObject->GetTransform()->Rotate(D3DXToRadian(45.f), D3DXToRadian(90.f), D3DXToRadian(160.f));
	pStaticObject->GetTransform()->Translate(0.07f, 0.f, 0.02f);
	AddChild(pStaticObject, "Bip01-R-Hand");
	pStaticObject->SetActivated(false);
	m_pWeapons[TYPE_BLADE] = pStaticObject;
	m_pAttackColliders[TYPE_BLADE] = pStaticObject->GetColliderFromTag(L"PlayerAttack");
	m_pAttackColliders[TYPE_BLADE]->SetDamage(VORPALBLADE_DAMAGE);

	// 2) Hobby Horse
	pStaticObject = CHobbyHorse::Create(m_pGraphicDev);
	pStaticObject->GetTransform()->Translate(0.05f, 0.f, 0.f);
	AddChild(pStaticObject, "Bip01-R-Hand");
	pStaticObject->SetActivated(false);
	m_pWeapons[TYPE_HORSE] = pStaticObject;
	m_pAttackColliders[TYPE_HORSE] = pStaticObject->GetColliderFromTag(L"PlayerAttack");
	m_pAttackColliders[TYPE_HORSE]->SetDamage(HOBBYHORSE_DAMAGE);

	// 3) Gun
	CDynamicObject* pDynamicObject = CDynamicObject::Create(m_pGraphicDev);
	pDynamicObject->SetRenderInfo(L"Gun");
	pDynamicObject->GetTransform()->Rotate(D3DXToRadian(0.f), D3DXToRadian(170.f), D3DXToRadian(90.f));
	pDynamicObject->GetTransform()->Translate(0.06f, 0.25f, 0.05f);
	AddChild(pDynamicObject, "Bip01-L-Hand");
	pDynamicObject->SetActivated(false);
	m_pWeapons[TYPE_GUN] = pDynamicObject;
	m_pAttackColliders[TYPE_GUN] = nullptr;

	SetWeaponType(TYPE_BLADE);
	
		
	return S_OK;
}

int CAliceW::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (!m_pMap) {
		m_pMap = dynamic_cast<CMap*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Map").begin());
	}
	if (!m_pStateMgr->ConfirmValidState())
		return 1;

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_I)) {
		m_pAttribute->Damaged(1000.1f);
	}

	// 부모 먼저 렌더러에 들어가야 올바르게 자식도 transform 됨.
	m_pRenderer->Update(_fDeltaTime);	

	CGameObject::Update_Object(_fDeltaTime);

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_CAPSLOCK)) {
		ToggleLockOn();
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_TAB)) {
		ChangeLockOnTarget();
	}

	if (m_pTargetObject && m_pTargetObject->GetComponent<CAttribute>()->IsDead()) {
		if (!ChangeLockOnTarget())
			ReleaseLockOn();
	}

	if (!m_bIsSmalled && IsSmalling()) {
		_vec3 vScale = GetTransform()->GetScale();
		
		if (vScale.x <= 0.3f) {
			vScale = _vec3(0.3f, 0.3f, 0.3f);
			m_bIsSmalled = true;
		}
		else
			vScale *= 0.7f;
		GetTransform()->SetScale(vScale);
	}
	else if (m_bIsSmalled && !IsSmalling()) {
		_vec3 vScale = GetTransform()->GetScale();

		if (vScale.x >= 1.f) {
			vScale = _vec3(1.f, 1.f, 1.f);
			m_bIsSmalled = false;
		}
		else 
			vScale *= 1.3f;
		GetTransform()->SetScale(vScale);
	}

	m_pStateMgr->Update(_fDeltaTime);
	m_pMesh->Play_Animation(_fDeltaTime);

	Engine::CNaviMesh* pNaviMesh = m_pMap->GetNaviMesh();
	_vec3 vCurrentPos = GetTransform()->GetPos();

	//pNaviMesh->CorrectPosAndVelBySliding(vCurrentPos, *GetPhysics());
	//_vec3 vSlidingXZ = pNaviMesh->GetSlidedVelocity(vCurrentPos);
	//vSlidingXZ.y = 0.f;

	//if (D3DXVec3LengthSq(&vSlidingXZ) > 0.f ) {
	//	D3DXVec3Normalize(&vSlidingXZ, &vSlidingXZ);
	//	_vec3 vVelocityXZ = GetPhysics()->GetVelocity();
	//	vVelocityXZ.y = 0.f;

	//	if (D3DXVec3LengthSq(&vVelocityXZ) > 0.f) {
	//		_float fDot = D3DXVec3Dot(&vSlidingXZ, &vVelocityXZ);
	//		if (fDot < 0.f) {
	//			vVelocityXZ -= fDot * vSlidingXZ;
	//			GetPhysics()->SetVelocityXZ(_vec2(vVelocityXZ.x, vVelocityXZ.z));
	//		}
	//	}
	//	/*else {
	//		m_v
	//	}*/
 //	}

	_vec3 vTargetPos = m_pPhysics->GetUpdatedPos(_fDeltaTime);		// 물리 계산
	vTargetPos = pNaviMesh->GetSlidedPos(vTargetPos);
	
	_vec3 vSettedPos = vTargetPos;
	
	if (IsFalling(_fDeltaTime)) {
		// 추락 중이라면, 적절한 셀을 찾는다.
		_int iCellIndex = pNaviMesh->GetNaviIndexByPos(vCurrentPos, vTargetPos + _vec3(0.f, -1.0f, 0.f));
		if (-1 != iCellIndex && pNaviMesh->GetCell(iCellIndex)->GetTagIndex() != Engine::CCell::TYPE_SLIDE_EXIT) {
			m_bIsLanded = true;
			m_iCellIndex = iCellIndex;
			//pNaviMesh->Set_NaviIndex(iCellIndex);
			GetPhysics()->SetVelocityY(0.f);
		}
	}
	else if(IsLanded()){	
		if (!IsSliding(_fDeltaTime))
			GetPhysics()->SetVelocityY(0.f);

		vSettedPos = m_pMap->GetNaviMesh()->Move_OnNaviMesh(m_iCellIndex, &vCurrentPos, &(vTargetPos + _vec3(0.f, -1.0f, 0.f)), GetPhysics());

		if (vSettedPos == vTargetPos + _vec3(0.f, -1.0f, 0.f)) {
			vSettedPos += _vec3(0.f, 1.f, 0.f);
			m_bIsLanded = false;
		}
	}
	// 이동 확정
	GetTransform()->SetPos(vSettedPos);
	m_bIsSmalling = false;

	return 0;
}

void CAliceW::Render_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	m_pMesh->UpdateAnimation();
	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	if (!m_bIsVisible)
		return;

	m_pRenderer->Render(m_pShader->Get_EffectHandle());
	//Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"M_Buffer_TriCol");
	//m_pCollider->Render_MeshCollider(Engine::COL_TRUE, &m_pTransform->GetObjectMatrix());
}

_bool CAliceW::LoadColliders(const _tchar* _pFileName)
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

void CAliceW::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (IsDead() || IsDashing())
		return;

	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Trigger") == 0) {
		switch (dynamic_cast<CTrigger*>(_tCollisionInfo.pCollidedObject)->GetTriggerType()) {
		case TRIGGER::TYPE_DEATH:
			GetAttribute()->SetHP(0.f);
			return;
		}
	}
	else if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"EnemyAttack") == 0
		|| lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"EnemyAttack_L") == 0
		|| lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"EnemyAttack_R") == 0) {
		if (m_pAttribute->RegisterAttacker(_tCollisionInfo.pCollidedCollider)) {
			// 어태커에 등록이 성공했다는 것은 기존 어태커가 등록되지 않았음을 의미하므로 데미지가 들어간다
			m_pAttribute->Damaged(/*_tCollisionInfo.pCollidedCollider->GetDamage()*/0);
			_vec3 vToOwner = GetTransform()->GetPos() - _tCollisionInfo.pCollidedCollider->GetTransform()->GetPos();
			vToOwner.y = 0.f;
			D3DXVec3Normalize(&vToOwner, &vToOwner);
			GetPhysics()->SetVelocityXZ(_vec2(vToOwner.x, vToOwner.z) * ALICE_RUN_SPEED * 2.f);
			GetPhysics()->SetResistanceCoefficientXZ(0.8f);
			m_pInGameUI->GetBloodScreen()->StartBloodScreen();
		}
	}
	else if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Press") == 0) {
		m_pAttribute->Damaged(1000.f);
	}
	else if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Platform") == 0) {
	/*	if (GetPhysics()->GetVelocity().y > 0.f)
			return;
*/
		if (IsFalling(0.f)) {
			m_bIsLanded = true;
		}

		CPlatform* pPlatform = dynamic_cast<CPlatform*>(_tCollisionInfo.pCollidedObject);
		_vec3 vDeltaPos = pPlatform->GetDeltaPos();
		GetPhysics()->SetVelocityY(0.f);
		GetTransform()->SetPosY(pPlatform->GetHeight());
		GetTransform()->Translate(vDeltaPos.x, 0.f, vDeltaPos.z);
	}
	else if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"MovingPlatform") == 0) {
		/*	if (GetPhysics()->GetVelocity().y > 0.f)
				return;
	*/
		if (IsFalling(0.f)) {
			m_bIsLanded = true;
		}

		CMovingPlatform* pMovingPlatform = dynamic_cast<CMovingPlatform*>(_tCollisionInfo.pCollidedObject);
		_vec3 vDeltaPos = pMovingPlatform->GetDeltaPos();
		GetPhysics()->SetVelocityY(0.f);
		GetTransform()->SetPosY(pMovingPlatform->GetHeight());
		GetTransform()->Translate(vDeltaPos.x, 0.f, vDeltaPos.z);
	}
	else if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Small") == 0) {
		m_bIsSmalling = true;
	}

	//if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_AABB) {
	//	
	//}
	//else if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_OBB) {
	//	_int a = 10;
	//}
	//else {
	//	
	//}
}

void CAliceW::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (IsDead())
		return;

	if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_AABB) {
		_int a = 10;
	}
	else if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_OBB) {
		_int a = 10;
	}
	else {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"EnemyAttack") == 0
			|| lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"EnemyAttack_L") == 0
			|| lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"EnemyAttack_R") == 0) {
			// 충돌하지 않았다면 어태커에서 제거한다.
			m_pAttribute->ReleaseAttacker(_tCollisionInfo.pCollidedCollider);
		}
	}
}

//void CAliceW::AttachItemToBone(Engine::CGameObject * _pItemObject, const _tchar * _pBoneName)
//{
//}
//
//void CAliceW::DetachFromBone()
//{
//}

CAliceW * CAliceW::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAliceW*	pInstance = new CAliceW(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CAliceW::Free(void)
{
	Engine::Safe_Release(m_pStateMgr);
	CGameObject::Free();
}

void CAliceW::SetWeaponType(E_WEAPON_TYPE _eWeaponType)
{
	if (_eWeaponType >= TYPE_END)
		return;

	if (m_eWeaponType < TYPE_END)
		m_pWeapons[m_eWeaponType]->SetActivated(false);
	m_eWeaponType = _eWeaponType;
	m_pWeapons[_eWeaponType]->SetActivated(true);
	if (_eWeaponType < TYPE_GUN)
		m_pWeapons[_eWeaponType]->GetColliderFromTag(L"PlayerAttack")->SetActivated(false);
	/*else if (_eWeaponType == TYPE_GUN)
		dynamic_cast<CDynamicObject*>(m_pWeapons[TYPE_GUN])->GetDynamicMesh()->Set_AnimationSet(ANIM::WP3_Idle_Pose);*/
}

_bool CAliceW::IsMoving(const _float & _fDeltaTime, _vec3 * _pDir)
{
	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
		return false;

	_matrix matView;
	GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	_vec3 vCamRight = _vec3(matView._11, matView._12, matView._13);
	vCamRight.y = 0.f;
	D3DXVec3Normalize(&vCamRight, &vCamRight);
	_vec3 vCamLook = _vec3(matView._31, matView._32, matView._33);
	vCamLook.y = 0.f;
	D3DXVec3Normalize(&vCamLook, &vCamLook);
	_vec3 vDir = _vec3(0.f, 0.f, 0.f);

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_A)) {
		vDir -= vCamRight;
	}
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_D)) {
		vDir += vCamRight;
	}
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_W)) {
		vDir += vCamLook;
	}
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_S)) {
		vDir -= vCamLook;
	}

	if (D3DXVec3LengthSq(&vDir) != 0.f) {
		if (_pDir) {
			*_pDir = *D3DXVec3Normalize(&vDir, &vDir);
		}
		return true;
	}

	return false;
}

_bool CAliceW::IsSmalling()
{
	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
		return false;

	return Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_LCONTROL) || Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_LCONTROL) || m_bIsSmalling;
}

_bool CAliceW::IsAttackOn(const _float & _fDeltaTime)
{
	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
		return false;

	return Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB);
}

_bool CAliceW::IsAttacking(const _float & _fDeltaTime)
{
	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
		return false;

	return Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(Engine::DIM_LB);
}

_bool CAliceW::IsGunModeReleased()
{
	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
		return false;

	return Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_RB);
}

_bool CAliceW::IsJumpOn(const _float & _fDeltaTime)
{
	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
		return false;

	return !IsSmalling() && Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_SPACE);
}

_bool CAliceW::IsDashOn()
{
	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
		return false;

	return !IsSmalling() && Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_LSHIFT);
}

_bool CAliceW::IsDashing()
{
	return m_bIsDashing;
}

_bool CAliceW::IsSliding(const _float & _fDeltaTime)
{
	return IsLanded() && GetMap()->GetNaviMesh()->GetCell(m_iCellIndex)->GetTagIndex() == Engine::CCell::TYPE_SLIDE;
}

_bool CAliceW::IsFloatingOn(const _float & _fDeltaTime)
{
	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
		return false;

	return Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_SPACE);
}

_bool CAliceW::IsFalling(const _float & _fDeltaTime)
{
	return GetPhysics()->GetVelocity().y <= 0.f && !m_bIsLanded;
}

_bool CAliceW::IsRunOn(const _float& _fDeltaTime, _vec3 * _pDir)
{
	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
		return false;

	_vec3 vDir;
	if (!IsMoving(_fDeltaTime, &vDir))
		return false;

	// 플레이어 방향 전환이 이루어졌다면 회전시키기
	_vec3 vRotAxis = Engine::GetRotationAxis(GetTransform()->GetLook(), vDir);
	_float vRotAngle = Engine::GetRotationAngle(GetTransform()->GetLook(), vDir) * 0.25f;
	GetTransform()->RotateByAxis(vRotAngle, vRotAxis);

	if (_pDir)
		*_pDir = vDir;

	return true;
	
}

_bool CAliceW::IsDead() const
{
	return m_pAttribute->IsDead();
}

_bool CAliceW::IsWeaponChanging()
{
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_1)) {
		SetWeaponType(TYPE_BLADE);
		return true;
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_2)) {
		if (m_pInGameUI && !m_pInGameUI->GetWeaponLock()->GetHobbyHorseUI()->IsLocked())
			SetWeaponType(TYPE_HORSE);
		return true;
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_3)) {
		if (m_pInGameUI && !m_pInGameUI->GetWeaponLock()->GetGunUI()->IsLocked())
			SetWeaponType(TYPE_GUN);
		return true;
	}

	return false;
}

_bool CAliceW::IsBombOn()
{
	if (!m_pInGameUI || m_pInGameUI->GetWeaponLock()->GetBunnyBombUI()->IsLocked())
		return false;

	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
		return false;

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_Q)) {
		if (Engine::GetLayer(L"Environment")->GetLayerList(L"BunnyBomb").empty())
			return true;
		CBunnyBomb* pBunnyBomb = dynamic_cast<CBunnyBomb*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"BunnyBomb").begin());
		if (pBunnyBomb) {
			pBunnyBomb->Bomb();
			return false;
		}
		else 
			return true;
	}
	return false;
}

void CAliceW::ToggleLockOn()
{
	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
		return;

	if (m_bIsLockOn) {
		ReleaseLockOn();
	}
	else {
		auto& rMonsterList = Engine::GetLayer(L"Environment")->GetLayerList(L"Monster");
		
		if (rMonsterList.empty()) 
			return;					// 몬스터가 존재하지 않으므로 락온을 활성화하지 않는다.

		m_pTargetObject = nullptr;
		for (auto iter = rMonsterList.begin(); iter != rMonsterList.end(); ++iter) {
			if ((*iter)->IsValid()) {
				CAttribute* pAttribute = (*iter)->GetComponent<CAttribute>();
				if (pAttribute && !pAttribute->IsDead()) {
					m_pTargetObject = (*iter);
					break;
				}
			}
		}

		if (m_pTargetObject) {
			m_bIsLockOn = true;	// 타겟을 찾았다면 락온을 활성화한다.
			Engine::CCameraMgr* pCameraMgr = dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin());
			pCameraMgr->ChangeCameraController(2, 0.2f);
		}
			
	}
}

void CAliceW::ReleaseLockOn()
{
	m_bIsLockOn = false;
	m_pTargetObject = nullptr;
	Engine::CCameraMgr* pCameraMgr = dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin());
	pCameraMgr->ChangeCameraController(0, 0.5f);
}

_bool CAliceW::ChangeLockOnTarget()
{
	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
		return false;

	if (!m_bIsLockOn || !m_pTargetObject) {
		ReleaseLockOn();
		return false;
	}

	auto& rMonsterList = Engine::GetLayer(L"Environment")->GetLayerList(L"Monster");
	auto iter = find(rMonsterList.begin(), rMonsterList.end(), m_pTargetObject);

	if (iter == rMonsterList.end()) {
		// 타겟 객체를 찾지 못했다면, 타겟 객체는 죽어서 해제되었던가 원래 없었던 경우.
		ReleaseLockOn();
		return false;
	}

	do {
		++iter;
		if (iter == rMonsterList.end()) {
			iter = rMonsterList.begin();
		}

		if ((*iter)->IsValid()) {
			CAttribute* pAttribute = (*iter)->GetComponent<CAttribute>();
			if (pAttribute && !pAttribute->IsDead()) {
				break;
			}
		}
	} while ((*iter) != m_pTargetObject);

	if ((*iter) != m_pTargetObject) {
		m_pTargetObject = (*iter);
		return true;
	}
		
	return false;
}

//_bool CAliceW::ProcessMoveXZ(const _float& _fDeltaTime)
//{
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
//	_float fLengthSq = D3DXVec3LengthSq(&vDir);
//	if (fLengthSq > 0.f) {
//		// 플레이어 방향 전환이 이루어졌다면 회전시키기
//		_vec3 vRotAxis = Engine::GetRotationAxis(GetTransform()->GetLook(), vDir);
//		_float vRotAngle = Engine::GetRotationAngle(GetTransform()->GetLook(), vDir) * 0.25f;
//		GetTransform()->RotateByAxis(vRotAngle, vRotAxis);
//		return true;
//	}
//
//	return false;
//}


// 쿼터니언 회전을 통해 카메라 축과 일치시키기
/*_matrix matView, matViewRot, matRotSlerp;
m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
D3DXMatrixInverse(&matView, NULL, &matView);

Engine::ExtractRotationMatrix(&matViewRot, &matView);

_matrix matCurRot;
Engine::ExtractRotationMatrix(&matCurRot, &m_pTransform->GetObjectMatrix());
Engine::GetRotationMatrixSlerp(&matRotSlerp, &matCurRot, &matViewRot, 0.1f);
m_pTransform->ResetRightUpLook(
	&(_vec3(matRotSlerp._31, matRotSlerp._32, matRotSlerp._33) + m_pTransform->GetPos()),
	&WORLD_Y_AXIS
);

_vec3 vDir = m_vTargetPos - m_pTransform->GetPos();
if (D3DXVec3LengthSq(&vDir) > 0.5f) {
	D3DXVec3Normalize(&vDir, &vDir);
	m_pPhysics->SetDirection(vDir);
	m_pPhysics->MoveByDelta(_fDeltaTime);
}*/