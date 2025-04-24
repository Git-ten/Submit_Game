#pragma once
#include<string>

class Sound
{
public:

	enum class TYPE
	{
		NONE,
		MUSIC,//BGM
		WAVE,//効果音
	};

	//コンストラクタ
	Sound(void);
	//コンストラクタ
	Sound(const TYPE& type, std::string path);

	//開放処理
	void Release(void);

	//読み込み処理
	void Load(void);

	//エフェクトをプレイ
	void Play(int type);
	//エフェクトをストップ
	void Stop(void);
	//再生中かをチェック
	bool IsCheckPlay(void)const;

private:

	//サウンドの種類
	TYPE type_;

	//サウンドのパス
	std::string path_;

	//サウンドのハンドル
	int handleId_;

};

