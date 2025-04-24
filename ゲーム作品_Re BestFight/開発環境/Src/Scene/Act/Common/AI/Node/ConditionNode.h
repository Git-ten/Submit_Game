#pragma once
#include<functional>
#include "NodeBase.h"
class ConditionNode : public NodeBase
{
public:

	//コンストラクタ
	ConditionNode(int index, std::function<int(void)> condition);
	//デストラクタ
	~ConditionNode(void)override;

	//条件を満たしたか
	int IsCondition(void)const override;

private:

	using ConditionFunc = std::function<int(void)>;

	//条件関数を格納
	//実行はノードの作成元
	ConditionFunc condition_;

};