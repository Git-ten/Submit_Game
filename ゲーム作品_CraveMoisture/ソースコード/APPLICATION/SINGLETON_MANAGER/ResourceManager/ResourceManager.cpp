#include <cassert>
#include <DxLib.h>
#include "../../Application.h"
#include "../InputManager/InputManager.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	assert(instance_ == nullptr || !"ResourceManagerが作成されているのに再び生成しようとしています");

	instance_ = new ResourceManager();
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
	resourcesMap_.clear();
	loadedMap_.clear();

	
	DataPaths dataPath = Application::GetInstance().GetDataPaths();
	Resource res;

	auto& input = InputManager::GetInstance();

	AddImageKeboard(dataPath);
	AddImageController(dataPath);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "TitleBack.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TITLE_BACK, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "TitleName.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TITLE_NAME, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "PushAnyButton.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::PUSH_ANY_BUTTON, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "ShatDown.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SHUT_DOWN, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "LoadGage.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::LOAD_GAGE, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "LoadGage_Case.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::LOAD_GAGE_CASE, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "LoadBack.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::LOAD_BACK, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "GameOver.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::GAMEOVER_BACK, res);
	


	// 複数画像同時ロード---------------------------------------------
	res = Resource(Resource::RESOURCE_TYPE::IMGS, dataPath.PATH_IMAGE + "Numbers_4.png", 
		5, 2,
		102, 128);
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::NUMBERS, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMGS, dataPath.PATH_IMAGE + "Numbers_4_Red.png", 
		5, 2,
		102, 128);
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::NUMBERS_RED, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMGS, dataPath.PATH_IMAGE + "Symbols.png",
		5, 2,
		102, 128);
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SYMBOLS, res);
	// ---------------------------------------------------------------



	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "MenuCase.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::MENU_CASE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Question.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_QUESTION, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Yes.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::YES, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "No.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::NO, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "SelectCircle.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SELECT_CIRCLE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "PauseBack.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::PAUSE_GAME_BACK, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "ReturnTitle.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::PAUSE_RETURN_TITLE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "ReturnGame.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::PAUSE_RETURN_GAME, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "ResultTime.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::RESULT_TIME, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "ResultDamage.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::RESULT_DAMAGE, res);



	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "LevelSelectBack.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SELECT_LEVEL_BACK, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "LevelEasy.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::LEVEL_EASY, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "LevelNormal.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::LEVEL_NORMAL, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "LevelHard.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::LEVEL_HARD, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "EvilGoa_Icon.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SELECT_ENEMY_EVILGOA, res);



	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "SkyDome/NightSky/NightSky.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SKYDOME_NIGHT, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "SkyDome/SunSky/SunSky.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SKYDOME_SUN, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "SkyDonut/SunSky/SunSkyDonut.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SKYDONUT_SUN, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "PlayerJet/PlayerJet.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::DEFAULT_JET, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "EnemyJet/EnemyJet_2.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::ENEMY_DEFAULT_JET, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "EnemyJet/MobEnemy01.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::ENEMY_MOB_01, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "EnemyJet/MobEnemy02.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::ENEMY_MOB_02, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "EnemyJet/bee_boss.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::ENEMY_BEE_BOSS, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "Hero/Hero.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::PLAYER_HERO, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "Demon/Demon.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::ENEMY_DEMON, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "Bear/Tutorial_Bear.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::ENEMY_BEAR, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "Stage/Ruins/ruins_4.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::STAGE_RUINS, res);

	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "Weapon/Sword/Sword.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::HERO_SWORD, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "Gate/GateMist_2.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::GATE_RUINS, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MODEL, dataPath.PATH_MODEL + "Stage/Ground/Ground.mv1");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::STAGE_GROUND, res);



	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "PlayerHp.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::PLAYER_HP, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "PlayerHp_Case.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::PLAYER_HP_CASE, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "EnemyHp.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::ENEMY_HP, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "EnemyHp_Case.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::ENEMY_HP_CASE, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "DownGage.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::DOWN_GAGE, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "DownGage_Case.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::DOWN_GAGE_CASE, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "ContinueRepair.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::REPAIR_ICON, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "TechniqueCase_Unleash.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::ICON_CASE, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "EvilGoa_Name.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::EVIL_GOA_NAME, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "SkillGage.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SKILL_GAGE, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "SkillGage_Max.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SKILL_GAGE_MAX, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "SkillGage_Case.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SKILL_GAGE_CASE, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "WildBear_Name.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::WILD_BEAR_NAME, res);





	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MODEL + "Texture/Sph_Metal.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SPHERE_METAL, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MODEL + "Sword/textures/Sword_XR-8_Metallic.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SWORD_METALIC, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MODEL + "Sword/textures/Sword_XR-8_Roughness.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SWORD_ROUGHNESS, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MODEL + "Texture/Roughness.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::ROUGHNESS, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MODEL + "Stage/Ruins/textures/Ground048_1K-JPG_Roughness.jpg");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::RUINS_ROUGHNESS, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MODEL + "Hero/textures/Dreyar_specular.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::HERO_ROUGHNESS, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MODEL + "Stage/Ground/texture/grass_Roughness.jpg");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::GROUND_ROUGHNESS, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MODEL + "Texture/Thunder_8.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SWORD_LOCUS_TEXTURE, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MODEL + "Texture/Noise6.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::NOISE, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MODEL + "Texture/Claw_4.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::CLAW_TEXTURE, res);
	
	res = Resource(Resource::RESOURCE_TYPE::MOVIE, dataPath.PATH_MOVIE + "ResultMovie_Hero.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::RESULT_MOVIE_HERO, res);
	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_UI.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_UI, res);



	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MOVIE + "Tutorial_Camera.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_CAMERA_MOVIE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MOVIE + "Tutorial_Camera.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_CAMERA_MOVIE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MOVIE + "Tutorial_Move.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_MOVE_MOVIE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MOVIE + "Tutorial_Jump.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_JUMP_MOVIE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MOVIE + "Tutorial_Avoid.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_AVOID_MOVIE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MOVIE + "Tutorial_Attack.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_ATTACK_MOVIE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MOVIE + "Tutorial_Dash.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_DASH_MOVIE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MOVIE + "Tutorial_Charge.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_CHARGE_MOVIE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MOVIE + "Tutorial_Special.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_SPECIAL_MOVIE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MOVIE + "Tutorial_JustAvoidAttack.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_JUST_AVOID_ATTACK_MOVIE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MOVIE + "Tutorial_Technique.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_TECHNIQUE_MOVIE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MOVIE + "Tutorial_Skill.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_SKILL_MOVIE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_MOVIE + "Tutorial_Battle.mp4");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_BATTLE_MOVIE, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
	instance_ = nullptr;
}

Resource ResourceManager::Load(const RESOURCE_ID& id)
{
	Resource res = _Load(id);
	return res;
}

int ResourceManager::LoadModelDuplicate(const RESOURCE_ID& id)
{
	Resource res = _Load(id);

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

const bool ResourceManager::IsNullptr(void)
{
	return instance_ == nullptr;
}

void ResourceManager::AddImageKeboard(const DataPaths& dataPath)
{
	Resource res;

	if (InputManager::GetInstance().GetJPadNum() <= 0)
	{
		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "DeleteKey.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SHUT_DOWN_DELETE, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "DeleteButton.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::DELETE_TUTORIAL, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "ContinueRepair.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::REPAIR_ICON, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "SkillGage_Case.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SKILL_GAGE_CASE, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Camera.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_CAMERA, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Move.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_MOVE, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Jump.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_JUMP, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Avoid.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_AVOID, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Attack.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_ATTACK, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Technique.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_TECHNIQUE, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Skill.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_SKILL, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_CameraExplain.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_CAMERA_EXPLAIN, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_MoveExplain.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_MOVE_EXPLAIN, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_JumpExplain.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_JUMP_EXPLAIN, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_AvoidExplain.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_AVOID_EXPLAIN, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_AttackExplain.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_ATTACK_EXPLAIN, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_TechniqueExplain.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_TECHNIQUE_EXPLAIN, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_SkillExplain.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_SKILL_EXPLAIN, res);

	}

	
	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Dash.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_DASH, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Charge.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_CHARGE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Special.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_SPECIAL, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_JustAvoidAttack.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_JUST_AVOID_ATTACK, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Battle.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_BATTLE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_End.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_END, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Mouse.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_MOUSE, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "WASD_Key.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_WASD_KEY, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "R_Key.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_R_KEY, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Q_Key.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_Q_KEY, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Space_Key.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_SPACE_KEY, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "AllKey.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_ALL_KEY, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_DashExplain.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_DASH_EXPLAIN, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_ChargeExplain.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_CHARGE_EXPLAIN, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_SpecialExplain.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_SPECIAL_EXPLAIN, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_JustAvoidAttackExplain.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_JUST_AVOID_ATTACK_EXPLAIN, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_FinalExplain.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_BATTLE_EXPLAIN, res);

}

void ResourceManager::AddImageController(const DataPaths& dataPath)
{
	Resource res;

	if (InputManager::GetInstance().GetJPadNum() > 0)
	{
		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "DeleteKey_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SHUT_DOWN_DELETE, res);


		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Camera_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_CAMERA, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Move_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_MOVE, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Jump_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_JUMP, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Avoid_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_AVOID, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Attack_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_ATTACK, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Technique_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_TECHNIQUE, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_Skill_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_SKILL, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_CameraExplain_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_CAMERA_EXPLAIN, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_MoveExplain_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_MOVE_EXPLAIN, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_JumpExplain_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_JUMP_EXPLAIN, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_AvoidExplain_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_AVOID_EXPLAIN, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_AttackExplain_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_ATTACK_EXPLAIN, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_TechniqueExplain_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_TECHNIQUE_EXPLAIN, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Tutorial_SkillExplain_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_SKILL_EXPLAIN, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "ContinueRepair_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::REPAIR_ICON, res);

		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "SkillGage_Case_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::SKILL_GAGE_CASE, res);
	
		res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "DeleteButton_Contoroller.png");
		resourcesMap_.emplace(ResourceManager::RESOURCE_ID::DELETE_TUTORIAL, res);
	}

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Contorolle_D_Pad.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_D_PAD, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Contorolle_X.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_X_BUTTON, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Contorolle_A.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_A_BUTTON, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Contorolle_B.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_B_BUTTON, res);

	res = Resource(Resource::RESOURCE_TYPE::IMG, dataPath.PATH_IMAGE + "Contoroller.png");
	resourcesMap_.emplace(ResourceManager::RESOURCE_ID::TUTORIAL_CONTOROLLER, res);

}

ResourceManager::ResourceManager(void)
{

}

Resource ResourceManager::_Load(const RESOURCE_ID& id)
{
	const auto& lPair = loadedMap_.find(id);

	// 既にロード済み
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// 登録されたリソースの中から検索
	const auto& rPair = resourcesMap_.find(id);
	assert(rPair != resourcesMap_.end() || !"ResourceManager::_Loadで登録されていないリソースをロードしようとした");


	// ロード処理
	rPair->second.Load();
	Resource ptrRes = Resource(rPair->second);
	loadedMap_.emplace(id, ptrRes);

	return ptrRes;
}