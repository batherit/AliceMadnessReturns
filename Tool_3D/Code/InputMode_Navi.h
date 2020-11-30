#ifndef InputMode_Navi_h__
#define InputMode_Navi_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CNaviMeshInputProcessor;
class CInputMode_Navi : public Engine::CInputMode
{
public: // ������, �Ҹ���
	explicit  CInputMode_Navi(Engine::CInputModeMgr* _pInputModeMgr);
	virtual ~CInputMode_Navi(void);

public:
	// CInputMode��(��) ���� ��ӵ�
	virtual _int ProcessInput(const _float & _fDeltaTime) override;

private:
	CEditScene* m_pEditScene = nullptr;
	vector<_vec3> m_vecTriangle;
	CNaviMeshInputProcessor* m_pNaviMeshInputProcessor = nullptr;
};

END

#endif // !NaviMap_h__
