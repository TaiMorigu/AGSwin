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
	std::vector<Enemy*> enemys_;
	const Vector2& playerPos;
	const Utility::DIRECTION& playerDir;

	int image_;

	// 敵の画像ファイル名
	std::string imgFName;
	// 敵の画像サイズ
	Vector2I size;

};