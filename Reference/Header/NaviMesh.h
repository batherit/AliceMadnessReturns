#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Base.h"
#include "Mesh.h"
#include "Cell.h"

BEGIN(Engine)

class ENGINE_DLL CNaviMesh : public CMesh
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

public:
	void		Set_NaviIndex(const _ulong& dwIndex) { m_dwIndex = dwIndex; }

public:
	HRESULT		Ready_NaviMeshes(void);
	void		Render_NaviMeshes(void);
	_vec3		Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);
	_bool		LoadNaviMeshFromFile(const _tchar* _pFilePath);

private:
	HRESULT		AddCell(const _vec3& _vV1, const _vec3& _vV2, const _vec3& _vV3);
	HRESULT		Link_Cell(void);

private:
	vector<CCell*>			m_vecCell;
	_ulong					m_dwIndex;

public:
	static CNaviMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);

};

END
#endif // NaviMesh_h__
