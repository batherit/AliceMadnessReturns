#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer(void);
	virtual ~CRenderer(void);

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Ready(void) override;
	virtual int Update(const _float & _fDeltaTime) override;
	void	Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject);
	void	Render_GameObject(void);
	void	Clear_RenderGroup(void);
	static const _tchar* GetComponentTag() { return L"Renderer"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

private:
	list<CGameObject*>				m_RenderGroup[RENDER_END];

private:
	virtual CComponent*	Clone() { 
		CRenderer* pRenderer = GetInstance();
		Safe_AddRef(pRenderer);
		return pRenderer; }
	virtual void Free(void);

	
};

END
#endif // Renderer_h__
