#ifndef Scene_h__
#define Scene_h__


#include "Engine_Define.h"
#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene abstract : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);	// 老馆 积己磊
	explicit CScene(const CScene& rhs);			// 汗荤 积己磊
	virtual ~CScene();

public:
	//virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) { return 0; }
	//CComponent*	   Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CLayer* GetLayer(const _tchar* pLayerTag) const;
	void CollectInvalidObjects();

public:
	virtual void ResetScene(void) = 0;
	virtual HRESULT Ready(void) = 0;
	virtual _int Update(const _float& _fDeltaTime);
	virtual void Render(void) = 0;

public:
	virtual void Free(void);

protected:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	map<const _tchar*, CLayer*>		m_mapLayer;

};

END

#endif