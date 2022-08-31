#include "stdafx.h"
#include "ChaseState.h"

#include "AttackState.h"
#include "IdleState.h"
#include "DamagedState.h"
#include "DownState.h"

#include "Enemy.h"
#include "Player.h"

std::unique_ptr<EnemyState> ChaseState::HandleInput(Enemy* enemy, Player* player)
{
	if (enemy->IsDamaged())
	{
		if (enemy->HPIsBelowZero())
		{
			return std::make_unique<DownState>();
		}
		return std::make_unique<DamagedState>();
	}
	
	if (enemy->IsSearchPlayer())
	{
		if (enemy->IsRangeAttackPlayer())
		{
			return std::make_unique<AttackState>();
		}
		m_toTarget = player->GetPosition() - enemy->GetPosition();
		return nullptr;
	}
	
	return std::make_unique<IdleState>();
}

void ChaseState::Update(Enemy* enemy)
{
	m_toTarget.Normalize();
	enemy->SetVelocity(m_toTarget * 60.0f);
	enemy->SetPlayAnimation(Enemy::enAnimationClip_Walk, 1.0f);
	return;
}
