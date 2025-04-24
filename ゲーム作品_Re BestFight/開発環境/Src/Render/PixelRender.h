#pragma once
#include"../Utility/VectorHeader.h"
#include"PixelMaterial.h"

class PixelRender
{
public:

	//ピクセルシェーダバッファに使用するスロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;
	//ポリゴンの頂点数
	static constexpr int NUM_VERTEX = 4;
	//ポリゴンインデックスの数
	static constexpr int NUM_VERTEX_IDX = 6;
	//ポリゴン数
	static constexpr int NUM_POLYGON = 2;

	//コンストラクタ
	PixelRender(const PixelMaterial& pixelMaterial);

	//デストラクタ
	~PixelRender(void);

	//ポリゴン作成
	void MakeSquereVertex(Vector2 pos, Vector2 size);
	void MakeSquereVertex(void);

	//座標のセット
	void SetPos(Vector2 pos);
	//大きさのセット
	void SetSize(Vector2 size);

	//描画
	void Draw(void);

private:

	//座標
	Vector2 pos_;
	//大きさ
	Vector2 size_;

	//ポリゴンデータ
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	//ポリゴンインデックスデータ
	WORD indexes_[NUM_VERTEX_IDX];

	//マテリアルデータ
	const PixelMaterial& pixelMaterial_;

};