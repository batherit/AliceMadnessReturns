#ifndef CrushingFist_h__
#define CrushingFist_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAttribute;
class CCrushingFist : public Engine::CGameObject
{
private: // ������, �Ҹ���
	explicit  CCrushingFist(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CCrushingFist(const  CCrushingFist& rhs);
	virtual ~CCrushingFist(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo);

public:
	static CCrushingFist*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CStaticMesh* GetDynamicMesh() const { return m_pMesh; }
	
	void SetCrushingFistInfo(const _vec3& _vEndPos, _float _fPressTime, _float _fKeepTime, _float _fBackTime);
	void EventOn();
	
private:
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;

	CAttribute* m_pPlayerAttribute = nullptr;

	_float m_fElapsedTime = 0.f;	//���� �ð�
	_float m_fPressTime = 1.f;		// ���� �Ϸ������ �ð�
	_float m_fKeepTime = 1.f;		// ���� �����Ǵ� �ð�
	_float m_fBackTime = 1.f;		// ���ڸ��� ������� �ɸ��� �ð�.
	_vec3 m_vStartPos;
	_vec3 m_vEndPos;

	_bool m_bIsOn = false;
	_bool m_bIsOff = false;
	_bool m_bIsEnd = false;
	_bool m_bIsEventOn = false;
};

END

#endif // !LogoObject_h__