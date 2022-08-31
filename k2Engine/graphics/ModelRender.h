#pragma once

#include "MyRenderer.h"

class RenderingEngine;
/// <summary>
/// �X�L�����f�������_�[�B
/// </summary>
class ModelRender : public IRenderer 
{
public:
	ModelRender() {}
	/// <summary>
	/// �������B�ʏ�͂��̊֐��ŏ��������Ă��������B
	/// �f�B�t�@�[�h�����_�����O�B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X�B</param>
	/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
	/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
	/// <param name="">���f���̏�����B</param>
	/// <param name="">true�Ȃ�e��������B</param>
	void Init(const char* filePath,
		AnimationClip* animationClips = nullptr,
		int numAnimationClips = 0,
		EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
		bool isShadowReciever = true);
	/// <summary>
	/// �������B
	/// �t�H���[�h�����_�����O�B
	/// ���̊֐��ŏ������������f���̓��C�g�A�V���h�E�Ȃǂ̉e�����󂯂܂���B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X�B</param>
	/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
	/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
	/// <param name="">���f���̏�����B</param>
	/// <param name="">true�Ȃ�e��������B</param>
	void InitForwardRendering(const char* filePath,
		AnimationClip* animationClips = nullptr,
		int numAnimationClips = 0,
		EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
		bool isShadowReciever = true);
	/// <summary>
	/// Sky�p�B
	/// </summary>
	/// <param name="initData">���f���f�[�^�B</param>
	void InitForwardRendering(ModelInitData& initData);
	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈���B
	/// </summary>
	void Draw(RenderContext& rc);

	/// <summary>
	/// �A�j���[�V�����Đ��B
	/// </summary>
	/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ��B</param>
	/// <param name="interpolateTime">�⊮����(�P�ʁF�b�B)</param>
	void PlayAnimation(int animNo, float interpolateTime = 0.0f)
	{
		m_animation.Play(animNo, interpolateTime);
	}

	/// <summary>
	/// �A�j���[�V�����̍Đ����H
	/// </summary>
	bool IsPlayingAnimation() const
	{
		return m_animation.IsPlaying();
	}

	/// <summary>
	/// ���f�����擾�B
	/// </summary>
	/// <returns>���f��</returns>
	Model& GetModel()
	{
		return m_zprepassModel;
	}

	/// <summary>
	/// ���W�A��]�A�g���S�čX�V�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	/// <param name="rotation">��]�B</param>
	/// <param name="scale">�g��B</param>
	void SetTRS(const Vector3& pos, const Quaternion& rotation, const Vector3& scale)
	{
		SetPosition(pos);
		SetRotation(rotation);
		SetScale(scale);
	}

	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// ��]��ݒ�B
	/// </summary>
	/// <param name="rotation">��]�B</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// �傫���B
	/// </summary>
	/// <param name="scale">�傫���B</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// �V���h�E�L���X�^�[�̃t���O��ݒ肷��
	/// </summary>
	void SetShadowCasterFlag(bool flag)
	{
		m_isShadowCaster = flag;
	}
	/// <summary>
	/// �A�j���[�V�����Đ��̑��x��ݒ肷��B
	/// </summary>
	/// <param name="animationSpeed">���l�̕������{�ɂ���B</param>
	void SetAnimationSpeed(const float animationSpeed)
	{
		m_animationSpeed = animationSpeed;
	}
	/// <summary>
	/// �{�[���̖��O����{�[���ԍ��������B
	/// </summary>
	/// <param name="boneName">�{�[���̖��O</param>
	/// <returns>�{�[���ԍ��B������Ȃ������ꍇ��-1���Ԃ��Ă��܂��B</returns>
	int FindBoneID(const wchar_t* boneName) const
	{
		return m_skeleton.FindBoneID(boneName);
	}
	/// <summary>
	/// �{�[���ԍ�����{�[�����擾�B
	/// </summary>
	/// <param name="boneNo">�{�[���ԍ�</param>
	/// <returns>�{�[��</returns>
	Bone* GetBone(int boneNo) const
	{
		return m_skeleton.GetBone(boneNo);
	}
	void AddAnimationEvent(AnimationEventListener eventListener)
	{
		m_animation.AddAnimationEventListener(eventListener);
	}
private:
	/// <summary>
	/// �X�P���g���̏������B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X�B</param>
	void InitSkeleton(const char* filePath);
	/// <summary>
	/// �A�j���[�V�����̏������B
	/// </summary>
	/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
	/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
	/// <param name="enModelUpAxis">���f���̏�����B</param>
	void InitAnimation(AnimationClip* animationClips,
		int numAnimationClips, 
		EnModelUpAxis enModelUpAxis);
	/// <summary>
	/// �V���h�E�}�b�v�ւ̕`��p�X����Ă΂�鏈���B
	/// </summary>
	/// <param name="rc">�����_�����O�R���e�L�X�g</param>
	/// <param name="ligNo">���C�g�ԍ�</param>
	/// <param name="shadowMapNo">�V���h�E�}�b�v�ԍ�</param>
	/// <param name="lvpMatrix">���C�g�r���[�v���W�F�N�V�����s��</param>
	void OnRenderShadowMap(
		RenderContext& rc,
		int ligNo,
		int shadowMapNo,
		const Matrix& lvpMatrix) override;
	/// <summary>
	/// ZPrepass����Ă΂�鏈���B
	/// </summary>
	/// <param name="rc"></param>
	void OnZPrepass(RenderContext& rc) override;
	/// <summary>
	/// G-Buffer�`��p�X����Ă΂�鏈���B
	/// </summary>
	void OnRenderToGBuffer(RenderContext& rc) override;
	/// <summary>
	/// �t�H���[�h�����_�[�p�X����Ă΂�鏈���B
	/// </summary>
	void OnForwardRender(RenderContext& rc) override;
private:
	/// <summary>
	 /// ���ʂ̏���������
	 /// </summary>
	 /// <param name="renderingEngine">�����_�����O�G���W��</param>
	 /// <param name="tkmFilePath">tkm�t�@�C���p�X</param>
	void InitCommon(RenderingEngine& renderingEngine, const char* tkmFilePath);
private:
	AnimationClip*				m_animationClips = nullptr;			//�A�j���[�V�����N���b�v�B
	int							m_numAnimationClips = 0;			//�A�j���[�V�����N���b�v�̐��B
	Vector3 					m_position = Vector3::Zero;			//���W�B
	Quaternion	 				m_rotation = Quaternion::Identity;	//��]�B
	Vector3						m_scale = Vector3::One;				//�g�嗦�B
	EnModelUpAxis				m_enFbxUpAxis = enModelUpAxisZ;		//FBX�̏�����B
	Animation					m_animation;						//�A�j���[�V�����B
	Model						m_zprepassModel;					//ZPrepass�ŕ`�悳��郂�f��
	Model						m_forwardRenderModel;				//�t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��郂�f��
	Model						m_renderToGBufferModel;				//RenderToGBuffer�ŕ`�悳��郂�f��
	Model						m_shadowModels[NUM_DEFERRED_LIGHTING_DIRECTIONAL_LIGHT][NUM_SHADOW_MAP];	//�V���h�E�}�b�v�ɕ`�悷�郂�f��
	bool						m_isUpdateAnimation = true;			//�A�j���[�V�������X�V����H
	Skeleton					m_skeleton;							//���B
	bool						m_isShadowCaster = true;			//�V���h�E�L���X�^�[�t���O
	float						m_animationSpeed = 1.0f;
};
