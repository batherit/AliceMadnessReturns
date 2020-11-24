#ifndef Stone_h__
#define Stone_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CSphereCollider;
class COptimization;
//class CCalculator;

END

BEGIN(Client)
class CSphereRenderer;
class CStone : public Engine::CGameObject
{
private:
	explicit CStone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStone(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);
	//void		SetUp_OnTerrain(void);
	void		Key_Input(const _float& fTimeDelta);
	//_vec3		PickUp_OnTerrain(void);

private:
	Engine::CStaticMesh*		m_pMesh = nullptr;
	Engine::CTransform*			m_pTransform = nullptr;
	Engine::CRenderer*			m_pRenderer = nullptr;
	Engine::CSphereCollider*	m_pCollider = nullptr;
	Engine::COptimization*		m_pOptimization = nullptr;
	CSphereRenderer*			m_pSphere = nullptr;

public:
	static CStone*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

END
#endif // Stone_h__
