#include"GameScene.h"

#include<DxLib.h>
#include<cmath>

#include"../../Application/Application.h"
#include"../../Manager/Input/KeyManager.h"
#include"../../scene/SceneManager/SceneManager.h"
#include"../../Manager/Sound/SoundManager.h"
#include"../../Object/Enemy/Enemynormal.h"

int GameScene::hitStop_ = 0;

int GameScene::slow_ = 0;
int GameScene::slowInter_ = 0;

int GameScene::shake_ = 0;
ShakeKinds GameScene::shakeKinds_ = ShakeKinds::DIAG;
ShakeSize GameScene::shakeSize_ = ShakeSize::MEDIUM;

GameScene::GameScene():
	mainScreen_(-1),
	stage_(nullptr),
	player_(nullptr),
	collision_(nullptr)
{
}

GameScene::~GameScene()
{
}

void GameScene::Load(void)
{
	this->Release();

	// 画面演出用の一時画面
	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

	// 当たり判定
	collision_ = new Collision();

	// ステージ
	stage_ = new StageBase();
	stage_->Load();

	// プレイヤー
	player_ = new Player(this);
	player_->Load();

}

void GameScene::Init(void)
{
#pragma region 画面演出用変数の初期化
	// ヒットストップカウンターの初期化
	hitStop_ = 0;

	// スローカウンターの初期化
	slow_ = 0;
	slowInter_ = 5;

	// 画面揺れ関係の初期化-----------------------------------------------------------
	shake_ = 0;
	shakeKinds_ = ShakeKinds::DIAG;
	shakeSize_ = ShakeSize::MEDIUM;
	//--------------------------------------------------------------------------------

	startTimer_ = GetNowCount();
	limitTime_;
	isClear = false;
#pragma endregion

	stage_->Init();
	player_->Init();
}

void GameScene::Update(void)
{
	// 敵の更新
	size_t size = enemy_.size(); // 敵のテーブルの要素数を取得
	for (int ii = 0; ii < size; ii++) {
		enemy_[ii]->Update();
	}

	// エンカウンター
	if (stage_->GetMapType() == StageBase::MAP_TYPE::E_MIYPE_GROUND)enCounter++;
	if (enCounter > ENCOUNT) {

		// 敵の生成
		Enemy* e = nullptr;

		// ランダムに種別を決める
		int rr = GetRand(static_cast<int>(Enemy::ENEMY_TYPE::E_TYPE_MAX) - 1);
		Enemy::ENEMY_TYPE rType = static_cast<Enemy::ENEMY_TYPE>(rr);
		// 種別に対応した派生クラスのインスタンスを生成
		switch (rType) {
		case Enemy::ENEMY_TYPE::E_TYPE_NORMAL:
			e = new Enemynormal();
			break;
			//case EnemyBase::ENEMY_TYPE::E_TYPE_FLY:
			//	e = new EnemyFly();
			//	break;
			//case EnemyBase::ENEMY_TYPE::E_TYPE_FIRE:
			//	e = new EnemyFire();
			//	break;
			//case EnemyBase::ENEMY_TYPE::E_TYPE_LIZARD_SMALL:
			//	e = new EnemyLizardSmall();
			//	break;
			//case EnemyBase::ENEMY_TYPE::E_TYPE_LIZARD_BIG:
			//	e = new EnemyLizardBig();
			//	break;
			//case EnemyBase::ENEMY_TYPE::E_TYPE_DRAGON:
			//	e = new EnemyDragon();
			//	break;
			//case EnemyBase::ENEMY_TYPE::E_TYPE_BOSS:
			//	e = new EnemyBoss();
			//	break;
		}

		if (e != nullptr) {
			e->Load();
			e->Init();
			// 可変長配列に要素を追加する
			enemy_.push_back(e);
			enCounter = 0; // エンカウンターをリセット
		}
	}

#pragma region 画面演出
	if (hitStop_ > 0) { hitStop_--; return; }
	if (shake_ > 0) { shake_--; }
	if (slow_ > 0) {
		slow_--;
		if (slow_ % slowInter_ != 0) { return; }
	}
#pragma endregion

#pragma region 更新処理

	stage_->Update();
	player_->Update();

	// 当たり判定
	collision_->Check();
#pragma endregion

#pragma region デバッグ用シーン遷移
	if (KEY::GetIns().GetInfo(KEY_TYPE::GAME_END).down) {
		SceneManager::GetIns().ChangeScene(SCENE_ID::TITLE);
		return;
	}
	if (KEY::GetIns().GetInfo(KEY_TYPE::ENTER).down) {
		SceneManager::GetIns().ChangeScene(SCENE_ID::CLEAR);
		return;
	}

	int elapsed = GetNowCount() - startTimer_;
	int remain = limitTime_ - elapsed;

	if (remain <= 0 && !isClear) {
		isClear = true;
		SceneManager::GetIns().ChangeScene(SCENE_ID::CLEAR);
		return;
	}


#pragma endregion
}

void GameScene::Draw(void)
{
#pragma region 描画先変更(画面演出用)
	SetDrawScreen(mainScreen_);
	ClearDrawScreen();
#pragma endregion

#pragma region 描画処理
	// 汎用定数～～～～～～～～～
	using app = Application;
	int xx = app::SCREEN_SIZE_X;
	int yy = app::SCREEN_SIZE_Y;
	int x = xx / 2;
	int y = yy / 2;
	//～～～～～～～～～～～～～

	stage_->Draw();
	player_->Draw();

	int elapsed = GetNowCount() - startTimer_;
	int remain = max(0, limitTime_ - elapsed);
	DrawFormatString(300, 0, GetColor(255, 255, 255),
		"残り時間：%.2f秒", remain / 1000.0f);

	int fontSize = 50;
	SetFontSize(fontSize);
	DrawString(0, 0, "ゲームシーン", 0xffffff);
	SetFontSize(16);

#pragma endregion

#pragma region メイン画面への演出も含めた描画
	SetDrawScreen(DX_SCREEN_BACK);

	Vector2I s = ShakePoint();
	DrawGraph(s.x, s.y, mainScreen_, true);
#pragma endregion
}

void GameScene::Release(void)
{
	if (player_) {
		player_->Release();
		delete player_;
		player_ = nullptr;
	}
	if (stage_) {
		stage_->Release();
		delete stage_;
		stage_ = nullptr;
	}

	if (collision_) {
		collision_->Clear();
		delete collision_;
		collision_ = nullptr;
	}

	DeleteGraph(mainScreen_);
}

void GameScene::Shake(ShakeKinds kinds, ShakeSize size, int time)
{
	if ((abs(shake_ - time) > 10) || shake_ <= 0)shake_ = time;
	shakeKinds_ = kinds;
	shakeSize_ = size;
}

Vector2I GameScene::ShakePoint(void)
{
	Vector2I ret = {};

	if (shake_ > 0) {
		int size = shake_ / 5 % 2;
		size *= 2;
		size -= 1;
		switch (shakeKinds_)
		{
		case GameScene::WID:ret.x = size;
			break;
		case GameScene::HIG:ret.y = size;
			break;
		case GameScene::DIAG:ret = size;
			break;
		case GameScene::ROUND:
			size = shake_ / 3 % 12; size++;
			ret = { (int)((shakeSize_ * 1.5f) * cos(size * 30.0f)),(int)((shakeSize_ * 1.5f) * sin(size * 30.0f)) };
			break;
		}

		if (shakeKinds_ != ShakeKinds::ROUND) { ret *= shakeSize_; }

		DrawGraph(0, 0, mainScreen_, true);
	}

	return ret;
}