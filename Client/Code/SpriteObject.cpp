#include "pch.h"
#include "SpriteObject.h"

BEGIN(Client)
CSpriteObject::CSpriteObject(LPDIRECT3DDEVICE9 pGraphicDev) 
	:
	CGameObject(pGraphicDev)
{
	//AddComponent<Engine::CRcTex>();
	//m_pTextureBoard = GetComponent<Engine::CRcTex>();

	//AddComponent<CRectRenderer>();
	//m_pRenderer = GetComponent<CRectRenderer>();
	//m_pRenderer->SetDevice(m_pGraphicDev);

	//m_pRenderer->SetTextureTag(L"Texture_Cloud");
	//m_pRenderer->SetTextureCnt(0);
	//m_pRenderer->SetBufferTag(L"Buffer_RcTex");
}

CSpriteObject::~CSpriteObject() 
{
}

void CSpriteObject::SetMultiTexture(const _tchar * _szMultiTextureTag)
{
	//m_pRenderer->SetTextureTag(_szMultiTextureTag);
	//m_pRenderer->SetTextureCnt(0);
}

_int CSpriteObject::UpdateAnim(const _float _fDeltaTime)
{
	// ��ȯ�� ����
	// 0 : Anim�� ���������� ������Ʈ �Ǿ���. ���� �ִ� ����.
	// 1 : ���� Anim ���°� ��ȿȭ �����̸� ���ο� ���°� �Է��� �䱸��.
	if (m_stAnimInfo.iFrameCount == 0) return 1;
	if (m_stAnimInfo.iFrameCount == 1) return 0;
	if (m_stAnimInfo.iCountToRepeat != 0 && m_stAnimInfo.iCountToRepeat <= m_stAnimProcessingInfo.iRepeatedCount) return 1;
	// _anim_processing_info�� �����Ѵ�.

	m_stAnimProcessingInfo.fAnimElapsedTime += _fDeltaTime;
	if (m_stAnimProcessingInfo.fAnimElapsedTime > m_stAnimInfo.fTotalTime) {
		// �ִϸ��̼� �ѹ����� �� ��.
		if (0 != m_stAnimInfo.iCountToRepeat) {
			// ���� �ִ��� �ƴ϶��.
			m_stAnimProcessingInfo.iRepeatedCount++;
			if (m_stAnimInfo.iCountToRepeat <= m_stAnimProcessingInfo.iRepeatedCount) {
				m_stAnimProcessingInfo.fAnimElapsedTime = m_stAnimInfo.fTotalTime;
				return 1;
			}
		}
		else {
			// ���� �ִ��̶��.	
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

void CSpriteObject::Free(void) {
	CGameObject::Free();
}
END