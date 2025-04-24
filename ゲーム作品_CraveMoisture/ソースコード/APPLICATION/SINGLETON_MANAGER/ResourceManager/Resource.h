#pragma once
#include <string>
#include <vector>

class Resource
{
public:
	
	// リソースタイプ
	enum class RESOURCE_TYPE
	{
		NONE,
		IMG,	//通常の画像
		NORM,	//法線マップ
		IMGS,	//分割画像
		MODEL,	//3Dモデル
		MOVIE,	//動画
	};

	// コンストラクタ
	Resource(void);
	// コンストラクタ
	explicit Resource(const RESOURCE_TYPE& type, const std::string& path);
	// コンストラクタ(IMGS用)
	explicit Resource(const RESOURCE_TYPE& type, const std::string& path, int numX, int numY, int sizeX, int sizeY);
	// デストラクタ
	~Resource(void);


	// 解放
	void Release(void);
	//部分開放
	void Release(int handleId);


	// 読み込み
	void Load(void);


	// 複数画像ハンドルを別配列にコピー
	void CopyHandle(int* imgs);


	// リソースタイプ
	RESOURCE_TYPE resType_;


	// リソースの読み込み先
	std::string path_;


	// 画像とモデルのハンドルID
	int handleId_;


	// IMGS::LoadDivGraph用
	std::vector<int>handleIds_;
	int numX_;
	int numY_;
	int sizeX_;
	int sizeY_;


	// モデル複製用
	std::vector<int> duplicateModelIds_;

};

