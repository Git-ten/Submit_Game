#pragma once
#include "../../APPLICATION/SINGLETON_MANAGER/EffectResourceManager/EffectResourceManager.h"
#include "../../Common/DataLoad/DataImageDiv.h"
#include "../../Utility/VectorHeader.h"
#include "Image.h"

class ImageDiv
{
public:

	// コンストラクタ
	// 呼び出した時点で画像データをロードする
	ImageDiv(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id);
	ImageDiv(const LoadData::LoadDataInfo& info, const EffectResourceManager::EFFECT_RESOURCE_ID& id);
	// デストラクタ
	~ImageDiv(void);

	// 指定した画像の描画
	void DrawImageDiv(const int number);

	// 指定した画像を指定の座標に描画
	void DrawImageDiv(const int number, const Position2D& pos);

	void SetPos(const Position2D& pos);

	// 画像の座標を取得
	const Position2D& GetPos(void)const;
	// 画像の座標を取得(参照)
	Position2D& GetPosReference(void);

	// 矩形情報を参照
	Plane2D& GetPlaneReference(void);


	// 画像のサイズを取得
	Size2D GetSize(void)const;

	// 画像の枚数を取得
	int GetImageDataSize(void);

private:

	// 画像の情報
	DataImageDiv imgInfos_;

};