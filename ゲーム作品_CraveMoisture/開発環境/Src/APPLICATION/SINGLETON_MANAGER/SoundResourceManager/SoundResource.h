#pragma once
#include <string>

class SoundResource
{
public:

	// コンストラクタ
	SoundResource(void);
	// コンストラクタ
	explicit SoundResource(const std::string& path);
	// デストラクタ
	~SoundResource(void);

	// 解放
	void Release(void);
	//部分開放
	void Release(int handleId);


	// 読み込み
	void Load(void);


	// リソースの読み込み先
	std::string path_;


	// サウンドのハンドルID
	int handleId_;

};