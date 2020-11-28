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
	virtual int Update(const _float & _fDeltaTime) override;
	virtual void Render() override;

	static CMeshRenderer* Create();
	virtual CComponent*	Clone();

	static const _tchar* GetComponentTag() { return L"MeshRenderer"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

	void SetRenderInfo(RENDERID _eRenderID, CMesh* _pMesh);

private:
	virtual void Free(void);

private:
	CMesh* m_pMesh = nullptr;
};

END
#endif // Renderer_h__
