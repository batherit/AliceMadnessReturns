#include "Transform.h"

USING(Engine)

CTransform::CTransform()
{
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(const CTransform & rhs)
	:
	CComponent(rhs)
{
	m_matWorld = rhs.m_matWorld;
}

CTransform::~CTransform()
{
}

CTransform * CTransform::Create()
{
	CTransform*	pInstance = new CTransform();

	if (FAILED(pInstance->Ready()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTransform::Clone(void)
{
	return new CTransform(*this);
}

void CTransform::Free(void)
{
}

void CTransform::Scaling(_float _fDeltaX, _float _fDeltaY, _float _fDeltaZ) {
	m_vScale.x += _fDeltaX;
	m_vScale.y += _fDeltaY;
	m_vScale.z += _fDeltaZ;
}
void CTransform::Rotate(_float _fDeltaX, _float _fDeltaY, _float _fDeltaZ) {
	m_vAngle.x += _fDeltaX;
	m_vAngle.y += _fDeltaY;
	m_vAngle.z += _fDeltaZ;

	SetAngle(m_vAngle);
}
void CTransform::Translate(_float _fDeltaX, _float _fDeltaY, _float _fDeltaZ) {
	m_vPos.x += _fDeltaX;
	m_vPos.y += _fDeltaY;
	m_vPos.z += _fDeltaZ;
}
void CTransform::Translate(const _vec3& _vDeltaMove) {
	m_vPos.x += _vDeltaMove.x;
	m_vPos.y += _vDeltaMove.y;
	m_vPos.z += _vDeltaMove.z;
}
void CTransform::RotateByRight(_float _fDeltaAngle) {
	_matrix m_matRot;
	D3DXMatrixRotationAxis(&m_matRot, &m_vRight, _fDeltaAngle);
	D3DXVec3TransformNormal(&m_vLook, &m_vLook, &m_matRot);
	D3DXVec3TransformNormal(&m_vUp, &m_vUp, &m_matRot);
	NormalizeAxis();

	_matrix matRot = GetLocalRotationMatrix();
	_qt qtRot;
	D3DXQuaternionRotationMatrix(&qtRot, &matRot);
	m_vAngle = ConvQuaternionToYawPitchRoll(qtRot);
}
void CTransform::RotateByUp(_float _fDeltaAngle) {
	_matrix m_matRot;
	D3DXMatrixRotationAxis(&m_matRot, &m_vUp, _fDeltaAngle);
	D3DXVec3TransformNormal(&m_vLook, &m_vLook, &m_matRot);
	D3DXVec3TransformNormal(&m_vRight, &m_vRight, &m_matRot);
	NormalizeAxis();

	_matrix matRot = GetLocalRotationMatrix();
	_qt qtRot;
	D3DXQuaternionRotationMatrix(&qtRot, &matRot);
	m_vAngle = ConvQuaternionToYawPitchRoll(qtRot);
}
void CTransform::RotateByLook(_float _fDeltaAngle) {
	_matrix m_matRot;
	D3DXMatrixRotationAxis(&m_matRot, &m_vLook, _fDeltaAngle);
	D3DXVec3TransformNormal(&m_vRight, &m_vRight, &m_matRot);
	D3DXVec3TransformNormal(&m_vUp, &m_vUp, &m_matRot);
	NormalizeAxis();

	_matrix matRot = GetLocalRotationMatrix();
	_qt qtRot;
	D3DXQuaternionRotationMatrix(&qtRot, &matRot);
	m_vAngle = ConvQuaternionToYawPitchRoll(qtRot);
}
void CTransform::RotateByAxis(_float _fDeltaAngle, const _vec3& _vRotAxis) {
	_matrix m_matRot;
	D3DXMatrixRotationAxis(&m_matRot, &_vRotAxis, _fDeltaAngle);
	D3DXVec3TransformNormal(&m_vRight, &m_vRight, &m_matRot);
	D3DXVec3TransformNormal(&m_vUp, &m_vUp, &m_matRot);
	D3DXVec3TransformNormal(&m_vLook, &m_vLook, &m_matRot);
	NormalizeAxis();

	_matrix matRot = GetLocalRotationMatrix();
	_qt qtRot;
	D3DXQuaternionRotationMatrix(&qtRot, &matRot);
	m_vAngle = ConvQuaternionToYawPitchRoll(qtRot);
}
void CTransform::NormalizeAxis() {
	D3DXVec3Normalize(&m_vRight, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vLook, &m_vLook);
}

void CTransform::SetAngle(const _vec3& _vAngle) {
	m_vAngle = _vAngle;

	_qt qtRot = ConvAngleToQuaternion(m_vAngle);
	_matrix matRot;
	D3DXMatrixRotationQuaternion(&matRot, &qtRot);
	m_vRight.x = matRot._11;	m_vRight.y = matRot._12;	m_vRight.z = matRot._13;
	m_vUp.x = matRot._21;		m_vUp.y = matRot._22;		m_vUp.z = matRot._23;
	m_vLook.x = matRot._31;		m_vLook.y = matRot._32;		m_vLook.z = matRot._33;

	NormalizeAxis();
}
// 축 세팅
void CTransform::ResetRightUpLook(const _vec3 *pAt, const _vec3 *pUp) {
	_matrix matRUL;
	D3DXMatrixIdentity(&matRUL);

	// Look축을 구합니다.
	D3DXVec3Normalize(&m_vLook, &(*pAt - GetPos()));
	// Right축을 구합니다.
	D3DXVec3Cross(&m_vRight, pUp, &m_vLook);
	D3DXVec3Normalize(&m_vRight, &m_vRight);
	// Up축을 구합니다.
	D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);

	_matrix matRot = GetLocalRotationMatrix();
	_qt qtRot;
	D3DXQuaternionRotationMatrix(&qtRot, &matRot);
	m_vAngle = ConvQuaternionToYawPitchRoll(qtRot);
}
_matrix CTransform::GetParentMatrix(E_COORD_TYPE _eCoordType) const {
	D3DXMATRIX matParent;
	D3DXMatrixIdentity(&matParent);

	CGameObject* pParent = m_pOwner->GetParent();
	if (pParent) matParent = pParent->GetTransform()->GetObjectMatrix(_eCoordType);

	// 부모행렬을 얻지 못했다면, 항등행렬을 반환한다.
	return matParent;
}
_matrix CTransform::GetObjectMatrix(E_COORD_TYPE _eCoordType) const {
	_matrix matWorld;
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
	matWorld._41 = m_vPos.x;
	matWorld._42 = m_vPos.y;
	matWorld._43 = m_vPos.z;

	switch (_eCoordType)
	{
	case COORD_TYPE_WORLD:
		// 부
		matWorld *= GetParentMatrix(_eCoordType);
		break;
	}

	return matWorld;
}
_vec3 CTransform::GetPos(E_COORD_TYPE _eCoordType) const {
	D3DXVECTOR3 vPos = m_vPos;

	switch (_eCoordType) {
	case COORD_TYPE_WORLD:
		D3DXVec3TransformCoord(&vPos, &vPos, &GetParentMatrix(COORD_TYPE_WORLD));
		break;
	}

	return vPos;
}

_matrix CTransform::GetLocalRotationMatrix() const
{
	_matrix matRot;

	D3DXMatrixIdentity(&matRot);
	matRot._11 = m_vRight.x;	matRot._12 = m_vRight.y;	matRot._13 = m_vRight.z;
	matRot._21 = m_vUp.x;		matRot._22 = m_vUp.y;		matRot._23 = m_vUp.z;
	matRot._31 = m_vLook.x;		matRot._32 = m_vLook.y;		matRot._33 = m_vLook.z;

	return matRot;
}

//_matrix CTransform::GetWorldMatrix(void)
//{
//	//_matrix matWorld, matScale, matRot, matTrans;
//	//D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
//	//D3DXMatrixRotationYawPitchRoll(&matRot, m_vRot.y, m_vRot.x, m_vRot.z);
//	
//	//D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
//	//matWorld = matScale * matRot * matTrans;
//
//	_matrix matWorld, matTrans;
//	D3DXMatrixIdentity(&matWorld);
//	matWorld._11 = m_vRight.x * m_vScale.x;
//	matWorld._12 = m_vRight.y * m_vScale.x;
//	matWorld._13 = m_vRight.z * m_vScale.x;
//	matWorld._21 = m_vUp.x * m_vScale.y;
//	matWorld._22 = m_vUp.y * m_vScale.y;
//	matWorld._23 = m_vUp.z * m_vScale.y;
//	matWorld._31 = m_vLook.x * m_vScale.z;
//	matWorld._32 = m_vLook.y * m_vScale.z;
//	matWorld._33 = m_vLook.z * m_vScale.z;
//	
//	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
//	matWorld *= matTrans;
//
//	return matWorld;
//}
