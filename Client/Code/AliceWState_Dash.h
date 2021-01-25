#pragma once

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CAliceW;
class CAliceWState_Dash :
	public CState<CAliceW>
{
#define DASH_TIME 0.6f
public:
	CAliceWState_Dash(CAliceW& _rOwner, const _vec3& _vDir);
	virtual ~CAliceWState_Dash();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void Free(void) override;

private:
	_vec3 m_vDir;
	_float m_fElapsedTime = 0.f;
	_float m_fTickTime = 0.f;
	Engine::CLayer* m_pLayer = nullptr;
};

END
