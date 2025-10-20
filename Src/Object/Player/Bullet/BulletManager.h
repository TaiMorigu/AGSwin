#pragma once

#include<vector>
#include"Bullet.h"

class GameScene;

class BulletManager
{
public:
	BulletManager(GameScene* gInst, const Vector2& playerPos, const Utility::DIRECTION& playerDir);
	~BulletManager();

	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void Create(Bullet::BulletType type);

private:
	GameScene* gInst;
	const Vector2& playerPos;
	const Utility::DIRECTION& playerDir;
	int image_;
	std::vector<Bullet*> bullets_;
};