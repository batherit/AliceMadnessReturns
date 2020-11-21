#ifndef InputMode_Terrain_h__
#define InputMode_Terrain_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CInputMode_Terrain : public Engine::CInputMode
{
public: // ������, �Ҹ���
	explicit  CInputMode_Terrain(Engine::CInputModeMgr* _pInputModeMgr);
	virtual ~CInputMode_Terrain(void);

public:
	// CInputMode��(��) ���� ��ӵ�
	virtual _int ProcessInput(const _float & _fDeltaTime) override;

};

END

#endif // !TerrainMap_h__
