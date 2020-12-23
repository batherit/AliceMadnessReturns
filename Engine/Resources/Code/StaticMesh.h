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
	_vec3*		Get_VtxPos(void) { return m_pVtxPos; }
	_ulong		GetNumIndices() const { return m_dwNumIndices; }
	INDEX16*		GetIndices() const { return m_pIndices; }

public:
	HRESULT		Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	virtual void		Render_Meshes(LPD3DXEFFECT _pEffect = 0) override;

private:
	LPD3DXMESH			m_pOriMesh;	// ���� �ε� ������ �����ϴ� �޽� �İ�ü
	LPD3DXMESH			m_pMesh; // �븻 ������ �����Ͽ� ��ȯ��Ų �޽� �İ�ü

	LPD3DXBUFFER		m_pAdjacency; // ������ �̿��� ������ �����ϱ� ���� ù��° �ּҸ� �����Ϸ��� ��� ����
	LPD3DXBUFFER		m_pSubset; // ����� ���� == �ؽ����� ���� == ������ ����
	D3DXMATERIAL*		m_pMtrl;
	_ulong				m_dwSubsetCnt;

	_ulong				m_dwStride;
	_ulong				m_dwNumVtx;
	_vec3*				m_pVtxPos;
	_ulong				m_dwNumIndices;
	INDEX16*			m_pIndices;

	LPDIRECT3DTEXTURE9*	m_ppTextures;

public:
	static const _tchar* GetComponentTag() { return L"StaticMesh"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

	static CStaticMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent* Clone(void) override;
	virtual void Free(void) override;

};

END
#endif // StaticMesh_h__
