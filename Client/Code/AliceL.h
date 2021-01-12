#ifndef AliceL_h__
#define AliceL_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CMap;
class CAttribute;
template<typename T> class CStateMgr;
class CAliceL : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CAliceL(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CAliceL(const  CAliceL& rhs);
	virtual ~CAliceL(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CAliceL*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	CStateMgr<CAliceL>* GetStateMgr() const { return m_pStateMgr; }
	Engine::CDynamicMesh* GetDynamicMesh() const { return m_pMesh; }
	Engine::CPhysics* GetPhysics() const { return m_pPhysics; }

	_bool IsMoving(const _float& _fDeltaTime, _vec3* _pDir = nullptr);
	_bool IsRunOn(const _float&, _vec3* _pDir = nullptr);
	CMap* GetMap() const { return m_pMap; }
	_int GetCellIndex() const { return m_iCellIndex; }

private:
	CMap* m_pMap = nullptr;

	CStateMgr<CAliceL>* m_pStateMgr = nullptr;
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;



	_int m_iCellIndex = -1;
};

END

#endif // !LogoObject_h__
