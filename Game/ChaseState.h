#pragma once

#include "EnemyState.h"

class ChaseState : public EnemyState
{
public:
	ChaseState() {}
	~ChaseState() {}
	std::unique_ptr<EnemyState> HandleInput(Enemy* enemy, Player* player);
	void Update(Enemy* enemy);
private:
	Vector3				m_toTarget = Vector3::Zero;
};

