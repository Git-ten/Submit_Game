#pragma once
#include <string>
#include <vector>
#include "../ActorData.h"

class PlayerData : public ActorData
{
public:
    
    int dashSpeed_;
    int jumpPower_;
    float flinchParam_;
    float flinchParamMax_;
    int avoidSpeed_;
    float skillGage_;
    float skillGageMax_;

    float chargeStep;                                   // チャージ中のタイミング
    float attackCollSkill_;

    std::string weaponBone;

};

