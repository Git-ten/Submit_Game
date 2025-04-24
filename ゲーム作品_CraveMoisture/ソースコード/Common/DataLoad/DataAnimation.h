#pragma once
#include <string>

class DataAnimation
{
public:

	std::string path;	// アニメーションのデータパス
	float speed;		// アニメーションの速度
	float start;		// アニメーションスタートステップ
	float end;			// アニメーション終了ステップ
	bool isLoop;		// ループするか

};