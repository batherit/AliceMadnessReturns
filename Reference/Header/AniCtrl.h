#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl : public CBase
{
private:
	explicit CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAniCtrl(const CAniCtrl& rhs);
	virtual ~CAniCtrl(void);

public: 
	LPD3DXANIMATIONCONTROLLER		Get_AnimationCtrl(void) { return m_pAniCtrl; }
	_bool							Is_AnimationSetEnd(void);

public:
	HRESULT			Ready_AnimationCtrl(void);

	void			Set_AnimationSet(const _uint& iIndex);

	void			Play_Animation(const _float& fTimeDelta);

private:
	LPD3DXANIMATIONCONTROLLER			m_pAniCtrl;

	_uint								m_iCurrentTrack;
	_uint								m_iNewTrack;

	_float								m_fAccTime;
	_uint								m_iOldAniIdx;

	_double								m_dPeriod = 0.0;

public:
	static CAniCtrl*			Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAniCtrl*			Create(const CAniCtrl& rhs);
	virtual void				Free(void);
};

END

#endif // AniCtrl_h__
