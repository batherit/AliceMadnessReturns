#include "pch.h"
#include "MineBombAttack.h"
#include "AliceW.h"
#include "Attribute.h"
#include "PlateEffect.h"
#include "Ship.h"

CMineBombAttack::CMineBombAttack(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CMineBombAttack::CMineBombAttack(const CMineBombAttack & rhs)
	:
	CGameObject(rhs)
{
}

CMineBombAttack::~CMineBombAttack(void)
{
}

HRESULT CMineBombAttack::Ready_Object(void)
{
	return S_OK;
}

int CMineBombAttack::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if ((m_fLifeTime -= _fDeltaTime) <= 0.f) {
		m_pAttackCollider->SetActivated(false);
		return 1;
	}
	else if (m_fLifeTime <= -0.2f) {
		SetValid(false);
	}

	return 0;
}

void CMineBombAttack::Render_Object(void)
{
}

void CMineBombAttack::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0) {
		CShip* pShip = dynamic_cast<CShip*>(_tCollisionInfo.pCollidedObject);
		if (pShip && !pShip->IsAttacked()) {
			_tCollisionInfo.pCollidedObject->GetComponent<CAttribute>()->Damaged(2.f);
			pShip->Attacked();
		}
	}
}

void CMineBombAttack::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
}

CMineBombAttack * CMineBombAttack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMineBombAttack*	pInstance = new CMineBombAttack(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CMineBombAttack::Free(void)
{
	CGameObject::Free();
}

void CMineBombAttack::SetMineBombAttackInfo(const _vec3 & _vPos)
{
	// �浹ü ����
	m_pCullingSphere = Engine::CColliderObject_Sphere::Create(m_pGraphicDev);
	m_pCullingSphere->SetColliderTag(L"CULL");
	AddCollider(m_pCullingSphere);
	m_pCullingSphere->SetRadius(10.f);

	m_pAttackCollider = Engine::CColliderObject_Sphere::Create(m_pGraphicDev);
	m_pAttackCollider->SetColliderTag(L"EnemyAttack");
	AddCollider(m_pAttackCollider);
	// �浹ü ������ ����
	m_pAttackCollider->SetRadius(8.f);

	// ��ġ ����
	GetTransform()->SetPos(_vPos);

	// TODO : ���� ����Ʈ �߰�
	_matrix matRot;
	D3DXMatrixRotationZ(&matRot, D3DX_PI / 4.f);

	CPlateEffect* pEffect = nullptr;
	_vec3 vAxisX = WORLD_X_AXIS;
	for (_int i = 0; i < 10; ++i) {
		D3DXVec3TransformNormal(&vAxisX, &vAxisX, &matRot);
		pEffect = CPlateEffect::Create(m_pGraphicDev);
		pEffect->SetPlateEffectInfo(L"EFT_Smoke", _vPos + vAxisX * 3.f, _vec2(1.9f, 1.9f), _vec2(2.5f, 2.5f), 0.f, 1.f, _vec3(0.8f, 0.8f, 0.8f));
		pEffect->GetPhysics()->SetVelocity(vAxisX * 35.f);
		pEffect->GetPhysics()->SetResistanceCoefficientXZ(0.9f);
		pEffect->GetPhysics()->SetResistanceCoefficientY(0.9f);
		pEffect->GetPhysics()->SetGravity(-Engine::GetNumberBetweenMinMax(3.f, 4.f));
		Engine::GetLayer(L"Environment")->Add_GameObject(L"Effect", pEffect);
	}

	// �浹�Ŵ����� ��ü �߰�
	Engine::CCollisionMgr::GetInstance()->AddGameObject(this);
}