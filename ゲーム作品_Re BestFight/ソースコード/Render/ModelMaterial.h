#pragma once
#include<string>
#include<vector>
#include<map>
#include<DxLib.h>
class ModelMaterial
{
public:
	static constexpr int MAX_TEXTURES = 4;

	enum class TEXADDRESS
	{
		NONE = 0,
		WARP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};

	//コンストラクタ
	ModelMaterial(
		std::string shaderFileNameVS,
		int constBufFloat4SizeVS,
		std::string shaderFileNamePS,
		int constBufFloat4SizePS);

	//デストラクタ
	~ModelMaterial(void);

	//バッファ
	void AddConstBufVS(const FLOAT4& contBuf);
	void AddConstBufPS(const FLOAT4& contBuf);
	void SetConstBufVS(int idx, const FLOAT4& contBuf);
	void SetConstBufPS(int idx, const FLOAT4& contBuf);

	//テキストバッファ
	void AddTextureBuf(int slot, int handle);
	void SetTextureBuf(int slot, int handle);

	//ロードしたシェーダを取得
	int GetShaderVS(void)const;
	int GetShaderPS(void)const;
	//登録したバッファを取得
	int GetConstBufVS(void)const;
	int GetConstBufPS(void)const;

	//バッファの中身を取得
	const std::vector<FLOAT4>& GetConstBufsVS(void)const;
	const std::vector<FLOAT4>& GetConstBufsPS(void)const;
	const std::map<int, int>& GetTextures(void)const;

	//テキストバッファの取得
	TEXADDRESS GetTextureAddress(void)const;

	//テキストアドレスのセット
	void SetTextureAddress(TEXADDRESS texA);

private:

	//シェーダ
	int shaderPS_;
	int shaderVS_;

	//バッファのサイズ
	int constBufFloat4SizePS_;
	int constBufFloat4SizeVS_;

	//作成したバッファ
	int constBufPS_;
	int constBufVS_;

	//テキストアドレス
	TEXADDRESS texAddress_;

	//登録したバッファ
	std::vector<FLOAT4>constBufsPS_;
	std::vector<FLOAT4>constBufsVS_;

	//登録したテクスチャ
	//first:slot, second:ハンドルID
	std::map<int, int>textures_;

};