#include "Loading.h"

#include <DxLib.h>

#include"../../Application/Application.h"
#include"../../Utility/Utility.h"

Loading* Loading::instance_ = nullptr;

// コンストラクタ
Loading::Loading():
	isLoading_(false),
	loadTimer_(0)
{}

// デストラクタ
Loading::~Loading()
{}

// 初期化
void Loading::Init(void)
{
	loadTimer_ = 0;
	isLoading_ = false;
}

// 読み込み
void Loading::Load(void)
{
}

// 更新
void Loading::Update(void)
{
	loadTimer_++;

	// 読込中のものがなくなったら or 最低ロード時間経過
	if (GetASyncLoadNum() == 0 && loadTimer_ >= MIN_LOAD_TIME)
	{
		// ロード終了
		Init();
	}
	// 読み込み中
	else
	{
		// ロード画面を動作させるならここに記述
	}
}

// 描画
void Loading::Draw(void)
{
	int fontSize = 50;
	SetFontSize(fontSize);
	DrawString(0, 0, "ロード中", 0xffffff);
	SetFontSize(16);
}

// 解放
void Loading::Release(void)
{
}

// 非同期読み込みに切り替える
void Loading::StartAsyncLoad(void)
{
	isLoading_ = true;
	// 非同期読み込み開始
	SetUseASyncLoadFlag(true);
}

// 同期読み込みに切り替える
void Loading::EndAsyncLoad(void)
{
	// 非同期読み込み終了
	SetUseASyncLoadFlag(false);
}
