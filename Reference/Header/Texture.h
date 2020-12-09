#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CResources
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture(void);


public:
	HRESULT		Ready_Texture(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt);
	void		Render_Texture(const _uint& iIndex = 0);
	
	// 셰이더 컴객체를 통해 텍스처를 세팅하기 위한 함수
	void		Set_Texture(LPD3DXEFFECT& pEffect, const char* pConstantName, const _uint& iIndex = 0);
	
	static const _tchar* GetComponentTag() { return L"Texture"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

private:
	vector<IDirect3DBaseTexture9*>			m_vecTexture;

public:
	static CTexture*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt);
	virtual CComponent*	Clone(void) override;

private:
	virtual void		Free(void);
};
END
#endif // Texture_h__
