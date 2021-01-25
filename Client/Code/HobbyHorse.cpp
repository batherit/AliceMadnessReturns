#include "pch.h"
#include "HobbyHorse.h"
#include "Attribute.h"
#include "EFT_HobbyHorseAttack.h"

CHobbyHorse::CHobbyHorse(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CStaticObject(pGraphicDev)
{
}

CHobbyHorse::CHobbyHorse(const CHobbyHorse & rhs)
	:
	CStaticObject(rhs)
{
}

CHobbyHorse::~CHobbyHorse(void)
{
}

HRESULT CHobbyHorse::Ready_Object(void)
{
	CStaticObject::Ready_Object();
	SetRenderInfo(L"HobbyHorse");

	m_pAttackCollider = GetColliderFromTag(L"PlayerAttack");
	m_pAttackCollider->SetDamage(HOBBYHORSE_DAMAGE);

	m_pAttribute = AddComponent<CAttribute>();

	//GetTransform()->SetScaleXYZ(0.002f, 0.002f, 0.002f);

	return S_OK;
}

int CHobbyHorse::Update_Object(const _float & _fDeltaTime)
{
	CStaticObject::Update_Object(_fDeltaTime);

	return 0;
}

void CHobbyHorse::Render_Object(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CStaticObject::Render_Object();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CHobbyHorse::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Monster") == 0) {
		if (m_pAttribute->RegisterAttacker(_tCollisionInfo.pCollidedCollider)) {
			// 어태커에 등록이 성공했다는 것은 기존 어태커가 등록되지 않았음을 의미하므로 데미지가 들어간다
			_tCollisionInfo.pCollidedObject->GetComponent<CAttribute>()->Damaged(/*HOBBYHORSE_DAMAGE*/0);

			CEFT_HobbyHorseAttack* pEffect = CEFT_HobbyHorseAttack::Create(m_pGraphicDev);
			pEffect->SetInfo((_tCollisionInfo.pCollidedCollider->GetTransform()->GetPos() + _tCollisionInfo.pCollidedMyCollider->GetTransform()->GetPos()) * 0.5f);
			Engine::GetLayer(L"Environment")->Add_GameObject(L"Effect", pEffect);
		}
	}
}

void CHobbyHorse::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Monster") == 0) {
		m_pAttribute->ReleaseAttacker(_tCollisionInfo.pCollidedCollider);
	}
}

CHobbyHorse * CHobbyHorse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHobbyHorse*	pInstance = new CHobbyHorse(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CHobbyHorse::Free(void)
{
	CStaticObject::Free();
}
