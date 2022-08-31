#pragma once
#include "EnemyState.h"

class IdleState : public EnemyState
{
public:
	IdleState(){}
	~IdleState(){}
	std::unique_ptr<EnemyState> HandleInput(Enemy* enemy, Player* player);
	void Update(Enemy* enemy);
private:

};

