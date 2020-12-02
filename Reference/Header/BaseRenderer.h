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
	virtual int Update(const _float & _fDeltaTime) PURE;
	virtual void	Render() PURE;
	void SetRenderID(RENDERID _eRenderID) { m_eRenderID = _eRenderID; }
	RENDERID GetRenderID() const { return m_eRenderID; }

protected:
	void RegisterToRenderer();

private:
	virtual CComponent*	Clone() PURE;
	virtual void Free(void) PURE;

protected:
	RENDERID m_eRenderID = RENDERID::RENDER_END;
	CRenderer* m_pRenderer = nullptr;
};

END
#endif // Renderer_h__