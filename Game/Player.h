#pragma once

//�N���X�錾�B
class Lever;
class Collision;
class Game;

/// <summary>
/// �v���C���[�B
/// </summary>
class Player : public IGameObject
{
public:
	//�v���C���[�̃X�e�[�g�B
	enum EnPlayerState {
		enPlayerState_Idle,					//�ҋ@�B
		enPlayerState_Walk,					//�����B
		enPlayerState_Run,					//����B
		enPlayerState_Attack,				//�U���B
		enPlayerState_MagicAttack,			//���@�U���B
		enPlayerState_PushLever,			//���o�[�������B
		enPlayerState_ReceiveDamage,		//�_���\�W�󂯂��B
		enPlayerState_Down,					//HP��0�B
		enPlayerState_Clear					//�N���A�[�B
	};
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾�B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
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
	/// ��������(�X�e�[�g)���ǂ������擾�B
	/// </summary>
	/// <returns>��������(�X�e�[�g)�Ȃ�true�B</returns>
	bool IsEnableMove() const
	{
		return m_playerState != enPlayerState_Attack &&
			m_playerState != enPlayerState_MagicAttack &&
			m_playerState != enPlayerState_ReceiveDamage &&
			m_playerState != enPlayerState_PushLever &&
			m_playerState != enPlayerState_Down &&
			m_playerState != enPlayerState_Clear;
	}
private:
	/// <summary>
	/// �ړ������B
	/// </summary>
	void Move();
	/// <summary>
	/// ��]�����B
	/// </summary>
	void Rotation();
	/// <summary>
	/// �U�����̏����B
	/// </summary>
	void Attack();
	/// <summary>
	/// �G�̍U���Ƃ̓����蔻�菈���B
	/// </summary>
	void Collision();
	/// <summary>
	/// �U���̍ۂ̓����蔻��p�̃R���W�������쐬����B
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// ���@�U���̃t�@�C���[�{�[�����쐬����B
	/// </summary>
	void MakeFireBall();
	/// <summary>
	/// ���o�[���������́A�����蔻��p�̃R���W�������쐬����B
	/// </summary>
	void MakePushLeverCollision();
	/// <summary>
	/// �U�������ۂ̃G�t�F�N�g���쐬����B
	/// </summary>
	void MakeSlashingEffect();
	/// <summary>
	/// �A�j���[�V�����̍Đ��B
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ManageState();
	/// <summary>
	/// �A�j���[�V�����C�x���g�p�̊֐��B
	/// </summary>
	/// <param name="clipName">�A�j���[�V�����̖��O�B</param>
	/// <param name="eventName">�A�j���[�V�����C�x���g�̃L�[�̖��O�B</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ����B
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// �����X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// �U���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// ���@�U���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessMagicAttackStateTransition();
	/// <summary>
	/// ���o�[�������X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessPushLeverStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// �_�E���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// �N���A�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessClearStateTransition();
	
	//�A�j���[�V�����B
	enum EnAnimationClip {
		enAnimationClip_Idle,				//�ҋ@�A�j���[�V�����B	
		enAnimationClip_Walk,				//�����A�j���[�V�����B
		enAnimationClip_Run,				//����A�j���[�V�����B
		enAnimationClip_Attack,				//�U���A�j���[�V�����B
		enAnimationClip_MagicAttack,		//���@�U���A�j���[�V�����B
		enAnimationClip_Damage,				//��_���[�W�A�j���[�V�����B
		enAnimationClip_Down,				//�_�E���A�j���[�V�����B
		enAnimationClip_PushLever,			//���o�[�������A�j���[�V�����B
		enAnimationClip_Winner,				//�����A�j���[�V�����B
		enAnimationClip_Num,				//�A�j���[�V�����̐��B
	};
	AnimationClip			m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	ModelRender				m_modelRender;								//���f�������_�\�B
	Vector3					m_position;									//���W�B
	Vector3					m_moveSpeed;								//�ړ����x�B
	Vector3					m_forward = Vector3::AxisZ;					//�v���C���[�̐��ʃx�N�g���B
	Quaternion				m_rotation;									//�N�H�[�^�j�I���B
	CharacterController		m_charaCon;									//�L�����N�^�[�R���g���[���[�B
	EnPlayerState			m_playerState = enPlayerState_Idle;			//�v���C���[�X�e�[�g�B
	bool					m_isUnderAttack = false;					//�U�����Ȃ�true�B
	int						m_swordBoneId = -1;							//���ɐݒ肵���{�[����ID�B
	int						m_hp = 10;									//HP�B
	Game*					m_game = nullptr;							//�Q�[���B
};

