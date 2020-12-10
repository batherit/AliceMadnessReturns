#ifndef ColliderObject_Sphere_h__
#define	ColliderObject_Sphere_h__

#include "Engine_Define.h"
#include "ColliderObject.h"

BEGIN(Engine)
class CSphereCol;
class CPolygonRenderer;
class ENGINE_DLL CColliderObject_Sphere : public CColliderObject
{
protected:
	explicit CColliderObject_Sphere(LPDIRECT3DDEVICE9	pGraphicDev);	// 일반 생성자
	explicit CColliderObject_Sphere(const CColliderObject_Sphere& rhs);			// 복사 생성자
	virtual ~CColliderObject_Sphere();

public:
	virtual HRESULT Ready_Object(void);
	virtual _int Update_Object(const _float& _fDeltaTime);
	virtual void Render_Object(void);

public:
	// 파일 입출력 관련
	virtual _bool SaveInfo(FILE* _fpOut) { return true; };
	virtual _bool LoadInfo(FILE* _fpIn) { return true; };
	virtual _bool SaveInfo(HANDLE& _hfOut) { return true; };
	virtual _bool LoadInfo(HANDLE& _hfIn) { return true; };

	virtual void Free(void) override;
	static CColliderObject_Sphere* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	void SetColliderTag(const _tchar* _pColliderTag) { lstrcpy(m_tcColliderTag, _pColliderTag); }
	const _tchar* GetColliderTag() const { return m_tcColliderTag; }

	void SetRadius(_float _fRadius);
	_float GetRadius() const { return m_fRadius; }

private:
	_float m_fRadius = 1.f;
	CSphereCol* m_pSphereBuffer = nullptr;
	CPolygonRenderer* m_pRenderer = nullptr;
};

END

#endif