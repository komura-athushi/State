#include "k2EnginePreCompile.h"
#include "ModelRender.h"
#include "RenderingEngine.h"

void ModelRender::InitForwardRendering(const char* filePath,
	AnimationClip* animationClips,
	int numAnimationClips,
	EnModelUpAxis enModelUpAxis,
	bool isShadowReciever)
{
	InitSkeleton(filePath);
	InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
	ModelInitData initData;
	//tkm�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_tkmFilePath = filePath;
	//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_fxFilePath = "Assets/shader/Model.fx";
	//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
	initData.m_vsEntryPointFunc = "VSMain";

	if (m_animationClips != nullptr) {
		//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
		initData.m_vsSkinEntryPointFunc = "VSSkinMain";
		//�X�P���g�����w�肷��B
		initData.m_skeleton = &m_skeleton;
	}

	//���f���̏�������w�肷��B
	initData.m_modelUpAxis = enModelUpAxis;
	
	initData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//�쐬�����������f�[�^�����ƂɃ��f��������������A
	m_forwardRenderModel.Init(initData);
	InitCommon(*g_renderingEngine, filePath);
}

void ModelRender::InitForwardRendering(ModelInitData& initData)
{
	InitSkeleton(initData.m_tkmFilePath);

	initData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//�쐬�����������f�[�^�����ƂɃ��f��������������B
	m_forwardRenderModel.Init(initData);
	InitCommon(*g_renderingEngine, initData.m_tkmFilePath);
}

void ModelRender::Init(const char* filePath,
	AnimationClip* animationClips,
	int numAnimationClips,
	EnModelUpAxis enModelUpAxis,
	bool isShadowReciever)
{
	InitSkeleton(filePath);
	InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
	ModelInitData modelInitData;
	modelInitData.m_fxFilePath = "Assets/shader/RenderToGBufferFor3DModel.fx";
	modelInitData.m_vsEntryPointFunc = "VSMain";

	if (m_animationClips != nullptr) {
		//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
		modelInitData.m_vsSkinEntryPointFunc = "VSMainSkin";
		//�X�P���g�����w�肷��B
		modelInitData.m_skeleton = &m_skeleton;
	}
	if (isShadowReciever) {
		modelInitData.m_psEntryPointFunc = "PSMainShadowReciever";
	}
	//���f���̏�������w�肷��B
	modelInitData.m_modelUpAxis = enModelUpAxis;

	modelInitData.m_tkmFilePath = filePath;
	modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	modelInitData.m_colorBufferFormat[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	modelInitData.m_colorBufferFormat[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	modelInitData.m_colorBufferFormat[3] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	modelInitData.m_colorBufferFormat[4] = DXGI_FORMAT_R8G8B8A8_UNORM;

	m_renderToGBufferModel.Init(modelInitData);

	InitCommon(*g_renderingEngine, filePath);
}

void ModelRender::InitSkeleton(const char* filePath)
{
	//�X�P���g���̃f�[�^��ǂݍ��݁B
	std::string skeletonFilePath = filePath;
	int pos = (int)skeletonFilePath.find(".tkm");
	skeletonFilePath.replace(pos, 4, ".tks");
	m_skeleton.Init(skeletonFilePath.c_str());
}

void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
{
	m_animationClips = animationClips;
	m_numAnimationClips = numAnimationClips;
	if (m_animationClips != nullptr) {
		m_animation.Init(m_skeleton,
			m_animationClips,
			numAnimationClips);
	}
}

void ModelRender::InitCommon(RenderingEngine& renderingEngine, const char* tkmFilePath)
{
	//ZPrepass�`��p�̃��f����������
	{

		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_fxFilePath = "Assets/shader/ZPrepass.fx";
		if (m_animationClips != nullptr) {
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
		}
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		m_zprepassModel.Init(modelInitData);
	}
	//�V���h�E�}�b�v�`��p�̃��f����������
	{
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		if (m_animationClips != nullptr) {
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
		}
		
		modelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
		for (
			int ligNo = 0;
			ligNo < NUM_DEFERRED_LIGHTING_DIRECTIONAL_LIGHT;
			ligNo++)
		{
			m_shadowModels[ligNo][0].Init(modelInitData);
			m_shadowModels[ligNo][1].Init(modelInitData);
			m_shadowModels[ligNo][2].Init(modelInitData);
		}
	}

}

void ModelRender::Update()
{

	m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	if (m_renderToGBufferModel.IsInited()) {
		m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}
	if (m_forwardRenderModel.IsInited()) {
		m_forwardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}
	for (auto& models : m_shadowModels) {
		for (auto& model : models) {
			model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
	}
	if (m_skeleton.IsInited()) {
		if (m_renderToGBufferModel.IsInited())
		{
			m_skeleton.Update(m_renderToGBufferModel.GetWorldMatrix());
		}
		if (m_forwardRenderModel.IsInited())
		{
			m_skeleton.Update(m_forwardRenderModel.GetWorldMatrix());
		}
	}
	
	//�A�j���[�V������i�߂�B
	m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
}

void ModelRender::Draw(RenderContext& rc)
{
	g_renderingEngine->AddRenderObject(this);
}
void ModelRender::OnRenderShadowMap(
	RenderContext& rc,
	int ligNo,
	int shadowMapNo,
	const Matrix& lvpMatrix)
{
	if (m_isShadowCaster) {
		m_shadowModels[ligNo][shadowMapNo].Draw(rc, g_matIdentity, lvpMatrix);
	}
}

void ModelRender::OnZPrepass(RenderContext& rc)
{
	m_zprepassModel.Draw(rc);
}
void ModelRender::OnRenderToGBuffer(RenderContext& rc)
{
	if (m_renderToGBufferModel.IsInited()) {
		m_renderToGBufferModel.Draw(rc);
	}
}
void ModelRender::OnForwardRender(RenderContext& rc)
{
	if (m_forwardRenderModel.IsInited()) {
		m_forwardRenderModel.Draw(rc);
	}
}