#include "k2EnginePreCompile.h"
#include "FontRender.h"
#include "RenderingEngine.h"

void FontRender::Draw(RenderContext& rc)
{
	if (m_text == nullptr)
	{
		return;
	}
	g_renderingEngine->AddRenderObject(this);
}