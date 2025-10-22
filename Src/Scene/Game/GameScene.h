#pragma once

#include<vector>
#include<list>
#include"../SceneBase.h"

#include"../../Common/Vector2.h"

#include"../../Manager/Collision/Collision.h"


#include"../../Object/Stage/StageBase.h"
#include"../../Object/Player/Player.h"
#include"../../Object/Enemy/EnemyManager.h"


class GameScene : public SceneBase
{
public:
	GameScene();
	~GameScene()override;

	// 読み込み
	void Load(void)override;
	// 初期化処理
	void Init(void)override;
	// 更新ステップ
	void Update(void)override;
	// 描画処理
	void Draw(void)override;
	// 解放処理
	void Release(void)override;


	// ヒットストップ演出
	static void HitStop(int time = 20) { hitStop_ = time; }

	// スロー演出
	static void Slow(int time = 10, int inter = 5) { slow_ = time; slowInter_ = inter; }

	static constexpr int ENCOUNT = 60;

	// 画面揺れの種類
	enum ShakeKinds { WID/*横揺れ*/, HIG/*縦揺れ*/, DIAG/*斜め揺れ*/, ROUND/*くるくる*/ };
	// 画面揺れの大きさ
	enum ShakeSize { SMALL = 3/*小さく*/, MEDIUM = 5/*中くらい*/, BIG = 8, /*大きく*/ };

	/// <summary>
	/// 画面揺らし
	/// </summary>
	/// <param name="kinds">揺れ方(enum ShakeKinds を使用)</param>
	/// <param name="size">揺れる大きさ(enum ShakeSize を使用)</param>
	/// <param name="time">揺れる時間(フレーム数)</param>
	static void Shake(ShakeKinds kinds = ShakeKinds::DIAG, ShakeSize size = ShakeSize::MEDIUM, int time = 20);

	StageBase* GetLpStage(void) { return stage_; }

private:

	Collision* collision_;

	StageBase* stage_;
	Player* player_;
	EnemyManager* enemy_;

	// ヒットストップカウンター
	static int hitStop_;

	// スローカウンター
	static int slow_;
	static int slowInter_;

    int startTimer_;
	int limitTime_ = 20000;
	bool isClear;

	int spawnCounter = 0; // 敵の出現間隔カウンター

	// 画面揺れ------------------------
	int mainScreen_;
	static int shake_;
	static ShakeKinds shakeKinds_;
	static ShakeSize shakeSize_;
	Vector2I ShakePoint(void);
	//---------------------------------
};


using ShakeKinds = GameScene::ShakeKinds;
using ShakeSize = GameScene::ShakeSize;