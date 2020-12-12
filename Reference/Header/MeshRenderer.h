#ifndef MeshRenderer_h__
#define MeshRenderer_h__

#include "Engine_Define.h"
#include "BaseRenderer.h"

BEGIN(Engine)
class CMesh;
class ENGINE_DLL CMeshRenderer : public CBaseRenderer
{
private:
	explicit CMeshRenderer(void);
	explicit CMeshRenderer(const CMeshRenderer& rhs);
	virtual ~CMeshRenderer(void);

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Ready(void) override;
	virtual _int Update(const _float & _fDeltaTime) override;
	virtual void Render(LPD3DXEFFECT _pEffect = 0, _uint _uiPassIndex = 0) override;

	static CMeshRenderer* Create();
	virtual CComponent*	Clone();

	static const _tchar* GetComponentTag() { return L"MeshRenderer"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

	void SetRenderInfo(RENDERID _eRenderID, CMesh* _pMesh);
	void SetWorldMatrix(const _matrix& _matWorld) { m_matWorld = _matWorld; }

private:
	virtual void Free(void);

private:
	CMesh* m_pMesh = nullptr;
	_matrix m_matWorld;
};

END
#endif // Renderer_h__
