#ifndef Loading_h__
#define Loading_h__

#include "Define.h"
#include "Base.h"

BEGIN(Client)

class CLoading : public CBase 
{
public:
	enum LOADINGID {LOADING_STAGE, LOADING_BOSS, LOADING_END };

private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading(void);

public:
	LOADINGID			Get_LoadingID(void) const { return m_eLoading; }
	CRITICAL_SECTION*	Get_Crt(void) { return &m_Crt; }
	_bool				Get_Finish(void) const { return m_bFinish; }
	const _tchar*		Get_LoadString(void) { return m_szLoading; }

public:
	static _uint	CALLBACK Thread_Main(void* pArg);

public:
	HRESULT		Ready_Loading(LOADINGID eLoading);
	_uint		Loading_ForStage(void);

private:
	_uint LoadBuffers();
	_uint LoadTextures();
	_uint LoadCommonMeshes();
	_uint LoadPoolSceneMeshes();
	_uint LoadPlaySceneMeshes();
	_uint LoadMiniGameSceneMeshes();
	_uint LoadPlay2SceneMeshes();
	_uint LoadBossSceneMeshes();

private:
	HANDLE				m_hThread;

	CRITICAL_SECTION	m_Crt;
	LOADINGID			m_eLoading;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	
	_bool				m_bFinish;
	_tchar				m_szLoading[256];

public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading);
private:
	virtual void	Free(void);

};
END

#endif // Loading_h__
