#ifndef GiantAliceW_h__
#define GiantAliceW_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CMap;
class CCat;
class CAttribute;
//class CUI_SpeechBubble;
template<typename T> class CStateMgr;
class CGiantAliceW : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CGiantAliceW(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CGiantAliceW(const  CGiantAliceW& rhs);
	virtual ~CGiantAliceW(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CGiantAliceW*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	CStateMgr<CGiantAliceW>* GetStateMgr() const { return m_pStateMgr; }
	Engine::CDynamicMesh* GetDynamicMesh() const { return m_pMesh; }
	Engine::CPhysics* GetPhysics() const { return m_pPhysics; }

	_bool IsAttackOn(const _float& _fDeltaTime);
	_bool IsAttacking(const _float& _fDeltaTime);
	_bool IsMoving(const _float& _fDeltaTime, _vec3* _pDir = nullptr);
	_bool IsRunOn(const _float&, _vec3* _pDir = nullptr);
	CMap* GetMap() const { return m_pMap; }
	_int GetCellIndex() const { return m_iCellIndex; }

	Engine::CGameObject* GetHead() const { return m_pHead; }
	Engine::CColliderObject* GetLeftFoot() const { return m_pLeftFoot; }
	Engine::CColliderObject* GetRightFoot() const { return m_pAttackCollider; }
	Engine::CColliderObject* GetAttackCollider() const { return m_pAttackCollider; }

private:
	CMap* m_pMap = nullptr;
	CCat* m_pCat = nullptr;

	Engine::CGameObject* m_pHead = nullptr;
	Engine::CColliderObject* m_pLeftFoot = nullptr;
	Engine::CColliderObject* m_pAttackCollider = nullptr;

	CStateMgr<CGiantAliceW>* m_pStateMgr = nullptr;
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;

	//CUI_SpeechBubble* m_pSpeechBubble;

	_int m_iCellIndex = -1;
};

END

#endif // !LogoObject_h__
