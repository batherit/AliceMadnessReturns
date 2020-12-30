#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
	typedef list<CGameObject*>	LayerList;
#define OBJECTLIST_TCHAR	L"ObjectList"

private:
	explicit CLayer(void);
	virtual ~CLayer();

public:
	HRESULT		Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);
	HRESULT		Add_GameObject(CGameObject* _pGameObject);

	LayerList&	GetLayerList(const _tchar* _pObjTag);
	LayerList&	GetLayerList();

	HRESULT		Ready_Layer(void);
	_int		Update_Layer(const _float& fTimeDelta);

private:
	void CollectInvalidObjects();

private:
	map<const _tchar*, LayerList>		m_mapObjectList;

public:
	static CLayer*		Create(void);
	virtual void		Free(void);
};

END
#endif // Layer_h__
