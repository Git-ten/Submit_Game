#pragma once
#include "../../APPLICATION/SINGLETON_MANAGER/ResourceManager/ResourceManager.h"
#include "../ActorData.h"

class EnemyData : public ActorData
{
public:

    // ラグタイム情報
    struct RagTimesData
    {
        float attackRagTime_1;
        float attackRagTime_2;
        float attackRagTime_3;
        float attackRagTime_4;
        float attackRagTime_5;
        float attackRagTime_6;
        float attackAwakeRagTime;
    };

    // 当たり判定アニメステップ
    struct CollisionStepData
    {
        float attackCollStep_1;
        float attackCollStep_2;
        float attackCollStep_3;
        float attackCollStep_4;
        float attackCollStep_5;
        float attackCollStep_6;
        float attackCollAwake;
    };

    RagTimesData ragTimeData_;

    CollisionStepData collStepData_;

    ResourceManager::RESOURCE_ID enemyNameId_ = ResourceManager::RESOURCE_ID::EVIL_GOA_NAME;

    int dashSpeed_;
    int jumpPower_;
    float downGage_;
    float downGageMax_;
    float flinchParam_;
    float flinchParamMax_;

    std::vector<std::string>collBones_;
};