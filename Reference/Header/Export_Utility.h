#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "KeyMgr.h"
#include "ProtoMgr.h"

#include "GameObject.h"
#include "Camera.h"

#include "Component.h"
#include "Transform.h"
#include "Renderer.h"
#include "SphereCollider.h"

BEGIN(Engine)

// management
// Get
//inline CComponent*	   Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);

inline HRESULT Create_Management(CManagement** ppManagement);
//inline HRESULT	SetUp_Scene(CScene* pScene);
//inline _int	Update_Scene(const _float& fTimeDelta);
//inline void	Render_Scene(void);

// protomgr
inline HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pInstance);
inline CComponent*		Clone(const _tchar* pProtoTag);

// Renderer
inline CRenderer*		Get_Renderer(void);

inline void Release_Utility(void);

#include "Export_Utility.inl"
END

#endif // Export_Utility_h__
