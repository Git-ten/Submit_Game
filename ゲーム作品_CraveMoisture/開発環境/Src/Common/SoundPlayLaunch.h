#pragma once
#include <map>
#include "../APPLICATION/SINGLETON_MANAGER/SoundResourceManager/SoundResourceManager.h"

class SoundPlayLaunch
{
public:

	// コンストラクタ
	SoundPlayLaunch(void);
	// デストラクタ
	~SoundPlayLaunch(void);

	// 再生するエフェクトを登録
	void AddPlaySound(const SoundResourceManager::SOUND_RESOURCE_ID& id, const int handleId);

	// プレイ
	void Play(const SoundResourceManager::SOUND_RESOURCE_ID& id, int type, bool isReplay = false);

	// ストップ
	void Stop(const SoundResourceManager::SOUND_RESOURCE_ID& id);

	// 再生状態を取得
	const bool IsPlaySound(const SoundResourceManager::SOUND_RESOURCE_ID& id);

	// 指定する音リソースのハンドルIDを取得
	const int GetHandleId(const SoundResourceManager::SOUND_RESOURCE_ID& id)const;

private:

	using RESOURCE_SOUND = std::map<SoundResourceManager::SOUND_RESOURCE_ID, int>;

private:

	RESOURCE_SOUND resourceSound_;

};