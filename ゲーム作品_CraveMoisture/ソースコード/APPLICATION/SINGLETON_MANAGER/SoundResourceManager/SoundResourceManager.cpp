#include <cassert>
#include <DxLib.h>
#include "../../Application.h"
#include "../../../Common/DataLoad/DataPaths.h"
#include "SoundResourceManager.h"

SoundResourceManager* SoundResourceManager::instance_ = nullptr;

void SoundResourceManager::CreateInstance(void)
{
	assert(instance_ == nullptr || !"SoundResourceManagerが作成されているのに再び生成しようとしています");

	instance_ = new SoundResourceManager();
	instance_->Init();
}

SoundResourceManager& SoundResourceManager::GetInstance(void)
{
	return *instance_;
}

void SoundResourceManager::Init(void)
{
	resourcesMap_.clear();
	loadedMap_.clear();


	DataPaths dataPath = Application::GetInstance().GetDataPaths();
	SoundResource res;


	res = SoundResource(dataPath.PATH_MUSIC + "March_of_Steel.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::TITLE, res);

	res = SoundResource(dataPath.PATH_MUSIC + "Do_not_go_against_me.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::GAME, res);

	res = SoundResource(dataPath.PATH_WAVE + "WalkGround.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::WALK, res);

	res = SoundResource(dataPath.PATH_WAVE + "RunGround.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::RUN, res);

	res = SoundResource(dataPath.PATH_WAVE + "SwordSwing.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::SWING, res);

	res = SoundResource(dataPath.PATH_WAVE + "SwordAttack.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::SWORD_ATTACK, res);

	res = SoundResource(dataPath.PATH_WAVE + "Damage.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::DAMAGE, res);

	res = SoundResource(dataPath.PATH_WAVE + "Pary.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::PARY, res);

	res = SoundResource(dataPath.PATH_WAVE + "JustAvoid.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::JUST_AVOID, res);

	res = SoundResource(dataPath.PATH_WAVE + "DashSonic.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::DASH_SONIC, res);

	res = SoundResource(dataPath.PATH_WAVE + "Avoid.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::AVOID, res);

	res = SoundResource(dataPath.PATH_WAVE + "Jump.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::JUMP, res);

	res = SoundResource(dataPath.PATH_WAVE + "Electro.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::ELECTRO, res);

	res = SoundResource(dataPath.PATH_WAVE + "Thunder.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::THUNDER, res);

	res = SoundResource(dataPath.PATH_WAVE + "Spark.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::SPARK, res);

	res = SoundResource(dataPath.PATH_WAVE + "Slash_Deviation.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::SLASH_FINISH, res);

	res = SoundResource(dataPath.PATH_WAVE + "Repair.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::REPAIR, res);

	res = SoundResource(dataPath.PATH_WAVE + "Charge.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::CHARGE, res);

	res = SoundResource(dataPath.PATH_WAVE + "EvilGoa_Slash.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::EVIL_GOA_SLASH, res);

	res = SoundResource(dataPath.PATH_WAVE + "EvilGoa_Bom.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::EVIL_GOA_BOM, res);

	res = SoundResource(dataPath.PATH_WAVE + "EvilGoa_Roar.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::EVIL_GOA_ROAR, res);

	res = SoundResource(dataPath.PATH_WAVE + "EvilGoa_Awake_1.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::EVIL_GOA_AWAKE_1, res);

	res = SoundResource(dataPath.PATH_WAVE + "EvilGoa_Awake_2.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::EVIL_GOA_AWAKE_2, res);

	res = SoundResource(dataPath.PATH_WAVE + "Meteo.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::METEO, res);

	res = SoundResource(dataPath.PATH_WAVE + "Enemy_Flinch.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::ENEMY_FLINCH, res);

	res = SoundResource(dataPath.PATH_WAVE + "Enemy_Down.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::ENEMY_DOWN, res);

	res = SoundResource(dataPath.PATH_WAVE + "GameOver.mp3");
	resourcesMap_.emplace(SOUND_RESOURCE_ID::GAME_OVER, res);

}

void SoundResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void SoundResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
	instance_ = nullptr;
}

SoundResource SoundResourceManager::Load(const SOUND_RESOURCE_ID& id)
{
	SoundResource res = _Load(id);
	return res;
}

const bool SoundResourceManager::IsNullptr(void)
{
	return instance_ == nullptr;
}

SoundResourceManager::SoundResourceManager(void)
{
}

SoundResource SoundResourceManager::_Load(const SOUND_RESOURCE_ID& id)
{
	const auto& lPair = loadedMap_.find(id);

	// 既にロード済み
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// 登録されたリソースの中から検索
	const auto& rPair = resourcesMap_.find(id);
	assert(rPair != resourcesMap_.end() || !"SoundResourceManager::_Loadで登録されていないリソースをロードしようとした");


	// ロード処理
	rPair->second.Load();
	SoundResource ptrRes = SoundResource(rPair->second);
	loadedMap_.emplace(id, ptrRes);

	return ptrRes;
}