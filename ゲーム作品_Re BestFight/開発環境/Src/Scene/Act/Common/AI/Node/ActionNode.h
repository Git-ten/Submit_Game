#pragma once
#include<functional>
#include "NodeBase.h"
class ActionNode : public NodeBase
{
public:

	//コンストラクタ
	ActionNode(int index, std::function<void(void)> action);
	//デストラクタ
	~ActionNode(void)override;

	//更新
	void Update(void)override;

	//アクションが終了処理をしているか
	bool IsEndAction()const override;

	//アクションが終了したかをセット
	void SetIsEndAction(bool flg)override;

private:

	using ActionFunc = std::function<void(void)>;

	//アクション関数を格納
	//実行はノードの作成元
	ActionFunc action_;

	//処理中か否か
	bool IsEndAction_;

};