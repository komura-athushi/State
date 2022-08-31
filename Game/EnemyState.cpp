#include "stdafx.h"
#include "EnemyState.h"

#include "Enemy.h"

void EnemyState::Enter(Enemy* enemy)
{
	enemy->SetIsUnderAttack(false);
}
