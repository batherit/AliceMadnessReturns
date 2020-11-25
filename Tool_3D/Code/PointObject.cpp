#include "pch.h"
#include "PointObject.h"

CPointObject::CPointObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CPointObject::CPointObject(const CPointObject & rhs)
	:
	CGameObject(rhs)
{
}

CPointObject::~CPointObject(void)
{
}

HRESULT CPointObject::Ready_Object(void)
{
	return E_NOTIMPL;
}

int CPointObject::Update_Object(const _float & _fDeltaTime)
{
	return 0;
}

void CPointObject::Render_Object(void)
{
}

CPointObject * CPointObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CPointObject::Free(void)
{
}

void CPointObject::SetPosition(const _vec3 & _vPos)
{
	GetTransform()->SetPos(_vPos);
}

_vec3 CPointObject::GetPosition() const
{
	return GetTransform()->GetPos();
}

void CPointObject::Translate(const _vec3 & _vDeltaMove) const
{
	return GetTransform()->Translate(_vDeltaMove);
}

_bool CPointObject::IsPickedByRay(const Engine::PICKINGRAYINFO & _stRayInfo)
{
	return _bool();
}

