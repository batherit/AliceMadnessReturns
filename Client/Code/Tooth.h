#ifndef Tooth_h__
#define Tooth_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAliceW;
class CTooth : public Engine::CGameObject
{
private: // ������, �Ҹ���
	explicit  CTooth(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CTooth(const  CTooth& rhs);
	virtual ~CTooth(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CTooth*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CStaticMesh* GetDynamicMesh() const { return m_pMesh; }

public:
	void SetPopInfo(_vec3 _vArrivalPos);

private:
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CColliderObject* m_pCollider = nullptr;
	CAliceW* m_pAlice = nullptr;
	_vec3 m_vArrivalPos;
	_bool m_bIsArrived = true;
	_float m_fRadian = 0.f;
};

END

#endif // !LogoObject_h__