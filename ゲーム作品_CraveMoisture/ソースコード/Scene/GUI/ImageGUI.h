#pragma once
#include <memory>
#include <functional>
#include "Image.h"

class ImageGUI
{
public:

	// コンストラクタ
	ImageGUI(const std::function<bool(void)>&conditionFunc, const std::function<void(void)>& runFunc);
	// デストラクタ
	~ImageGUI(void);

	// GUIとして使用する画像を登録
	void SetImage(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id);

	// GUIの実行条件をさらに追加
	void AddCondition(const std::function<bool(void)>& conditionFunc);

	// GUIの更新(trueでRunning)
	void UpdateGUI(void);

	// GUIの描画
	void DrawGUI(void);

	// 当たり判定用の枠を取得
	const Plane2D& GetCollPlane(void)const;

	// セットされている画像の座標を取得
	const Position2D& GetPos(void)const;

private:

	using CONDITION = std::function<bool(void)>;
	using RUN = std::function<void(void)>;

private:

	// 画像のインスタンス
	std::unique_ptr<Image>images_;

	// 実行条件
	CONDITION condition_;
	// 実行関数
	RUN run_;

};