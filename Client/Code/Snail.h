#ifndef Snail_h__
#define Snail_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CSnail : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CSnail(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CSnail(const  CSnail& rhs);
	virtual ~CSnail(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CSnail*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CStaticMesh* GetDynamicMesh() const { return m_pMesh; }

private:
	void PopTooth(_int _iToothNum, _bool _bIsArrived = false);

private:
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
};

END

#endif // !LogoObject_h__