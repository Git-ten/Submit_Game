#pragma once
#include<memory>
#include<DxLib.h>

class ActorBase;

class Collider
{
public:

	//デフォルトの線分当たり判定距離
	static constexpr float DEFAULT_DIS = 50.0f;

	//コンストラクタ
	Collider(std::weak_ptr<ActorBase>player, std::weak_ptr<ActorBase>enemy);
	Collider(std::weak_ptr<ActorBase>player);

	//デストラクタ
	~Collider(void);

	//線分と線分の当たり判定
	bool CollisionLine2Line(void);
	//三角形とカプセルの当たり判定
	bool CollisionTriangle2Model(VERTEX3D* ver);
	//球とカプセルの当たり判定
	bool CollisionSphere2Cap(const SphereF& sph);
	//フィールドの当たり判定(円形)
	void CollisionCircle2Vector(std::weak_ptr<ActorBase>actor, const float& radius);
	void CollisionCircle2Vector(std::weak_ptr<Camera>camera, const float& radius);

private:

	//呼び出す側のアクター
	std::weak_ptr<ActorBase>player_;

	//呼び出す側の相手アクター
	std::weak_ptr<ActorBase>enemy_;

};