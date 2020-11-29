#ifndef InputMode_Map_h__
#define InputMode_Map_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CInputMode_Map : public Engine::CInputMode
{
public: // ������, �Ҹ���
	explicit  CInputMode_Map(Engine::CInputModeMgr* _pInputModeMgr);
	virtual ~CInputMode_Map(void);

public:
	// CInputMode��(��) ���� ��ӵ�
	virtual _int ProcessInput(const _float & _fDeltaTime) override;
};

END

#endif // !TerrainMap_h__
