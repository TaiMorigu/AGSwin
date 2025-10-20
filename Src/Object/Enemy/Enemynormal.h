#pragma once
#include "Enemy.h"
class Enemynormal : public Enemy
{
public:
	Enemynormal(GameScene* gs, const Vector2& playerPos) :Enemy(gs,playerPos) {}
	void SetEnemyParam(void);

	void OnCollision(UnitBase* other)override;
};

