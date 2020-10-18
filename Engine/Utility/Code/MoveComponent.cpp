#include "MoveComponent.h"

USING(Engine)

CMoveComponent::CMoveComponent()
{
	D3DXMatrixIdentity(&m_matWorld);
}

CMoveComponent::CMoveComponent(const CMoveComponent & rhs)
	:
	CComponent(rhs)
{
	m_matWorld = rhs.m_matWorld;
}

CMoveComponent::~CMoveComponent()
{
}

CMoveComponent * CMoveComponent::Create()
{
	CMoveComponent*	pInstance = new CMoveComponent();

	if (FAILED(pInstance->Ready()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CMoveComponent::Clone(void)
{
	return new CMoveComponent(*this);
}

void CMoveComponent::Free(void)
{
}

_matrix CMoveComponent::GetWorldMatrix(void)
{
	//_matrix matWorld, matScale, matRot, matTrans;
	//D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	//D3DXMatrixRotationYawPitchRoll(&matRot, m_vRot.y, m_vRot.x, m_vRot.z);
	
	//D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	//matWorld = matScale * matRot * matTrans;

	_matrix matWorld, matTrans;
	D3DXMatrixIdentity(&matWorld);
	matWorld._11 = m_vRight.x * m_vScale.x;
	matWorld._12 = m_vRight.y * m_vScale.x;
	matWorld._13 = m_vRight.z * m_vScale.x;
	matWorld._21 = m_vUp.x * m_vScale.y;
	matWorld._22 = m_vUp.y * m_vScale.y;
	matWorld._23 = m_vUp.z * m_vScale.y;
	matWorld._31 = m_vLook.x * m_vScale.z;
	matWorld._32 = m_vLook.y * m_vScale.z;
	matWorld._33 = m_vLook.z * m_vScale.z;
	
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	matWorld *= matTrans;

	return matWorld;
}
