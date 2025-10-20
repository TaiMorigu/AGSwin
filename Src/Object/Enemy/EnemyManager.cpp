#include"EnemyManager.h"
#include"Enemynormal.h"

EnemyManager::EnemyManager(GameScene* gInst, const Vector2& playerPos, const Utility::DIRECTION& playerDir):
	gInst(gInst),
	playerPos(playerPos),
	playerDir(playerDir),

	enemys_(),
	image_(-1)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Load(void)
{
	
}

void EnemyManager::Init(void)
{
	enemys_.reserve(100);
}

void EnemyManager::Update(void)
{
	for (auto& e : enemys_) { e->Update(); }
}

void EnemyManager::Draw(void)
{
	for (auto& e : enemys_) { e->Draw(); }
}

void EnemyManager::Release(void)
{
	for (auto& e : enemys_) {
		if (!e) { continue;}
		e->Release();
		delete e;
		e = nullptr;
	}
	enemys_.clear();
}

void EnemyManager::Create(Enemy::ENEMY_TYPE type)
{
	bool recycle = false;
	
	for (auto& e : enemys_) {
		if (e->GetUnit().isAlive_ == false) {
			recycle = true;
			switch (type)
			{
			case Enemy::ENEMY_TYPE::E_TYPE_NORMAL:
				e->Init();
				break;
			}
			break;
		}
	}

	if (recycle) { return;}

	enemys_.emplace_back(new Enemynormal(gInst, playerPos));
	enemys_.back()->Load();
	enemys_.back()->Init();

	switch (type)
	{
	case Enemy::ENEMY_TYPE::E_TYPE_NORMAL:
		enemys_.back()->Init();
		break;
	}
}
