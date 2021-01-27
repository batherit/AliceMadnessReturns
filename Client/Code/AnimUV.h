#pragma once

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CAnimUV : public Engine::CComponent {
protected:
	explicit CAnimUV();
	explicit CAnimUV(const CAnimUV& Physics);
	virtual ~CAnimUV(void);
public:
	virtual HRESULT Ready() override;
	virtual _int	Update(const _float& fTimeDelta) override;

public:
	static const _tchar* GetComponentTag() { return L"AnimUV"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }
	static CComponent* Create();
	virtual CComponent* Clone(void) override;
	virtual void Free(void) override;

public:
	// 애니메이션 관련 함수들
	virtual void SetNewAnimInfo(const AnimInfo& _stAnimInfo) {
		m_stAnimInfo = _stAnimInfo;
		ZeroMemory(&m_stAnimProcessingInfo, sizeof(m_stAnimProcessingInfo));
		m_stAnimProcessingInfo.iCurrentIndex = m_stAnimInfo.iStartFrameIndex;
	}
	_int UpdateAnim(const _float& _fDeltaTime);
	_bool IsAnimPlaying(void) { return 0 == UpdateAnim(0.f); }
	AnimInfo& GetAnimInfo(void) { return m_stAnimInfo; }
	_float GetAnimProgress(void) const { return m_stAnimProcessingInfo.fAnimElapsedTime / m_stAnimInfo.fTotalTime; }
	_float GetAnimU(void) const { return (m_stAnimProcessingInfo.iCurrentIndex % m_stAnimInfo.iCol) * GetGapU(); }
	_float GetAnimV(void) const { return (m_stAnimProcessingInfo.iCurrentIndex / m_stAnimInfo.iCol) * GetGapV(); }
	inline _float GetGapU() const { return 1.f / m_stAnimInfo.iCol; }
	inline _float GetGapV() const { return 1.f / m_stAnimInfo.iRow; }

private:

	AnimInfo m_stAnimInfo;
	AnimProcessingInfo m_stAnimProcessingInfo;
};
END