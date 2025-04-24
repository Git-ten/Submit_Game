#pragma once
#include<memory>
#include<vector>
#include<functional>

#define INT_INDEX(a)(static_cast<int>(a))

class NodeBase;
class Edge;

class BehaviorBase
{
public:

	//最大ノード探索回数
	static constexpr int MAX_NODE_SEARCH = 3;
	//コスト無し(現在コストの初期化用)
	static constexpr float NONE_COST = 0.0f;
	//デフォルトのノードコスト
	static constexpr float DEFAULT_NODE_COST = 0.1f;
	//インデックスなし
	static constexpr int NONE_INDEX = -1;

	//コンストラクタ
	BehaviorBase(void);

	//仮想デストラクタ
	virtual ~BehaviorBase(void);

	//初期化
	void Init(void);

	//仮想更新
	virtual void Update(void) = 0;

	//デバッグ用描画
	virtual void DebugDraw(void);

	void SetNowIndex(int index);
	int GetNowIndex(void)const;

	//ノードの終了処理
	void EndNodeAction(int num);

protected:

	using NodesData = std::vector<std::unique_ptr<NodeBase>>;
	using EdgesData = std::vector<std::unique_ptr<Edge>>;

	//ノード
	NodesData nodes_;
	//エッジ
	EdgesData edges_;

	//ノード探索中の現在地
	int nowIndex_;
	//現在いるノードのコスト
	float nowCost_;



	//内部でのみ使用する関数
	void InitNode(void);
	void InitEdge(void);
	//仮想ノード作成
	virtual void CreateNode(void);
	//仮想ノードをエッジで結合
	virtual void ConnectNode(void);
	//継承先で共通して使用する関数
	bool CheckNode(int index);
	bool CheckEdgeAll(void);
	void AddConditionNode(int index, std::function<int(void)> func);
	void AddActionNode(int index, std::function<void(void)> func);
	void AddConnectEdge(int fromIdx, int toIdx, float cost);
	int SearchNodeIdx(int index);
	int SearchEdgeIdx(int fromIdx, int toIdx);
	//条件を満たした条件ノードを
	//ルールにのとって取捨選択し、次のノードのインデックスを返す
	virtual int ConditionBranch(void);
	void NodeTransition(int index, int& Idx);
};