#pragma once

#include "EnemyState.h"

class DownState : public EnemyState
{
public:
	DownState() {}
	~DownState() {}
	std::unique_ptr<EnemyState> HandleInput(Enemy* enemy, Player* player);
	void Update(Enemy* enemy);
private:

};