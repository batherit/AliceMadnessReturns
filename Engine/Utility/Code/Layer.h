#ifndef Layer_h__
#define Layer_h__

#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer(void);
	virtual ~CLayer();

public:
	CComponent*	   Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);

public:
	HRESULT		Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);
	HRESULT		Ready_Layer(void);
	_int		Update_Layer(const _float& fTimeDelta);
	void		Render_Layer(void);

private:
	map<const _tchar*, CGameObject*>			m_mapObject;

public:
	static CLayer*		Create(void);
	virtual void		Free(void);
};

END
#endif // Layer_h__
