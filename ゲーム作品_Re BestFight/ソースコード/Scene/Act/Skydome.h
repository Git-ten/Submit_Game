#pragma once
#include<memory>
#include"../../Utility/VectorHeader.h"

class Transform;
class ActorBase;
class ModelMaterial;
class ModelRender;

class SkyDome
{
public:

	//コンストラクタ
	SkyDome(std::weak_ptr<ActorBase>player, std::weak_ptr<ActorBase>enemy);

	//デストラクタ
	~SkyDome(void);

	//初期化
	void Init(void);
	//更新
	void Update(void);
	//描画
	void Draw(void);

private:

	//大きさ
	static constexpr Size3DF SKYDOME_SCALE = { 200.0f, 200.0f, 200.0f };
	//ディフューズカラー
	static constexpr Color3D DIFFUSE_COLOR = { 0.8f, 0.8f, 0.8f };

	std::unique_ptr<Transform>transform_;

	//頂点マテリアル
	std::unique_ptr<ModelMaterial>material_;
	//頂点レンダー
	std::unique_ptr<ModelRender>render_;

	//プレイヤーのデータを共有
	std::weak_ptr<ActorBase>player_;
	//CPUのデータを共有
	std::weak_ptr<ActorBase>enemy_;



	//内部でのみ使用する関数
	void InitTransform(void);
	void InitShader(void);
	void DrawSkydome(void);

	//ディフューズカラー
	void SetDiffuseColor(const Color3D& color);
	void SetDefDiffuseColor(void);

};