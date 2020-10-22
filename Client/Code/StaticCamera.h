#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CStaticCamera : public Engine::CCamera
{
private: // ������, �Ҹ���
	explicit  CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CStaticCamera(const  CStaticCamera& rhs);
	virtual ~CStaticCamera(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;

public:
	static CStaticCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetOffset(const _vec3& _vOffset) { m_vOffset = _vOffset; }

private:
	Engine::CTransform* m_pTransform = nullptr;
	_vec3 m_vOffset{ 0.f, 0.f, 0.f };
};

END

#endif // !StaticCamera_h__
