#ifndef PolygonRenderer_h__
#define PolygonRenderer_h__

#include "Engine_Define.h"
#include "BaseRenderer.h"

BEGIN(Engine)
class CVIBuffer;
class CTexture;
class ENGINE_DLL CPolygonRenderer : public CBaseRenderer
{
private:
	explicit CPolygonRenderer(void);
	explicit CPolygonRenderer(const CPolygonRenderer& rhs);
	virtual ~CPolygonRenderer(void);

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Ready(void) override;
	virtual _int Update(const _float & _fDeltaTime) override;
	virtual void Render(LPD3DXEFFECT _pEffect = 0, _uint _uiPassIndex = 0) override;

	static CPolygonRenderer* Create();
	virtual CComponent*	Clone();

	static const _tchar* GetComponentTag() { return L"PolygonRenderer"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

	void SetRenderInfo(RENDERID _eRenderID, CVIBuffer* _pBuffer, CTexture* _pTexture = nullptr, LPD3DXEFFECT _pEffect = nullptr);
	void SetTextureIndex(const _uint& _uiTextureIndex) { m_uiTextureIndex = _uiTextureIndex; }

	void SetWorldMatrix(const _matrix& _matWorld) { m_matWorld = _matWorld; }

private:
	virtual void Free(void);

protected:
	CVIBuffer* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	_uint m_uiTextureIndex = 0;
	D3DMATERIAL9 m_stMtrl;
	_matrix m_matWorld;
};

END
#endif // Renderer_h__
