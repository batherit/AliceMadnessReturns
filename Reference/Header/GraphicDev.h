#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public CBase
{
	DECLARE_SINGLETON(CGraphicDev)

private:
	explicit CGraphicDev(void);
	virtual ~CGraphicDev(void);

public:
	LPDIRECT3DDEVICE9	Get_GraphicDev(void) { return m_pGraphicDev; }
	LPD3DXSPRITE		GetSprite(void) { return m_pSprite; }
	//LPD3DXFONT			Get_Font(void) { return m_pFont; }

public:
	HRESULT		Ready_GraphicDev(HWND hWnd, 
								 WINMODE eMode,
								 const _uint& iSizeX,
								 const _uint& iSizeY, 
								 CGraphicDev** ppGraphicDev);

	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End(void);

	/*void RenderText(const wstring& _wstrText, const _vec3& _vPos, const _vec3& _vScale =  _vec3(1.f, 1.f, 1.f), const _vec3& _vRot = _vec3(0.f, 0.f, 0.f), D3DXCOLOR _clRenderColor = D3DCOLOR_ARGB(255, 255, 255, 255));*/
	void RenderSprite(LPDIRECT3DTEXTURE9 _texture,
		const _vec3& _position,
		const _vec3& _scale,
		const _vec3& _angle,
		const D3DCOLOR& _color);

private:
	LPDIRECT3D9			m_pSDK;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPD3DXSPRITE		m_pSprite;
	//LPD3DXFONT			m_pFont;
public:
	virtual void Free(void);

};

END
#endif // GraphicDev_h__
