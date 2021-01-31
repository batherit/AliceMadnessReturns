#ifndef DataMgr_h__
#define DataMgr_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
//class CGameObject;
class CAliceW;
class CShip;
class CDataMgr : public CBase
{
	DECLARE_SINGLETON(CDataMgr)
private:
	explicit CDataMgr(void);
	virtual ~CDataMgr(void);

public:
	void SetValidData(_bool _bIsValidData) { m_bIsValidData = _bIsValidData; }
	_bool IsValidData() const { return m_bIsValidData; }

	void SaveAliceWData(CAliceW* _pAliceW);
	void SaveShipData(CShip* _pShip);
	_int GetToothNum() const { return m_iToothNum; }
	_float GetCurHP() const { return m_fCurHP; }
	_float GetMaxHP() const { return m_fMaxHP; }

private:
	virtual void Free(void);

private:
	_bool m_bIsValidData = false;

	_int m_iToothNum = 0;
	_float m_fCurHP = 64.f;
	_float m_fMaxHP = 64.f;
};

END
#endif // DataMgr_h__
