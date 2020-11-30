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
public: // ������, �Ҹ���
	explicit  CInputProcessor_Navi(Engine::CInputModeMgr* _pInputProcessorMgr);
	virtual ~CInputProcessor_Navi(void);

public:
	virtual _int ProcessInput(const _float & _fDeltaTime) override;

public:
	// InputProcessor�� ����� �Լ���
	NAVIMESH_TAB::E_PICKMODE GetPickModeFromTab() const;
	void PickTerrain();
	void PickVertex();
	void ClearPickedVertices();
	void PickTriangle();
	void DragVertex();

private:
	// �Է� ó���ڴ� ó���� ������ UI�� �ݿ��ϱ� ���Ͽ� Tab �����͸� ���Ѵ�.
	CEditScene* m_pEditScene = nullptr;
	CNaviMeshTab* m_pNaviMeshTab = nullptr;
	vector<_vec3> m_vecTriangle;
};

END

#endif // !NaviMap_h__
