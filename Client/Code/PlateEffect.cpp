#include "pch.h"
#include "PlateEffect.h"

CPlateEffect::CPlateEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CPlateEffect::CPlateEffect(const CPlateEffect & rhs)
	:
	CGameObject(rhs)
{
}

CPlateEffect::~CPlateEffect(void)
{
}

HRESULT CPlateEffect::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CPolygonRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_ALPHA, 
		dynamic_cast<Engine::CVIBuffer*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex")), 
		nullptr);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_PlateEffect"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	return S_OK;
}

_int CPlateEffect::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if ((m_fLifeTime -= _fDeltaTime) <= 0.f) {
		m_fLifeTime = 0.f;
		switch (m_eDestroyType) {
		case DESTROY_UNVALID:
			SetValid(false);
			break;
		case DESTROY_UNACTIVATED:
			SetActivated(false);
			break;
		}
		return 1;
	}

	_float fT = 1.f - m_fLifeTime / m_fMaxLifeTime;
	_vec2 vNewSize = _vec2(Engine::GetValueByWeight(fT, m_vStartSize.x, m_vEndSize.x), Engine::GetValueByWeight(fT, m_vStartSize.y, m_vEndSize.y));
	GetTransform()->SetScaleXYZ(vNewSize.x, vNewSize.y, 1.f);

	m_pRenderer->Update(_fDeltaTime);
	return 0;
}

void CPlateEffect::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	
	auto pEffect = m_pShader->Get_EffectHandle();
	pEffect->SetFloat("g_fT", 1.f - m_fLifeTime / m_fMaxLifeTime);
	m_pRenderer->Render(pEffect);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CPlateEffect * CPlateEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlateEffect*	pInstance = new CPlateEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CPlateEffect::Free(void)
{
	CGameObject::Free();
}

void CPlateEffect::SetPlateEffectInfo(const _tchar* _pTextureTag, const _vec3 & _vPos, const _vec2 & _vStartSize, const _vec2 & _vEndSize, _float _fAngleZ, _float _fLifeTime, E_DESTROY_TYPE _eDestroyType)
{
	// �ؽ��� ����
	m_pRenderer->SetTexture(static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, _pTextureTag)));

	// ����Ʈ ��ġ ����
	GetTransform()->SetPos(_vPos);

	// ����Ʈ �ʱ� ������ ����
	m_vStartSize = _vStartSize;
	GetTransform()->SetScaleXYZ(m_vStartSize.x, m_vStartSize.y, 1.f);
	m_vEndSize = _vEndSize;

	// ����Ʈ�� ī�޶������� ������ ȸ��
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	_vec3 vNewLook = _vec3(matView._41, matView._42, matView._43) - _vPos;		// ī�޶� ��ġ�� ���ϴ� look���͸� ����.
	D3DXVec3Normalize(&vNewLook, &vNewLook);
	GetTransform()->ResetRightUpLook(&(_vPos + vNewLook), &WORLD_Y_AXIS);		// ������ ȸ��

	// _fAngleZ��ŭ Z�� ȸ��
	GetTransform()->RotateByLook(_fAngleZ);

	// ������Ÿ�� ����
	m_fLifeTime = _fLifeTime;
	m_fMaxLifeTime = _fLifeTime;

	// �Ҹ� Ÿ�� ����
	m_eDestroyType = DESTROY_UNVALID;

	// Ȱ��ȭ
	SetActivated(true);
}
