#include"TitleScene.h"

#include<DxLib.h>

#include"../../Utility/Utility.h"

#include"../../Application/Application.h"
#include"../SceneManager/SceneManager.h"
#include"../../Manager/Sound/SoundManager.h"
#include"../../Manager/Input/KeyManager.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Load(void)
{
}
void TitleScene::Init(void)
{
}
void TitleScene::Update(void)
{

#pragma region デバッグ用シーン遷移
	if (KEY::GetIns().GetInfo(KEY_TYPE::ENTER).down) {
		SceneManager::GetIns().ChangeScene(SCENE_ID::GAME);
		return;
	}
	if (KEY::GetIns().GetInfo(KEY_TYPE::GAME_END).down) {
		Application::GetIns().GameEnd();
		return;
	}
#pragma endregion
}
void TitleScene::Draw(void)
{
	using app = Application;
	int xx = app::SCREEN_SIZE_X;
	int yy = app::SCREEN_SIZE_Y;
	int x = xx / 2;
	int y = yy / 2;

	int fontSize = 50;
	SetFontSize(fontSize);
	DrawString(0, 0, "タイトルシーン", 0xffffff);
	SetFontSize(16);
}

void TitleScene::Release(void)
{
}