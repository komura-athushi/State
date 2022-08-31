#include "stdafx.h"
#include "Player.h"

#include "Game.h"
#include "FireBall.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{
	//�A�j���[�V���������[�h����B
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/human/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/human/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/human/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/human/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/human/magicattack.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/human/receivedamage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/human/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_PushLever].Load("Assets/animData/human/push_lever.tka");
	m_animationClips[enAnimationClip_PushLever].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Winner].Load("Assets/animData/human/winner.tka");
	m_animationClips[enAnimationClip_Winner].SetLoopFlag(false);
	//���f�������_�\���������B
	m_modelRender.Init("Assets/modelData/human/human.tkm",m_animationClips,enAnimationClip_Num);

	m_modelRender.SetPosition(m_position);

	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		20.0f,			//���a�B
		100.0f,			//�����B
		m_position		//���W�B
	);

	//���ɐݒ肵���{�[����ID���擾����B
	m_swordBoneId = m_modelRender.FindBoneID(L"Sword");

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

	//�e�G�t�F�N�g�����[�h����B
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/efk/enemy_slash_01.efk");
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/efk/cast_fire.efk");

	//�e�T�E���h�����[�h����B
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/magic.wav");
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/slash.wav");
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/hit_pitch.wav");

	m_game = FindGO<Game>("game");
	return true;
}

void Player::Update()
{
	//�ړ������B
	Move();
	//��]�����B
	Rotation();
	//�����蔻��B
	Collision();
	//�U�������B
	Attack();
	//�A�j���[�V�����̍Đ��B
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ����B
	ManageState();

	//���f���̍X�V�B
	m_modelRender.Update();
}

void Player::Move()
{
	//�ړ��ł��Ȃ���Ԃł���΁A�ړ������͂��Ȃ��B
	if (IsEnableMove() == false)
	{
		return;
	}
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//���̃t���[���̈ړ��ʂ����߂�B
	//���X�e�B�b�N�̓��͗ʂ��󂯎��B
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//�J�����̑O�������ƉE�������擾�B
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ���ʂł̑O�������A�E�����ɕϊ�����B
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ�����̈ړ����x���N���A�B
	m_moveSpeed += cameraForward * lStick_y * 250.0f;	//�������ւ̈ړ����x�����Z�B
	m_moveSpeed += cameraRight * lStick_x * 250.0f;		//�E�����ւ̈ړ����x�����Z�B
	//����y���ړ��͖����B
	/*if (g_pad[0]->IsTrigger(enButtonA) //A�{�^���������ꂽ��
		&& m_charaCon.IsOnGround()  //���A�n�ʂɋ�����
		) {
		//�W�����v����B
		m_moveSpeed.y = 400.0f;	//������ɑ��x��ݒ肵�āA
	}

	m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();
	*/

	//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	m_modelRender.SetPosition(modelPosition);
}

void Player::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̐��ʃx�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::Collision()
{
	//��_���[�W�A�_�E�����A�N���A���̓_���[�W��������Ȃ��B
	if (m_playerState == enPlayerState_ReceiveDamage ||
		m_playerState == enPlayerState_Down ||
		m_playerState == enPlayerState_Clear)
	{
		return;
	}

	{
		//�G�̍U���p�̃R���W�����̔z����擾����B
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
		//�z���for���ŉ񂷁B
		for (auto collision : collisions)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_charaCon))
			{
				//HP�����炷�B
				m_hp -= 1;
				//HP��0�ɂȂ�����B
				if (m_hp == 0)
				{
					//�_�E��������B
					m_playerState = enPlayerState_Down;
				}
				//HP��0�ł͂Ȃ�������B
				else {
					//��_���[�W���A�N�V������������B
					m_playerState = enPlayerState_ReceiveDamage;
				}
				//���ʉ����Đ�����B
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(7);
				se->Play(false);
				se->SetVolume(0.7f);
				return;
			}
		}
	}

	{
		//�G�̌������t�@�C���[�{�[���̃R���W�����̔z����擾����B
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_fireball");
		//�z���for���ŉ񂷁B
		for (auto collision : collisions)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_charaCon))
			{
				//HP�����炷�B
				m_hp -= 1;
				//HP��0�ɂȂ�����B
				if (m_hp == 0)
				{
					//�_�E��������B
					m_playerState = enPlayerState_Down;
				}
				//HP��0�ł͂Ȃ�������B
				else {
					//��_���[�W���A�N�V������������B
					m_playerState = enPlayerState_ReceiveDamage;
				}
				//���ʉ����Đ�����B
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(7);
				se->Play(false);
				se->SetVolume(0.7f);
				return;
			}
		}
	}
}

void Player::Attack()
{
	//�U�����łȂ��Ȃ�A���������Ȃ��B
	if (m_playerState != enPlayerState_Attack)
	{
		return;
	}

	//�U�����蒆�ł���΁B
	if (m_isUnderAttack == true)
	{
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision();
	}
}

void Player::MakeAttackCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	//���̃{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(110.0f, 15.0f, 15.0f));
	//���̃{�[���̃��[���h�s����R���W�����ɓK�p������B
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("player_attack");
}

void Player::MakeFireBall()
{
	//�t�@�C���[�{�[�����쐬����B
	FireBall* fireBall = NewGO<FireBall>(0);
	Vector3 fireBallPosition = m_position;
	//���W��������ɂ���B
	fireBallPosition.y += 70.0f;
	fireBall->SetPosition(fireBallPosition);
	fireBall->SetRotation(m_rotation);
	//�p�҂̓v���C���[�ɂ���B
	fireBall->SetEnMagician(FireBall::enMagician_Player);
}

void Player::MakePushLeverCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	collisionPosition += m_forward * 50.0f;
	//����̃R���W�������쐬����B
	collisionObject->CreateSphere(collisionPosition, Quaternion::Identity, 70.0f);
	//���O��t����B
	collisionObject->SetName("player_lever");
}

void Player::MakeSlashingEffect()
{
	//�U���G�t�F�N�g�𔭐�������B
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(1);
	effectEmitter->SetScale(Vector3::One * 11.0f);
	Vector3 effectPosition = m_position;
	//���W��������ɂ���B
	effectPosition.y += 70.0f;
	effectEmitter->SetPosition(effectPosition);
	Quaternion rotation;
	rotation = m_rotation;
	//��]������B
	rotation.AddRotationDegY(360.0f);
	rotation.AddRotationDegZ(180.0f);
	effectEmitter->SetRotation(rotation);
	//�G�t�F�N�g���Đ�����B
	effectEmitter->Play();

	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(3);
	se->Play(false);
	se->SetVolume(0.8f);
}

void Player::ProcessCommonStateTransition()
{
	//�G��S�ł�������B
	if (m_game->IsWannihilationEnemy())
	{
		//�N���A�X�e�[�g�Ɉڍs����B
		m_playerState = enPlayerState_Clear;
		return;
	}
	//Y�{�^���������ꂽ��B
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		//�U���X�e�[�g�Ɉڍs����B
		m_playerState = enPlayerState_Attack;
		//�t���O��false�ɂ���B
		m_isUnderAttack = false;
		return;
	}
	//X�{�^���������ꂽ��B
	else if (g_pad[0]->IsTrigger(enButtonX))
	{
		//���@�U���X�e�[�g�Ɉڍs����B
		m_playerState = enPlayerState_MagicAttack;
		//���ʉ��𔭐�������B
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(0);
		se->SetVolume(0.5f);
		se->Play(false);
		//�G�t�F�N�g�𔭐�������B
		EffectEmitter* effect = NewGO<EffectEmitter>(0);
		effect->Init(2);
		Vector3 effectPosition = m_position;
		effect->SetPosition(m_position);
		effect->SetScale(Vector3::One * 10.0f);
		effect->Play();
		return;
	}
	//A�{�^���������ꂽ��B
	else if (g_pad[0]->IsTrigger(enButtonA))
	{
		//���o�[�������X�e�[�g�Ɉڍs����B
		m_playerState = enPlayerState_PushLever;
		//���o�[���������߂̃R���W�������쐬����B
		MakePushLeverCollision();
		return;
	}

	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�ړ����x�����ȏゾ������B
		if (m_moveSpeed.LengthSq() >= 200.0f * 200.0f)
		{
			//�X�e�[�g�𑖂�ɂ���B
			m_playerState = enPlayerState_Run;
			return;
		}
		else
		{
			//�����ɂ���B
			m_playerState = enPlayerState_Walk;
			return;
		}

	}
	//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
	else
	{
		//�X�e�[�g��ҋ@�ɂ���B
		m_playerState = enPlayerState_Idle;
		return;
	}
}

void Player::ProcessIdleStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void Player::ProcessWalkStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void Player::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Player::ProcessMagicAttackStateTransition()
{
	//���@�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Player::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Player::ProcessPushLeverStateTransition()
{
	//���o�[�������A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Player::ProcessDownStateTransition()
{
	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�Q�[���I�[�o�[��ʒm����B
		m_game->NotifyGameOver();
		
	}
}

void Player::ProcessClearStateTransition()
{
	//�����A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�Q�[���N���A��ʒm����B
		m_game->NotifyGameClear();
	}
}

void Player::ManageState()
{
	switch (m_playerState)
	{
	//�ҋ@�X�e�[�g�̎��B
	case enPlayerState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
	//�����X�e�[�g�̎��B
	case enPlayerState_Walk:
		//�����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessWalkStateTransition();
		break;
	//����X�e�[�g�̎��B
	case enPlayerState_Run:
		//����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessRunStateTransition();
		break;
	//�U���X�e�[�g�̎��B
	case enPlayerState_Attack:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
		break;
	//���@�U���X�e�[�g�̎��B
	case enPlayerState_MagicAttack:
		//���@�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessMagicAttackStateTransition();
		break;
	//���o�[�������X�e�[�g�̎��B
	case enPlayerState_PushLever:
		//���o�[�������X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessPushLeverStateTransition();
		break;
	//��_���[�W���X�e�[�g�̎��B
	case enPlayerState_ReceiveDamage:
		//��_���[�W���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessReceiveDamageStateTransition();
		break;
	//�_�E���X�e�[�g�̎��B
	case enPlayerState_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessDownStateTransition();
		break;
	//�N���A�X�e�[�g�̎��B
	case enPlayerState_Clear:
		//�N���A�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessClearStateTransition();
		break;
	}
}

void Player::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_playerState)
	{
	//�ҋ@�X�e�[�g�̎��B
	case enPlayerState_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
	//�����X�e�[�g�̎��B
	case enPlayerState_Walk:
		m_modelRender.SetAnimationSpeed(1.2f);
		//�����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
	//����X�e�[�g�̎��B
	case enPlayerState_Run:
		m_modelRender.SetAnimationSpeed(1.2f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
	//�U���X�e�[�g�̎��B
	case enPlayerState_Attack:
		m_modelRender.SetAnimationSpeed(1.5f);
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	//���@�U���X�e�[�g�̎��B
	case enPlayerState_MagicAttack:
		m_modelRender.SetAnimationSpeed(1.2f);
		//���@�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_MagicAttack, 0.1f);
		break;
	//���o�[�������X�e�[�g�̎��B
	case enPlayerState_PushLever:
		//���o�[�������A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_PushLever, 0.1f);
		break;
	//��_���[�W�X�e�[�g�̎��B
	case enPlayerState_ReceiveDamage:
		//��_���[�W�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
	//�_�E���X�e�[�g�̎��B
	case enPlayerState_Down:
		//�_�E���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	//�N���A�X�e�[�g�̎��B
	case enPlayerState_Clear:
		//�N���A�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Winner, 0.1f);
		break;
	default:
		break;
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//�L�[�̖��O���uattack_start�v�̎��B
	if (wcscmp(eventName, L"attack_start") == 0) {
		//�a���G�t�F�N�g���쐬����B
		MakeSlashingEffect();
		//�U�����ɂ���B
		m_isUnderAttack = true;
	}
	//�L�[�̖��O���uattack_end�v�̎��B
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//�U�����I���B
		m_isUnderAttack = false;
	}
	//�L�[�̖��O���umagic_attack�v�̎��B
	else if (wcscmp(eventName, L"magic_attack") == 0) {
		//�t�@�C���[�{�[�����쐬����B
		MakeFireBall();
	}
}

void Player::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
}