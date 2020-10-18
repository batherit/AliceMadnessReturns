#pragma once
#ifndef Management_h__
#define Management_h__

#include "Engine_Define.h"
#include "Base.h"

#include "CSceneMgr.h"
#include "Component.h"
#include "CGameObject.h"
#include "MoveComponent.h"


BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement(void);
	virtual ~CManagement(void);

public:
	CComponent * Get_Component(const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID);

public:
	// ¾À °ü·Ã
	_bool ConfirmValidScene();
	void SetNextScene(CScene* _pNextScene);
	void RequestSceneInit();
	void UpdateScene(const _float& _fDeltaTime);
	void RenderScene(void);

public:
	CSceneMgr* GetSceneMgr() const { return m_pSceneMgr; }

private:
	CSceneMgr* m_pSceneMgr = nullptr;

public:
	virtual void Free(void);

};

END
#endif // GraphicDev_h__

