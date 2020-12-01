#ifndef InputProcessor_Map_h__
#define InputProcessor_Map_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

class CMapTab;
BEGIN(Client)

class CInputProcessor_Map : public Engine::CInputProcessor
{
public: // 생성자, 소멸자
	explicit  CInputProcessor_Map(Engine::CInputProcessorMgr* _pInputProcessorMgr);
	virtual ~CInputProcessor_Map(void);

public:
	// CInputProcessor을(를) 통해 상속됨
	virtual _int ProcessInput(const _float & _fDeltaTime) override;

private:
	CEditScene* m_pEditScene = nullptr;
	CMapTab* m_pMapTab = nullptr;
};

END

#endif // !TerrainMap_h__
