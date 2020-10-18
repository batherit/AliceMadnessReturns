#pragma once

#include "framework.h"
#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CRectRenderer;
END

BEGIN(Client)
class CSpriteObject abstract : public Engine::CGameObject
{
public: // 생성자, 소멸자
	explicit CSpriteObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpriteObject(void);

	// public 함수, 변수
//public:
//	HRESULT		Ready(void);
//	_int		Update(const _float& fTimeDelta);
//	void		Render(void);

public:
	void SetMultiTexture(const _tchar * _szMultiTextureTag);
	// 애니메이션 관련 함수들
	void SetNewAnimInfo(const AnimInfoLine& _stAnimInfo) {
		m_stAnimInfo = _stAnimInfo;
		ZeroMemory(&m_stAnimProcessingInfo, sizeof(m_stAnimProcessingInfo));
		m_stAnimProcessingInfo.iCurrentIndex = m_stAnimInfo.iStartFrameIndex;
	}
	_int UpdateAnim(const _float _fDeltaTime);
	_int GetCurrentAnimIndex() { return m_stAnimProcessingInfo.iCurrentIndex; }
	bool IsAnimPlaying() { return 0 == UpdateAnim(0.f); }
	AnimInfoLine& GetAnimInfo() { return m_stAnimInfo; }
	_float GetAnimProgress() const { return m_stAnimProcessingInfo.fAnimElapsedTime / m_stAnimInfo.fTotalTime; }

	// 생성, 소멸 함수
public:
	virtual void	Free(void);

protected:
	//Engine::CRcTex* m_pTextureBoard = nullptr;
	CRectRenderer* m_pRenderer = nullptr;

	AnimInfoLine m_stAnimInfo;
	AnimProcessingInfo m_stAnimProcessingInfo;
};
END