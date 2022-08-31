#include "stdafx.h"
#include "DownState.h"

#include "Enemy.h"

std::unique_ptr<EnemyState> DownState::HandleInput(Enemy* enemy, Player* player)
{
	if (enemy->IsPlayAnimation())
	{
		return nullptr;
	}
	enemy->Death();
	return nullptr;
}

void DownState::Update(Enemy* enemy)
{
	enemy->SetVelocity(Vector3::Zero);
	enemy->SetPlayAnimation(Enemy::enAnimationClip_Down, 1.6f);
	return;
}
