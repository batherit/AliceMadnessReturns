#include "Component.h"
#include "Physics.h"

BEGIN(Engine)

CPhysics::CPhysics() :
	CComponent(){

}

CPhysics::CPhysics(const CPhysics& Physics) :
	CComponent(Physics) {

}

CPhysics::~CPhysics() {

}

HRESULT CPhysics::Ready() {

	return S_OK;
}

_int CPhysics::Update(const _float& fTimeDelta) {

	return 0;
}

CComponent* CPhysics::Create() {
	CPhysics* pInstance = new CPhysics;

	if (FAILED(pInstance->Ready()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CPhysics::Clone(void) {
	return new CPhysics(*this);
}

void CPhysics::Free(void) {

}

END
