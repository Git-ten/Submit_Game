#pragma once
#include "../Enemy/EnemyState.h"
#include "EnemyAIBase.h"

class TutorialBearAI : public EnemyAIBase
{
public:

	//�R���X�g���N�^
	TutorialBearAI(EnemyState::STATE& state,
		const DIR_BACK& dirBack,
		const EnemyData::RagTimesData& ragTimes);
		//�f�X�g���N�^
	~TutorialBearAI(void)override;

private:

	//�m�[�h���G�b�W�Ō���
	void ConnectEdge(void)override;

};