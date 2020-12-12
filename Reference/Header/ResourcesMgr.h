#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Engine_Define.h"
#include "Base.h"

#include "TriCol.h"
#include "CRcCol.h"
#include "SphereCol.h"
#include "ManualCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "CubeCol.h"

#include "Texture.h"

#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"

BEGIN(Engine)

class ENGINE_DLL CResourcesMgr : public CBase 
{
	DECLARE_SINGLETON(CResourcesMgr)

private:
	explicit CResourcesMgr(void);
	virtual ~CResourcesMgr(void);

public:
	HRESULT		Reserve_ContainerSize(const _ushort& wSize);
	HRESULT		Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev,
							const _ushort& wContainerIdx,
							const _tchar* pBufferTag, 
							BUFFERID eID, 
							_ulong dwCntX = 1, 
							_ulong dwCntZ = 1, 
							_ulong dwItv = 1);

	HRESULT		Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
							const _ushort& wContainerIdx,
							const _tchar* pTextureTag,
							TEXTURETYPE eType, 
							const _tchar* pFilePath, 
							const _uint& iCnt = 0);

	HRESULT		Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev,
							const _ushort& wContainerIdx,
							const _tchar* pMeshTag,
							MESHTYPE eType,
							const _tchar* pFilePath,
							const _tchar* pFileName);


	void		Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag);
	void		Render_Texture(const _ushort& wContainerIdx, const _tchar* pTextureTag, const _uint& iIndex);
	CComponent*	Clone(const _ushort& wContainerIdx, const _tchar* pResourcesTag);
	CComponent* GetOriResource(const _ushort& _wContainerIdx, const _tchar* _pResourcesTag);

private:
	CResources*		Find_Resources(const _ushort& wContainerIdx, const _tchar* pResourcesTag);
	
private:
	map<const _tchar*, CResources*>*		m_pmapResource;
	_ushort									m_wSize = 0;

public:
	virtual void Free(void);
};


END
#endif // ResourcesMgr_h__
