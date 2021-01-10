#ifndef MirrorPad_h__
#define MirrorPad_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAliceW;
class CPlatform;
class CMirrorPad : public Engine::CGameObject
{
#define PRESS_TIME 1.f
private: // 생성자, 소멸자
	explicit  CMirrorPad(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CMirrorPad(const  CMirrorPad& rhs);
	virtual ~CMirrorPad(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CMirrorPad*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CStaticMesh* GetStaticMesh() const { return m_pMesh; }
	void SetPos(_vec3 _vInitPos) { 
		m_vInitPos = _vInitPos; 
		m_vEndPos = _vInitPos; 
		m_vEndPos.y -= 0.4f;
		GetTransform()->SetPos(_vInitPos); }
	void AddPlatform(CPlatform* _pPlatform) {
		m_vecPlatforms.emplace_back(_pPlatform);
	}

	void SetLinkIndex(_int _iLinkIndex) { m_iLinkIndex = _iLinkIndex; }
	_int GetLinkIndex() const { return m_iLinkIndex; }

private:
	_int m_iLinkIndex = -1;

	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	_vec3 m_vInitPos;
	_vec3 m_vEndPos;
	_bool m_bIsOn = false;
	vector<CPlatform*> m_vecPlatforms;
};

END

#endif // !LogoObject_h__