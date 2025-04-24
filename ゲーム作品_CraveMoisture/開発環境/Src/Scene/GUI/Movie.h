#pragma once
#include "../../Application/Singleton_Manager/ResourceManager/ResourceManager.h"
#include "../../Common/DataLoad/LoadDataImage.h"
#include "../../Common/DataLoad/DataImage.h"

/// <summary>
/// ※ 動画クラスではあるが、本質的には画像情報の羅列であるため、
///    Imageと同等の扱いである
/// </summary>

class Movie
{
public:

	// コンストラクタ
	// 呼び出した時点で画像データをロードする
	Movie(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id);
	// デストラクタ
	~Movie(void);

	// 動画再生(引数で再生タイプを選ぶ)
	void Play(int type);
	// 動画の停止
	void Stop(void);

	// 描画
	void DrawMovie(void);

private:

	// データの情報
	// IDだけは動画のもの
	DataImage movieInfo_;

};