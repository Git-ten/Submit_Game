#pragma once
#include <vector>
#include <memory>
#include "../APPLICATION/SINGLETON_MANAGER/ResourceManager/ResourceManager.h"
#include "../APPLICATION/SINGLETON_MANAGER/EffectResourceManager/EffectResourceManager.h"
#include "../SCENE/GUI/ImageDiv.h"

class SpliteAnime
{
public:

	// コンストラクタ
	SpliteAnime(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id);
	SpliteAnime(const LoadData::LoadDataInfo& info, const EffectResourceManager::EFFECT_RESOURCE_ID& id);
	// デストラクタ
	~SpliteAnime(void);

	void Update(void);
	void Draw(void);

	void PlaySpliteAnime(bool isLoop = false);

	void SetUpAnimePos(const Position2D& pos);

	void EndAnime(void);

	// 現在の座標
	const Position2D& GetPos(void);
	// 現在の座標の参照
	Position2D& GetPosReference(void);

	// 矩形情報情報を参照
	Plane2D& GetPlaneReference(void);

	bool IsPlayAnime(void);

private:

	// 分割画像
	std::unique_ptr<ImageDiv>imageAnime_;

	int nowNumber_;
	int frame_;
	float speed_;
	float animationCounter_;
	bool isPlayAnime_;
	bool isLoopAnime_;

};