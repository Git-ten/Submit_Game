#pragma once
#include <string>
#include <vector>

class EffectResource
{
public:

	// リソースタイプ
	enum class EFFECT_TYPE
	{
		NONE,
		EFFEKSEER_3D,	// エフェクシア3D
		EFFEKSEER_2D,	// エフェクシア2D
		SPLITE,			// スプライトアニメエフェクト
	};

	// コンストラクタ
	EffectResource(void);
	// コンストラクタ
	explicit EffectResource(const EFFECT_TYPE& type, const std::string& path);
	explicit EffectResource(const EFFECT_TYPE& type, const std::string& path, int numX, int numY, int sizeX, int sizeY);
	// デストラクタ
	~EffectResource(void);


	// 解放
	void Release(void);
	//部分開放
	void Release(int handleId);


	// 読み込み
	void Load(void);


	// リソースタイプ
	EFFECT_TYPE resType_;


	// リソースの読み込み先
	std::string path_;


	// エフェクトのハンドルID
	int handleId_;
	
	// Splite::LoadDivGraph用
	std::vector<int>handleIds_;
	int numX_;
	int numY_;
	int sizeX_;
	int sizeY_;


	// エフェクト複製用
	std::vector<int> duplicateEffectIds_;

};