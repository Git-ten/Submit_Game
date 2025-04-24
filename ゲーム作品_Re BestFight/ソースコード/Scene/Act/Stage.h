#pragma once
#include<memory>
#include"../../Utility/VectorHeader.h"

class Transform;
class ActorBase;
class Collider;
class ModelMaterial;
class ModelRender;

class Stage
{
public:

	static constexpr Size3DF STAGE_SCALE = { 2.0f, 2.0f, 2.0f };//ステージの大きさ
	static constexpr float COLL_RADIUS_CIRCLE = 2760.0f * 2.0f;//ステージの半径

	//コンストラクタ
	Stage(std::weak_ptr<ActorBase>player, std::weak_ptr<ActorBase>enemy, std::weak_ptr<Camera>camera);

	//デストラクタ
	~Stage(void);

	//初期化
	void Init(void);
	//更新
	void Update(void);
	//描画
	void Draw(void);

	//カメラの当たり判定
	void CollisionCamera(void);

private:

	//ポイントライトの初期座標
	static constexpr Position3DF POINT_LIGHT_POS = { 0.0f, 6000.0f, 3000.0f };
	//ポイントライトの色
	static constexpr Color3D POINT_LIGHT_COLOR = { 0.8f, 0.8f, 0.6f};
	//ポイントライトの強さ
	static constexpr float POINT_LIGHT_POWER = 6000.0f;
	//ディフューズカラー
	static constexpr Color3D DIFFUSE_COLOR = {0.8f, 0.8f, 0.8f};

	//頂点マテリアル
	std::unique_ptr<ModelMaterial>material_;
	//頂点レンダー
	std::unique_ptr<ModelRender>render_;
	//デバッグ用
	std::unique_ptr<Transform>transform_;
	//プレイヤーのデータを共有
	std::weak_ptr<ActorBase>player_;
	//CPUのデータを共有
	std::weak_ptr<ActorBase>enemy_;

	//当たり判定のインスタンス
	std::unique_ptr<Collider>collider_;

	//共有するカメラ
	std::weak_ptr<Camera>camera_;



	//内部でのみ使用する関数
	void InitTransform(void);
	void InitShader(void);

	//当たり判定
	void Collision(void);
	//アクターの当たり判定
	void CollisionActor(void);
	//ステージの描画
	void DrawStage(void);
	//予備ポイントライトシェーダ定数をセットする
	void SetSparePointLightShader(float power, const Position3DF& pos, const Color3D& color);
	//ディフューズカラー
	void SetDiffuseColor(const Color3D& color);
	void SetDefDiffuseColor(void);

};

