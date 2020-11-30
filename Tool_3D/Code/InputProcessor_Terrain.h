#ifndef InputProcessor_Terrain_h__
#define InputProcessor_Terrain_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CInputProcessor_Terrain : public Engine::CInputProcessor
{
public: // ������, �Ҹ���
	explicit  CInputProcessor_Terrain(Engine::CInputProcessorMgr* _pInputProcessorMgr);
	virtual ~CInputProcessor_Terrain(void);

public:
	// CInputProcessor��(��) ���� ��ӵ�
	virtual _int ProcessInput(const _float & _fDeltaTime) override;

};

END

#endif // !TerrainMap_h__
