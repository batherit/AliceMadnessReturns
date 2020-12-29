#ifndef MadCapA_h__
#define MadCapA_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CMap;
template<typename T> class CStateMgr;
class CMadCapA : public Engine::CGameObject
{
private: // ������, �Ҹ���
	explicit  CMadCapA(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CMadCapA(const  CMadCapA& rhs);
	virtual ~CMadCapA(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CMadCapA*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	CStateMgr<CMadCapA>* GetStateMgr() const { return m_pStateMgr; }
	Engine::CDynamicMesh* GetDynamicMesh() const { return m_pMesh; }
	Engine::CPhysics* GetPhysics() const { return m_pPhysics; }

	Engine::CGameObject* GetTargetObject() const { return m_pTargetObject; }
	void SetTargetObject(Engine::CGameObject* _pTargetObject) { m_pTargetObject = _pTargetObject; }
	void SetDead(const _bool& _bIsDead) { m_bIsDead = _bIsDead; }
	void SetLanded(const _bool& _bIsLanded) { m_bIsLanded = _bIsLanded; }

	_bool IsFalling(const _float& _fDeltaTime);
	_bool IsDead() const { return m_bIsDead; }
	_bool IsLanded() const { return m_bIsLanded; }
	CMap* GetMap() const { return m_pMap; }
	_int GetCellIndex() const { return m_iCellIndex; }

private:
	CMap* m_pMap = nullptr;
	Engine::CGameObject* m_pTargetObject = nullptr;

	CStateMgr<CMadCapA>* m_pStateMgr = nullptr;
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	_int m_iCellIndex = -1;

	_bool m_bIsDead = false;
	_bool m_bIsLanded = false;
};

END

#endif // !LogoObject_h__