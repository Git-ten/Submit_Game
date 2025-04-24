#pragma once
#include <vector>
#include <memory>
#include "../APPLICATION/SINGLETON_MANAGER/ResourceManager/ResourceManager.h"
#include "../APPLICATION/SINGLETON_MANAGER/EffectResourceManager/EffectResourceManager.h"
#include "../SCENE/GUI/ImageDiv.h"

class SpliteAnime
{
public:

	// �R���X�g���N�^
	SpliteAnime(const LoadData::LoadDataInfo& info, const ResourceManager::RESOURCE_ID& id);
	SpliteAnime(const LoadData::LoadDataInfo& info, const EffectResourceManager::EFFECT_RESOURCE_ID& id);
	// �f�X�g���N�^
	~SpliteAnime(void);

	void Update(void);
	void Draw(void);

	void PlaySpliteAnime(bool isLoop = false);

	void SetUpAnimePos(const Position2D& pos);

	void EndAnime(void);

	// ���݂̍��W
	const Position2D& GetPos(void);
	// ���݂̍��W�̎Q��
	Position2D& GetPosReference(void);

	// ��`�������Q��
	Plane2D& GetPlaneReference(void);

	bool IsPlayAnime(void);

private:

	// �����摜
	std::unique_ptr<ImageDiv>imageAnime_;

	int nowNumber_;
	int frame_;
	float speed_;
	float animationCounter_;
	bool isPlayAnime_;
	bool isLoopAnime_;

};