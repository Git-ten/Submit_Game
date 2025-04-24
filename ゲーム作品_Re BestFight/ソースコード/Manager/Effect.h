#pragma once
#include<string>
#include"../Utility/VectorHeader.h"

class Effect
{
public:

	//エフェクトの種類
	enum class TYPE
	{
		NONE,
		EFFKSEER3D,//エフェクシア2D
		EFFKSEER2D,//エフェクシア3D
	};

	//コンストラクタ
	Effect(void);
	Effect(const TYPE& type, std::string path);

	//デストラクタ
	~Effect(void);

	//メモリ開放
	void Release(void);

	//読み込み
	void Load(void);

	//エフェクトをプレイ
	void Play(void);
	//エフェクトをストップ
	void Stop(void);
	//再生中かをチェック
	bool IsCheckPlay(void)const;

	//エフェクトの情報をセット
	void SetPos(Position3DF pos);
	void SetRot(Rot3DF rot);
	void SetScl(Size3DF scl);
	void SetSpeed(float speed);
	void SetRGB(int r, int g, int b, int a);
	Position3DF GetPos(void)const;

private:

	//エフェクトの種類
	TYPE type_;
	//エフェクトデータパス
	std::string path_;
	//リソース
	int resHandleId_;
	//プレイ
	int playHandleId_;

	//エフェクトの情報
	Position3DF pos_;

};