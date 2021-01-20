#include "pch.h"
#include "Bullet.h"
#include "Weapon.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CStaticObject(pGraphicDev)
{
}

CWeapon::CWeapon(const CWeapon & rhs)
	:
	CStaticObject(rhs)
{
}

CWeapon::~CWeapon(void)
{
}

HRESULT CWeapon::Ready_Object(void)
{
	CStaticObject::Ready_Object();
	//SetRenderInfo(L"Bullet");
	//m_pPhysics = AddComponent<Engine::CPhysics>();

	//GetTransform()->SetScaleXYZ(0.002f, 0.002f, 0.002f);

	return S_OK;
}

int CWeapon::Update_Object(const _float & _fDeltaTime)
{
	//GetTransform()->SetPos(m_pPhysics->GetUpdatedPos(_fDeltaTime));

	//if ((m_fDeathLength -= _fDeltaTime * m_pPhysics->GetSpeed()) <= 0.f) {
	//	SetValid(false);
	//	return 1;
	//}

	CStaticObject::Update_Object(_fDeltaTime);

	return 0;
}

void CWeapon::Render_Object(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CStaticObject::Render_Object();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CWeapon::OnCollision(Engine::CollisionInfo)
{
	//SetValid(false);
}

CWeapon * CWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWeapon*	pInstance = new CWeapon(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CWeapon::Free(void)
{
	CStaticObject::Free();
}

void CWeapon::SetWeaponInfo(const _tchar * _tcWeaponTag, const _tchar* _tcAttackColliderTag, _float _fDamage)
{
	SetRenderInfo(_tcWeaponTag);
	m_pAttackCollider = GetColliderFromTag(_tcAttackColliderTag);
	SetDamage(_fDamage);
}

//void CWeapon::SetBulletInfo(_vec3 _vPos, _vec3 _vDir, _float _fSpeed, _float _fDeathLength)
//{
//	GetTransform()->SetPos(_vPos);
//	m_pPhysics->SetVelocity(_vDir * _fSpeed);
//	m_fDeathLength = _fDeathLength;
//}
