#ifndef CubeCol_h__
#define CubeCol_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeCol : public CVIBuffer
{
protected:
	explicit CCubeCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeCol(const CCubeCol& rhs);
	virtual ~CCubeCol(void);

public:
	virtual HRESULT	Ready_Buffer(void) override;
	virtual void	Render_Buffer(void) override;
	static const _tchar* GetComponentTag() { return L"CubeCol"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

public:
	static CCubeCol*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void);

private:
	LPD3DXMESH m_pCubeMesh = nullptr;
	LPD3DXMESH m_pClone = nullptr;
};
END

#endif // TriCol_h__