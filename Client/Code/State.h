#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"

BEGIN(Client)

template<typename T>
class CState abstract : public CBase
{
public:
	CState(T& _rOwner);
	virtual ~CState();

public:
	virtual void OnLoaded(void) = 0;
	virtual int Update(const _float& _fDeltaTime) = 0;
	//virtual void LateUpdate(void) = 0;
	virtual void OnExited(void) = 0;
	virtual int AlwaysUpdate(const _float& _fDeltaTime) { return 0; }
	// CBase을(를) 통해 상속됨
	virtual void Free(void) override;

protected:
	T& m_rOwner;

	
};

template<typename T>
inline CState<T>::CState(T& _rOwner)
	:
	m_rOwner(_rOwner)
{

}

template<typename T>
inline CState<T>::~CState()
{
}

template<typename T>
inline void CState<T>::Free(void)
{
}

END