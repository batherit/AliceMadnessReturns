#ifndef SkyBox_h__
#define SkyBox_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;
class CRenderer;
class CTransform;
class CPolygonRenderer;

END

BEGIN(Client)
class CSkyBox : public Engine::CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkyBox(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
	Engine::CTexture* GetTexture() const { return m_pTexture; }

private:
	Engine::CCubeTex*				m_pCubeTex = nullptr;
	Engine::CTexture*			m_pTexture = nullptr;
	Engine::CPolygonRenderer*			m_pRenderer = nullptr;

public:
	static CSkyBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

END
#endif // SkyBox_h__
