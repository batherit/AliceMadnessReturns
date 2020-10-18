#include "pch.h"
#include "CMonster.h"
#include "CLogoObject.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CGameObject * pTarget)
	:
	CGameObject(pGraphicDev),
	m_pTarget(pTarget)
{
}

CMonster::CMonster(const CMonster & rhs)
	:
	CGameObject(rhs),
	m_pTarget(rhs.m_pTarget)
{
}

CMonster::~CMonster(void)
{
}

HRESULT CMonster::Ready_Object(void)
{
	AddComponent<Engine::CMoveComponent>();

	return S_OK;
}

int CMonster::Update_Object(const _float & _fDeltaTime)
{
	Engine::CMoveComponent* pMoveComponent = GetComponent<Engine::CMoveComponent>();
	Engine::CMoveComponent* pTargetMoveComponent = m_pTarget->GetComponent<Engine::CMoveComponent>();

	_vec3 vToTarget = pTargetMoveComponent->GetPos() - pMoveComponent->GetPos();
	D3DXVec3Normalize(&vToTarget, &vToTarget);

	if (D3DXVec3Dot(&vToTarget, &pMoveComponent->GetUp()) <= 0.99f) {
		// 가고자 하는 방향이 Up축과 일치하지 않는다면,
		_vec3 vRotAxis;
		_float fRotRadian;
		_vec3 vLook = pMoveComponent->GetLook();
		_vec3 vTargetLook = pTargetMoveComponent->GetLook();

		_float fDot = D3DXVec3Dot(&pMoveComponent->GetUp(), &vToTarget);
		Engine::Clamp(&fDot, -1.f, 1.f);
		if (fDot <= 0.99f) {
			// Up 축이 일치하지 않는다면,
			// Up 축을 일치시키기 위해 지연된 회전을 한다.
			fRotRadian = acosf(fDot);
			D3DXVec3Cross(&vRotAxis, &pMoveComponent->GetUp(), &vToTarget);
			D3DXVec3Normalize(&vRotAxis, &vRotAxis);
			if (D3DXVec3Length(&vRotAxis) < 0.000001f) {
				// 완전히 반대 방향이라서 회전축을 생성하지 못하면, 
				// 회전축을 Look축으로 삼는다. 
				vRotAxis = vLook;
			}
			pMoveComponent->RotateByAxis(fRotRadian * _fDeltaTime, vRotAxis);
		}

		_vec3 vTargetRight;
		D3DXVec3Cross(&vTargetRight, &_vec3(0.f, 0.f, -1.f), &pMoveComponent->GetUp());
		D3DXVec3Normalize(&vTargetRight, &vTargetRight);
		if (D3DXVec3Length(&vTargetRight) < 0.000001f) {
			vTargetRight = pMoveComponent->GetRight();
		}
		fDot = D3DXVec3Dot(&vTargetRight, &pMoveComponent->GetRight());
		Engine::Clamp(&fDot, -1.f, 1.f);
		if (fDot <= 0.99f) {
			fRotRadian = acosf(fDot);
			pMoveComponent->RotateByUp(fRotRadian * _fDeltaTime);
		}
	}

	//pMoveComponent->SetToXYZ(vToTarget);
	pMoveComponent->SetToXYZ(pMoveComponent->GetUp());
	pMoveComponent->SetSpeed(2.f);
	pMoveComponent->MoveByDelta(_fDeltaTime);

	return 0;
}

void CMonster::Render_Object(void)
{
	Engine::CMoveComponent* pMoveComponent = nullptr;
	pMoveComponent = GetComponent<Engine::CMoveComponent>();

	if (!pMoveComponent) return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &pMoveComponent->GetWorldMatrix());

	Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"M_Buffer_TriCol");
}

CMonster * CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CGameObject * pTarget)
{
	CMonster*	pInstance = new CMonster(pGraphicDev, pTarget);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CMonster::Free(void)
{
	CGameObject::Free();
}
