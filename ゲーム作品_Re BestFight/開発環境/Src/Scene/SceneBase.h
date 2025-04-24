#pragma once
#include<memory>
#include"../Utility/VectorHeader.h"

#define INT_CAST(a)(static_cast<int>(a))

class Camera;

class SceneBase
{
public:

	//コンストラクタ
	SceneBase(void);

	//デストラクタ
	virtual ~SceneBase(void);

	//仮想初期化
	virtual void Init(void) = 0;
	//仮想更新
	virtual void Update(void) = 0;
	//仮想描画
	virtual void Draw(void) = 0;
	//仮想メモリ開放
	virtual void Release(void) = 0;
	//カメラ取得
	std::weak_ptr<Camera>GetCamera(void)const;

protected:

	//デバッグ用のカメラインスタンス
	std::shared_ptr<Camera>camera_;



	//内部でのみ使用する関数
	//マップ座標に変換
	//マップ座標はApplicationクラスで定義したスクリーンの大きさから計算する
	Position2D World2MapPos(const Position2D& worldPos);
	//マップの描画
	void DrawMap(void);

};

