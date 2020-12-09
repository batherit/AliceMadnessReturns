#ifndef Shader_h__
#define Shader_h__

#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader :public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader(void);

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Ready(void) override { return S_OK; }
	virtual int Update(const _float & _fDeltaTime) override { return 0; }

public:
	LPD3DXEFFECT		Get_EffectHandle(void) { return m_pEffect; }

public:
	HRESULT		Ready_Shader(const _tchar* pFilePath);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;

	LPD3DXEFFECT				m_pEffect;		// 쉐이더를 담당하는 컴객체

	LPD3DXBUFFER				m_pErrMsg;		// 컴파일 시점에 에러메세지를 저장하기 위한 버퍼 공간

public:
	static CShader*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath);
	virtual CComponent*	Clone(void);
	virtual void		Free(void);


	

};


END
#endif // Shader_h__
