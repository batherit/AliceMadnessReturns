#ifndef Trigger_h__
#define Trigger_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CTrigger : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CTrigger(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CTrigger(const  CTrigger& rhs);
	virtual ~CTrigger(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CTrigger*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

	virtual _bool SaveInfo(HANDLE& _hfOut) override;
	virtual _bool LoadInfo(HANDLE& _hfIn) override;


public:
	void SetTriggerType(TRIGGER::E_TYPE _eTriggerType) { m_eTriggerType = _eTriggerType; }
	TRIGGER::E_TYPE GetTriggerType() const { return m_eTriggerType; }

	void SetSortingOrderIndex(_int _iSortingOrderIndex) { m_iSortingOrderIndex = _iSortingOrderIndex; }
	_int GetSortingOrderIndex() const { return m_iSortingOrderIndex; }

	void SetStageIndex(_int _iStageIndex) { m_iStageIndex = _iStageIndex; }
	_int GetStageIndex() const { return m_iStageIndex; }

	void SetSpawnIndex(_int _iSpawnerIndex) { m_iSpawnIndex = _iSpawnerIndex; }
	_int GetSpawnIndex() const { return m_iSpawnIndex; }

	void SetMonsterTag(const _tchar* _tcMonsterTag) { lstrcpy(m_tcMonsterTag, _tcMonsterTag); }
	const _tchar* GetMonsterTag() const { return m_tcMonsterTag; }

public:
	void AddSpawner(CTrigger* _pSpawner) { m_vecSpawners.emplace_back(_pSpawner); }
	void Spawn();

private:
	TRIGGER::E_TYPE m_eTriggerType = TRIGGER::TYPE_END;
	_int m_iSortingOrderIndex = 0;
	_int m_iStageIndex = -1;
	_int m_iSpawnIndex = -1;
	_tchar m_tcMonsterTag[50] = L"NULL";
	
	vector<CTrigger*> m_vecSpawners;
};

END

#endif // !LogoObject_h__
