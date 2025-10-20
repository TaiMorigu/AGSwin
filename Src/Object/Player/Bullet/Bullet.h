#pragma once

#include"../../UnitBase.h"

class GameScene;

class Bullet : public UnitBase
{
public:
	static constexpr int BULLET_SIZE_WID = 60; // 弾の横サイズ
	static constexpr int BULLET_SIZE_HIG = 40; // 弾の縦サイズ
	//static constexpr int ANIM_NUMS = 2; // 弾のアニメーション数
	static constexpr float MOVE_SPEED = 5.0f; // 移動速度
	static constexpr int ANIM_INTERVAL = 5; // 弾のアニメーションの更新間隔
	static constexpr int ALIVE_TIME = 30; // 弾の生存時間
	static constexpr int BLAST_SIZE_WID = 32; // 爆発のサイズ
	static constexpr int BLAST_SIZE_HIG = 32;
	static constexpr int BLAST_ANIM_XNUM = 4; // 爆発画像ファイル内の横方向のパターン数
	static constexpr int BLAST_ANIM_YNUM = 4; // 爆発画像ファイル内の縦方向のパターン数
	static constexpr int BLAST_ANIM_MAX = BLAST_ANIM_XNUM * BLAST_ANIM_YNUM;
	// 爆発アニメーションのパターン数
	static constexpr int BLAST_ANIM_INTERVAL = 2; // 爆発アニメーションの更新間隔

	// 弾の状態
	enum class STATUS {
		E_STAT_NONE,
		E_STAT_MOVE, // 弾の移動
		E_STAT_BLAST, // 爆発
		E_STAT_END, // 弾の動作終了
		E_STAT_MAX,
		
	};

	enum class BulletType {
		NORMAL,
		ORBIT,
	};

	Bullet(GameScene* gs, const Vector2& playerPos);
	~Bullet(void);
	void ImageLoad(int image) { img = image; }
	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;

	// 弾を生成する
	void Create(Vector2 pos, Utility::DIRECTION dir);
	// 弾の生成が可能かどうか
	bool IsEnableCreate(void);
	// 弾が発射状態かどうかを取得
	// bool : true = 発射中 / false = 日発射状態
	bool IsShotState(void) { return bNowStat == STATUS::E_STAT_MOVE; }
	// 弾の座標を取得
	
	// 爆発表示開始
	void BlastOn(Vector2 pos);
	void CreateOrbit(float rad, float speed, int time);


	void OnCollision(UnitBase* other)override;

	
private:
	GameScene* gInst; // ゲームシーンクラスのインスタンスのポインタ
	const Vector2& playerPos;

	int img; // 弾の画像のハンドル番号テーブル
	int blastImage[BLAST_ANIM_MAX]; // 爆発画像のハンドル番号テーブル

	// 円運動用
	bool isOrbit = false;         // 円運動中かどうか
	float radius = 50.0f;         // 回る半径
	float angle = 0.0f;           // 現在の角度
	float angularSpeed = 0.1f;    // 回転速度（ラジアン/frame）
	int orbitTime = 120;           // 回る時間（フレーム）

	BulletType bulletType = BulletType::NORMAL;

	Vector2 bVec; // 弾の移動方向のベクトル(単位ベクトル)

	STATUS bNowStat; // 弾の状態

	int bDir; // 弾の移動方向
	int aliveCounter; // 弾の生存時間のカウンター

	int blastAnimCounter; // 爆発のアニメーションカウンター

	Vector2 blastPos; // 爆発の座標

	// 弾の状態毎の更新処理
	void UpdateMove(void);
	void UpdateBlast(void);
	void UpdateEnd(void);
	// 弾の状態毎の描画処理
	void DrawMove(void);
	void DrawBlast(void);
	void DrawEnd(void);
	// 状態を遷移させる
	void ChangeStatus(STATUS stat);

	
};


