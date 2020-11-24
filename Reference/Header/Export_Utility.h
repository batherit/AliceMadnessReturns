#ifndef Export_Utility_h__
#define Export_Utility_h__

// Manage
#include "Management.h"
#include "KeyMgr.h"
#include "ProtoMgr.h"
#include "InputMode.h"
#include "InputModeMgr.h"

#include "GameObject.h"
#include "Camera.h"

// Components
#include "Component.h"
#include "Transform.h"
#include "Renderer.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "Physics.h"
#include "Optimization.h"

BEGIN(Engine)

// management
// Get
//inline CComponent*	   Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline CLayer* GetLayer(const _tchar* _pLayerTag);

inline HRESULT Create_Management(CManagement** ppManagement);
//inline HRESULT	SetUp_Scene(CScene* pScene);
//inline _int	Update_Scene(const _float& fTimeDelta);
//inline void	Render_Scene(void);

// protomgr
inline HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pInstance);
inline CComponent*		Clone(const _tchar* pProtoTag);
inline CComponent*		GetOriProto(const _tchar* _pProtoTag);

// Renderer
inline CRenderer*		Get_Renderer(void);

inline void Release_Utility(void);

#include "Export_Utility.inl"
END

#endif // Export_Utility_h__
