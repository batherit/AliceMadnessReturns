#ifndef JumpPad_h__
#define JumpPad_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAttribute;
class CJumpPad : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CJumpPad(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CJumpPad(const  CJumpPad& rhs);
	virtual ~CJumpPad(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CJumpPad*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CDynamicMesh* GetDynamicMesh() const { return m_pMesh; }

private:
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	CAttribute* m_pAttribute = nullptr;

	_bool m_bIsLaunched = false;
};

END

#endif // !LogoObject_h__