#include <DxLib.h>
#include "Enemynormal.h"

void Enemynormal::SetEnemyParam(void)
{
	// 敵の画像ファイル名
	imgFName += "FLYING.png";
	// 敵の画像サイズ
	size = { 81, 71 };
	unit_.para_.size = size.ToFloat();

	// 敵の移動速度
	unit_.para_.speed = 3.0f;

	unit_.hp_ = hpMax = 1;
}

void Enemynormal::OnCollision(UnitBase* other)
{
}
