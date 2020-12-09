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
	// CComponent��(��) ���� ��ӵ�
	virtual HRESULT Ready(void) override { return S_OK; }
	virtual int Update(const _float & _fDeltaTime) override { return 0; }

public:
	LPD3DXEFFECT		Get_EffectHandle(void) { return m_pEffect; }

public:
	HRESULT		Ready_Shader(const _tchar* pFilePath);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;

	LPD3DXEFFECT				m_pEffect;		// ���̴��� ����ϴ� �İ�ü

	LPD3DXBUFFER				m_pErrMsg;		// ������ ������ �����޼����� �����ϱ� ���� ���� ����

public:
	static CShader*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath);
	virtual CComponent*	Clone(void);
	virtual void		Free(void);


	

};


END
#endif // Shader_h__
