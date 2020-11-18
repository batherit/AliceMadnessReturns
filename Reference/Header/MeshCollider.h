#ifndef MeshCollider_h__
#define MeshCollider_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CMeshCollider : public CComponent
{
private:
	explicit CMeshCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMeshCollider(const CMeshCollider& rhs);
	virtual ~CMeshCollider(void);

public:
	const _vec3*		Get_Min(void) { return &m_vMin; }
	const _vec3*		Get_Max(void) { return &m_vMax; }
	const _matrix*		Get_CollMatrix(void) { return &m_matCollMatrix; }

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Ready(void) override;
	virtual int Update(const _float & _fDeltaTime) override;

	HRESULT		Ready_MeshCollider(const _vec3* pVtxPos, const _ulong& dwNumVtxCnt, const _ulong& dwStride);
	void		Render_MeshCollider(COLLTYPE eType, const _matrix* pMeshColliderMatrix);

	static const _tchar* GetComponentTag() { return L"MeshMeshCollider"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }
private:
	_vec3		m_vMin, m_vMax;
	_matrix		m_matCollMatrix;

#ifdef _DEBUG
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];

#endif

public:
	static CMeshCollider*			Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pVtxPos, const _ulong& dwNumVtxCnt, const _ulong& dwStride);
	virtual CComponent*			Clone(void);
	virtual void				Free(void);

	
};
END
#endif // MeshCollider_h__
