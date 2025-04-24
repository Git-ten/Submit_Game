#pragma once
#include <memory>
#include "../../Utility/VectorHeader.h"

class IsDistance
{
public:

	// �ߋ�������
	static constexpr float CONDITION_NEAR_DISTANCE = 1500.0f;
	// ���ߋ�������
	static constexpr float CONDITION_MOST_NEAR_DISTANCE = 500.0f;
	// ����������
	static constexpr float CONDITION_MEDIUM_DISTANCE = 3000.0;
	// ����������
	static constexpr float CONDITION_FAR_DISTANCE = 5000.0;


	// �R���X�g���N�^
	IsDistance(Position3DF& pos, Position3DF& targetPos):
		targetPos_(targetPos),
		pos_(pos)
	{
	}
	// �f�X�g���N�^
	~IsDistance(void)
	{
	}

	//�ߋ����ł��邩
	const bool IsNearDistance(void)
	{
		const float dis = VSize(VSub(targetPos_, pos_));
		return dis <= CONDITION_NEAR_DISTANCE;
	}
	//�G��鐡�O�Ȃقǋ߂�
	const bool IsMostNearDistance(void)
	{
		const float dis = VSize(VSub(targetPos_, pos_));
		return dis <= CONDITION_MOST_NEAR_DISTANCE;
	}
	//�������ł��邩
	const bool IsMediumDistance(void)
	{
		const float dis = VSize(VSub(targetPos_, pos_));
		return dis <= CONDITION_MEDIUM_DISTANCE;
	}
	//�������ł��邩
	const bool IsFarDistance(void)
	{
		const float dis = VSize(VSub(targetPos_, pos_));
		return dis <= CONDITION_FAR_DISTANCE;
	}

	// ����A�N�^�[�Ǝg�p�҃A�N�^�[�Ƃ̋���
	const float DistanceActors(void)
	{
		return VSize(VSub(targetPos_, pos_));
	}

	// ����A�N�^�[����g�p�҃A�N�^�[�܂ŕ����x�N�g��
	const Dir3DF DirShareActorToActor(void)
	{
		return VNorm(VSub(targetPos_, pos_));
	}

private:

	// ����A�N�^�[�̏��
	const Position3DF& targetPos_;

	// �g�p�҃A�N�^�[�̍��W
	const Position3DF& pos_;

	// 

};