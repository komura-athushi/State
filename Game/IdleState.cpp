#include "stdafx.h"
#include "IdleState.h"

#include "AttackState.h"
#include "ChaseState.h"
#include "DamagedState.h"
#include "DownState.h"

#include "Enemy.h"

std::unique_ptr<EnemyState> IdleState::HandleInput(Enemy* enemy, Player* player)
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
		return std::make_unique<ChaseState>();
	}
	return nullptr;
}

void IdleState::Update(Enemy* enemy)
{
	enemy->SetVelocity(Vector3::Zero);
	enemy->SetPlayAnimation(Enemy::enAnimationClip_Idle, 1.0f);
	return;
}
