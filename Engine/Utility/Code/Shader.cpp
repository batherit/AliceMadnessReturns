#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pEffect(nullptr)
	, m_pErrMsg(nullptr)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CShader::CShader(const CShader& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pEffect(rhs.m_pEffect)
	, m_pErrMsg(rhs.m_pErrMsg)
{
	Safe_AddRef(m_pGraphicDev);
	Safe_AddRef(m_pEffect);
	Safe_AddRef(m_pErrMsg);
}

Engine::CShader::~CShader(void)
{

}

HRESULT Engine::CShader::Ready_Shader(const _tchar* pFilePath)
{
	// m_pEffect ���� �ְ�, m_pErrMsg ���� ���� ��� : ���̴� �ڵ尡 ���������� �ۼ���
	// m_pEffect ���� ����, m_pErrMsg ���� �ִ� ��� : ���̴� �ڵ� ���°� ��������, ������ ���
	// m_pEffect ���� �ְ�, m_pErrMsg ���� �ִ� ��� : �����ϴ� ���� ������ ������ ��� �ִ� ���


	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, 
										pFilePath,
										NULL, // ��ũ���� �ּ� (���̴� ���ο� �����ϴ�)
										NULL,  // ���� ������ �ּ�(���̴� ���ο� �����ϴ�)
										D3DXSHADER_DEBUG, // ���̴��� ������ϰڴٴ� �ǹ�(�ܺ� ���̺귯���� �̿��� ���� ����Ͽ� �־���� ����)
										NULL,		// ���� ���ҽ� ������ ��� ������ ���ΰ�(Null�� ��� �⺻������ ��ġ�� ������ ��)
										&m_pEffect, 
										&m_pErrMsg))) // ������� �Ұ��������� ���� �Ǵ� ��� ���� ���ڿ� ������ �����صδ� �뵵
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Error", MB_OK);
		return E_FAIL;
	}

	// ��� ��Ȳ
	else if (nullptr != m_pErrMsg)
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_warning", MB_OK);
	}

	return S_OK;
}

Engine::CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
{
	CShader*	pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(pFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* Engine::CShader::Clone(void)
{
	return new CShader(*this);
}

void Engine::CShader::Free(void)
{
	Safe_Release(m_pErrMsg);
	Safe_Release(m_pEffect);
	Safe_Release(m_pGraphicDev);
}
