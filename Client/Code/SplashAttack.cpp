#include "pch.h"
#include "SplashAttack.h"
#include "AliceW.h"
#include "Attribute.h"
#include "PlateEffect.h"

CSplashAttack::CSplashAttack(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CSplashAttack::CSplashAttack(const CSplashAttack & rhs)
	:
	CGameObject(rhs)
{
}

CSplashAttack::~CSplashAttack(void)
{
}

HRESULT CSplashAttack::Ready_Object(void)
{
	m_pAttribute = AddComponent<CAttribute>();

	return S_OK;
}

int CSplashAttack::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if ((m_fLifeTime -= _fDeltaTime) <= 0.f) {
		SetValid(false);
		return 1;
	}

	return 0;
}

void CSplashAttack::Render_Object(void)
{
}

void CSplashAttack::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), m_tcTargetTag) == 0) {
		if (m_pAttribute->RegisterAttacker(_tCollisionInfo.pCollidedCollider)) {
			CAttribute* pAttribute = _tCollisionInfo.pCollidedObject->GetComponent<CAttribute>();
			if (pAttribute && IsInRange(_tCollisionInfo.pCollidedCollider)) {
				pAttribute->Damaged(m_fDamage);
			}
		}
	}
}

void CSplashAttack::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), m_tcTargetTag) == 0) {
		m_pAttribute->ReleaseAttacker(_tCollisionInfo.pCollidedCollider);
	}
}

CSplashAttack * CSplashAttack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSplashAttack*	pInstance = new CSplashAttack(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CSplashAttack::Free(void)
{
	CGameObject::Free();
}

void CSplashAttack::SetSplashAttackInfo(const _tchar* _pColliderTag, const _tchar * _pTargetTag, const _vec3 & _vPos, _float _fDamage, _float _fRadius, _float _fLifeTime, const _vec3 & _vAttackDirXZ, _float _fSplashRangeDegree)
{
	// �浹ü ����
	m_pCullingSphere = Engine::CColliderObject_Sphere::Create(m_pGraphicDev);
	m_pCullingSphere->SetColliderTag(L"CULL");
	AddCollider(m_pCullingSphere);
	m_pCullingSphere->SetRadius(_fRadius * 1.5f);

	m_pAttackCollider = Engine::CColliderObject_Sphere::Create(m_pGraphicDev);
	m_pAttackCollider->SetColliderTag(_pColliderTag);
	AddCollider(m_pAttackCollider);
	// �浹ü ������ ����
	m_pAttackCollider->SetRadius(_fRadius);
	// �浹ü ������ ����
	//m_pAttackCollider->SetDamage(_fDamage);
	m_fDamage = _fDamage;
	
	// ��ġ ����
	GetTransform()->SetPos(_vPos);

	// Ÿ�� �±� ����
	lstrcpy(m_tcTargetTag, _pTargetTag);
	
	// ������ Ÿ�� ����
	m_fLifeTime = _fLifeTime;

	// ���� ���� ����
	m_vAttackDirXZ = _vAttackDirXZ;

	// ���÷��� ���� �� ����
	m_fSplashRangeDegree = _fSplashRangeDegree;

	// �浹�Ŵ����� ��ü �߰�
	Engine::CCollisionMgr::GetInstance()->AddGameObject(this);

	// TODO : ���� ����Ʈ �߰�
	_vec3 vAttackDirXZ = _vAttackDirXZ;
	_matrix matRot;
	D3DXMatrixRotationY(&matRot, -D3DXToRadian(m_fSplashRangeDegree));			
	D3DXVec3TransformNormal(&vAttackDirXZ, &vAttackDirXZ, &matRot);			// ���� ������ ���÷��� ���� ���������� ����
	_int iParticleNum = static_cast<_int>(Engine::GetValueByWeight(D3DXToRadian(m_fSplashRangeDegree) / D3DX_PI, 1.f, 40.f));
	_float fRotGap = D3DXToRadian(m_fSplashRangeDegree * 2.f) / (iParticleNum + 1);
	D3DXMatrixRotationY(&matRot, fRotGap);
	//D3DXVec3TransformNormal(&vAttackDirXZ, &vAttackDirXZ, &matRot);	

	CPlateEffect* pEffect = nullptr;
	for (_int i = 0; i < iParticleNum; ++i) {
		D3DXVec3TransformNormal(&vAttackDirXZ, &vAttackDirXZ, &matRot);
		pEffect = CPlateEffect::Create(m_pGraphicDev);
		pEffect->SetPlateEffectInfo(L"EFT_Smoke", _vPos + vAttackDirXZ * _fRadius, _vec2(0.8f, 0.8f), _vec2(1.f, 1.f), 0.f, 1.f, _vec3(0.8f, 0.8f, 0.8f));
		pEffect->GetPhysics()->SetVelocityXZ(_vec2(vAttackDirXZ.x, vAttackDirXZ.z) * Engine::GetNumberBetweenMinMax(3.f, 5.f));
		pEffect->GetPhysics()->SetResistanceCoefficientXZ(Engine::GetNumberBetweenMinMax(0.85f, 0.95f));
		pEffect->GetPhysics()->SetGravity(-Engine::GetNumberBetweenMinMax(3.f, 4.f));
		Engine::GetLayer(L"Environment")->Add_GameObject(L"Effect", pEffect);
	}
}

_bool CSplashAttack::IsInRange(Engine::CGameObject * _pTargetObject)
{
	// ������ �浹�Ǿ��ٴ� �����Ͽ� SplashRange���ο� TargetObject�� �����ϴ����� ���캾�ϴ�.
	_vec3 vToTargetXZ = _pTargetObject->GetTransform()->GetPos() - GetTransform()->GetPos();
	vToTargetXZ.y = 0.f;
	_float fLength = D3DXVec3Length(&vToTargetXZ);
	if (fLength <= 0.f)
		// ������ ���� ������ �ִ� ���̹Ƿ� �浹�� ó���Ѵ�.
		return true;

	vToTargetXZ /= fLength;	// normalize;

	// ���ݹ��� ���Ϳ� vToTargetXZ ������ ������ ���Ѵ�.
	_float fRotDegree = D3DXToDegree(Engine::GetRotationAngle(vToTargetXZ, m_vAttackDirXZ));
	if (fRotDegree <= m_fSplashRangeDegree)
		return true;
	return false;
}
