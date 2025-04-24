#pragma once
#include"../../Utility/VectorHeader.h"
#include<vector>
#include<map>
#include<string>
#include<DxLib.h>

class ModelRenderer
{
public:

	//レンダーのテクスチャーアドレス
	enum class TEXADDRESS
	{
		NONE = 0,
		WARP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};


	//シェーダーの情報
	struct SHADER_INFO
	{
		//頂点
		int shaderPS;						// Pixelシェーダーのハンドル
		int shaderVS;						// 頂点シェーダーのハンドル
		int constBufFloat4SizePS;			// Pixel定数バッファのデータサイズ(FLOAT4の倍数)
		int constBufFloat4SizeVS;			// 頂点定数バッファのデータサイズ(FLOAT4の倍数)
		int constBufPS;						// Pixel定数バッファのハンドル
		int constBufVS;						// 頂点定数バッファのハンドル
		TEXADDRESS texAddress;				// テクスチャーアドレス
		std::vector<FLOAT4>constBufsPS;		// Pixel定数バッファ
		std::vector<FLOAT4>constBufsVS;		// 頂点定数バッファ
		std::map<int, int>textures;			// 使用するテクスチャー
	};


	//シェーダー名
	enum class SHADER_NAME
	{
		NONE,
		NORMAL,						// 普通の頂点シェーダ
		NORMAL_POLYGON,				// ポリゴンを通常頂点シェーダで描画
		NO_NORM_SHADOW,				// 影表現(法線無し)
		NORM_SHADOW,				// 陰表現
		SKIN_MESH_NORM_4,			// 通常のスキンメッシュ
		SKIN_MESH_NORM_8,			// 通常の8スキンメッシュ
		METAL,						// 金属感
		SKIN_METAL,					// スキンメッシュの金属感
		SKIN_METAL_NORM_8,			// 法線あり8スキンメッシュの金属感
		RIM_LIGHT,					// リムライト
		RIM_LIGHT_SKIN,				// スキンメッシュのリムライト
		DISSOLVE,					// ディゾルブ
	};


	//シェーダのバッファ内で意味を持たないという意味
	static constexpr float SHADER_NONE_VALUE = 1.0f;


	//バッファサイズが0
	static constexpr int BUF_SIZE_ZERO = 0;


	//通常のスキンメッシュのそれぞれのシェーダーのバッファサイズ
	static constexpr int SKIN_MASH_BUF_SIZE_PS = 3;


	//メタリックのピクセルシェーダのバッファサイズ
	static constexpr int METAL_BUF_SIZE_PS = 2;
	//スキンメッシュメタリックのピクセルシェーダのバッファサイズ
	static constexpr int SKIN_METAL_BUF_SIZE_PS = 2;
	// リムライトの頂点シェーダのバッファサイズ
	static constexpr int RIM_LIGHT_BUF_SIZE_VS = 1;
	// ディゾルブの頂点シェーダのバッファサイズ
	static constexpr int DISSOLVE_BUF_SIZE_PS = 1;

	// ラフネステクスチャのレジスタ番号
	static constexpr int ROUGHNESS_INDEX = 11;
	// メタリックテクスチャのレジスタ番号
	static constexpr int METALIC_INDEX = 12;
	// スフィアマップテクスチャのレジスタ番号
	static constexpr int SPHERE_INDEX = 13;

	//色コード
	//灰色
	static constexpr Color3DF GREY_COLOR = { 0.66f, 0.66f, 0.66f };
	// 晴れの日の明るさ
	static constexpr Color3DF CLEAR_SKY = { 0.3f, 0.3f, 0.3f};
	//ちょっと白い黒
	static constexpr Color3DF FEW_BLACK_COLOR = { 0.1f, 0.1f, 0.1f };
	//太陽光の色
	static constexpr Color3DF SUN_COLOR = { 0.99f, 0.72f, 0.42f };


	//拡散光(デフォルト)
	static constexpr Color3DF DEFAULT_DIFFUSE_COLOR = { 0.8f, 0.8f, 0.8f };
	//環境光(デフォルト)
	static constexpr Color3DF DEFAULT_SPECULAR_COLOR = { 0.1f, 0.1f, 0.1f };
	//反射光(デフォルト)
	static constexpr Color3DF DEFAULT_REFLECTION_COLOR = { 1.0f, 1.0f, 1.0f };


	//光の強さ(弱)
	static constexpr float DIRECTION_LIGHT_POWER_WEAK = 0.1f;
	//光の強さ(中)
	static constexpr float DIRECTION_LIGHT_POWER_MEDIUM = 0.45f;
	//光の強さ(強)
	static constexpr float DIRECTION_LIGHT_POWER_STRONG = 0.7f;


	//コンストラクタ
	ModelRenderer(int handle);
	//デストラクタ
	virtual ~ModelRenderer(void);


	// 描画
	void Draw(void);


	// 使用するシェーダをセット
	void SetShaderInfo(SHADER_NAME name,
		int constBufFloat4SizePS, int constBufFloat4SizeVS, int constBufFloat4SizeGE);


	// バッファを追加登録
	void AddConstBufPS(FLOAT4 contBufPS);
	void AddConstBufVS(FLOAT4 contBufVS);
	void SetConstBufPS(int idx, FLOAT4 contBufPS);
	void SetConstBufVS(int idx, FLOAT4 contBufVS);


	//テクスチャーをセット
	void SetTextureBuf(int idx, int handle);


	//テクスチャーアドレスをセット
	void SetTextureAddress(TEXADDRESS texA);

protected:

	//現在使用中のシェーダー
	SHADER_NAME name_;


	//対象モデル
	int modelId_;


	//シェーダ情報
	SHADER_INFO shaderInfo_;
	//シェーダーの種類ごとのパスを格納
	std::map<SHADER_NAME, std::string>shaderNamesPS_;//ピクセルシェーダーパス
	std::map<SHADER_NAME, std::string>shaderNamesVS_;//頂点シェーダーパス



	//内部でのみ使用する関数
	//シェーダ名を初期化
	void InitNames(void);

	//シェーダのセット
	void SetReserveVS(void);
	void SetReservePS(void);

	// 継承先で使用するシェーダをセットする際に行う処理
	virtual void SetShaderSpecial(void);

	// 継承先で行う描画
	virtual void DrawSpecial(void);

};