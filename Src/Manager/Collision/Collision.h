#pragma once

#include"../../Object/UnitBase.h"

class Collision
{
public:
	Collision();
	~Collision();

	static void AddAlly(UnitBase* obj) { if (obj) allyObjects_.emplace_back(obj); }
	static void AddAlly(std::vector<UnitBase*> obj) { for (auto& o : obj) { AddAlly(o); } }

	static void AddEnemy(UnitBase* obj) { if (obj) allyObjects_.emplace_back(obj); }
	static void AddEnemy(std::vector<UnitBase*> obj) { for (auto& o : obj) { AddEnemy(o); } }

	void Clear() { allyObjects_.clear(); enemyObjects_.clear(); }
	void Check();


private:
	static std::vector<UnitBase*> allyObjects_;
	static std::vector<UnitBase*> enemyObjects_;

	// 当たり判定の形の振り分け
	bool IsHit(const Base& a, const Base& b);

	/// <summary>
	/// 円形同士の当たり判定
	/// </summary>
	/// <param name="u1"></param>
	/// <param name="u2"></param>
	/// <returns></returns>
	const bool Circle(const Base& u1, const Base& u2)const;

	/// <summary>
	/// 矩形同士の当たり判定
	/// </summary>
	/// <param name="u1"></param>
	/// <param name="u2"></param>
	/// <returns></returns>
	const bool Rect(const Base& u1, const Base& u2)const;

	/// <summary>
	/// 楕円同士の当たり判定
	/// </summary>
	/// <param name="u1"></param>
	/// <param name="u2"></param>
	/// <returns></returns>
	const bool Ellipse(const Base& u1, const Base& u2) const;

	/// <summary>
	/// 楕円と矩形の当たり判定
	/// </summary>
	/// <param name="ellipse">楕円</param>
	/// <param name="rect">矩形</param>
	/// <param name="invici">無敵考慮</param>
	/// <returns></returns>
	const bool EllipseAndRect(const Base& ellipse, const Base& rect) const;

	/// <summary>
	/// 円形と矩形の当たり判定
	/// </summary>
	/// <param name="circle">円形</param>
	/// <param name="rect">矩形</param>
	/// <returns></returns>
	const bool CircleAndRect(const Base& circle, const Base& rect)const;

	/// <summary>
	/// 円形と楕円の当たり判定
	/// </summary>
	/// <param name="circle">円形</param>
	/// <param name="ellipse">楕円</param>
	/// <param name="invici">無敵判定を考慮するか</param>
	/// <returns></returns>
	const bool CircleAndEllipse(const Base& circle, const Base& ellipse) const;

	//円形の情報をまとめる構造体(引数で使う)
	struct CircleInfo
	{
		Vector2 pos;
		float radius;
	};

	/// <summary>
	/// 円形と点の当たり判定
	/// </summary>
	/// <param name="circle"></param>
	/// <param name="pixel"></param>
	/// <returns></returns>
	const bool CircleAndPixel(const CircleInfo& circle, const Vector2& pixel)const;

	//矩形の情報をまとめる構造体(引数で使う)
	struct RectInfo
	{
		Vector2 pos;
		Vector2 size;
	};

	/// <summary>
	/// 矩形と点の当たり判定
	/// </summary>
	/// <param name="rect">矩形</param>
	/// <param name="pixel">点</param>
	/// <returns></returns>
	const bool RectAndPixel(const RectInfo& rect, const Vector2& pixel)const;
};