#pragma once
#include <DxLib.h>
#include <vector>

class PoligonRenderer
{
public:

	// コンストラクタ
	PoligonRenderer(void);
	// デストラクタ
	~PoligonRenderer(void);

	void Draw(std::vector<VERTEX3DSHADER>&vertex, std::vector<WORD>& indexs);

	// テクスチャのセット
	void SetTexture(int texture);

private:

	// 3Dポリゴン用のピクセルシェーダ
	int shaderPoligonPS_;
	// 3Dポリゴン用の頂点シェーダ
	int shaderPoligonVS_;

	// 貼り付けるテクスチャ
	int texture_;

};