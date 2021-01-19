#ifndef PigFly_h__
#define PigFly_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CRoute;
class CUI_SpeechBubble;
template<typename T> class CStateMgr;
class CPigFly : public Engine::CGameObject
{
public:
	enum E_EVENT_TYPE { TYPE_DOMINO, TYPE_FIST, TYPE_END };
	enum E_STEP { STEP_RELAX, STEP_ATTACKED, STEP_ACTION, STEP_TALKING, STEP_PROCEE_EVENT, STEP_END };
private: // 생성자, 소멸자
	explicit  CPigFly(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CPigFly(const  CPigFly& rhs);
	virtual ~CPigFly(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CPigFly*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	_bool LoadRoutePosList(const _tchar* _pRouteFilePath);
	
	Engine::CDynamicMesh* GetDynamicMesh() const { return m_pMesh; }
	Engine::CPhysics* GetPhysics() const { return m_pPhysics; }
	CRoute* GetRoute() const { return m_pRoute; }

	void ShoutOut(_float _fOffsetX, _float _fOffsetY, const wstring & _wstrText);

public:
	void SetEventType(E_EVENT_TYPE _eEventType) { m_eEventType = _eEventType; }
	_bool ProcessEvent();

private:
	E_EVENT_TYPE m_eEventType = TYPE_END;
	E_STEP m_eStep = STEP_RELAX;
	_int m_iTriggerGauge = 0;

	_bool m_bIsOn = false;				// true가 되면 사전 이벤트를 시작하게 됨.
	_bool m_bIsEventPlaying = false;	// true가 되변 본 이벤트를 시작하게 됨.

	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	CRoute* m_pRoute = nullptr;

	_float m_fTalkingTime = 3.f;
	CUI_SpeechBubble* m_pSpeechBubble = nullptr;
};

END

#endif // !LogoObject_h__
