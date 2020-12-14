#ifndef ColliderObject_AABB_h__
#define	ColliderObject_AABB_h__

#include "Engine_Define.h"
#include "ColliderObject.h"

BEGIN(Engine)
class CCubeCol;
class CPolygonRenderer;
class ENGINE_DLL CColliderObject_AABB : public CColliderObject
{
protected:
	explicit CColliderObject_AABB(LPDIRECT3DDEVICE9	pGraphicDev);	// �Ϲ� ������
	explicit CColliderObject_AABB(const CColliderObject_AABB& rhs);			// ���� ������
	virtual ~CColliderObject_AABB();

public:
	virtual HRESULT Ready_Object(void);
	virtual _int Update_Object(const _float& _fDeltaTime);
	virtual void Render_Object(void);

public:
	// ���� ����� ����
	virtual _bool SaveInfo(FILE* _fpOut) { return true; };
	virtual _bool LoadInfo(FILE* _fpIn) { return true; };
	virtual _bool SaveInfo(HANDLE& _hfOut);
	virtual _bool LoadInfo(HANDLE& _hfIn);

	virtual void Free(void) override;
	static CColliderObject_AABB* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void GetCollisionInfo(_vec3& _vMin, _vec3& _vMax);
	BOXINFO GetBoxInfo();

private:
	CCubeCol* m_pCubeBuffer = nullptr;
	CPolygonRenderer* m_pRenderer = nullptr;
};

END

#endif