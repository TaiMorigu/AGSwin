#include"BulletManager.h"

BulletManager::BulletManager(GameScene* gInst, const Vector2& playerPos, const Utility::DIRECTION& playerDir):
	gInst(gInst),
	playerPos(playerPos),
	playerDir(playerDir),

	bullets_(),
	image_(-1)
{
}

BulletManager::~BulletManager()
{
}

void BulletManager::Load(void)
{
	image_ = LoadGraph("Data/Image/object/weapon/sword.png");
}

void BulletManager::Init(void)
{
	bullets_.reserve(100);
}

void BulletManager::Update(void)
{
	for (auto& b : bullets_) { b->Update(); }


}

void BulletManager::Draw(void)
{
	for (auto& b : bullets_) { b->Draw(); }
}

void BulletManager::Release(void)
{
	for (auto& b : bullets_) {
		if (!b) {continue;}
		b->Release();
		delete b;
		b = nullptr;
	}
	bullets_.clear();
}

void BulletManager::Create(Bullet::BulletType type)
{
	bool recycle = false;

	for (auto& b : bullets_) {
		if (b->GetUnit().isAlive_ == false) {
			recycle = true;
			switch (type)
			{
			case Bullet::BulletType::NORMAL:
				b->Create(playerPos, playerDir);
				break;
			case Bullet::BulletType::ORBIT:
				b->CreateOrbit(80.0f, 0.1f, 600);
				break;
			}
			break;
		}
	}

	if (recycle) { return; }

	bullets_.emplace_back(new Bullet(gInst, playerPos));
	bullets_.back()->ImageLoad(image_);
	bullets_.back()->Load();
	bullets_.back()->Init();
	switch (type)
	{
	case Bullet::BulletType::NORMAL:
		bullets_.back()->Create(playerPos, playerDir);
		break;
	case Bullet::BulletType::ORBIT:
		bullets_.back()->CreateOrbit(80.0f, 0.1f, 600);
		break;
	}
}
