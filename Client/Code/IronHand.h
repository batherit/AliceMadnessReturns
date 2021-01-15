#ifndef IronHand_h__
#define IronHand_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CIronHand : public Engine::CGameObject
{
private: // ������, �Ҹ���
	explicit  CIronHand(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CIronHand(const  CIronHand& rhs);
	virtual ~CIronHand(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);

public:
	static CIronHand*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CStaticMesh* GetDynamicMesh() const { return m_pMesh; }
	void SetIronHandInfo(const _vec3& _vStartPos, _float _fActivationTime);

private:
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;

	_float m_fActivationTime = 0.f;	// �۵��� �ð�
	_float m_fElapsedTime = 0.f;	//���� �ð�
	_float m_fKeepTime = 1.f;		// ������ �����Ǵ� �ð�
	_vec3 m_vStartPos;
	_vec3 m_vEndPos;
	_bool m_bIsOn = false;
	_bool m_bIsOff = false;
	_bool m_bIsEnd = false;
};

END

#endif // !LogoObject_h__