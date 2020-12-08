#ifndef InputProcessor_Collider_h__
#define InputProcessor_Collider_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

class CColliderTab;
BEGIN(Client)

class CInputProcessor_Collider : public Engine::CInputProcessor
{
public: // ������, �Ҹ���
	explicit  CInputProcessor_Collider(Engine::CInputProcessorMgr* _pInputProcessorMgr);
	virtual ~CInputProcessor_Collider(void);

public:
	// CInputProcessor��(��) ���� ��ӵ�
	virtual _int ProcessInput(const _float & _fDeltaTime) override;

private:
	CEditScene* m_pEditScene = nullptr;
	CColliderTab* m_pColliderTab = nullptr;
};

END

#endif // !TerrainCollider_h__