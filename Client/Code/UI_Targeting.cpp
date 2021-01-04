#include "pch.h"
#include "UI_Targeting.h"
#include "UI_Image.h"
#include "Attribute.h"
#include "AliceW.h"

CUI_Targeting::CUI_Targeting(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_Targeting::CUI_Targeting(const CUI_Targeting & rhs)
	:
	CGameObject(rhs)
{
}

CUI_Targeting::~CUI_Targeting(void)
{
}

HRESULT CUI_Targeting::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CPolygonRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_UI, dynamic_cast<Engine::CVIBuffer*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex")), static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"UI_Targeting")));
	GetTransform()->SetScaleXYZ(0.3f, 0.3f, 1.f);
	m_fOffsetY = 1.5f;
	return S_OK;
}

_int CUI_Targeting::Update_Object(const _float & _fDeltaTime)
{
	// m_fCurHP °»½Å

	CAliceW* pAlice = dynamic_cast<CAliceW*>(m_pPlayer);
	if (pAlice && pAlice->GetTargetObject()) {
		_vec3 vTargetPos = pAlice->GetTargetObject()->GetTransform()->GetPos();
		m_fRadian += D3DXToRadian(100.f) * _fDeltaTime;
		if (m_fRadian > 2.f * D3DX_PI) 
			m_fRadian -= 2.f * D3DX_PI;
		vTargetPos.y += (m_fOffsetY + Engine::GetValueByWeight(Engine::GetWeightByCos(m_fRadian), -0.2f, 0.2f));
		GetTransform()->SetPos(vTargetPos);

		_vec3 vToUI = GetTransform()->GetPos() - m_pPlayer->GetTransform()->GetPos();
		vToUI.y = 0.f;
		_float fLength = D3DXVec3Length(&vToUI);
		if (fLength > 0.f) {
			vToUI /= fLength;
			_vec3 vLook = GetTransform()->GetLook();
			_vec3 vRotAxis = Engine::GetRotationAxis(vLook, vToUI);
			_float fRotAngle = Engine::GetRotationAngle(vLook, vToUI);
			GetTransform()->RotateByAxis(fRotAngle, vRotAxis);
		}
	}
	else
		return 0;

	m_pRenderer->Update(_fDeltaTime);
	return 0;
}

void CUI_Targeting::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pRenderer->Render();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CUI_Targeting * CUI_Targeting::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Targeting*	pInstance = new CUI_Targeting(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_Targeting::Free(void)
{
	CGameObject::Free();
}

void CUI_Targeting::SetPlayer(Engine::CGameObject * _pPlayer)
{
	if (!_pPlayer)
		return;

	m_pPlayer = _pPlayer;
}
