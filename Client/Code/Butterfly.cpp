#include "pch.h"
#include "Butterfly.h"

CButterfly::CButterfly(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CButterfly::CButterfly(const CButterfly & rhs)
	:
	CGameObject(rhs)
{
}

CButterfly::~CButterfly(void)
{
}

HRESULT CButterfly::Ready_Object(void)
{
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Butterfly"));
	m_mapComponent[Engine::ID_STATIC][Engine::CStaticMesh::GetComponentTag()] = m_pMesh;

	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_ALPHA, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Alpha"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	// Physics
	m_pPhysics = AddComponent<Engine::CPhysics>();

	return S_OK;
}

_int CButterfly::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if ((m_fLifeTime -= _fDeltaTime) <= 0.f) {
		m_fLifeTime = 0.f;
		switch (m_eDestroyType) {
		case DESTROY_INVALID:
			SetValid(false);
			break;
		case DESTROY_UNACTIVATED:
			SetActivated(false);
			break;
		}
		return 1;
	}
	else if (m_fLifeTime <= m_fMaxLifeTime * 0.4f) {
		m_fSize = Engine::GetWeightByValue(m_fLifeTime, 0.f, m_fMaxLifeTime * 0.4f) * m_fMaxSize;
		GetTransform()->SetScaleZ(m_fSize);
	}

	//_float fT = 1.f - m_fLifeTime / m_fMaxLifeTime;
	//_vec2 vNewSize = _vec2(Engine::GetValueByWeight(fT, m_vStartSize.x, m_vEndSize.x), Engine::GetValueByWeight(fT, m_vStartSize.y, m_vEndSize.y));
	//GetTransform()->SetScaleXYZ(vNewSize.x, vNewSize.y, 1.f);

	m_fRadian += D3DXToRadian(WING_SPEED_DEGREE) * _fDeltaTime;
	if (m_fRadian >= 2.f * D3DX_PI) {
		m_fRadian -= 2.f * D3DX_PI;
	}

	// 날개짓 표현
	_float fT = Engine::GetWeightByCos(m_fRadian);
	GetTransform()->SetScaleX(m_fSize * Engine::GetValueByWeight(fT, 0.01f, 1.f));
	GetTransform()->SetScaleY(m_fSize * Engine::GetValueByWeight(fT, 1.f, 0.01f));


	GetTransform()->SetPos(GetPhysics()->GetUpdatedPos(_fDeltaTime, Engine::CTransform::COORD_TYPE_LOCAL));
	
	m_pRenderer->Update(_fDeltaTime);
	return 0;
}

void CButterfly::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pRenderer->Render(m_pShader->Get_EffectHandle());
}

CButterfly * CButterfly::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CButterfly*	pInstance = new CButterfly(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CButterfly::Free(void)
{
	CGameObject::Free();
}

void CButterfly::SetButterflyInfo(const _vec3 & _vPos, const _vec3 & _vDir, _float _fSpeed, _float _fSize, _float _fLifeTime, E_DESTROY_TYPE _eDestroyType)
{
	GetTransform()->SetPos(_vPos);
	GetTransform()->SetScaleXYZ(_fSize, _fSize, _fSize);
	m_fSize = _fSize;
	m_fMaxSize = _fSize;
	GetPhysics()->SetDirection(_vDir);
	GetPhysics()->SetSpeed(_fSpeed);

	// X축을 _vDir에 맞춘다.
	/*_vec3 vRight = GetTransform()->GetRight();
	_vec3 vRotAxis = Engine::GetRotationAxis(vRight, _vDir);
	_float fRotAngle = Engine::GetRotationAngle(vRight, _vDir);
	GetTransform()->RotateByAxis(fRotAngle, vRotAxis);*/
	if(_vDir != WORLD_Y_AXIS)
		GetTransform()->ResetRightUpLook(&(_vPos + _vDir), &WORLD_Y_AXIS);
	else {
		SetValid(false);
		return;
	}
	//// Y축을 WORLD_Y_AXIS에 맞춘다.
	//_vec3 vUp = GetTransform()->GetUp();
	//vRotAxis = Engine::GetRotationAxis(vUp, WORLD_Y_AXIS);
	//fRotAngle = Engine::GetRotationAngle(vUp, WORLD_Y_AXIS);
	//GetTransform()->RotateByAxis(fRotAngle, vRotAxis);

	m_fLifeTime = _fLifeTime;
	m_fMaxLifeTime = _fLifeTime;

	m_eDestroyType = _eDestroyType;
}
