#ifndef InputProcessor_Terrain_h__
#define InputProcessor_Terrain_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CInputProcessor_Terrain : public Engine::CInputProcessor
{
public: // 생성자, 소멸자
	explicit  CInputProcessor_Terrain(Engine::CInputProcessorMgr* _pInputProcessorMgr);
	virtual ~CInputProcessor_Terrain(void);

public:
	// CInputProcessor을(를) 통해 상속됨
	virtual _int ProcessInput(const _float & _fDeltaTime) override;

};

END

#endif // !TerrainMap_h__
