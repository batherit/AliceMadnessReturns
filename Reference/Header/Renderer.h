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
	void	Render_GameObject(LPDIRECT3DDEVICE9 & pGraphicDev);
	void	Clear_RenderGroup(void);
	HRESULT	Ready_Renderer(LPDIRECT3DDEVICE9& pGraphicDev);

	static const _tchar* GetComponentTag() { return L"Renderer"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

private:
	void		Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_UI(LPDIRECT3DDEVICE9& pGraphicDev);

	void		Render_Deferred(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_LightAcc(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Blend(LPDIRECT3DDEVICE9& pGraphicDev);

private:
	list<CGameObject*>				m_RenderGroup[RENDER_END];
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;

private:
	virtual CComponent*	Clone() { 
		CRenderer* pRenderer = GetInstance();
		Safe_AddRef(pRenderer);
		return pRenderer; }
	virtual void Free(void);
};

END
#endif // Renderer_h__
