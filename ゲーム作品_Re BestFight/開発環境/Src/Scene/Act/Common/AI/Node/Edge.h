#pragma once

class Edge
{
public:

	//コンストラクタ
	Edge(int from, int to, float cost);

	//デストラクタ
	~Edge(void);

	//コストを取得
	float GetCost(void)const;
	//前ノードのインデックスを取得
	int GetPrevIndex(void)const;
	//次ノードのインデックスを取得
	int GetNextIndex(void)const;

private:

	int idxFrom_;
	int idxTo_;

	float passCost_;

};