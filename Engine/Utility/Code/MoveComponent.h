#ifndef MoveComponent_h__
#define	MoveComponent_h__

#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CMoveComponent final : public CComponent
{
public:
	enum E_COORD_TYPE { COORD_TYPE_LOCAL, COORD_TYPE_WORLD, COORD_TYPE_END };

protected:
	explicit CMoveComponent();	// �Ϲ� ������
	explicit CMoveComponent(const CMoveComponent& rhs);			// ���� ������
	virtual ~CMoveComponent();

public:
	virtual HRESULT Ready(void) { return S_OK; }
	virtual int Update(const _float& _fDeltaTime) { return 0; }
	static const _tchar* GetComponentTag() { return L"MoveComponent"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

public:
	static CMoveComponent* Create();
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void);

public:
	void MoveByDelta(const _float& _fDeltaTime) { 
		m_vPos.x += (m_vDir.x * m_fSpeed * _fDeltaTime); 
		m_vPos.y += (m_vDir.y * m_fSpeed * _fDeltaTime);
		m_vPos.z += (m_vDir.z * m_fSpeed * _fDeltaTime);
	}
	void Scaling(_float _fDeltaX, _float _fDeltaY, _float _fDeltaZ) {
		m_vScale.x += _fDeltaX;
		m_vScale.y += _fDeltaY;
		m_vScale.z += _fDeltaZ;
	}
	void Rotate(_float _fDeltaX, _float _fDeltaY, _float _fDeltaZ) {
		m_vRot.x += _fDeltaX;
		m_vRot.y += _fDeltaY;
		m_vRot.z += _fDeltaZ;
	}
	void Translate(_float _fDeltaX, _float _fDeltaY, _float _fDeltaZ) {
		m_vPos.x += _fDeltaX;
		m_vPos.y += _fDeltaY;
		m_vPos.z += _fDeltaZ;
	}
	
	// ������
	void SetScaleX(_float _fScaleX) { m_vScale.x = _fScaleX; }
	void SetScaleY(_float _fScaleY) { m_vScale.y = _fScaleY; }
	void SetScaleZ(_float _fScaleZ) { m_vScale.z = _fScaleZ; }
	void SetScaleXYZ(_float _fScaleX, _float _fScaleY, _float _fScaleZ) { SetScaleX(_fScaleX); SetScaleY(_fScaleY); SetScaleZ(_fScaleZ); }
	void SetScaleXYZ(_vec3 _vScale) { m_vScale = _vScale; }
	
	// ȸ��
	void RotateByRight(_float _fDeltaAngle) {
		_matrix m_matRot;
		D3DXMatrixRotationAxis(&m_matRot, &m_vRight, _fDeltaAngle);
		D3DXVec3TransformNormal(&m_vLook, &m_vLook, &m_matRot);
		D3DXVec3TransformNormal(&m_vUp, &m_vUp, &m_matRot);
		NormalizeAxis();
	}
	void RotateByUp(_float _fDeltaAngle) {
		_matrix m_matRot;
		D3DXMatrixRotationAxis(&m_matRot, &m_vUp, _fDeltaAngle);
		D3DXVec3TransformNormal(&m_vLook, &m_vLook, &m_matRot);
		D3DXVec3TransformNormal(&m_vRight, &m_vRight, &m_matRot);
		NormalizeAxis();
	}
	void RotateByLook(_float _fDeltaAngle) {
		_matrix m_matRot;
		D3DXMatrixRotationAxis(&m_matRot, &m_vLook, _fDeltaAngle);
		D3DXVec3TransformNormal(&m_vRight, &m_vRight, &m_matRot);
		D3DXVec3TransformNormal(&m_vUp, &m_vUp, &m_matRot);
		NormalizeAxis();
	}
	void RotateByAxis(_float _fDeltaAngle, const _vec3& _vRotAxis) {
		_matrix m_matRot;
		D3DXMatrixRotationAxis(&m_matRot, &_vRotAxis, _fDeltaAngle);
		D3DXVec3TransformNormal(&m_vRight, &m_vRight, &m_matRot);
		D3DXVec3TransformNormal(&m_vUp, &m_vUp, &m_matRot);
		D3DXVec3TransformNormal(&m_vLook, &m_vLook, &m_matRot);
		NormalizeAxis();
	}
	void NormalizeAxis() {
		D3DXVec3Normalize(&m_vRight, &m_vRight);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Normalize(&m_vLook, &m_vLook);
	}

	// �̵�/��ġ
	void SetX(_float _fX) { m_vPos.x = _fX; }
	void SetY(_float _fY) { m_vPos.y = _fY; }
	void SetZ(_float _fZ) { m_vPos.z = _fZ; }
	void SetXYZ(_float _fX, _float _fY, _float _fZ) { SetX(_fX); SetY(_fY); SetZ(_fZ); }
	void SetXYZ(_vec3 _vPos) { m_vPos = _vPos; }

	// ����
	void SetToX(_float _fToX) { m_vDir.x = _fToX; }
	void SetToY(_float _fToY) { m_vDir.y = _fToY; }
	void SetToZ(_float _fToZ) { m_vDir.z = _fToZ; }
	void SetToXYZ(_float _fToX, _float _fToY, _float _fToZ) { SetToX(_fToX); SetToY(_fToY); SetToZ(_fToZ); D3DXVec3Normalize(&m_vDir, &m_vDir); }
	void SetToXYZ(_vec3 _vDir) { m_vDir = _vDir; D3DXVec3Normalize(&m_vDir, &m_vDir); }

	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; Clamp(&m_fSpeed, -m_fMaxSpeed, m_fMaxSpeed); }
	void SetMaxSpeed(float _fMaxSpeed) { m_fMaxSpeed = _fMaxSpeed; }
	
	_matrix GetParentMatrix(E_COORD_TYPE _eCoordType = COORD_TYPE_WORLD) const {
		D3DXMATRIX matParent;
		D3DXMatrixIdentity(&matParent);

		CGameObject* pParent = m_pOwner->GetParent();
		if (pParent) {
			CMoveComponent* pParentMoveComponent = pParent->GetComponent<CMoveComponent>();
			if(pParentMoveComponent)
				matParent = pParentMoveComponent->GetObjectMatrix(_eCoordType);
		}
			
		// �θ������ ���� ���ߴٸ�, �׵������ ��ȯ�Ѵ�.
		return matParent;
	}
	_matrix GetObjectMatrix(E_COORD_TYPE _eCoordType = COORD_TYPE_WORLD) const {
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
			// ��
			matWorld *= GetParentMatrix(_eCoordType);
			break;
		}

		return matWorld;
	}
	_vec3 GetPos(E_COORD_TYPE _eCoordType = COORD_TYPE_WORLD) const {
		D3DXVECTOR3 vPos = m_vPos;

		switch (_eCoordType) {
		case COORD_TYPE_WORLD:
			D3DXVec3TransformCoord(&vPos, &vPos, &GetParentMatrix(COORD_TYPE_WORLD));
			break;
		}

		return vPos;
	}
	_vec3 GetRight() const { return m_vRight; }
	_vec3 GetUp() const { return m_vUp; }
	_vec3 GetLook() const { return m_vLook; }


	
private:
	_vec3 m_vRight{ 1.f, 0.f, 0.f };
	_vec3 m_vUp{ 0.f, 1.f,0.f };
	_vec3 m_vLook{ 0.f, 0.f, 1.f };
	_vec3 m_vScale{ 1.f, 1.f, 1.f };
	_vec3 m_vRot{ 0.f, 0.f, 0.f };			// Radian
	_vec3 m_vPos{ 0.f, 0.f, 0.f };
	_vec3 m_vDir{ 1.f, 0.f, 0.f };
	_vec3 m_vAngle{ 0.f, 0.f, 0.f };
	//_vec3 m_vPosOffset
	_matrix m_matWorld;
	_float m_fMaxSpeed = 9876543210.f;
	_float m_fSpeed = 0.f;
};

END

#endif