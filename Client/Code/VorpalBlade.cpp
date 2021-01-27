#include "pch.h"
#include "VorpalBlade.h"
#include "Attribute.h"
#include "EFT_SlashAttack.h"

CVorpalBlade::CVorpalBlade(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CStaticObject(pGraphicDev)
{
}

CVorpalBlade::CVorpalBlade(const CVorpalBlade & rhs)
	:
	CStaticObject(rhs)
{
}

CVorpalBlade::~CVorpalBlade(void)
{
}

HRESULT CVorpalBlade::Ready_Object(void)
{
	CStaticObject::Ready_Object();
	SetRenderInfo(L"VorpalBlade");

	// 삭제는 해당 객체가 책임진다. => 추출된 콜라이더는 자식으로 간주되지 않기 때문.
	m_pTop = ExtractColliderFromTag(L"Top");
	m_pTop->SetActivated(false);
	m_pBottom = ExtractColliderFromTag(L"Bottom");
	m_pBottom->SetActivated(false);

	m_pAttackCollider = GetColliderFromTag(L"PlayerAttack");
	m_pAttackCollider->SetDamage(VORPALBLADE_DAMAGE);

	m_pAttribute = AddComponent<CAttribute>();

	m_pTrail = dynamic_cast<Engine::CTrail*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_Trail"));
	m_pRenderer = AddComponent<Engine::CPolygonRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_ALPHA, m_pTrail, static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"EFT_Trail_BG")));
	
	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Trail"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);


	return S_OK;
}

int CVorpalBlade::Update_Object(const _float & _fDeltaTime)
{
	CStaticObject::Update_Object(_fDeltaTime);

	//if () {
	if (m_pAttackCollider->IsActivated()) {
		if (m_TrailVtxList.size() > 11) {
			m_TrailVtxList.pop_back();
		}
		m_TrailVtxList.emplace_front(make_pair(m_pTop->GetTransform()->GetPos(), m_pBottom->GetTransform()->GetPos()));
	}
	else {
		//m_pTrail->Clear_Vertex();
		m_TrailVtxList.clear();
	}

	//if ((m_fTickTime += _fDeltaTime) >= 1.f) {
	//	m_pTrail->Clear_Vertex();
	//	m_TrailVtxList.clear();
	//	m_fTickTime = 0.f;
	//}
	//else {
		
	//}

	return 0;
}

void CVorpalBlade::Render_Object(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CStaticObject::Render_Object();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pTrail->Clear_Vertex();
	m_pTrail->SetTrailList(&m_TrailVtxList);
	m_pRenderer->Render(m_pShader->Get_EffectHandle());
}

void CVorpalBlade::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Monster") == 0) {
		if (m_pAttribute->RegisterAttacker(_tCollisionInfo.pCollidedCollider)) {
			// 어태커에 등록이 성공했다는 것은 기존 어태커가 등록되지 않았음을 의미하므로 데미지가 들어간다
			_tCollisionInfo.pCollidedObject->GetComponent<CAttribute>()->Damaged(/*VORPALBLADE_DAMAGE*/0);

			CEFT_SlashAttack* pEffect = CEFT_SlashAttack::Create(m_pGraphicDev);
			pEffect->SetInfo((_tCollisionInfo.pCollidedCollider->GetTransform()->GetPos() + _tCollisionInfo.pCollidedMyCollider->GetTransform()->GetPos()) * 0.5f);
			Engine::GetLayer(L"Environment")->Add_GameObject(L"Effect", pEffect);
		}
	}
}

void CVorpalBlade::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Monster") == 0) {
		m_pAttribute->ReleaseAttacker(_tCollisionInfo.pCollidedCollider);
	}
}

CVorpalBlade * CVorpalBlade::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVorpalBlade*	pInstance = new CVorpalBlade(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CVorpalBlade::Free(void)
{
	Engine::Safe_Release(m_pTop);
	Engine::Safe_Release(m_pBottom);
	Engine::Safe_Release(m_pTrail);
	CStaticObject::Free();
}
