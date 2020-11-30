#ifndef InputProcessor_Navi_h__
#define InputProcessor_Navi_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

class CNaviMeshTab;
BEGIN(Client)
class CEditScene;
class CInputProcessor_Navi : public Engine::CInputMode
{
public: // 생성자, 소멸자
	explicit  CInputProcessor_Navi(Engine::CInputModeMgr* _pInputProcessorMgr);
	virtual ~CInputProcessor_Navi(void);

public:
	virtual _int ProcessInput(const _float & _fDeltaTime) override;

public:
	// InputProcessor가 사용할 함수들
	NAVIMESH_TAB::E_PICKMODE GetPickModeFromTab() const;
	void PickTerrain();
	void PickVertex();
	void ClearPickedVertices();
	void PickTriangle();
	void DragVertex();

private:
	// 입력 처리자는 처리된 내용을 UI에 반영하기 위하여 Tab 포인터를 지닌다.
	CEditScene* m_pEditScene = nullptr;
	CNaviMeshTab* m_pNaviMeshTab = nullptr;
	vector<_vec3> m_vecTriangle;
};

END

#endif // !NaviMap_h__
