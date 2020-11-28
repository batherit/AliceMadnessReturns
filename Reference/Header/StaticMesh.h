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
	const _ulong&		Get_NumVtx(void) { return m_dwNumVtx; }
	const _ulong&		Get_Stride(void) { return m_dwStride; }
	const _vec3*		Get_VtxPos(void) { return m_pVtxPos; }

public:
	HRESULT		Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	virtual void		Render_Meshes(void) override;

private:
	LPD3DXMESH			m_pOriMesh;	// 최초 로드 시점에 생성하는 메쉬 컴객체
	LPD3DXMESH			m_pMesh; // 노말 정보를 삽입하여 변환시킨 메쉬 컴객체

	LPD3DXBUFFER		m_pAdjacency; // 인접한 이웃의 정보를 접근하기 위해 첫번째 주소를 보관하려는 멤버 변수
	LPD3DXBUFFER		m_pSubset; // 서브셋 개수 == 텍스쳐의 개수 == 재질의 개수
	D3DXMATERIAL*		m_pMtrl;
	_ulong				m_dwSubsetCnt;

	_ulong				m_dwStride;
	_ulong				m_dwNumVtx;
	_vec3*				m_pVtxPos;

	LPDIRECT3DTEXTURE9*	m_ppTextures;

public:
	static CStaticMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent* Clone(void) override;
	virtual void Free(void) override;

};

END
#endif // StaticMesh_h__
