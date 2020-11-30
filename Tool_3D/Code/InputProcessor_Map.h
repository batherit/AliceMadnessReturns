#ifndef InputProcessor_Map_h__
#define InputProcessor_Map_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CInputProcessor_Map : public Engine::CInputProcessor
{
public: // ������, �Ҹ���
	explicit  CInputProcessor_Map(Engine::CInputProcessorMgr* _pInputProcessorMgr);
	virtual ~CInputProcessor_Map(void);

public:
	// CInputProcessor��(��) ���� ��ӵ�
	virtual _int ProcessInput(const _float & _fDeltaTime) override;
};

END

#endif // !TerrainMap_h__
