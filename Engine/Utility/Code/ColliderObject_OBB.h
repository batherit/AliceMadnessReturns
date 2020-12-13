#ifndef ColliderObject_OBB_h__
#define	ColliderObject_OBB_h__

#include "Engine_Define.h"
#include "ColliderObject.h"

BEGIN(Engine)
class CCubeCol;
class CPolygonRenderer;
class ENGINE_DLL CColliderObject_OBB : public CColliderObject
{
protected:
	explicit CColliderObject_OBB(LPDIRECT3DDEVICE9	pGraphicDev);	// 일반 생성자
	explicit CColliderObject_OBB(const CColliderObject_OBB& rhs);			// 복사 생성자
	virtual ~CColliderObject_OBB();

public:
	virtual HRESULT Ready_Object(void);
	virtual _int Update_Object(const _float& _fDeltaTime);
	virtual void Render_Object(void);

public:
	// 파일 입출력 관련
	virtual _bool SaveInfo(FILE* _fpOut) { return true; };
	virtual _bool LoadInfo(FILE* _fpIn) { return true; };
	virtual _bool SaveInfo(HANDLE& _hfOut);
	virtual _bool LoadInfo(HANDLE& _hfIn);

	virtual void Free(void) override;
	static CColliderObject_OBB* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void SetRadius(_float _fRadius);
	_float GetRadius() const { return m_fRadius; }
	_float GetRadiusW() const;

private:
	CCubeCol* m_pCubeBuffer = nullptr;
	CPolygonRenderer* m_pRenderer = nullptr;
	_float m_fRadius = 1.f;
};

END

#endif