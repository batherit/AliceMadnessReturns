#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

public:
	enum E_FOG_TYPE { FOG_NONE, FOG_SPHERE, FOG_HEIGHT, FOG_ALL, FOG_END};
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
	
public:
	void SetFogType(const E_FOG_TYPE& _eFogType) { m_eFogType = _eFogType; }
	void SetSphereFogInfo(_float _fFogStart, _float _fFogEnd, const _vec3& _vFogColor, _float _fFogSphereDensity) {
		m_fFogStart = _fFogStart;
		m_fFogEnd = _fFogEnd;
		m_vFogSphereColor = _vFogColor;
		m_fFogSphereDensity = _fFogSphereDensity;
	}
	void SetHeightFogInfo(_float _fFogHeightStart, _float _fFogHeightEnd, const _vec3& _vFogColor, _float _fFogHeightDensity) {
		m_fFogHeightStart = _fFogHeightStart;
		m_fFogHeightEnd = _fFogHeightEnd;
		m_vFogHeightColor = _vFogColor;
		m_fFogHeightDensity = _fFogHeightDensity;
	}

private:
	void		Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	//void		Render_NonAlpha_Deferred(LPDIRECT3DDEVICE9& pGraphicDev);
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

	E_FOG_TYPE m_eFogType = FOG_NONE;
	_float m_fFogEnd;
	_float m_fFogStart;
	_vec3 m_vFogSphereColor;
	_float m_fFogSphereDensity;
	_float m_fFogHeightEnd;
	_float m_fFogHeightStart;
	_vec3 m_vFogHeightColor;
	_float m_fFogHeightDensity;

private:
	virtual CComponent*	Clone() { 
		CRenderer* pRenderer = GetInstance();
		Safe_AddRef(pRenderer);
		return pRenderer; }
	virtual void Free(void);
};

END
#endif // Renderer_h__
