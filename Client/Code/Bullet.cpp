#include "pch.h"
#include "Bullet.h"
#include "Attribute.h"
#include "EFT_BulletAttack.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CStaticObject(pGraphicDev)
{
}

CBullet::CBullet(const CBullet & rhs)
	:
	CStaticObject(rhs)
{
}

CBullet::~CBullet(void)
{
}

HRESULT CBullet::Ready_Object(void)
{
	CStaticObject::Ready_Object();
	SetRenderInfo(L"Bullet");

	GetColliderFromTag(L"PlayerAttack")->SetDamage(BULLET_DAMAGE);

	m_pPhysics = AddComponent<Engine::CPhysics>();

	GetTransform()->SetScaleXYZ(0.002f, 0.002f, 0.002f);

	return S_OK;
}

int CBullet::Update_Object(const _float & _fDeltaTime)
{
	GetTransform()->SetPos(m_pPhysics->GetUpdatedPos(_fDeltaTime));

	if ((m_fDeathLength -= _fDeltaTime * m_pPhysics->GetSpeed()) <= 0.f) {
		SetValid(false);
		return 1;
	}
	else if (!IsActivated() && (m_fElapsedTime += _fDeltaTime) >= 1.f) {
		SetValid(false);
		return 1;
	}

	CStaticObject::Update_Object(_fDeltaTime);

	return 0;
}

void CBullet::Render_Object(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CStaticObject::Render_Object();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBullet::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Monster") == 0) {
		CAttribute* pAttribute = _tCollisionInfo.pCollidedObject->GetComponent<CAttribute>();
		if (pAttribute) 
			pAttribute->Damaged(BULLET_DAMAGE + Engine::GetNumberBetweenMinMax(0.f, 1.f));

		CEFT_BulletAttack* pEffect = CEFT_BulletAttack::Create(m_pGraphicDev);
		pEffect->SetInfo((_tCollisionInfo.pCollidedCollider->GetTransform()->GetPos() + _tCollisionInfo.pCollidedMyCollider->GetTransform()->GetPos()) * 0.5f);
		Engine::GetLayer(L"Environment")->Add_GameObject(L"Effect", pEffect);

		SetActivated(false);
	}
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBullet*	pInstance = new CBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CBullet::Free(void)
{
	CStaticObject::Free();
}

void CBullet::SetBulletInfo(_vec3 _vPos, _vec3 _vDir, _float _fSpeed, _float _fDeathLength)
{
	GetTransform()->SetPos(_vPos);
	m_pPhysics->SetVelocity(_vDir * _fSpeed);
	m_fDeathLength = _fDeathLength;
}
