#ifndef Cat_h__
#define Cat_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CUI_SpeechBubble;
class CMap;
template<typename T> class CStateMgr;
class CCat : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CCat(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CCat(const  CCat& rhs);
	virtual ~CCat(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	//virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	//virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CCat*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	//_bool LoadRoutePosList(const _tchar* _pRouteFilePath);

	CStateMgr<CCat>* GetStateMgr() const { return m_pStateMgr; }
	Engine::CDynamicMesh* GetDynamicMesh() const { return m_pMesh; }
	Engine::CPhysics* GetPhysics() const { return m_pPhysics; }

	//_bool IsTalking() const { return m_bIsTalking; }
	void ShoutOut(_float _fOffsetX, _float _fOffsetY, const wstring & _wstrText);
	_bool IsGoOn() const { return m_bIsGoOn; }
	void SetGoOn(_bool _bIsGoOn, const _vec3& _vTargetPos) { 
		m_bIsGoOn = _bIsGoOn; 
		m_vTargetPos = _vTargetPos;
	}
	_vec3 GetTargetPos() const { return m_vTargetPos; }

private:
	CMap* m_pMap = nullptr;
	
	CStateMgr<CCat>* m_pStateMgr = nullptr;
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;

	_bool m_bIsTalking = false;
	CUI_SpeechBubble* m_pSpeechBubble = nullptr;

	_bool m_bIsGoOn = false;
	_vec3 m_vTargetPos;
	//_int m_iCellIndex = -1;
};

END

#endif // !LogoObject_h__
