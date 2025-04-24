#pragma once
#include<string>
#include<vector>
#include<DxLib.h>
class PixelMaterial
{
public:

	static constexpr int MAX_TEXTURES = 4;

	enum class TEXADDRESS
	{
		NONE=0,
		WARP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};

	//コンストラクタ
	PixelMaterial(std::string shaderFileName, int constBufFloat4Size);

	//デストラクタ
	~PixelMaterial(void);

	//バッファ
	void AddConstBuf(const FLOAT4& contBuf);
	void SetConstBuf(int idx, const FLOAT4& contBuf);

	//ロードしたシェーダを取得
	int GetShader(void)const;
	//登録したバッファを取得
	int GetConstBuf(void)const;

	//バッファの中身を取得
	const std::vector<FLOAT4>& GetConstBufs(void)const;
	const std::vector<int>& GetTextures(void)const;

	//テキストバッファ
	void SetTextureBuf(int handle);

	//テキストバッファの取得
	TEXADDRESS GetTextureAddress(void)const;

	//テキストアドレスのセット
	void SetTextureAddress(TEXADDRESS texA);

private:

	//シェーダ
	int shader_;

	//バッファのサイズ
	int constBufFloat4Size_;

	//作成したバッファ
	int constBuf_;

	//テキストアドレス
	TEXADDRESS texAddress_;

	//登録したバッファ
	std::vector<FLOAT4>constBufs_;

	//登録したテクスチャ
	std::vector<int>textures_;

};

