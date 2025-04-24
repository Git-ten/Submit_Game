#pragma once
#include"ModelMaterial.h"
class ModelRender
{
public:

	//頂点シェーダバッファに使用するスロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;
	//ピクセルシェーダバッファに使用するスロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	//コンストラクタ
	ModelRender(int modelId, ModelMaterial& modelMaterial);
	
	//デストラクタ
	~ModelRender(void);

	//描画
	void Draw(void);

private:

	//モデルID
	int modelId_;

	//マテリアルデータ
	ModelMaterial& modelMaterial_;

	//シェーダの準備
	void SetReserveVS(void);
	void SetReservePS(void);

};

