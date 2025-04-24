#pragma once
#include <string>
#include <vector>

class DataEnemyControlleParam
{
public:

	float sphereSize;                                   // 自身の当たり判定球の半径
    std::vector<float>attackCollStep_1;                 // 攻撃_1の当たり判定アニメーションステップ
    std::vector<float>attackCollStep_2;                 // 攻撃_2の当たり判定アニメーションステップ
    std::vector<float>attackCollStep_3;                 // 攻撃_3の当たり判定アニメーションステップ
    std::vector<float>attackCollStep_4;                 // 攻撃_4の当たり判定アニメーションステップ
    std::vector<float>attackCollStep_5;                 // 攻撃_5の当たり判定アニメーションステップ
    std::vector<float>attackCollStep_6;                 // 攻撃_6の当たり判定アニメーションステップ

    std::vector<float>attackCollStep_AwakeAttack;       // 覚醒攻撃の当たり判定アニメーションステップ
    std::vector<float>attackCollStep_3_Awake;           // 覚醒攻撃_3の当たり判定アニメーションステップ
    std::vector<float>attackCollStep_6_Awake;           // 覚醒攻撃_3の当たり判定アニメーションステップ

    std::vector<std::string>collisionBones;             // 当たり判定を行うボーン名
    std::vector<float>collisionSphereSize_;             // 当たり判定ボーンのそれぞれの当たり判定球の半径

    float nearDistance;                                 // 近い距離
    float mostNearDistance;                             // 触れる直前なほど近い距離
    float mediumDisntance;                              // 中距離
    float farDistance;                                  // 遠距離

    float attackRagTime_1;                              // 攻撃_1のラグ
    float attackRagTime_2;                              // 攻撃_2のラグ
    float attackRagTime_3;                              // 攻撃_3のラグ
    float attackRagTime_4;                              // 攻撃_4のラグ
    float attackRagTime_5;                              // 攻撃_5のラグ
    float attackRagTime_6;                              // 攻撃_6のラグ
    float attackAwakeRagTime;                           // 覚醒攻撃のラグ

};