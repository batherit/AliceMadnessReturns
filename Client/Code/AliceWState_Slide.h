#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CAliceW;
class CAliceWState_Slide :
	public CState<CAliceW>
{
public:
	CAliceWState_Slide(CAliceW& _rOwner);
	virtual ~CAliceWState_Slide();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void Free(void) override;
	
private:
	void PlaySlideSound(const _float& _fDeltaTime);

private:
	_float m_fSlideSoundTickTime = 0.f;
};

END
