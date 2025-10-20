#pragma once

#include"../Common/Vector2.h"
#include"../Utility/Utility.h"

enum class CollisionShape { NON = -1, CIRCLE, RECTANGLE, ELLIPSE, MAX };

struct Base
{
	//パラメーター
	struct {
		CollisionShape colliShape = CollisionShape::NON;	//当たり判定の形

		Vector2 size = { -1.0f,-1.0f };						//サイズ
		float radius = -1.0f;								//半径
		Vector2 center = { 0.0f,0.0f };						//中心
		float speed = -1;									//スピード
	}para_;

	// 生存フラグ
	bool isAlive_ = false;

	// 生存フラグによる当たり判定の早期リターンを行うかどうか
	bool aliveCollision_ = true;

	// 座標
	Vector2 pos_ = { -1.0f,-1.0f };


	// ヒットポイント
	int hp_ = -1;


	// 無敵判定
	bool isInvici_ = true;
	int inviciCounter_ = 0;
};

class UnitBase
{
public:
	UnitBase();
	virtual ~UnitBase() = 0;

	virtual void Load(void) = 0;
	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;

	const Base& GetUnit(void)const { return unit_; }

	virtual void OnCollision(UnitBase* other) = 0;

protected:
	Base unit_;
	void Invi(void);
};

