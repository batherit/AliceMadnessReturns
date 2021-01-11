#ifndef BunnyBomb_h__
#define BunnyBomb_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CMap;
class CUI_BunnyBomb;
class CBunnyBomb : public Engine::CGameObject
{
#define BOOM_TIME 20.f

private: // 생성자, 소멸자
	explicit  CBunnyBomb(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CBunnyBomb(const  CBunnyBomb& rhs);
	virtual ~CBunnyBomb(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);

public:
	static CBunnyBomb*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CDynamicMesh* GetDynamicMesh() const { return m_pMesh; }
	Engine::CPhysics* GetPhysics() const { return m_pPhysics; }
	_bool IsFalling(const _float& _fDeltaTime);
	_bool IsLanded() const { return m_bIsLanded; }
	void Bomb();

private:
	CMap* m_pMap = nullptr;
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	Engine::CColliderObject* m_pCollider = nullptr;

	_int m_iCellIndex = -1;
	_bool m_bIsLanded = false;
	_float m_fElapsedTime = 0.f;

	CUI_BunnyBomb* m_pBunnyBombUI = nullptr;
};

END

#endif // !LogoObject_h__