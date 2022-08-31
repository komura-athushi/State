#include "stdafx.h"
#include "Enemy.h"

#include "Game.h"
#include "FireBall.h"
#include "Player.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

#include "EnemyState.h"
#include "IdleState.h"

#include <time.h>
#include <stdlib.h>

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/enemy/magicattack.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/enemy/receivedamage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/enemy/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, enAnimationClip_Num);
	
	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);

	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		20.0f,			//���a�B
		100.0f,			//�����B
		m_position		//���W�B
	);

	//���̃{�[����ID���擾����B
	m_swordBoneId = m_modelRender.FindBoneID(L"Sword");

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/efk/enemy_slash_01.efk");
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/efk/cast_fire.efk");
	
	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/magic.wav");
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/slash.wav");
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/hit.wav");

	m_player = FindGO<Player>("player");
	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	m_enemyState = std::make_unique<IdleState>();
	return true;
}

void Enemy::Update()
{	
	ProcessState();
	Move();
	Rotation();
	//���f���̍X�V�B
	m_modelRender.Update();
}

void Enemy::ProcessState()
{
	auto enemyState = m_enemyState->HandleInput(this, m_player);
	if (enemyState)
	{
		m_enemyState = std::move(enemyState);
		m_enemyState->Enter(this);
	}
	m_enemyState->Update(this);
}

void Enemy::Move()
{
	m_position = m_charaCon.Execute(m_velocity, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position + Vector3(0.0f, 5.0f, 0.0f));
}


void Enemy::Rotation()
{
	if (fabsf(m_velocity.x) < 0.001f
		&& fabsf(m_velocity.z) < 0.001f) {
		//m_velocity.x��m_velocity.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_velocity.x / m_velocity.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_velocity.x, m_velocity.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	m_forward.Normalize();
}

void Enemy::SetPlayAnimation(const EnAnimationClip enAnimationClip, const float animationSpeed)
{
	m_modelRender.SetAnimationSpeed(animationSpeed);
	m_modelRender.PlayAnimation(enAnimationClip,0.5f);
}

const bool Enemy::IsSearchPlayer() const
{
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[�ɂ�����x�߂�������.�B
	if (diff.LengthSq() <= 1000.0 * 1000.0f)
	{
		//�G�l�~�[����v���C���[�Ɍ������x�N�g���𐳋K������B
		diff.Normalize();
		//�G�l�~�[�̐��ʂ̃x�N�g���ƁA�G�l�~�[����v���C���[�Ɍ������x�N�g���́B
		//����(cos��)�����߂�B
		float cos = m_forward.Dot(diff);
		if (cos >= 1.0f)
		{
			cos = 1.0f;
		}
		//����(cos��)����p�x(��)�����߂�B
		float angle = acosf(cos);
		//�p�x(��)��120����菬������΁B
		if (angle <= (Math::PI / 180.0f) * 120.0f)
		{
			//�v���C���[���������I
			return true;
		}
	}
	//�v���C���[���������Ȃ������B
	return false;
}

const bool Enemy::IsRangeAttackPlayer() const
{
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[�ɂ�����x�߂�������B
	if (diff.LengthSq() <= 150.0 * 150.0f)
	{
		return true;
	}
	return false;
}

void Enemy::MakeAttackCollision()
{
	//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	//���̃{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(100.0f, 10.0f, 10.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
}


const bool Enemy::IsDamaged()
{
	{
		//�v���C���[�̍U���p�̃R���W�������擾����B
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
		//�R���W�����̔z���for���ŉ񂷁B
		for (auto collision : collisions)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_charaCon))
			{
				//HP��1���炷�B
				m_hp -= 1;
				
				//���ʉ����Đ�����B
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(4);
				se->Play(false);
				se->SetVolume(0.4f);
				return true;
			}
		}
	}

	{
		//�v���C���[�̃t�@�C���[�{�[���p�̃R���W�������擾����B
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_fireball");
		//for���Ŕz����񂷁B
		for (auto collision : collisions)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_charaCon))
			{
				//HP��1���炷�B
				m_hp -= 1;
				//���ʉ����Đ�����B
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(4);
				se->Play(false);
				se->SetVolume(0.6f);
				return true;
			}
		}
	}
	return false;
}

void Enemy::Death()
{
	Game* game = FindGO<Game>("game");
	//�|���ꂽ�G�l�~�[�̐���+1����B
	game->AddDefeatedEnemyNumber();
	//���g���폜����B
	DeleteGO(this);
}

void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//�L�[�̖��O���uattack_start�v�̎��B
	if (wcscmp(eventName, L"attack_start") == 0) {
		//�U���������true�ɂ���B
		m_isUnderAttack = true;
		//�U���G�t�F�N�g�𔭐�������B
		EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
		effectEmitter->Init(1);
		//�傫����ݒ肷��B
		effectEmitter->SetScale(Vector3::One * 12.0f);
		Vector3 effectPosition = m_position;
		//���W��������ɂ���B
		effectPosition.y += 70.0f;
		//���W��ݒ肷��B
		effectEmitter->SetPosition(effectPosition);
		Quaternion rotation;
		rotation = m_rotation;
		rotation.AddRotationDegY(360.0f);
		//��]��ݒ肷��B
		effectEmitter->SetRotation(rotation);
		//�G�t�F�N�g���Đ�����B
		effectEmitter->Play();
		//���ʉ����Đ�����B
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(3);
		se->Play(false);
		se->SetVolume(0.8f);
	}
	//�L�[�̖��O���uattack_end�v�̎��B
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//�U���������false�ɂ���B
		m_isUnderAttack = false;
	}
	//�L�[�̖��O���umagic_attack�v�̎��B
	else if (wcscmp(eventName, L"magic_attack") == 0) {
		//�t�@�C���[�{�[���̃I�u�W�F�N�g���쐬����B
		//MakeFireBall();
	}
}

/*
void Enemy::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_enemyState != enEnemyState_Chase)
	{
		return;
	}

	//�G�l�~�[���ړ�������B
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		//�d�͂�0�ɂ���B
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	//���W��ݒ肷��B
	m_modelRender.SetPosition(modelPosition);
}

/*
void Enemy::MakeFireBall()
{
	//�t�@�C���[�{�[���̃I�u�W�F�N�g���쐬����B
	FireBall* fireBall = NewGO<FireBall>(0);
	Vector3 fireBallPosition = m_position;
	//���W��������ɂ���B
	fireBallPosition.y += 70.0f;
	//���W��ݒ肷��B
	fireBall->SetPosition(fireBallPosition);
	//��]��ݒ肷��B
	fireBall->SetRotation(m_rotation);
	//�p�҂��G�l�~�[�ɐݒ肷��B
	fireBall->SetEnMagician(FireBall::enMagician_Enemy);
}
*/


/*
void Enemy::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
	Vector3 diff = m_player->GetPosition() - m_position;
	
	//�v���C���[����������B
	if (SearchPlayer() == true)
	{
		//�x�N�g���𐳋K������B
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff * 250.0f;
		//�U���ł��鋗���Ȃ�B
		if (IsCanAttack() == true)
		{
			//�����ɂ���āA�U�����邩�ҋ@�����邩�����肷��B	
			int ram = rand() % 100;
			if (ram > 30)
			{
				//�U���X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//�ҋ@�X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_Idle;
				return;
			}
		
		}
		//�U���ł��Ȃ������Ȃ�B
		else 
		{
			//�����ɂ���āA�ǐՂ����邩���@�U�������邩���肷��B	
			int ram = rand() % 100;
			if (ram > 40)
			{
				//�ǐՃX�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_Chase;
				return;
			}
			else {
				//���݂����@�U���X�e�[�g�Ȃ�B
				if (m_enemyState == enEnemyState_MagicAttack)
				{
					//�A���Ŗ��@���������Ȃ��悤�ɁB
					//�ǐՃX�e�[�g�ɑJ�ڂ���B
					m_enemyState = enEnemyState_Chase;
					return;
				}
				//���݂����@�U���X�e�[�g�łȂ��Ȃ�B
				else
				{
					//���@�U���X�e�[�g�ɑJ�ڂ���B
					m_enemyState = enEnemyState_MagicAttack;
					//���@�r���̃G�t�F�N�g���Đ�����B
					EffectEmitter* effect = NewGO<EffectEmitter>(0);
					effect->Init(2);
					Vector3 effectPosition = m_position;
					//���W��ݒ肷��B
					effect->SetPosition(m_position);
					//�傫����ݒ肷��B
					effect->SetScale(Vector3::One * 10.0f);
					effect->Play();
					return;
				}
			}
		}
	}
	//�v���C���[���������Ȃ���΁B
	else
	{
		//�ҋ@�X�e�[�g�ɑJ�ڂ���B
		m_enemyState = enEnemyState_Idle;
		return;
	
	}
}
*/

/*
void Enemy::ProcessDownStateTransition()
{
	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		Game* game = FindGO<Game>("game");
		//�|���ꂽ�G�l�~�[�̐���+1����B
		game->AddDefeatedEnemyNumber();
		//���g���폜����B
		DeleteGO(this);
	}
}
*/
/*
void Enemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_enemyState)
	{
	//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
	//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		m_modelRender.SetAnimationSpeed(1.2f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
	//�U���X�e�[�g�̎��B
	case enEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(1.6f);
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	//���@�U���X�e�[�g�̎��B
	case enEnemyState_MagicAttack:
		m_modelRender.SetAnimationSpeed(1.2f);
		//���@�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_MagicAttack, 0.1f);
		break;
	//��_���[�W�X�e�[�g�̎��B
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.3f);
		//��_���[�W�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
	//�_�E���X�e�[�g�̎��B
	case enEnemyState_Down:
		//�_�E���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}
*/

void Enemy::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
}