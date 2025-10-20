#include"ClearScene.h"

#include<DxLib.h>

#include"../../Utility/Utility.h"
#include"../../Application/Application.h"
#include"../SceneManager/SceneManager.h"
#include"../../Manager/Sound/SoundManager.h"

ClearScene::ClearScene()
{
}

ClearScene::~ClearScene()
{
}

void ClearScene::Load(void)
{
}

void ClearScene::Init(void)
{
}

void ClearScene::Update(void)
{
	if (

		(CheckHitKeyAll()) ||
		((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_B) != 0) ||
		((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A) != 0)

		) { SceneManager::GetIns().ChangeScene(SCENE_ID::TITLE); }
}

void ClearScene::Draw(void)
{
	using app = Application;
	int xx = app::SCREEN_SIZE_X;
	int yy = app::SCREEN_SIZE_Y;
	int x = xx / 2;
	int y = yy / 2;

	int fontSize = 50;
	SetFontSize(fontSize);
	DrawString(0, 0, "ÉNÉäÉA", 0xffffff);
	SetFontSize(16);
}

void ClearScene::Release(void)
{
}