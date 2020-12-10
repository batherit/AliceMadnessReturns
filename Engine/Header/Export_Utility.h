#ifndef Export_Utility_h__
#define Export_Utility_h__

// Manage
#include "Management.h"
#include "KeyMgr.h"
#include "ProtoMgr.h"
#include "InputProcessor.h"
#include "InputProcessorMgr.h"
#include "LightMgr.h"

// Object
#include "GameObject.h"
#include "ColliderObject_Sphere.h"
//#include "ColliderObject_Box.h"
#include "Camera.h"

// Components
#include "Component.h"
#include "Transform.h"
#include "Renderer.h"
#include "PolygonRenderer.h"
#include "MeshRenderer.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "Physics.h"
#include "Optimization.h"
#include "Shader.h"

BEGIN(Engine)

// management
// Get
//inline CComponent*	   Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline CLayer* GetLayer(const _tchar* _pLayerTag);

//inline HRESULT Create_Management(CManagement** ppManagement, LPDIRECT3DDEVICE9& pGraphicDev);
//inline HRESULT	SetUp_Scene(CScene* pScene);
//inline _int	Update_Scene(const _float& fTimeDelta);
//inline void	Render_Scene(void);

// protomgr
inline HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pInstance);
inline CComponent*		Clone(const _tchar* pProtoTag);
inline CComponent*		GetOriProto(const _tchar* _pProtoTag);

// LightMgr
inline 	const D3DLIGHT9*		Get_Light(const _uint& iIndex);
inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);

// Renderer
inline CRenderer*		Get_Renderer(void);

inline void Release_Utility(void);

#include "Export_Utility.inl"
END

#endif // Export_Utility_h__
