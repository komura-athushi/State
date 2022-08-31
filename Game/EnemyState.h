#pragma once

class Player;
class Enemy;

class EnemyState
{
public:
	EnemyState(){}
	virtual ~EnemyState() {}
	virtual std::unique_ptr<EnemyState> HandleInput(Enemy* enemy, Player* player)=0;
	virtual void Update(Enemy* enemy)=0;
	virtual void Enter(Enemy* enemy);
private:

};