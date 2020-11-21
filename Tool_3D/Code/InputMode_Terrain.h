#ifndef InputMode_Terrain_h__
#define InputMode_Terrain_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CInputMode_Terrain : public Engine::CInputMode
{
public: // 생성자, 소멸자
	explicit  CInputMode_Terrain(Engine::CInputModeMgr* _pInputModeMgr);
	virtual ~CInputMode_Terrain(void);

public:
	// CInputMode을(를) 통해 상속됨
	virtual _int ProcessInput(const _float & _fDeltaTime) override;

};

END

#endif // !TerrainMap_h__
