#include "Player.h"

#include <DxLib.h>

#include "../../Application/Application.h"
#include "../../Manager/Input/InputManager.h"
#include "../../Manager/Input/KeyManager.h"

#include"../../Scene/Game/GameScene.h"

#include"../Stage/StageBase.h"

Player::Player(GameScene* gs)
{
	gInst = gs;
}
Player::~Player(void)
{
}
// 初期化処理(最初の１回のみ実行)
void Player::Load(void)
{
	int err = LoadDivGraph("Data/Image/object/Player/player_stand.png",
		ANIM_NUMS,     // 総コマ数
		ANIM_NUMS,     
		1,             // 縦方向
		PLAYER_WID,    // 112
		PLAYER_HIG,    // 84
		player_img // 配列先頭
	);

	if (err == -1)
	{
		printfDx("player_stand.png 読み込み失敗\n");
	}

	unit_.para_.colliShape = CollisionShape::RECTANGLE;
	unit_.para_.size = { static_cast<float>(PLAYER_WID), static_cast<float>(PLAYER_HIG) };

	bullet = new BulletManager(gInst, unit_.pos_, playerDir);
	bullet->Load();
}
// ゲーム起動・再開時に必ず呼び出す処理
void Player::Init(void)
{
	unit_.pos_.x = PLAYER_WID / 2 + StageBase::MAP_CHIP_SIZE_WID;
	unit_.pos_.y = PLAYER_HIG / 2 + StageBase::MAP_CHIP_SIZE_HIG;
	// デフォルトのプレイヤーの向き
	playerDir = Utility::DIRECTION::E_DIR_DOWN;
	animCounter = 0;
	unit_.isAlive_ = true;
	unit_.hp_ = PLAYER_HP_MAX;

	bullet->Init();
	shotCounter = SHOT_INTERVAL; // 最初から撃てるようにする
}
// 更新処理
void Player::Update(void)
{
    animCounter++;
    if (animCounter > (ANIM_NUMS * ANIM_INTERVAL) * 10) animCounter = 0;

    float moveX = 0.0f;
    float moveY = 0.0f;

    // -----------------------------
    // キーボード入力
    if (CheckHitKey(KEY_INPUT_W)) moveY -= MOVE_SPEED;
    if (CheckHitKey(KEY_INPUT_S)) moveY += MOVE_SPEED;
    if (CheckHitKey(KEY_INPUT_A)) moveX -= MOVE_SPEED;
    if (CheckHitKey(KEY_INPUT_D)) moveX += MOVE_SPEED;

    // -----------------------------
    // ゲームパッド入力（PAD1固定）
    auto& pad = InputManager::GetInstance();
    auto padState = pad.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

    // アナログスティック
    moveX += (padState.AKeyLX / 32768.0f) * MOVE_SPEED;
    moveY += (padState.AKeyLY / 32768.0f) * MOVE_SPEED;

    // 方向ボタン（優先度高）
    if (pad.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::TOP)) moveY = -MOVE_SPEED;
    if (pad.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN)) moveY = MOVE_SPEED;
    if (pad.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT)) moveX = -MOVE_SPEED;
    if (pad.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)) moveX = MOVE_SPEED;

    // -----------------------------
    // 移動適用
    unit_.pos_.x += moveX;
    unit_.pos_.y += moveY;

    // 画面端補正（ワールド座標）
    if (unit_.pos_.x < PLAYER_WID / 2) unit_.pos_.x = PLAYER_WID / 2;
    if (unit_.pos_.x > (StageBase::MAP_CHIP_SIZE_WID * StageBase::MAP_GROUND_NUM_X) - PLAYER_WID / 2)
        unit_.pos_.x = (StageBase::MAP_CHIP_SIZE_WID * StageBase::MAP_GROUND_NUM_X) - PLAYER_WID / 2;

    if (unit_.pos_.y < PLAYER_HIG / 2) unit_.pos_.y = PLAYER_HIG / 2;
    if (unit_.pos_.y > (StageBase::MAP_CHIP_SIZE_HIG * StageBase::MAP_GROUND_NUM_Y) - PLAYER_HIG / 2)
        unit_.pos_.y = (StageBase::MAP_CHIP_SIZE_HIG * StageBase::MAP_GROUND_NUM_Y) - PLAYER_HIG / 2;

    // -----------------------------
    // 向き設定（移動方向優先）
    if (moveX < 0) playerDir = Utility::DIRECTION::E_DIR_LEFT;
    else if (moveX > 0) playerDir = Utility::DIRECTION::E_DIR_RIGHT;
    if (moveY < 0) playerDir = Utility::DIRECTION::E_DIR_UP;
    else if (moveY > 0) playerDir = Utility::DIRECTION::E_DIR_DOWN;


	if (++shotCounter > SHOT_INTERVAL) {
		shotCounter = 0;
		bullet->Create(Bullet::BulletType::NORMAL);
	}
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Z)) {
		bullet->Create(Bullet::BulletType::ORBIT);
	}
    // 弾の更新
    bullet->Update();
}
// 描画処理
void Player::Draw(void)
{
	int animNo = (animCounter / ANIM_INTERVAL) % ANIM_NUMS;
	Vector2I stpos = gInst->GetLpStage()->GetMapDispStPos();
	DrawGraph(unit_.pos_.x - PLAYER_WID / 2 - (stpos.x * StageBase::MAP_CHIP_SIZE_WID),
		unit_.pos_.y - PLAYER_HIG / 2 - (stpos.y * StageBase::MAP_CHIP_SIZE_HIG),
		player_img[animNo], true);

	for (int i = 0;i < unit_.hp_; i++) {
		DrawBox(50 + (i * 5), Application::SCREEN_SIZE_Y-30, 70 + (i + 5), Application::SCREEN_SIZE_X, GetColor(255, 1, 1), true);
	}

	bullet->Draw();

	
}
// 解放処理(最後の１回のみ実行)
void Player::Release(void)
{
	if (bullet) {
		bullet->Release();
		delete bullet;
		bullet = nullptr;
	}

	for (auto& id : player_img) {
		if (DeleteGraph(id) == -1) { 
			printfDx("player_stand.png 解放失敗\n");
		}
	}

}
// プレイヤーにダメージを与える
// Input:
// int dp : ダメージ
// Ountput:
// 無し
void Player::SetDamage(int dp)
{
	unit_.hp_ -= dp;
	if (unit_.hp_ <= 0) {
		unit_.hp_ = 0;
		unit_.isAlive_ = false;
	}
}

void Player::OnCollision(UnitBase* other)
{
}

