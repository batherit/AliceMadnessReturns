#ifndef Monster_h__
#define Monster_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CMonster : public Engine::CGameObject
{
private: // ������, �Ҹ���
	explicit  CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CMonster(const  CMonster& rhs);
	virtual ~CMonster(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CMonster*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetCameraForBillboard(Engine::CCamera* _pMainCamera) { m_pMainCamera = _pMainCamera; }

private:
	Engine::CRenderer* m_pRenderer = nullptr;
	Engine::CCamera* m_pMainCamera = nullptr;
};

END

#endif // !LogoObject_h__
