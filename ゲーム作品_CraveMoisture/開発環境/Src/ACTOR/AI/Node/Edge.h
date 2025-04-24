#pragma once
#include <cassert>
#include<functional>
#include<vector>

class Node;

class Edge
{
public:

	//コンストラクタ
	Edge(int from, 
		const std::vector<int>&end,
		const std::vector<std::function<bool(void)>>&condition,
		const std::vector<float>& weight) :

		fromIdx_(from),
		endIdxs_(end),
		weight_(weight),
		condition_(condition)
	{
	}


	//デストラクタ
	~Edge(void)
	{
	}

	//コストを取得
	const float GetCost(int number)const
	{
		assert(number < weight_.size() || !"登録したEdgeのコスト数以上は指定できません");
		assert(0 <= number || !"-1は許容できません");

		return weight_[number];
	}


	//フロムノードのインデックスを取得
	const int GetFromIndex(void)const
	{
		return fromIdx_;
	}

	//エンドノードの登録数を取得
	const int GetEndNodeNum(void)const
	{
		return static_cast<int>(endIdxs_.size());
	}

	//指定するエンドノードのインデックスを取得
	const int GetEndIndex(int number)const
	{
		assert(number < endIdxs_.size() || !"登録したEdgeのインデックス数以上は指定できません");
		assert(0 <= number || !"-1は許容できません");

		return endIdxs_[number];
	}


	//遷移条件を達成したか
	const bool IsCondition(int number)const
	{
		assert(number < condition_.size() || !"登録したEdgeのコンディション数以上は指定できません");
		assert(0 <= number || !"-1は許容できません");

		return condition_[number]();
	}

private:

	//遷移条件関数ポインタ
	using ConditionFunc = std::vector<std::function<bool(void)>>;

private:

	// 遷移条件
	ConditionFunc condition_;

	//フロムノードのインデックス
	int fromIdx_;
	//エンドノードのインデックス
	std::vector<int> endIdxs_;

	//エッジの重さ
	std::vector<float> weight_;

};