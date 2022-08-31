#pragma once
#include "EnemyState.h"

class AttackState : public EnemyState
{
public:
	AttackState() {}
	~AttackState() {}
	std::unique_ptr<EnemyState> HandleInput(Enemy* enemy, Player* player);
	void Update(Enemy* enemy);
private:

};
