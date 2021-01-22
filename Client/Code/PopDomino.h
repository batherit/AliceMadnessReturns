#ifndef PopDomino_h__
#define PopDomino_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAliceW;
class CPopDomino : public Engine::CGameObject
{
#define DOMINO_DELAY_TIME	1.5f
#define DOMINO_KEEP_TIME	0.5f

private: // 생성자, 소멸자
	explicit  CPopDomino(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CPopDomino(const  CPopDomino& rhs);
	virtual ~CPopDomino(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);

public:
	static CPopDomino*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CStaticMesh* GetDynamicMesh() const { return m_pMesh; }
	void EventOn();

private:
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	Engine::CTexture* m_pTexture = nullptr;
	CAliceW* m_pAlice = nullptr;
	//_bool m_bIsVisible = true;
	_bool m_bIsEventOn = false;
	_float m_fElapsedTime = 0.f;
	_float m_fKeepTime = 0.f;
	_bool m_bIsKeeped = false;
	_float m_fDissolveAmount = 1.f;
};

END

#endif // !LogoObject_h__