#pragma once
#include <string>
#include <vector>

class DataEnemyControlleParam
{
public:

	float sphereSize;                                   // ���g�̓����蔻�苅�̔��a
    std::vector<float>attackCollStep_1;                 // �U��_1�̓����蔻��A�j���[�V�����X�e�b�v
    std::vector<float>attackCollStep_2;                 // �U��_2�̓����蔻��A�j���[�V�����X�e�b�v
    std::vector<float>attackCollStep_3;                 // �U��_3�̓����蔻��A�j���[�V�����X�e�b�v
    std::vector<float>attackCollStep_4;                 // �U��_4�̓����蔻��A�j���[�V�����X�e�b�v
    std::vector<float>attackCollStep_5;                 // �U��_5�̓����蔻��A�j���[�V�����X�e�b�v
    std::vector<float>attackCollStep_6;                 // �U��_6�̓����蔻��A�j���[�V�����X�e�b�v

    std::vector<float>attackCollStep_AwakeAttack;       // �o���U���̓����蔻��A�j���[�V�����X�e�b�v
    std::vector<float>attackCollStep_3_Awake;           // �o���U��_3�̓����蔻��A�j���[�V�����X�e�b�v
    std::vector<float>attackCollStep_6_Awake;           // �o���U��_3�̓����蔻��A�j���[�V�����X�e�b�v

    std::vector<std::string>collisionBones;             // �����蔻����s���{�[����
    std::vector<float>collisionSphereSize_;             // �����蔻��{�[���̂��ꂼ��̓����蔻�苅�̔��a

    float nearDistance;                                 // �߂�����
    float mostNearDistance;                             // �G��钼�O�Ȃقǋ߂�����
    float mediumDisntance;                              // ������
    float farDistance;                                  // ������

    float attackRagTime_1;                              // �U��_1�̃��O
    float attackRagTime_2;                              // �U��_2�̃��O
    float attackRagTime_3;                              // �U��_3�̃��O
    float attackRagTime_4;                              // �U��_4�̃��O
    float attackRagTime_5;                              // �U��_5�̃��O
    float attackRagTime_6;                              // �U��_6�̃��O
    float attackAwakeRagTime;                           // �o���U���̃��O

};