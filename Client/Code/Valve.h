#ifndef Valve_h__
#define Valve_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAliceW;
class CValve : public Engine::CGameObject
{
#define VALVE_SMOKE_TICKTIME 0.06f
private: // ������, �Ҹ���
	explicit  CValve(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CValve(const  CValve& rhs);
	virtual ~CValve(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CValve*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CStaticMesh* GetDynamicMesh() const { return m_pMesh; }

private:
	Engine::CLayer* m_pLayer = nullptr;
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	Engine::CColliderObject* m_pCollider = nullptr;
	_float m_fRadian = 0.f;
	_float m_fTickTime = 0.f;
};

END

#endif // !LogoObject_h__