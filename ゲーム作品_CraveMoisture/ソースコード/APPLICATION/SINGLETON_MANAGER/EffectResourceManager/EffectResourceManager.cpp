#include <cassert>
#include <DxLib.h>
#include <EffekseerForDxLib.h>
#include "../../Application.h"
#include "../../../Common/DataLoad/DataPaths.h"
#include "EffectResourceManager.h"

EffectResourceManager* EffectResourceManager::instance_ = nullptr;

void EffectResourceManager::CreateInstance(void)
{
	assert(instance_ == nullptr || !"EffectResourceManagerが作成されているのに再び生成しようとしています");

	instance_ = new EffectResourceManager();
	instance_->Init();
}

EffectResourceManager& EffectResourceManager::GetInstance(void)
{
	return *instance_;
}

void EffectResourceManager::Init(void)
{
	resourcesMap_.clear();
	loadedMap_.clear();


	DataPaths dataPath = Application::GetInstance().GetDataPaths();
	EffectResource res;

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "Smoke.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::SMOKE, res);
	
	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "Smoke_Black.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::BLACK_SMOKE, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "Slash_Right.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_RIGHT, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "Slash_Left.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_LEFT, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "Slash_Horiz.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_HORIZ, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "Slash_Duo.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_DUO, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "Slash_Up.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_UP, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "Slash_Torio.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_TORIO, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "Slash_Deviation.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_DEVIATION, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "Slash_Accel.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_ACCEL, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "Slash_Flash.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_FLASH, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "Spark.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::SPARK, res);
	
	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "SonicBoom.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::SONIC_BOOM, res);
	
	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "Lightning.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::LIGHTNING, res);
	
	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT+ "ThunderBolt.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::THUNDER_BOLT, res);
	
	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT+ "FinishThunder.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::THUNDER_BOLT_ONE, res);



	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "DemonReadyAttack.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::ENEMY_ATTACK_READY, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "DemonClaw.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "DemonClaw_Right.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_RIGHT, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "DemonClaw_Left.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_LEFT, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "DemonClaw_Duo.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_DUO, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "DemonClaw_Destroy.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_DESTROY, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "DemonBom.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::BOMB, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "EvilGoa_Awake_1.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_1, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "EvilGoa_Awake_2.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_2, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "DemonAwakeSpark.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_ATTACK_1, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT + "DemonAwakeMeteo.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_ATTACK_2, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT+ "Stage_Fire.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::STAGE_FIRE, res);



	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT+ "Charge.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::CHARGE, res);
	
	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT+ "ChargeFinish.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::CHARGE_FINISH, res);
	
	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT+ "Repair.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::REPAIR, res);
	
	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT+ "DamageSpark.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::DAMAGE_SPARK, res);
	
	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT+ "PlayerDamage.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::PLAYER_DAMAGE_SPARK, res);
	
	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT+ "PhantomBullet.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::PHANTOM_BULLET, res);
	
	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT+ "DarkBoal.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::DARK_BULLET, res);
	


	res = EffectResource(EffectResource::EFFECT_TYPE::EFFEKSEER_3D, dataPath.PATH_EFFECT+ "Parys.efkefc");
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::PARYS, res);



	res = EffectResource(EffectResource::EFFECT_TYPE::SPLITE, dataPath.PATH_EFFECT + "high_explosion.png",
		4, 4,
		256, 256);
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::HIGHT_POLI_EXPLOSION, res);

	res = EffectResource(EffectResource::EFFECT_TYPE::SPLITE, dataPath.PATH_EFFECT + "low_explosion.png",
		4, 4,
		128, 128);
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::LOW_POLI_EXPLOSION, res);
	
	res = EffectResource(EffectResource::EFFECT_TYPE::SPLITE, dataPath.PATH_EFFECT + "shot.png",
		1, 1,
		32, 32);
	resourcesMap_.emplace(EffectResourceManager::EFFECT_RESOURCE_ID::PLAYER_BULLET, res);

}

void EffectResourceManager::Update(void)
{
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
	//エフェクトの更新
	UpdateEffekseer3D();
	UpdateEffekseer2D();
}

void EffectResourceManager::Draw2DEffect(void)
{
	// エフェクト2Dの描画
	DrawEffekseer2D();
}

void EffectResourceManager::Draw3DEffect(void)
{
	// エフェクト3Dの描画
	DrawEffekseer3D();
}

void EffectResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void EffectResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
	instance_ = nullptr;
}

EffectResource EffectResourceManager::Load(const EFFECT_RESOURCE_ID& id)
{
	EffectResource res = _Load(id);
	return res;
}

const EffectResource::EFFECT_TYPE EffectResourceManager::GetEffectType(const EFFECT_RESOURCE_ID& id)const
{
	const auto& ite = loadedMap_.find(id);
	assert(ite != loadedMap_.end() || !"ロードしていないエフェクトのエフェクトタイプは取得できません");

	return loadedMap_.at(id).resType_;
}

const bool EffectResourceManager::IsNullptr(void)
{
	return instance_ == nullptr;
}

EffectResourceManager::EffectResourceManager(void)
{
}

EffectResource EffectResourceManager::_Load(const EFFECT_RESOURCE_ID& id)
{
	const auto& lPair = loadedMap_.find(id);

	// 既にロード済み
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// 登録されたリソースの中から検索
	const auto& rPair = resourcesMap_.find(id);
	assert(rPair != resourcesMap_.end() || !"EffectResourceManager::_Loadで登録されていないリソースをロードしようとした");


	// ロード処理
	rPair->second.Load();
	EffectResource ptrRes = EffectResource(rPair->second);
	loadedMap_.emplace(id, ptrRes);

	return ptrRes;
}
