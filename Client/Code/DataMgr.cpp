#include "pch.h"
#include "DataMgr.h"
#include "AliceW.h"
#include "Ship.h"
#include "Attribute.h"

USING(Client)
IMPLEMENT_SINGLETON(CDataMgr)

CDataMgr::CDataMgr(void)
{

}

CDataMgr::~CDataMgr(void)
{
	Free();
}

void CDataMgr::SaveAliceWData(CAliceW * _pAliceW)
{
	m_iToothNum = _pAliceW->GetToothNum();
	CAttribute* pAttribute = _pAliceW->GetComponent<CAttribute>();
	m_fCurHP = pAttribute->GetHP();
	m_fMaxHP = pAttribute->GetMaxHP();
}

void CDataMgr::SaveShipData(CShip* _pShip) {
	m_iToothNum = _pShip->GetToothNum();
}

void CDataMgr::Free(void)
{
}