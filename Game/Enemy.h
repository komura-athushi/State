#pragma once

//�N���X�錾�B
class Player;

class EnemyState;

/// <summary>
/// �G�l�~�[�B
/// </summary>
class Enemy : public IGameObject
{
public:
	enum EnAnimationClip {						//�A�j���[�V�����B
		enAnimationClip_Idle,					//�ҋ@�A�j���[�V�����B
		enAnimationClip_Walk,					//�����A�j���[�V�����B
		enAnimationClip_Run,					//����A�j���[�V�����B
		enAnimationClip_Attack,					//�U���A�j���[�V�����B
		enAnimationClip_MagicAttack,			//���@�U���A�j���[�V�����B
		enAnimationClip_Damage,					//��_���[�W�A�j���[�V�����B
		enAnimationClip_Down,					//�_�E���A�j���[�V�����B
		enAnimationClip_Num,					//�A�j���[�V�����̐��B
	};
public:
	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W��ݒ肷��B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾����B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// HP��ݒ肷��B
	/// </summary>
	/// <param name="hp">HP�B</param>
	void SetHP(const int hp)
	{
		m_hp = hp;
	}
	/// <summary>
	/// ��]��ݒ肷��B
	/// </summary>
	/// <param name="rotation">��]�B</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �傫����ݒ肷��B
	/// </summary>
	/// <param name="scale">�傫���B</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// �ړ����x��ݒ肷��B
	/// </summary>
	/// <param name="velocity">�ړ����x�B</param>
	void SetVelocity(const Vector3& velocity)
	{
		m_velocity = velocity;
	}
	/// <summary>
	/// �v���C���[��T������B
	/// </summary>
	/// <returns>�v���C���[������������true�B</returns>
	const bool IsSearchPlayer() const;
	/// <summary>
	/// �A�j���[�V�������Đ�����B
	/// </summary>
	/// <param name="enAnimationClip">�A�j���[�V�����N���b�v�B</param>
	/// <param name="animationSpeed">�Đ����x�B</param>
	void SetPlayAnimation(const EnAnimationClip enAnimationClip,const float animationSpeed);
	const bool IsRangeAttackPlayer()const;
	void MakeAttackCollision();
	const bool GetIsUnderAttack() const
	{
		return m_isUnderAttack;
	}
	void SetIsUnderAttack(const bool isUnderAttack)
	{
		m_isUnderAttack = isUnderAttack;
	}
	const bool IsDamaged();
	void Death();
	const bool HPIsBelowZero() const
	{
		return m_hp <= 0;
	}
	const bool IsPlayAnimation() const
	{
		return m_modelRender.IsPlayingAnimation();
	}
private:
	void ProcessState();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	void Move();
	void Rotation();
private:
	
	AnimationClip				m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	ModelRender					m_modelRender;								//���f�������_�[�B
	Vector3						m_position;									//���W�B
	Vector3						m_forward = Vector3::AxisZ;					//�G�l�~�[�̐��ʃx�N�g���B
	Quaternion					m_rotation;									//��]�B
	Vector3						m_scale = Vector3::One;						//�傫���B
	CharacterController			m_charaCon;									//�L�����R���B
	int							m_swordBoneId = -1;							//���̃{�[����ID�B
	int							m_hp = 0;									//HP�B
	Player*						m_player = nullptr;							//�v���C���[�B
	Vector3						m_velocity = Vector3::Zero;
	bool						m_isUnderAttack;							//�U�����B
	std::unique_ptr<EnemyState> m_enemyState = nullptr;
};


