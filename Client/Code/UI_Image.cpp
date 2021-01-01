#include "pch.h"
#include "UI_Image.h"


CUI_Image::CUI_Image(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_Image::CUI_Image(const CUI_Image & rhs)
	:
	CGameObject(rhs)
{
}

CUI_Image::~CUI_Image()
{
}

HRESULT CUI_Image::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();
	return S_OK;
}

_int CUI_Image::Update_Object(const _float & _fDeltaTime)
{
	m_pRenderer->Add_RenderGroup(Engine::RENDER_UI, this);

	return 0;
}

void CUI_Image::Render_Object(void)
{
	if (!m_pTexture)
		return;

	D3DXMATRIX matScreen;
	// 보정 비율을 곱한다.
	D3DXMatrixScaling(&matScreen, m_fCorrectionRatioX, m_fCorrectionRatioY, 0.f);
	matScreen *= GetTransform()->GetObjectMatrix();
	// RenderOffset을 적용한다.
	//matScreen._41 += GetRenderOffsetX();
	//matScreen._42 += GetRenderOffsetY();
	//if (_pCamera) matScreen = _pCamera->GetScreenMatrix(matScreen);

	//// 컬링을 적용한다.
	//if (!IsObjInCamera(this, _pCamera)) return;

	// 객체를 그린다.
	
	Engine::CGraphicDev::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	Engine::CGraphicDev::GetInstance()->GetSprite()->SetTransform(&matScreen);
	Engine::CGraphicDev::GetInstance()->GetSprite()->Draw(
		static_cast<LPDIRECT3DTEXTURE9>(m_pTexture->GetTextureInfo(0).pTexture),
		&m_rcExtractionArea,
		&D3DXVECTOR3(
			static_cast<FLOAT>((m_rcExtractionArea.right - m_rcExtractionArea.left) >> 1),
			static_cast<FLOAT>((m_rcExtractionArea.bottom - m_rcExtractionArea.top) >> 1),
			0.f),
		nullptr, m_clRenderColor);
	Engine::CGraphicDev::GetInstance()->GetSprite()->End();
}

CUI_Image * CUI_Image::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Image*	pInstance = new CUI_Image(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_Image::Free(void)
{
	CGameObject::Free();
}

void CUI_Image::SetTexture(const _tchar * _pTextureTag)
{
	if (!_pTextureTag)
		return;

	m_pTexture = static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, _pTextureTag));
	Engine::TextureInfo tTextureInfo = m_pTexture->GetTextureInfo(0);

	if (m_pTexture) {
		GetTransform()->SetScaleXYZ(tTextureInfo.tImageInfo.Width, tTextureInfo.tImageInfo.Height, 0.f);
		SetExtractionArea(RECT({ 0, 0, static_cast<LONG>(tTextureInfo.tImageInfo.Width), static_cast<LONG>(tTextureInfo.tImageInfo.Height) }));
		_vec3 vPos = GetTransform()->GetPos();
		SetOutputArea(RECT(
			{
				static_cast<LONG>(vPos.x - m_rcExtractionArea.right * 0.5f),
				static_cast<LONG>(vPos.y - m_rcExtractionArea.bottom * 0.5f),
				static_cast<LONG>(vPos.x + m_rcExtractionArea.right * 0.5f),
				static_cast<LONG>(vPos.y + m_rcExtractionArea.bottom * 0.5f)
			}));
	}
}
