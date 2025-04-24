#pragma once
#include <string>
#include <functional>
#include "../../Application/Singleton_Manager/ResourceManager/ResourceManager.h"
#include "../../Common/DataLoad/LoadDataImage.h"
#include "../../Common/DataLoad/DataImage.h"

class Image
{
public:

	// 画像の種類
	enum IMAGE_TYPE
	{
		NORMAL,
		GAGE,
		CIRCLE_GAGE,
	};

	// コンストラクタ
	// 呼び出した時点で画像データをロードする
	Image(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id,
		const IMAGE_TYPE& type = IMAGE_TYPE::NORMAL, 
		const int addId = -1);
	// デストラクタ
	~Image(void);

	// ゲージのレートの参照をセットアップ
	void SetUpImageGageRate(float& gage, float maxGage, Color3D color, float addAngle = 0.0f);

	// ゲージに演出用のパラメータをセット
	void SetUpImageEventGageRate(float gage);

	// アニメーション
	// 引数で実行
	void AnimationImage(const std::function<void(void)>&animation);

	// 画像の描画
	void DrawImage(int modelId = -1);
	// 画像の矩形切り取り描画
	void DrawRectImage(const Position2D& rightPos, const Size2D& size);

	// 画像のゲージ描画
	void DrawGageImage(const Color3D& color = { 255, 255, 255});

	// 画像の円形切り取り描画
	void DrawCircleImage(const float rate);

	// 種類に沿った画像の描画
	void DrawTypeImage(void);

	// 透明度をセット
	void SetAlpha(const float alpha);

	// 当たり判定用の枠をセットアップ
	void SetUpCollision(void);

	// 座標のセット
	void SetPos(const Position2D& pos);
	
	// 使用している画像の取得
	const ResourceManager::RESOURCE_ID& GetNowId(void)const;

	// 画像の変更
	void ChangeImage(const ResourceManager::RESOURCE_ID& id);

	// 画像のハンドルID
	const int GetId(void)const;

	// 画像の左上座標
	const Position2D& GetLeftUpPos(void)const;
	// 画像の右上座標
	const Position2D& GetRightDownPos(void)const;
	// 画像の中心座標
	const Position2D& GetCenterPos(void)const;
	// 画像のサイズ
	const Size2D GetSize(void);

	// 画像の拡大率
	const double GetScl(void)const;
	// 画像のアングル
	const double GetAngle(void)const;

	// 当たり判定用の枠を取得
	const Plane2D& GetCollPlane(void)const;

private:

	IMAGE_TYPE imageType_;
	float* imageGageRate_;
	float prevImageGageRate_;
	float imageGageMaxRate_;
	float addImageGageAngle_;
	Color3D imageGageColor_;
	int addResource_;

	// データの情報
	DataImage imgInfo_;

	// リソースID
	ResourceManager::RESOURCE_ID resId_;

	// 画像のアルファ値
	float alpha_;

};