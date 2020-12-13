#ifndef ColliderObject_h__
#define	ColliderObject_h__

#include "Engine_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CColliderObject abstract : public CGameObject
{
protected:
	explicit CColliderObject(LPDIRECT3DDEVICE9	pGraphicDev);	// 일반 생성자
	explicit CColliderObject(const CColliderObject& rhs);			// 복사 생성자
	virtual ~CColliderObject();

public:
	virtual HRESULT Ready_Object(void) = 0;
	virtual _int Update_Object(const _float& _fDeltaTime);
	virtual void Render_Object(void) {};

public:
	// 파일 입출력 관련
	virtual _bool SaveInfo(FILE* _fpOut) = 0;
	virtual _bool LoadInfo(FILE* _fpIn) = 0;
	virtual _bool SaveInfo(HANDLE& _hfOut) = 0;
	virtual _bool LoadInfo(HANDLE& _hfIn) = 0;

	virtual void Free(void) override;

public:
	void SetColliderTag(const _tchar* _pColliderTag) { lstrcpy(m_tcColliderTag, _pColliderTag); }
	void SetColliderType(const E_COLLIDER_TYPE& _eColliderType) { m_eColliderType = _eColliderType; }
	
	const _tchar* GetColliderTag() const { return m_tcColliderTag; }
	E_COLLIDER_TYPE GetColliderType() const { return m_eColliderType; }

protected:
	_tchar m_tcColliderTag[MAX_PATH] = L"NULL";
	E_COLLIDER_TYPE m_eColliderType = TYPE_END;
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