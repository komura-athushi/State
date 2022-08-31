#include "k2EnginePreCompile.h"
#include "ToneMap.h"

ToneMap::ToneMap()
{

}

ToneMap::~ToneMap()
{

}

void ToneMap::Init(RenderTarget& mainRenderTarget)
{
	int width = g_graphicsEngine->GetFrameBufferWidth();
	int height = g_graphicsEngine->GetFrameBufferHeight();

	int number = 0;
	while (true)
	{
		width /= 2;
		height /= 2;
	
		if (width == 0)
		{
			width = 1;
		}
		if (height == 0)
		{
			height = 1;
		}

		float color[4] =
		{
			0.0f,0.0f,0.0f,0.0f
		};

		//�T�C�Y��1/4�ɂ��������_�\�^�[�Q�b�g���쐬�B
		auto renderPtr = std::make_unique<RenderTarget>();
		renderPtr.get()->Create(width, height,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			color);
		m_renderTargetVector.push_back(std::move(renderPtr));

		//���ϋP�x���v�Z���邽�߂̃X�v���C�g������������B

		SpriteInitData spriteInitData;
		if (number == 0)
		{
			spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
			spriteInitData.m_psEntryPoinFunc = "ExtractLumaPSMain";
		}
		else
		{
			spriteInitData.m_textures[0] = &m_renderTargetVector[number - 1].get()->GetRenderTargetTexture();
			spriteInitData.m_psEntryPoinFunc = "PSMain";
		}
		//�𑜓x��4/1�������́B
		spriteInitData.m_width = width;
		spriteInitData.m_height = height;
		// 2D�p�̃V�F�[�_�[���g�p����
		spriteInitData.m_fxFilePath = "Assets/shader/tonemap.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
	
		//�����ɂ̓����_�[�^�[�Q�b�g�̃t�H�[�}�b�g������B
		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
	
		auto spritePtr = std::make_unique<Sprite>();
		spritePtr.get()->Init(spriteInitData);

		m_spriteVector.push_back(std::move(spritePtr));

		if (width == 1 && height == 1)
		{
			break;
		}

		number++;
	}

	//���ϋP�x�𗘗p����tonemap�p�̃X�v���C�g������������B
	SpriteInitData spriteInitData;
	spriteInitData.m_textures[0] = &m_renderTargetVector[m_renderTargetVector.size() - 1].get()->GetRenderTargetTexture();
	spriteInitData.m_textures[1] = &mainRenderTarget.GetRenderTargetTexture();
	
	spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
	spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
	// 2D�p�̃V�F�[�_�[���g�p����
	spriteInitData.m_fxFilePath = "Assets/shader/tonemap.fx";
	spriteInitData.m_vsEntryPointFunc = "VSMain";
	spriteInitData.m_psEntryPoinFunc = "FinalPSMain";

	//�����ɂ̓����_�[�^�[�Q�b�g�̃t�H�[�}�b�g������B
	spriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();
	spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;

	spriteInitData.m_expandConstantBuffer = (void*)&m_tonemapBuffer;
	spriteInitData.m_expandConstantBufferSize = sizeof(TonemapBuffer) +
		(16 - (sizeof(TonemapBuffer) % 16));
	
	m_finalSprite.Init(spriteInitData);
}

void ToneMap::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	if (m_numberCalcRenderTarget == -1)
	{
		for (int i = 0; i < m_renderTargetVector.size(); i++)
		{
			RenderToLuma(rc, i);
		}
		m_numberCalcRenderTarget = 0;
	}
	else 
	{
		for (int i = 0; i < NUM_RENDER_TARGETS_DRAW_ONE_FRAME; i++)
		{
			RenderToLuma(rc, m_numberCalcRenderTarget);
			if (m_numberCalcRenderTarget == m_renderTargetVector.size() - 1)
			{
				m_numberCalcRenderTarget = 0;
				break;
			}
			else
			{
				m_numberCalcRenderTarget++;
			}
		}
	}
	//���C�������_�[�^�[�Q�b�g��PRESENT����RENDERTARGET�ցB
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	// �����_�����O�^�[�Q�b�g��ݒ�
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	//�`��B
	m_finalSprite.Draw(rc);
	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
	//TARGET����PRESENT�ցB
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}