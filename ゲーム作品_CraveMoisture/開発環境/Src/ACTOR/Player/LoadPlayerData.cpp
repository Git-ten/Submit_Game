#include <cassert>
#include "LoadPlayerData.h"
#include "../../Common/DataLoad/DataPaths.h"
#include "PlayerData.h"

PlayerData LoadPlayerData::LoadedPlayerData(
	const DataPaths& dataPath, const LoadData::LoadDataInfo& info)
{
	LoadData loadData;
	PlayerData paramData = PlayerData();

	const std::string PLAYER_DATA_NAME = dataPath.PATH_JSON + info.fileName;
	const std::string COLL_SPHERE_SIZE_NAME = "CollSphere";
	const std::string WEAPON_BONE_NAME = "WeaponBone";

	const std::string STATUS_PARAM_NAME = "Param";
	const std::string STATUS_HP_NAME = "hp";
	const std::string STATUS_HP_MAX_NAME = "hpMax";
	const std::string STATUS_SPEED_NAME = "speed";
	const std::string STATUS_DASH_SPEED_NAME = "dashSpeed";
	const std::string STATUS_JUMP_POWER_NAME = "jumpPower";
	const std::string STATUS_FLINCH_NAME = "flinchParam";
	const std::string STATUS_FLINCH_MAX_NAME = "flinchParamMax";
	const std::string STATUS_AVOID_SPEED_NAME = "avoidSpeed";
	const std::string STATUS_SKILL_GAGE_NAME = "skillGage";
	const std::string STATUS_SKILL_MAX_NAME = "skillGageMax";
	const std::string STATUS_ATTACK_POWER_NAME = "attackPower";

	const std::string STATUS_COLL_SKILL_NAME = "attackCollision_Skill";

	const std::string STATUS_CHARGE_STEP_NAME = "chargeStep";


	loadData.LoadStart(PLAYER_DATA_NAME);

	loadData.LoadedFloat(COLL_SPHERE_SIZE_NAME, "size", paramData.collitionSphereSize_);
	loadData.LoadedFloat(COLL_SPHERE_SIZE_NAME, "offset", paramData.collisionSphereOffset_);

	loadData.LoadedString(WEAPON_BONE_NAME, "param", paramData.weaponBone);

	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_HP_NAME, paramData.hp_);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_HP_MAX_NAME, paramData.maxHp_);
	loadData.LoadedInt(STATUS_PARAM_NAME, STATUS_SPEED_NAME, paramData.speed_);
	loadData.LoadedInt(STATUS_PARAM_NAME, STATUS_DASH_SPEED_NAME, paramData.dashSpeed_);
	loadData.LoadedInt(STATUS_PARAM_NAME, STATUS_JUMP_POWER_NAME, paramData.jumpPower_);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_FLINCH_NAME, paramData.flinchParam_);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_FLINCH_MAX_NAME, paramData.flinchParamMax_);
	loadData.LoadedInt(STATUS_PARAM_NAME, STATUS_AVOID_SPEED_NAME, paramData.avoidSpeed_);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_SKILL_GAGE_NAME, paramData.skillGage_);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_SKILL_MAX_NAME, paramData.skillGageMax_);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_POWER_NAME, paramData.attackPower_);

	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_COLL_SKILL_NAME, paramData.attackCollSkill_);

	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_CHARGE_STEP_NAME, paramData.chargeStep);

	loadData.LoadEnd();

	return paramData;
}
