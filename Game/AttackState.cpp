#include "stdafx.h"
#include "AttackState.h"

#include "IdleState.h"
#include "DamagedState.h"
#include "DownState.h"

#include "Enemy.h"

std::unique_ptr<EnemyState> AttackState::HandleInput(Enemy* enemy, Player* player)
{
	if (enemy->IsDamaged())
	{
		if (enemy->HPIsBelowZero())
		{
			return std::make_unique<DownState>();
		}
		return std::make_unique<DamagedState>();
	}

	if (enemy->IsPlayAnimation())
	{
		return nullptr;
	}
	return std::make_unique<IdleState>();
}

void AttackState::Update(Enemy* enemy)
{
	enemy->SetVelocity(Vector3::Zero);
	enemy->SetPlayAnimation(Enemy::enAnimationClip_Attack, 1.6f);
	if (enemy->GetIsUnderAttack())
	{
		enemy->MakeAttackCollision();
	}
	return;
}
