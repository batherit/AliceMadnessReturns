#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	HRESULT		Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void		Render_Meshes(void);

private:
	LPD3DXMESH			m_pMesh;
	LPD3DXBUFFER		m_pAdjacency; // 인접한 이웃의 정보를 접근하기 위해 첫번째 주소를 보관하려는 멤버 변수
	LPD3DXBUFFER		m_pSubset; // 서브셋 개수 == 텍스쳐의 개수 == 재질의 개수
	D3DXMATERIAL*		m_pMtrl;
	_ulong				m_dwSubsetCnt;

	LPDIRECT3DTEXTURE9*	m_ppTextures;

public:
	static CStaticMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent* Clone(void) override;
	virtual void Free(void) override;

};

END
#endif // StaticMesh_h__
