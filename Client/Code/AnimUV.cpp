#include "pch.h"
#include "AnimUV.h"

BEGIN(Client)

CAnimUV::CAnimUV() :
	CComponent() {

}

CAnimUV::CAnimUV(const CAnimUV& AnimUV) :
	CComponent(AnimUV) {

}

CAnimUV::~CAnimUV() {

}

HRESULT CAnimUV::Ready() {

	return S_OK;
}

_int CAnimUV::Update(const _float& fTimeDelta) {
	return UpdateAnim(fTimeDelta);
}

Engine::CComponent* CAnimUV::Create() {
	CAnimUV* pInstance = new CAnimUV;

	if (FAILED(pInstance->Ready()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* CAnimUV::Clone(void) {
	return new CAnimUV(*this);
}

void CAnimUV::Free(void)
{
}

int CAnimUV::UpdateAnim(const _float & _fDeltaTime)
{
	// 반환값 내용
	// 0 : Anim이 정상적으로 업데이트 되었음. 무한 애님 포함.
	// 1 : 현재 Anim 상태가 무효화 상태이며 새로운 상태값 입력을 요구함.
	if (m_stAnimInfo.iFrameCount == 0) return 1;
	if (m_stAnimInfo.iFrameCount == 1) return 0;
	if (m_stAnimInfo.iCountToRepeat != 0 && m_stAnimInfo.iCountToRepeat <= m_stAnimProcessingInfo.iRepeatedCount) return 1;
	// _anim_processing_info를 갱신한다.

	m_stAnimProcessingInfo.fAnimElapsedTime += _fDeltaTime;
	if (m_stAnimProcessingInfo.fAnimElapsedTime > m_stAnimInfo.fTotalTime) {
		// 애니메이션 한바퀴를 다 돎.
		if (0 != m_stAnimInfo.iCountToRepeat) {
			// 무한 애님이 아니라면.
			m_stAnimProcessingInfo.iRepeatedCount++;
			if (m_stAnimInfo.iCountToRepeat <= m_stAnimProcessingInfo.iRepeatedCount) {
				m_stAnimProcessingInfo.fAnimElapsedTime = m_stAnimInfo.fTotalTime;
				return 1;
			}
		}
		else {
			// 무한 애님이라면.	
			m_stAnimProcessingInfo.iCurrentIndex = m_stAnimInfo.iStartFrameIndex;
		}
		m_stAnimProcessingInfo.fAnimElapsedTime = 0.f;
	}

	float fCutTime = m_stAnimInfo.fTotalTime / m_stAnimInfo.iFrameCount;
	int iIndexOffset = static_cast<int>(m_stAnimProcessingInfo.fAnimElapsedTime / fCutTime);
	if (m_stAnimInfo.bIsReversePlay) iIndexOffset *= -1;
	m_stAnimProcessingInfo.iCurrentIndex = m_stAnimInfo.iStartFrameIndex + iIndexOffset;

	return 0;
}


END
