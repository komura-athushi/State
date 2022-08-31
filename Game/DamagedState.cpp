#include "stdafx.h"
#include "DamagedState.h"

#include "IdleState.h"

#include "Enemy.h"

std::unique_ptr<EnemyState> DamagedState::HandleInput(Enemy* enemy, Player* player)
{
	if (enemy->IsPlayAnimation())
	{
		return nullptr;
	}
	return std::make_unique<IdleState>();
}

void DamagedState::Update(Enemy* enemy)
{
	enemy->SetVelocity(Vector3::Zero);
	enemy->SetPlayAnimation(Enemy::enAnimationClip_Damage, 1.6f);
	return;
}
