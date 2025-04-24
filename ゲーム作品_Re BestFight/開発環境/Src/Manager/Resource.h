#pragma once
#include <string>
#include <vector>

class Resource
{
public:
	
	// リソースタイプ
	enum class TYPE
	{
		NONE,
		IMG,
		NORM,
		IMGS,
		MODEL,
	};

	// コンストラクタ
	Resource(void);
	// コンストラクタ
	Resource(const TYPE& type, const std::string& path);
	// コンストラクタ(IMGS用)
	Resource(const TYPE& type, const std::string& path, int numX, int numY, int sizeX, int sizeY);

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
	TYPE resType_;

	// リソースの読み込み先
	std::string path_;

	// 画像とモデルのハンドルID
	int handleId_;

	// IMGS::LoadDivGraph用
	int* handleIds_;
	int numX_;
	int numY_;
	int sizeX_;
	int sizeY_;

	// モデル複製用
	std::vector<int> duplicateModelIds_;

};

