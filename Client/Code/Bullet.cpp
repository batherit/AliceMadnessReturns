#include "pch.h"
#include "Bullet.h"

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

	CStaticObject::Update_Object(_fDeltaTime);

	return 0;
}

void CBullet::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CStaticObject::Render_Object();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBullet::OnCollision(Engine::CollisionInfo)
{
	SetValid(false);
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
