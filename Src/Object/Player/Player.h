#pragma once

#include"../UnitBase.h"

#include "Bullet/BulletManager.h"

class GameScene;

class Player : public UnitBase
{
public:
	static constexpr int PLAYER_WID = 96; // プレイヤーの横サイズ
	static constexpr int PLAYER_HIG = 84; // プレイヤーの縦サイズ
	static constexpr int ANIM_NUMS = 7; // 方向毎のアニメーション数
	static constexpr int CHARA_MAX = ANIM_NUMS * static_cast<int>(Utility::DIRECTION::E_DIR_MAX);
	static constexpr int MOVE_SPEED = 4; // 一回の移動量
	static constexpr int ANIM_INTERVAL = 10; // アニメーションの更新間隔
	static constexpr int PLAYER_HP_MAX = 100; // プレイヤーのHPの最大値
	Player(GameScene* gs);
	~Player(void);
	void Load(void)override;	
	void Init(void)override;	
	void Update(void)override;	
	void Draw(void)override;	
	void Release(void)override; 

	// ゲッター・セッター関数
	void SetPlayerPos(Vector2 mPos) { unit_.pos_ = mPos; }
	Utility::DIRECTION GetPlayerDir(void) { return playerDir; }
	void SetDamage(int dp);


	void OnCollision(UnitBase* other)override;

private:
	GameScene* gInst;
	BulletManager* bullet; // 弾クラスのインスタンスのポインタ
	// プレイヤー画像
	int player_img[ANIM_NUMS];

	// プレイヤーが向いている方向
	Utility::DIRECTION playerDir;
	// アニメーションカウンター
	int animCounter;

	const int SHOT_INTERVAL = 30; // 弾の発射間隔
	int shotCounter = 0; // 弾の発射間隔カウンター
};

