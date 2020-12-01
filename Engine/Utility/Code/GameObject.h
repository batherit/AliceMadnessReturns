#ifndef GameObject_h__
#define	GameObject_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)
class CComponent;
class CTransform;

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9	pGraphicDev);	// 일반 생성자
	explicit CGameObject(const CGameObject& rhs);			// 복사 생성자
	virtual ~CGameObject();

public:
	//CComponent* Get_Component(const _tchar* pComponentTag, COMPONENTID eID);
	CTransform* GetTransform() const { return m_pTransform; }

public:
	virtual HRESULT Ready_Object(void) = 0;
	virtual _int Update_Object(const _float& _fDeltaTime) = 0;
	virtual void Render_Object(void) {};

public:
	LPDIRECT3DDEVICE9 GetGraphicDev() const { return m_pGraphicDev; }

	void SetValid(_bool _bIsValid) { m_bIsValid = _bIsValid; }
	_bool IsValid() const { return m_bIsValid; }

	CComponent* GetComponent(const COMPONENTID eComponentID, const _tchar* pComponentTag);
	CComponent*	AddComponent(const COMPONENTID eComponentID, const _tchar* pComponentTag);
	template <class T>
	T* GetComponent() {
		const _tchar* tag = T::GetComponentTag();
		const COMPONENTID id = T::GetComponentID();
		return static_cast<T*>(GetComponent(id, tag));
	}
	template <class T>
	T* AddComponent() {
		const _tchar* tag = T::GetComponentTag();
		const COMPONENTID id = T::GetComponentID();
		return static_cast<T*>(AddComponent(id, tag));
	}
	_bool SetParent(CGameObject* _pParent, const char* _pBoneName = nullptr);
	CGameObject* GetParent() const { return m_pParent; }

	// 파일 입출력 관련
	virtual void SaveInfo(FILE* _fpOut) {};
	virtual void LoadInfo(FILE* _fpIn) {};
	virtual void SaveInfo(HANDLE& _hfOut) {};
	virtual void LoadInfo(HANDLE& _hfIn) {};

	virtual void Free(void);

private:
	CComponent*		Find_Component(const _tchar* pComponentTag, COMPONENTID eID);

protected:
	_bool m_bIsValid = true;
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	CGameObject* m_pParent = nullptr;
	map<const _tchar*, CComponent*>			m_mapComponent[ID_END];
	CTransform* m_pTransform = nullptr;
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