#ifndef MoveComponent_h__
#define	MoveComponent_h__

#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum E_COORD_TYPE { COORD_TYPE_LOCAL, COORD_TYPE_WORLD, COORD_TYPE_END };

protected:
	explicit CTransform();	// 일반 생성자
	explicit CTransform(const CTransform& rhs);			// 복사 생성자
	virtual ~CTransform();

public:
	virtual HRESULT Ready(void) { return S_OK; }
	virtual int Update(const _float& _fDeltaTime) { return 0; }
	static const _tchar* GetComponentTag() { return L"Transform"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

public:
	static CTransform* Create();
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void);

public:
	void Scaling(_float _fDeltaX, _float _fDeltaY, _float _fDeltaZ);
	void Rotate(_float _fDeltaX, _float _fDeltaY, _float _fDeltaZ);
	void Translate(_float _fDeltaX, _float _fDeltaY, _float _fDeltaZ);
	void Translate(const _vec3& _vDeltaMove);
	
	// 스케일
	void SetScale(const _vec3& _vScale) { m_vScale = _vScale; }
	void SetScaleX(_float _fScaleX) { m_vScale.x = _fScaleX; }
	void SetScaleY(_float _fScaleY) { m_vScale.y = _fScaleY; }
	void SetScaleZ(_float _fScaleZ) { m_vScale.z = _fScaleZ; }
	void SetScaleXYZ(_float _fScaleX, _float _fScaleY, _float _fScaleZ) { SetScaleX(_fScaleX); SetScaleY(_fScaleY); SetScaleZ(_fScaleZ); }
	void SetScaleXYZ(const _vec3& _vScale) { m_vScale = _vScale; }
	
	// 회전
	void SetAngle(const _vec3& _vAngle);
	void SetAngle(_float _fYaw, _float _fPitch, _float _fRoll) { SetAngle(_vec3(_fPitch, _fYaw, _fRoll)); }
	void RotateByRight(_float _fDeltaAngle);
	void RotateByUp(_float _fDeltaAngle);
	void RotateByLook(_float _fDeltaAngle);
	void RotateByAxis(_float _fDeltaAngle, const _vec3& _vRotAxis);
	void NormalizeAxis();

	// 축 세팅
	void ResetRightUpLook(const _vec3 *pAt, const _vec3 *pUp);

	// 이동/위치
	void SetPosX(_float _fX) { m_vPos.x = _fX; }
	void SetPosY(_float _fY) { m_vPos.y = _fY; }
	void SetPosZ(_float _fZ) { m_vPos.z = _fZ; }
	void SetPos(_float _fX, _float _fY, _float _fZ) { SetPosX(_fX); SetPosY(_fY); SetPosZ(_fZ); }
	void SetPos(_vec3 _vPos) { m_vPos = _vPos; }

	// 본 행렬 세팅
	void SetParentBoneMatrix(const _matrix* _pParentBoneMatrix) { m_pParentBoneMatrix = _pParentBoneMatrix; }
	
	_matrix GetParentMatrix(E_COORD_TYPE _eCoordType = COORD_TYPE_WORLD) const;
	_matrix GetParentBoneMatrix() const;
	_matrix GetObjectMatrix(E_COORD_TYPE _eCoordType = COORD_TYPE_WORLD) const;
	_vec3 GetPos(E_COORD_TYPE _eCoordType = COORD_TYPE_WORLD) const;
	_vec3 GetRight() const { return m_vRight; }
	_vec3 GetUp() const { return m_vUp; }
	_vec3 GetLook() const { return m_vLook; }

private:
	_matrix GetLocalRotationMatrix() const;
	
private:
	_vec3 m_vRight{ 1.f, 0.f, 0.f };
	_vec3 m_vUp{ 0.f, 1.f,0.f };
	_vec3 m_vLook{ 0.f, 0.f, 1.f };
	_vec3 m_vScale{ 1.f, 1.f, 1.f };
	_vec3 m_vAngle{ 0.f, 0.f, 0.f };			// Radian
	_vec3 m_vPos{ 0.f, 0.f, 0.f };
	// _vec3 m_vDir{ 1.f, 0.f, 0.f };
	_matrix m_matWorld;
	const _matrix* m_pParentBoneMatrix = nullptr;
	//_float m_fMaxSpeed = 9876543210.f;
	//_float m_fSpeed = 0.f;
};

END

#endif