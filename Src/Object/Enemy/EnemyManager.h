#pragma once
#include<vector>
#include"Enemy.h"

class GameScene;

class EnemyManager
{
public:
	EnemyManager(GameScene* gInst, const Vector2& playerPos, const Utility::DIRECTION& playerDir);
	~EnemyManager();

	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void Create(Enemy::ENEMY_TYPE type);

private:
	GameScene* gInst;
	const Vector2& playerPos;
	const Utility::DIRECTION& playerDir;
	int image_;
	std::vector<Enemy*> enemys_;

	const int ENEMY_SPAWN_INTERVAL = 5000; // ’e‚Ì”­ËŠÔŠu
	int spawnCounter = 0; // ’e‚Ì”­ËŠÔŠuƒJƒEƒ“ƒ^[
};