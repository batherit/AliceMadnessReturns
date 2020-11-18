#include "Export_Resouces.h"
// ResourcesMgr
// Get
CComponent*	Clone(const _ushort& wContainerIdx, const _tchar* pResourcesTag) {
	return CResourcesMgr::GetInstance()->Clone(wContainerIdx, pResourcesTag);
}

CComponent* GetOriResource(const _ushort& _wContainerIdx, const _tchar* _pResourcesTag) {
	return CResourcesMgr::GetInstance()->GetOriResource(_wContainerIdx, _pResourcesTag);
}
// Set
// General

HRESULT		Reserve_ContainerSize(const _ushort& wSize)
{
	return CResourcesMgr::GetInstance()->Reserve_ContainerSize(wSize);
}
HRESULT		Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pBufferTag, BUFFERID eID, _ulong dwCntX, _ulong dwCntZ, _ulong dwItv)
{
	return CResourcesMgr::GetInstance()->Ready_Buffer(pGraphicDev, wContainerIdx, pBufferTag, eID, dwCntX, dwCntZ, dwItv);
}
inline HRESULT Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const _tchar * pTextureTag, TEXTURETYPE eType, const _tchar * pFilePath, const _uint & iCnt)
{
	return CResourcesMgr::GetInstance()->Ready_Texture(pGraphicDev, wContainerIdx, pTextureTag, eType, pFilePath, iCnt);
}
inline HRESULT Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const _tchar * pMeshTag, MESHTYPE eType, const _tchar * pFilePath, const _tchar * pFileName)
{
	return CResourcesMgr::GetInstance()->Ready_Meshes(pGraphicDev, wContainerIdx, pMeshTag, eType, pFilePath, pFileName);
}
void		Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag)
{
	CResourcesMgr::GetInstance()->Render_Buffer(wContainerIdx, pBufferTag);
}

inline void Render_Texture(const _ushort & wContainerIdx, const _tchar * pTextureTag, const _uint & iIndex)
{
	CResourcesMgr::GetInstance()->Render_Texture(wContainerIdx, pTextureTag, iIndex);
}

// Release Resources
void Release_Resoures()
{
	CResourcesMgr::GetInstance()->DestroyInstance();
}
