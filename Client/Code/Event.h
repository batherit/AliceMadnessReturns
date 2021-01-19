#ifndef Platform_h__
#define Platform_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAliceW;
class CMirrorPad;
class CPlatform : public Engine::CGameObject
{
#define PRESS_TIME 1.f
private: // 생성자, 소멸자
	explicit  CPlatform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CPlatform(const  CPlatform& rhs);
	virtual ~CPlatform(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CPlatform*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CStaticMesh* GetStaticMesh() const { return m_pMesh; }
	void SetPos(_vec3 _vInitPos, _vec3 _vEndPos, _float _fPressTime) {
		m_vInitPos = _vInitPos;
		m_vEndPos = _vEndPos;
		m_fPressTime = _fPressTime;
		if (_fPressTime == 0.f) {
			abort();
		}
		m_vVelocity = (_vEndPos - _vInitPos) / _fPressTime;
		D3DXVec3Normalize(&m_vDir, &m_vVelocity);

		GetTransform()->SetPos(_vInitPos);
	}

	void SetLinkIndex(_int _iLinkIndex) { m_iLinkIndex = _iLinkIndex; }
	_int GetLinkIndex() const { return m_iLinkIndex; }

	void On(const _float& _fDeltaTime);
	void Off(const _float& _fDeltaTime);
	_float GetHeight() const { return GetTransform()->GetPos().y + 1.05f * GetTransform()->GetScale().y; }
	_vec3 GetDeltaPos() const { return m_vDeltaPos; }

private:
	_int m_iLinkIndex = -1;

	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	_vec3 m_vInitPos;
	_vec3 m_vEndPos;
	_vec3 m_vVelocity;
	_vec3 m_vDir;
	_float m_fPressTime = 2.f;
	_vec3 m_vDeltaPos = _vec3(0.f, 0.f, 0.f);
};

END

#endif // !LogoObject_h__