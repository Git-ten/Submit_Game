#pragma once
#include<functional>

class Node
{
private:

	//アクション関数ポインタ
	//更新用関数ポインタ
	using ActionFunc = std::function<void(void)>;

public:

	//コンストラクタ
	Node(int index, const ActionFunc& action) :
		nodeIndex_(index),
		action_(action)
	{

	}

	//デストラクタ
	~Node(void)
	{

	}


	//継承先で共通して使用する関数
	const int GetIndex(void)const
	{
		return nodeIndex_;
	}

	//アクション処理
	void NodeAction(void)
	{
		//アクションが登録されていなければ何もしない
		if (action_ == nullptr)return;

		action_();
	}

private:

	//更新関数ポインタ
	const ActionFunc action_;

	//自身のノード番号
	const int nodeIndex_;

};