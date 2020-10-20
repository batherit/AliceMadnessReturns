#ifndef GameObject_h__
#define	GameObject_h__

#include "Engine_Define.h"
#include "Base.h"


BEGIN(Engine)
class CComponent;

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9	pGraphicDev);	// 老馆 积己磊
	explicit CGameObject(const CGameObject& rhs);			// 汗荤 积己磊
	virtual ~CGameObject();

public:
	CComponent* Get_Component(const _tchar* pComponentTag, COMPONENTID eID);

public:
	virtual HRESULT Ready_Object(void) = 0;
	virtual _int Update_Object(const _float& _fDeltaTime) = 0;
	virtual void Render_Object(void) {};

public:
	CComponent* GetComponent(const COMPONENTID eComponentID, const _tchar* pComponentTag);
	void		AddComponent(const COMPONENTID eComponentID, const _tchar* pComponentTag);
	template <class T>
	T* GetComponent() {
		const _tchar* tag = T::GetComponentTag();
		const COMPONENTID id = T::GetComponentID();
		return static_cast<T*>(GetComponent(id, tag));
	}
	template <class T>
	void AddComponent() {
		const _tchar* tag = T::GetComponentTag();
		const COMPONENTID id = T::GetComponentID();
		AddComponent(id, tag);
	}
	void SetParent(CGameObject* _pParent) { m_pParent = _pParent; }
	CGameObject* GetParent() const { return m_pParent; }

	virtual void Free(void);

private:
	CComponent*		Find_Component(const _tchar* pComponentTag, COMPONENTID eID);

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	CGameObject* m_pParent = nullptr;
	map<const _tchar*, CComponent*>			m_mapComponent[ID_END];
};

END

#endif


/*
template<typename T>
void AddComponent() {
	Engine::COMPONENTID eComponentID = T::GetComponentID();
	auto iter = find_if(m_mapComponent[eComponentID].begin(), m_mapComponent[eComponentID].end(), CTag_Finder(T::GetComponentTag()));
	if (iter != m_mapComponent[eComponentID].end()) return;
	m_mapComponent[eComponentID].emplace(T::GetComponentTag(), T::Create());
}

template<typename T>
T* GetComponent() {
	Engine::COMPONENTID eComponentID = T::GetComponentID();
	auto iter = find_if(m_mapComponent[eComponentID].begin(), m_mapComponent[eComponentID].end(), CTag_Finder(T::GetComponentTag()));
	if (iter == m_mapComponent[eComponentID].end()) return nullptr;
	return static_cast<T*>(m_mapComponent[eComponentID][T::GetComponentTag()]);
}
*/