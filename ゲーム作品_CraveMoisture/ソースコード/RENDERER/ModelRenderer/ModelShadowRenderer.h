#pragma once
#include "ModelRenderer.h"

class ModelShadowRenderer : public ModelRenderer
{
public:

	//陰表現のピクセルシェーダのバッファサイズ
	static constexpr int SHADOW_BUF_SIZE_PS = 5;
	static constexpr int SHADOW_BUF_SIZE_VS = 8;

	// コンストラクタ
	ModelShadowRenderer(int handle);
	// デストラクタ
	~ModelShadowRenderer(void)override;

	//深度マップに描画するモデルのIDを追加登録
	//法線無し用
	void AddDepthModel(int modelId);
	void SetDepthModel(int idx, int modelId);
	//スキンメッシュ用
	void AddSkinDepthModel(int modelId);
	void SetSkinDepthModel(int idx, int modelId);

private:

	//深度マップに描画するモデル(法線無し)
	std::vector<int>depthModels_;
	//深度マップに描画するモデル(スキンメッシュ)
	std::vector<int>skinDepthModels_;

	//カメラのビュー行列
	MATRIX cameraView_;
	//カメラの射影行列
	MATRIX cameraProj_;

	//深度マップ
	int depthHandle_;
	//深度マップ描画用のシェーダハンドル
	int shaderDepthPS_;
	int shaderDepthVS_;
	int shaderSkinDepthVS_;//スキンメッシュ

	// 継承先で使用する追加シェーダ名の初期化
	void InitNamesSpecial(void);

	// 継承先で使用するシェーダをセットする際に行う処理
	void SetShaderSpecial(void)override;

	// 継承先で行う描画
	void DrawSpecial(void)override;

	//深度マップを描画
	void DrawDepth(void);

};