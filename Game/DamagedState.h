#pragma once
#include "EnemyState.h"

class DamagedState : public EnemyState
{
public:
	DamagedState() {}
	~DamagedState() {}
	std::unique_ptr<EnemyState> HandleInput(Enemy* enemy, Player* player);
	void Update(Enemy* enemy);
private:

};
