#ifndef Sword_h__
#define Sword_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CMeshRenderer;
//class CCalculator;
//class CCollider;

END

BEGIN(Client)
class CSword : public Engine::CGameObject
{
private:
	explicit CSword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSword(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);
	_bool		Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);

private:

	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CTransform*			m_pTransformCom = nullptr;
	Engine::CMeshRenderer*		m_pRendererCom = nullptr;
	//Engine::CCalculator*		m_pCalculatorCom = nullptr;
	//Engine::CCollider*		m_pColliderCom = nullptr;
	_bool						m_bColl = false;

	const	_matrix*			m_pParentBoneMatrix = nullptr;
	_matrix			m_ParentWorldMatrix;

public:
	static CSword*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

END
#endif // Sword_h__
