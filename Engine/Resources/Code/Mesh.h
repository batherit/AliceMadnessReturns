#ifndef Mesh_h__
#define Mesh_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CMesh: public CResources
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh(void);

public:
	virtual void		Render_Meshes(LPD3DXEFFECT _pEffect = 0) {};

public:
	virtual CComponent*		Clone(void)PURE;
	virtual void			Free(void);

};

END
#endif // Mesh_h__
