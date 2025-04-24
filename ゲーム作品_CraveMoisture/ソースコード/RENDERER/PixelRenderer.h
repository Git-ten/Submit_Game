#pragma once
#include"../Utility/VectorHeader.h"
#include<vector>
#include<map>
#include<string>
#include<DxLib.h>

class PixelRenderer
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
		int shader;						//シェーダーのハンドル
		int constBufFloat4Size;			//定数バッファのデータサイズ(FLOAT4の倍数)
		int constBuf;					//定数バッファのハンドル
		TEXADDRESS texAddress;			//テクスチャーアドレス
		std::vector<FLOAT4>constBufs;	//Pixel定数バッファ
		std::vector<int>textures;		//使用するテクスチャー
	};


	//シェーダー名
	enum class SHADER_NAME
	{
		MONOTONE,		// モノトーン
		SCANLINE,		// 走査線
		MIRAGE,			// 蜃気楼
		CORONA,			// 太陽コロナ
		LIGHT_CIRCLE,	// 光輪
		BRACK_OUT,		// 暗転
		BLUR,			// ブラー
	};


	//シェーダのバッファ内で意味を持たないという意味
	static constexpr float SHADER_NONE_VALUE = 1.0f;


	//バッファサイズが0
	static constexpr int BUF_SIZE_ZERO = 0;


	//蜃気楼のバッファサイズ
	static constexpr int BUF_SIZE_MIRAGE = 1;


	//太陽コロナのバッファサイズ
	static constexpr int BUF_SIZE_CORONA = 1;
	

	//コンストラクタ
	PixelRenderer(int num, Size2D size);
	//デストラクタ
	~PixelRenderer(void);


	// インデックスポリゴンをセット
	// 画面全体に張り付けるようにポリゴンを生成する
	void MakeSquereVertex(Position2D pos, Size2D size);


	//レンダリングした結果を描画
	void Draw(void);


	//シェーダ情報を追加
	void AddShaderInfo(SHADER_NAME name, int constBufFloat4Size);


	//バッファを追加登録
	void AddConstBuf(SHADER_NAME name, FLOAT4 contBuf);
	void SetConstBuf(SHADER_NAME name, int idx, FLOAT4 contBuf);


	//テクスチャーをセット
	void SetTextureBuf(SHADER_NAME name, int handle);


	//テクスチャーアドレスをセット
	void SetTextureAddress(SHADER_NAME name, TEXADDRESS texA);

private:

	//ポリゴンの頂点数
	static constexpr int NUM_VERTEX = 4;
	//ポリゴンの数
	static constexpr int NUM_POLYGON = 2;
	//三角ポリゴンの頂点数
	static constexpr int NUM_VERTEX_IDX = NUM_POLYGON * 3;


	//レンダリング結果の表示座標
	Position2D pos_;
	//レンダリング結果の画面の大きさ
	Size2D size_;


	//ポリゴン
	VERTEX2DSHADER vertexs_[NUM_VERTEX];


	//ポリゴンインデックス
	WORD indexes_[NUM_VERTEX_IDX];


	//スクリーンの個数
	int screenNum_;
	//スクリーン
	int* screens_;


	//シェーダ情報
	std::map<SHADER_NAME, SHADER_INFO>shaderInfo_;
	std::map<SHADER_NAME, std::string>shaderNames_;



	//内部でのみ使用する関数
	//シェーダ名を初期化
	void InitNames(void);

};