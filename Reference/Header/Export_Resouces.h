#ifndef Export_Resouces_h__
#define Export_Resouces_h__

#include "ResourcesMgr.h"

BEGIN(Engine)

// ResourcesMgr
// Get
inline CComponent*	Clone(const _ushort& wContainerIdx, const _tchar* pResourcesTag);
// Set
// General

inline HRESULT		Reserve_ContainerSize(const _ushort& wSize);
inline HRESULT		Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev,	const _ushort& wContainerIdx,	const _tchar* pBufferTag,	BUFFERID eID,	_ulong dwCntX = 1,	_ulong dwCntZ = 1,	_ulong dwItv = 1);
inline HRESULT		Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pFilePath, const _uint& iCnt = 1);

inline void		Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag);
inline void		Render_Texture(const _ushort& wContainerIdx, const _tchar* pTextureTag, const _uint& iIndex);

// Release Resources
inline void Release_Resoures();

#include "Export_Resouces.inl"

END
#endif // Export_Resouces_h__
