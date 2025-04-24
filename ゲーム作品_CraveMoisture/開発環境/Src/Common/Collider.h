#pragma once
#include<memory>
#include<DxLib.h>
#include"../../Utility/VectorHeader.h"

class Collider
{
public:

	//モデル全体をコリジョン情報にする
	//モデルのフレームインデックスとして使用
	static constexpr int ALL_MODEL_INFO_FRAME_IDX = -1;

	//コンストラクタ
	Collider(void);
	//デストラクタ
	~Collider(void);


	//2D用
	//矩形と点の当たり判定
	static bool CollisionRectToMouse(const Plane2D& plane, const Position2D& target);
	// 矩形と矩形の当たり判定
	static bool CollisionRectToRect(const Plane2D& plane1, const Plane2D& plane2);


	//3D
	//線分と線分の当たり判定
	static bool CollisionLine2Line(Position3DF start1, Position3DF end1, Position3DF start2, Position3DF end2);
	static bool CollisionLine2Line(Position3DF start1, Position3DF end1, Position3DF start2, Position3DF end2, float dis);
	//三角形とカプセルの当たり判定
	static bool CollisionTriangle2Model(const VERTEX3D* ver, const CapsuleF& cap);
	//球とカプセルの当たり判定
	static bool CollisionSphere2Cap(const SphereF& sph, const CapsuleF& cap);
	//球と線分の当たり判定
	static bool CollisionSphere2Line(const Line3DF& line, const SphereF& sph);
	//球と球の当たり判定
	static bool CollisionSphere2Sphere(const SphereF& sph_1, const SphereF& sph_2);
	//カプセルとモデルの当たり判定
	static bool CollisionCap2Model(int id, int flameIdx, const CapsuleF& cap);
	//球とモデルの当たり判定
	static bool CollisionSphere2Model(int id, int flameIdx, const SphereF& sph);
	static MV1_COLL_RESULT_POLY_DIM CollisionSphere2ModelInfo(int id, int flameIdx, const SphereF& sph);
	//線とモデルの当たり判定
	static bool CollisionLine2Model(int id, int flameIdx, const Line3DF& line);
	static MV1_COLL_RESULT_POLY CollisionLine2ModelInfo(int id, int flameIdx, const Line3DF& line);
	//線と三角形の当たり判定
	static bool CollisionLine2Triangle(const Line3DF& line, const Triangle3DF& tri);

};