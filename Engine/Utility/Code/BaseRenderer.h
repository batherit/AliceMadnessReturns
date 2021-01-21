#ifndef BaseRenderer_h__
#define BaseRenderer_h__

#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class CRenderer;
class ENGINE_DLL CBaseRenderer abstract : public CComponent 
{
protected:
	explicit CBaseRenderer(void);
	virtual ~CBaseRenderer(void);

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Ready(void) PURE;
	virtual _int Update(const _float & _fDeltaTime) PURE;
	virtual void	Render(LPD3DXEFFECT _pEffect = 0, _uint _uiPassIndex = 0) PURE;
	void SetRenderID(RENDERID _eRenderID) { m_eRenderID = _eRenderID; }
	RENDERID GetRenderID() const { return m_eRenderID; };

public:
	void SetWorldMatrix(const _matrix& _matWorld) { m_matWorld = _matWorld; }
	void SetViewMatrix(const _matrix& _matView) { m_matView = _matView; m_bIsViewCommited = false; }
	void SetProjMatirx(const _matrix& _matProj) { m_matProj = _matProj; m_bIsProjCommited = false; }

protected:
	void RegisterToRenderer();

private:
	virtual CComponent*	Clone() PURE;
	virtual void Free(void) PURE;

protected:
	RENDERID m_eRenderID = RENDERID::RENDER_END;
	CRenderer* m_pRenderer = nullptr;

	_matrix m_matWorld;
	_bool m_bIsViewCommited = true;
	_matrix m_matView;
	_bool m_bIsProjCommited = true;
	_matrix m_matProj;
};

END
#endif // Renderer_h__