#include "ColliderObject.h"


USING(Engine)

CColliderObject::CColliderObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CColliderObject::CColliderObject(const CColliderObject & rhs)
	:
	CGameObject(rhs)
{
}

CColliderObject::~CColliderObject()
{
	
}

_int CColliderObject::Update_Object(const _float & _fDeltaTime)
{
	// 콜라이더는 자식을 관리하지 않으므로 굳이 업데이트를 하진 않음.
	//CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CColliderObject::Free(void)
{
	CGameObject::Free();
}
