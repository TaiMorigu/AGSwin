#include"GameScene.h"

#include<DxLib.h>
#include<cmath>

#include"../../Application/Application.h"
#include"../../Manager/Input/KeyManager.h"
#include"../../scene/SceneManager/SceneManager.h"
#include"../../Manager/Sound/SoundManager.h"

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

	// âÊñ ââèoópÇÃàÍéûâÊñ 
	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

	// ìñÇΩÇËîªíË
	collision_ = new Collision();

	// ÉXÉeÅ[ÉW
	stage_ = new StageBase();
	stage_->Load();

	// ÉvÉåÉCÉÑÅ[
	player_ = new Player(this);
	player_->Load();

	enemy_ = new Enemy();

}

void GameScene::Init(void)
{
#pragma region âÊñ ââèoópïœêîÇÃèâä˙âª
	// ÉqÉbÉgÉXÉgÉbÉvÉJÉEÉìÉ^Å[ÇÃèâä˙âª
	hitStop_ = 0;

	// ÉXÉçÅ[ÉJÉEÉìÉ^Å[ÇÃèâä˙âª
	slow_ = 0;
	slowInter_ = 5;

	// âÊñ óhÇÍä÷åWÇÃèâä˙âª-----------------------------------------------------------
	shake_ = 0;
	shakeKinds_ = ShakeKinds::DIAG;
	shakeSize_ = ShakeSize::MEDIUM;
	//--------------------------------------------------------------------------------

	startTimer_ = GetNowCount();
	limitTime_ = 90000;
	isClear = false;
#pragma endregion

	stage_->Init();
	player_->Init();
}

void GameScene::Update(void)
{
#pragma region âÊñ ââèo
	if (hitStop_ > 0) { hitStop_--; return; }
	if (shake_ > 0) { shake_--; }
	if (slow_ > 0) {
		slow_--;
		if (slow_ % slowInter_ != 0) { return; }
	}
#pragma endregion

#pragma region çXêVèàóù

	stage_->Update();
	player_->Update();

	// ìñÇΩÇËîªíË
	collision_->Check();
#pragma endregion

#pragma region ÉfÉoÉbÉOópÉVÅ[ÉìëJà⁄
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
#pragma region ï`âÊêÊïœçX(âÊñ ââèoóp)
	SetDrawScreen(mainScreen_);
	ClearDrawScreen();
#pragma endregion

#pragma region ï`âÊèàóù
	// îƒópíËêîÅ`Å`Å`Å`Å`Å`Å`Å`Å`
	using app = Application;
	int xx = app::SCREEN_SIZE_X;
	int yy = app::SCREEN_SIZE_Y;
	int x = xx / 2;
	int y = yy / 2;
	//Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`

	stage_->Draw();
	player_->Draw();

	int elapsed = GetNowCount() - startTimer_;
	int remain = max(0, limitTime_ - elapsed);
	DrawFormatString(300, 0, GetColor(255, 255, 255),
		"écÇËéûä‘ÅF%.2fïb", remain / 1000.0f);

	int fontSize = 50;
	SetFontSize(fontSize);
	DrawString(0, 0, "ÉQÅ[ÉÄÉVÅ[Éì", 0xffffff);
	SetFontSize(16);

#pragma endregion

#pragma region ÉÅÉCÉìâÊñ Ç÷ÇÃââèoÇ‡ä‹ÇﬂÇΩï`âÊ
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