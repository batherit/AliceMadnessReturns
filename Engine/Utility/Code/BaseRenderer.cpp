#include "BaseRenderer.h"
#include "Export_Utility.h"

USING(Engine)

CBaseRenderer::CBaseRenderer(void)
{
	m_pRenderer = Engine::Get_Renderer();
	Safe_AddRef(m_pRenderer);
}

CBaseRenderer::~CBaseRenderer(void)
{
	Safe_Release(m_pRenderer);
}

void CBaseRenderer::RegisterToRenderer()
{
	if (RENDERID::RENDER_END <= m_eRenderID)
		return;

	m_pRenderer->Add_RenderGroup(m_eRenderID, m_pOwner);
}
