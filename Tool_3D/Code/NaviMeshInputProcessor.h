#ifndef NaviMeshInputProcessor_h__
#define NaviMeshInputProcessor_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"


class CNaviMeshTab;
BEGIN(Client)

class CNaviMeshInputProcessor : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CNaviMeshInputProcessor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CNaviMeshInputProcessor(const  CNaviMeshInputProcessor& rhs);
	virtual ~CNaviMeshInputProcessor(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;

public:
	static CNaviMeshInputProcessor*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	// InputMode에서 사용되는 함수이다.
	void Click();
	void Drag(const _float _fDeltaTime);
	void Idle(const _float _fDeltaTime);

	// TODO : NeviMeshTab에서 사용되는 함수 구현
	void SetPickMode(NAVIMESH_TAB::E_PICKMODE _ePickMode);

public:
	NAVIMESH_TAB::E_PICKMODE GetPickMode() const;

private:
	void PickTerrain();

private:
	// 입력 처리자는 처리된 내용을 UI에 반영하기 위하여 Tab 포인터를 지닌다.
	CNaviMeshTab* m_pNaviMeshTab = nullptr;
};

END

#endif // !NaviMeshInputProcessorMap_h__
