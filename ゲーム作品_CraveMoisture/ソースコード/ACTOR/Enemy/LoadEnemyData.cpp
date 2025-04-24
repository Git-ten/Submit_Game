#include <cassert>
#include "EnemyData.h"
#include "../../Common/DataLoad/DataPaths.h"
#include "../../Common/DataLoad/LoadData.h"
#include "LoadEnemyData.h"

EnemyData LoadEnemyData::LoadedEnemyData(
    const DataPaths& dataPath, const LoadData::LoadDataInfo& info)
{
	LoadData loadData;
	EnemyData paramData = EnemyData();


	const std::string PLAYER_DATA_NAME = dataPath.PATH_JSON + info.fileName;
	const std::string COLL_SPHERE_SIZE_NAME = "CollSphere";
	const std::string COLL_BONE_NAME = "collisionBone";

	const std::string STATUS_PARAM_NAME = "Param";
	const std::string STATUS_HP_NAME = "hp";
	const std::string STATUS_HP_MAX_NAME = "hpMax";
	const std::string STATUS_SPEED_NAME = "speed";
	const std::string STATUS_DASH_SPEED_NAME = "dashSpeed";
	const std::string STATUS_JUMP_POWER_NAME = "jumpPower";
	const std::string STATUS_DOWN_GAGE_NAME = "downGage";
	const std::string STATUS_DOWN_GAGE_MAX_NAME = "downGageMax";
	const std::string STATUS_FLINCH_NAME = "flinchParam";
	const std::string STATUS_FLINCH_MAX_NAME = "flinchParamMax";
	const std::string STATUS_ATTACK_POWER_NAME = "attackPower";

	const std::string STATUS_ATTACK_RAG_1 = "attackRagTime_1";
	const std::string STATUS_ATTACK_RAG_2 = "attackRagTime_2";
	const std::string STATUS_ATTACK_RAG_3 = "attackRagTime_3";
	const std::string STATUS_ATTACK_RAG_4 = "attackRagTime_4";
	const std::string STATUS_ATTACK_RAG_5 = "attackRagTime_5";
	const std::string STATUS_ATTACK_RAG_6 = "attackRagTime_6";
	const std::string STATUS_ATTACK_RAG_AWAKE = "attackAwakeRagTime";

	const std::string STATUS_ATTACK_COLL_1 = "attackCollision_1";
	const std::string STATUS_ATTACK_COLL_2 = "attackCollision_2";
	const std::string STATUS_ATTACK_COLL_3 = "attackCollision_3";
	const std::string STATUS_ATTACK_COLL_4 = "attackCollision_4";
	const std::string STATUS_ATTACK_COLL_5 = "attackCollision_5";
	const std::string STATUS_ATTACK_COLL_6 = "attackCollision_6";
	const std::string STATUS_ATTACK_COLL_AWAKE = "attackCollision_Awake";

	loadData.LoadStart(PLAYER_DATA_NAME);

	loadData.LoadedFloat(COLL_SPHERE_SIZE_NAME, "size", paramData.collitionSphereSize_);
	loadData.LoadedFloat(COLL_SPHERE_SIZE_NAME, "offset", paramData.collisionSphereOffset_);

	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_HP_NAME, paramData.hp_);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_HP_MAX_NAME, paramData.maxHp_);
	loadData.LoadedInt(STATUS_PARAM_NAME, STATUS_SPEED_NAME, paramData.speed_);
	loadData.LoadedInt(STATUS_PARAM_NAME, STATUS_DASH_SPEED_NAME, paramData.dashSpeed_);
	loadData.LoadedInt(STATUS_PARAM_NAME, STATUS_JUMP_POWER_NAME, paramData.jumpPower_);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_DOWN_GAGE_NAME, paramData.downGage_);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_DOWN_GAGE_MAX_NAME, paramData.downGageMax_);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_FLINCH_NAME, paramData.flinchParam_);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_FLINCH_MAX_NAME, paramData.flinchParamMax_);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_POWER_NAME, paramData.attackPower_);

	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_RAG_1, paramData.ragTimeData_.attackRagTime_1);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_RAG_2, paramData.ragTimeData_.attackRagTime_2);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_RAG_3, paramData.ragTimeData_.attackRagTime_3);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_RAG_4, paramData.ragTimeData_.attackRagTime_4);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_RAG_5, paramData.ragTimeData_.attackRagTime_5);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_RAG_6, paramData.ragTimeData_.attackRagTime_6);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_RAG_AWAKE, paramData.ragTimeData_.attackAwakeRagTime);
	
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_COLL_1, paramData.collStepData_.attackCollStep_1);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_COLL_2, paramData.collStepData_.attackCollStep_2);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_COLL_3, paramData.collStepData_.attackCollStep_3);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_COLL_4, paramData.collStepData_.attackCollStep_4);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_COLL_5, paramData.collStepData_.attackCollStep_5);
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_COLL_6, paramData.collStepData_.attackCollStep_6);
	
	loadData.LoadedFloat(STATUS_PARAM_NAME, STATUS_ATTACK_COLL_AWAKE, paramData.collStepData_.attackCollAwake);

	loadData.LoadedStringVector(STATUS_PARAM_NAME, COLL_BONE_NAME, paramData.collBones_);

	loadData.LoadEnd();

	return paramData;
}
