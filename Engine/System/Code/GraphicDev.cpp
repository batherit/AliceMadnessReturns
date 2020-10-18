#include "GraphicDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphicDev)

Engine::CGraphicDev::CGraphicDev(void)
	: m_pSDK(nullptr)
	, m_pGraphicDev(nullptr)
{

}

Engine::CGraphicDev::~CGraphicDev(void)
{
	Free();
}

HRESULT Engine::CGraphicDev::Ready_GraphicDev(HWND hWnd,
	WINMODE eMode,
	const _uint& iSizeX,
	const _uint& iSizeY,
	CGraphicDev** ppGraphicDev)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	NULL_CHECK_RETURN(m_pSDK, E_FAIL);

	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// 장치의 성능정보를 CAPS 구조체에 얻어옴
	FAILED_CHECK_RETURN(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&DeviceCaps),
		E_FAIL);

	_ulong	iFlag = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		iFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		iFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		Parameters;
	ZeroMemory(&Parameters, sizeof(D3DPRESENT_PARAMETERS));

	Parameters.BackBufferWidth = iSizeX;
	Parameters.BackBufferHeight = iSizeY;
	Parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	Parameters.BackBufferCount = 1;

	Parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	Parameters.MultiSampleQuality = 0;

	Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Parameters.hDeviceWindow = hWnd;

	Parameters.Windowed = eMode;

	Parameters.EnableAutoDepthStencil = TRUE;
	Parameters.AutoDepthStencilFormat = D3DFMT_D24S8;

	Parameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	Parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	FAILED_CHECK_RETURN(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hWnd, iFlag, &Parameters, &m_pGraphicDev), E_FAIL);

	*ppGraphicDev = this;

	return S_OK;
}

void Engine::CGraphicDev::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, Color, 1.f, 0);
	m_pGraphicDev->BeginScene();
}

void Engine::CGraphicDev::Render_End(void)
{
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, NULL, NULL);
}

void Engine::CGraphicDev::Free(void)
{
	_ulong	dwRefCnt = 0;

	if (dwRefCnt = Engine::Safe_Release(m_pGraphicDev))
		MSG_BOX("m_pGraphicDev Release Failed");

	if (dwRefCnt = Engine::Safe_Release(m_pSDK))
		MSG_BOX("m_pSDK Release Failed");
}

