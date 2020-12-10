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
	// �ݶ��̴��� �ڽ��� �������� �����Ƿ� ���� ������Ʈ�� ���� ����.
	//CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CColliderObject::Free(void)
{
	CGameObject::Free();
}
