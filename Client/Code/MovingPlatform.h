#ifndef MovingPlatform_h__
#define MovingPlatform_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAliceW;
class CMirrorPad;
class CMovingPlatform : public Engine::CGameObject
{
#define PRESS_TIME 1.f
private: // 생성자, 소멸자
	explicit  CMovingPlatform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CMovingPlatform(const  CMovingPlatform& rhs);
	virtual ~CMovingPlatform(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CMovingPlatform*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CStaticMesh* GetStaticMesh() const { return m_pMesh; }

	void SetMovingPlatformInfo(const _vec3& _vEndPos, _float _fMovingTime, _float _fKeepTime) {
		m_vStartPos = GetTransform()->GetPos();
		m_vEndPos = _vEndPos;
		m_fMovingTime = _fMovingTime;
		m_fKeepTime = _fKeepTime;
		m_fElapsedTime = 0.f;
	}
	_float GetHeight() const { return GetTransform()->GetPos().y + 2.6f * GetTransform()->GetScale().y; }
	_vec3 GetDeltaPos() const { return m_vDeltaPos; }

private:
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;

	CAliceW* m_pAliceW = nullptr;

	_vec3 m_vStartPos;
	_vec3 m_vEndPos;
	_float m_fMovingTime = 1.f;
	_float m_fKeepTime = 1.f;
	_float m_fElapsedTime = 0.f;
	_bool m_bIsOn = false;
	_bool m_bIsOff = false;
	_bool m_bIsEnd = false;
	
	_vec3 m_vDeltaPos = _vec3(0.f, 0.f, 0.f);
};

END

#endif // !LogoObject_h__