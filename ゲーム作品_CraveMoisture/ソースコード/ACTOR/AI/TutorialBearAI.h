#pragma once
#include "../Enemy/EnemyState.h"
#include "EnemyAIBase.h"

class TutorialBearAI : public EnemyAIBase
{
public:

	//コンストラクタ
	TutorialBearAI(EnemyState::STATE& state,
		const DIR_BACK& dirBack,
		const EnemyData::RagTimesData& ragTimes);
		//デストラクタ
	~TutorialBearAI(void)override;

private:

	//ノードをエッジで結合
	void ConnectEdge(void)override;

};