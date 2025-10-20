#include "Collision.h"

#include <cmath>
#include <algorithm>

std::vector<UnitBase*> Collision::allyObjects_ = {};
std::vector<UnitBase*> Collision::enemyObjects_ = {};

Collision::Collision()
{

}

Collision::~Collision()
{
}



void Collision::Check()
{
	for (auto& ally : allyObjects_) {
		for (auto& enemy : enemyObjects_) {
			const Base& ua = ally->GetUnit();
			const Base& ue = enemy->GetUnit();

			if ((ua.aliveCollision_ && !ua.isAlive_) || (ue.aliveCollision_ && !ue.isAlive_)) continue;

			if (ua.isInvici_ && ua.inviciCounter_ > 0)continue;
			if (ue.isInvici_ && ue.inviciCounter_ > 0)continue;

			if (IsHit(ua, ue)) {
				ally->OnCollision(enemy);
				enemy->OnCollision(ally);
			}
		}
	}
}

bool Collision::IsHit(const Base& a, const Base& b)
{
	Base workA = a;
	workA.pos_ += workA.para_.center;
	Base workB = b;
	workB.pos_ += workB.para_.center;

	const auto shapeA = a.para_.colliShape;
	const auto shapeB = b.para_.colliShape;

	// 無効な形状なら当たり判定しない
	if (shapeA == CollisionShape::NON || shapeB == CollisionShape::NON) return false;

	// 同じ形状同士
	if (shapeA == CollisionShape::CIRCLE && shapeB == CollisionShape::CIRCLE)
		return Circle(workA, workB);

	if (shapeA == CollisionShape::RECTANGLE && shapeB == CollisionShape::RECTANGLE)
		return Rect(workA, workB);

	if (shapeA == CollisionShape::ELLIPSE && shapeB == CollisionShape::ELLIPSE)
		return Ellipse(workA, workB);

	// 混合パターン
	if (shapeA == CollisionShape::CIRCLE && shapeB == CollisionShape::RECTANGLE)
		return CircleAndRect(workA, workB);

	if (shapeA == CollisionShape::RECTANGLE && shapeB == CollisionShape::CIRCLE)
		return CircleAndRect(workB, workA);

	if (shapeA == CollisionShape::CIRCLE && shapeB == CollisionShape::ELLIPSE)
		return CircleAndEllipse(workA, workB);

	if (shapeA == CollisionShape::ELLIPSE && shapeB == CollisionShape::CIRCLE)
		return CircleAndEllipse(workB, workA);

	if (shapeA == CollisionShape::ELLIPSE && shapeB == CollisionShape::RECTANGLE)
		return EllipseAndRect(workA, workB);

	if (shapeA == CollisionShape::RECTANGLE && shapeB == CollisionShape::ELLIPSE)
		return EllipseAndRect(workB, workA);

	// その他の未対応パターンは false
	return false;
}




const bool Collision::Circle(const Base& u1, const Base& u2) const
{
	//2点間のベクトルを作成
	Vector2 vec = { u1.pos_.x - u2.pos_.x,u1.pos_.y - u2.pos_.y };

	//作成したベクトルの大きさを計算
	float distance = sqrtf(vec.x * vec.x + vec.y * vec.y);

	//衝突判定
	if (distance < u1.para_.radius + u2.para_.radius)return true;


	return false;
}

const bool Collision::Rect(const Base& u1, const Base& u2) const
{
	//衝突判定に必要な情報をまとめる
	struct rec
	{
		float up;
		float down;
		float left;
		float right;
	};

	rec r1, r2;
	r1 = { u1.pos_.y - u1.para_.size.y / 2,	u1.pos_.y + u1.para_.size.y / 2, u1.pos_.x - u1.para_.size.x / 2, u1.pos_.x + u1.para_.size.x / 2 };
	r2 = { u2.pos_.y - u2.para_.size.y / 2,	u2.pos_.y + u2.para_.size.y / 2, u2.pos_.x - u2.para_.size.x / 2, u2.pos_.x + u2.para_.size.x / 2 };


	//衝突判定
	if (r1.up	 < r2.down &&
		r1.down  > r2.up &&
		r1.left	 < r2.right &&
		r1.right > r2.left) {
		return true;
	}

	return false;
}

const bool Collision::Ellipse(const Base& u1, const Base& u2) const
{
	//2点間のベクトルを作成
	Vector2 vec = { u1.pos_.x - u2.pos_.x,u1.pos_.y - u2.pos_.y };

	//各方向の半径の和を計算
	Vector2 radius = { (u1.para_.size.x + u2.para_.size.x) / 2,	(u1.para_.size.y + u2.para_.size.y) / 2 };


	// 正規化された距離での楕円衝突判定（楕円空間での距離が1以下なら衝突）
	float norm = ((vec.x * vec.x) / (radius.x * radius.x)) + ((vec.y * vec.y) / (radius.y * radius.y));

	return norm <= 1.0f;
}



const bool Collision::EllipseAndRect(const Base& ellipse, const Base& rect) const
{
	// 楕円の中心と半径（XとYで違う）
	Vector2 ellipseCenter = ellipse.pos_;
	Vector2 ellipseRadius = ellipse.para_.size / 2.0f;

	// 矩形の中心と半サイズ
	Vector2 rectCenter = rect.pos_;
	Vector2 rectHalfSize = rect.para_.size / 2.0f;

	// 楕円の空間にマッピングして矩形を評価（＝矩形を楕円の単位空間にスケーリング）
	Vector2 scale = { 1.0f / ellipseRadius.x, 1.0f / ellipseRadius.y };

	// 楕円→単位円空間に変換された矩形中心
	Vector2 scaledRectCenter = {
		(rectCenter.x - ellipseCenter.x) * scale.x,
		(rectCenter.y - ellipseCenter.y) * scale.y
	};

	// 楕円→単位円空間に変換された矩形サイズ
	Vector2 scaledRectHalfSize = {
		rectHalfSize.x * scale.x,
		rectHalfSize.y * scale.y
	};

	// 単位円とAABBの当たり判定（中心原点）
	float dx = ((std::abs(scaledRectCenter.x) - scaledRectHalfSize.x) > 0.0f)
		? (std::abs(scaledRectCenter.x) - scaledRectHalfSize.x)
		: 0.0f;

	float dy = ((std::abs(scaledRectCenter.y) - scaledRectHalfSize.y) > 0.0f)
		? (std::abs(scaledRectCenter.y) - scaledRectHalfSize.y)
		: 0.0f;

	return dx * dx + dy * dy <= 1.0f;
}

const bool Collision::CircleAndRect(const Base& circle, const Base& rect) const
{
	RectInfo work;

	work = { rect.pos_,{rect.para_.size.x,rect.para_.size.y + (circle.para_.radius * 2)} };
	if (RectAndPixel(work, circle.pos_))return true;

	work = { rect.pos_,{rect.para_.size.x + (circle.para_.radius * 2),rect.para_.size.y} };
	if (RectAndPixel(work, circle.pos_))return true;

	Vector2 top = { 0.0f,0.0f };
	Vector2 vec = { rect.pos_.x - circle.pos_.x,	rect.pos_.y - circle.pos_.y };
	if (vec.x > 0 && vec.y > 0) top = { rect.pos_.x - (rect.para_.size.x / 2),	rect.pos_.y - (rect.para_.size.y / 2) };

	if (vec.x < 0 && vec.y > 0) top = { rect.pos_.x + (rect.para_.size.x / 2),	rect.pos_.y - (rect.para_.size.y / 2) };

	if (vec.x > 0 && vec.y < 0) top = { rect.pos_.x - (rect.para_.size.x / 2),	rect.pos_.y + (rect.para_.size.y / 2) };

	if (vec.x < 0 && vec.y < 0) top = { rect.pos_.x + (rect.para_.size.x / 2),	rect.pos_.y + (rect.para_.size.y / 2) };

	CircleInfo topcircle = { top,circle.para_.radius };

	if (CircleAndPixel(topcircle, circle.pos_))return true;

	return false;
}

const bool Collision::CircleAndEllipse(const Base& circle, const Base& ellipse) const
{
	// 楕円の中心と半径
	Vector2 ellipseCenter = ellipse.pos_;
	Vector2 ellipseRadius = ellipse.para_.size / 2.0f;

	// 円の中心と半径
	Vector2 circleCenter = circle.pos_;
	float circleRadius = circle.para_.radius;

	// 円の半径分だけ楕円を膨らませる
	float a = ellipseRadius.x + circleRadius;
	float b = ellipseRadius.y + circleRadius;

	// 中心からの距離
	float dx = circleCenter.x - ellipseCenter.x;
	float dy = circleCenter.y - ellipseCenter.y;

	// 楕円の式で判定
	float eq = (dx * dx) / (a * a) + (dy * dy) / (b * b);

	return eq <= 1.0f;
}

const bool Collision::CircleAndPixel(const CircleInfo& circle, const Vector2& pixel) const
{
	Vector2 vec = { circle.pos.x - pixel.x,	circle.pos.y - pixel.y };

	float distance = sqrtf(vec.x * vec.x + vec.y * vec.y);

	if (distance < circle.radius)return true;

	return false;
}

const bool Collision::RectAndPixel(const RectInfo& rect, const Vector2& pixel) const
{
	if (rect.pos.x - rect.size.x / 2 < pixel.x &&
		rect.pos.y - rect.size.y / 2 < pixel.y &&
		rect.pos.x + rect.size.x / 2 > pixel.x &&
		rect.pos.y + rect.size.y / 2 > pixel.y) {
		return true;
	}
	return false;
}