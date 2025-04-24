#include "Collider.h"
#include "../Application/Application.h"
#include<DxLib.h>
#include<cmath>

#pragma region 定数宣言

// デフォルトの線分当たり判定距離
const static constexpr float DEFAULT_DIS = 50.0f;

#pragma endregion

//コンストラクタ
Collider::Collider(void)
{
}

//デストラクタ
Collider::~Collider(void)
{

}

bool Collider::CollisionRectToMouse(const Plane2D& plane, const Position2D& target)
{
	// 全画面表示の場合は、マウス座標を補正する
	const Size2DF& d = Application::GetInstance().GetReDeffrenceSize();
	Position2DF posF = Vector2F::ToVector2F(target);
	posF.x *= d.x;
	posF.y *= d.y;

	Position2D posD = Vector2F::ToVector2(posF);

	if (plane.leftUpPos.x <= posD.x &&
		plane.rightDownPos.x >= posD.x &&
		plane.leftUpPos.y <= posD.y &&
		plane.rightDownPos.y >= posD.y)
	{
		return true;
	}

	return false;
}

bool Collider::CollisionRectToRect(const Plane2D& plane1, const Plane2D& plane2)
{
	Position2D plane1_Center = { plane1.leftUpPos.x + (plane1.rightDownPos.x - plane1.leftUpPos.x) / 2,
								 plane1.leftUpPos.y + (plane1.rightDownPos.y - plane1.leftUpPos.y) / 2 };
	Position2D plane2_Center = { plane2.leftUpPos.x + (plane2.rightDownPos.x - plane2.leftUpPos.x) / 2,
								 plane2.leftUpPos.y + (plane2.rightDownPos.y - plane2.leftUpPos.y) / 2 };

	int disX = std::abs(plane1_Center.x - plane2_Center.x);
	int disY = std::abs(plane1_Center.y - plane2_Center.y);

	int sumPlaneX = plane1.rightDownPos.x - plane1.leftUpPos.x;
	sumPlaneX += plane2.rightDownPos.x - plane2.leftUpPos.x;
	sumPlaneX /= 2;

	int sumPlaneY = plane1.rightDownPos.y - plane1.leftUpPos.y;
	sumPlaneY += plane2.rightDownPos.y - plane2.leftUpPos.y;
	sumPlaneY /= 2;

	if (sumPlaneX > disX &&
		sumPlaneY > disY)
		return true;

	return false;
}

bool Collider::CollisionLine2Line(Position3DF start1, Position3DF end1, Position3DF start2, Position3DF end2)
{
	float distance = Segment_Segment_MinLength(start1, end1,
		start2, end2);

	//距離が50,0fより短い場合
	if (distance < DEFAULT_DIS)
	{
		return true;
	}

	return false;
}

bool Collider::CollisionLine2Line(Position3DF start1, Position3DF end1, Position3DF start2, Position3DF end2, float dis)
{
	float distance = Segment_Segment_MinLength(start1, end1,
		start2, end2);

	//距離が50,0fより短い場合
	if (distance < dis)
	{
		return true;
	}

	return false;
}

bool Collider::CollisionTriangle2Model(const VERTEX3D* ver, const CapsuleF& cap)
{
	//ポリゴン1つ(ver[0 ～ 2])しか判定しない
	bool hit = HitCheck_Capsule_Triangle(cap.top, cap.down,
		static_cast<float>(cap.r), ver[0].pos, ver[1].pos, ver[2].pos);

	return hit;
}

bool Collider::CollisionSphere2Cap(const SphereF& sph, const CapsuleF& cap)
{
	bool hit = HitCheck_Sphere_Capsule(sph.pos, static_cast<float>(sph.r),
		cap.top, cap.down, static_cast<float>(cap.r));

	return hit;
}

bool Collider::CollisionSphere2Line(const Line3DF& line, const SphereF& sph)
{
	bool hit = HitCheck_Line_Sphere(line.end, line.start, sph.pos, static_cast<float>(sph.r));

	return hit;
}

bool Collider::CollisionSphere2Sphere(const SphereF& sph_1, const SphereF& sph_2)
{
	bool hit = HitCheck_Sphere_Sphere(sph_1.pos, static_cast<float>(sph_1.r),
		sph_2.pos, static_cast<float>(sph_2.r));

	return hit;
}

bool Collider::CollisionCap2Model(int id, int flameIdx, const CapsuleF& cap)
{
	MV1RefreshCollInfo(id, flameIdx);
	const auto& hit = MV1CollCheck_Capsule(id, flameIdx, cap.top, cap.down, static_cast<float>(cap.r));
	int hitNum = hit.HitNum;
	MV1CollResultPolyDimTerminate(hit);

	return 0 < hitNum ? true : false;
}

bool Collider::CollisionSphere2Model(int id, int flameIdx, const SphereF& sph)
{
	MV1RefreshCollInfo(id, flameIdx);
	const auto& hit = MV1CollCheck_Sphere(id, flameIdx, sph.pos, static_cast<float>(sph.r));
	int hitNum = hit.HitNum;
	MV1CollResultPolyDimTerminate(hit);

	return 0 < hitNum ? true : false;
}

MV1_COLL_RESULT_POLY_DIM Collider::CollisionSphere2ModelInfo(int id, int flameIdx, const SphereF& sph)
{
	MV1RefreshCollInfo(id, flameIdx);
	const auto& hit = MV1CollCheck_Sphere(id, flameIdx, sph.pos, sph.r);
	return hit;
}

bool Collider::CollisionLine2Model(int id, int flameIdx, const Line3DF& line)
{
	MV1RefreshCollInfo(id, flameIdx);
	const auto& hit = MV1CollCheck_Line(id, flameIdx, line.start, line.end);
	return hit.HitFlag;
}

MV1_COLL_RESULT_POLY Collider::CollisionLine2ModelInfo(int id, int flameIdx, const Line3DF& line)
{
	MV1RefreshCollInfo(id, flameIdx);
	const auto& hit = MV1CollCheck_Line(id, flameIdx, line.start, line.end);
	return hit;
}

bool Collider::CollisionLine2Triangle(const Line3DF& line, const Triangle3DF& tri)
{
	const auto& hit = HitCheck_Line_Triangle(line.start, line.end, tri.top, tri.right, tri.left);

	return hit.HitFlag;
}