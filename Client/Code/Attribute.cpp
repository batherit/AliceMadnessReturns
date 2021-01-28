#include "pch.h"
#include "Attribute.h"

BEGIN(Client)

CAttribute::CAttribute() :
	CComponent() {

}

CAttribute::CAttribute(const CAttribute& Attribute) :
	CComponent(Attribute) {

}

CAttribute::~CAttribute() {

}

HRESULT CAttribute::Ready() {

	return S_OK;
}

_int CAttribute::Update(const _float& fTimeDelta) {
	return 0;
}

Engine::CComponent* CAttribute::Create() {
	CAttribute* pInstance = new CAttribute;

	if (FAILED(pInstance->Ready()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* CAttribute::Clone(void) {
	return new CAttribute(*this);
}

void CAttribute::Free(void) {
	m_AttackerList.clear();
}

_bool CAttribute::RegisterAttacker(Engine::CGameObject * _pAttacker)
{
	if (!_pAttacker)
		return false;

	auto iter = find(m_AttackerList.begin(), m_AttackerList.end(), _pAttacker);

	if (iter != m_AttackerList.end())
		return false;	// 이미 있으므로 종료

	m_AttackerList.emplace_back(_pAttacker);

	return true;
}

_bool CAttribute::ReleaseAttacker(Engine::CGameObject * _pAttacker)
{
	if (!_pAttacker)
		return false;

	auto iter = find(m_AttackerList.begin(), m_AttackerList.end(), _pAttacker);

	if (iter == m_AttackerList.end())
		return false;	// 없으므로 종료

	m_AttackerList.erase(iter);

	return true;
}

void CAttribute::ClearAttackerList()
{
}

END
