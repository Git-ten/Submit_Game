#pragma once
#include <memory>
#include <map>
#include <string>
#include <functional>
#include "../../Common/Time.h"

class Node;
class Edge;

class BehaviorBase
{
public:

	// コンストラクタ
	BehaviorBase(void);
	// 仮想デストラクタ
	virtual ~BehaviorBase(void);

	// 初期化
	void Init(void);
	// 更新
	void Update(void);


	// アクション終了後、少しだけAIの思考ループをロック
	void LockThinkShortTime(void);
	// アクション終了後、長くAIの思考ループをロック
	void LockThinkLongTime(void);

	// 完全ロック
	void PerfectLock(void);
	// 完全ロックを解除
	void UnPerfectLock(void);
	
	// アクションが終了した
	void EndAIAction(void);
	// アクションの終了状態を取得
	bool IsEndAIAction(void)const;

private:

	// このクラスでのみ使用するデータ型
	// ノード実体の連想配列(key:インデックス)
	using NodesData = std::map<int, std::shared_ptr<Node>>;
	// エッジ実体の連想配列(key:エッジ名)
	using EdgesData = std::map<std::string, std::unique_ptr<Edge>>;

private:

	typedef int NODE_INDEX;

	//AIのアクションが終了したか
	bool isEndAIAction_;


	//AIがロック中か
	bool IsLockThink(void)const;
	//ロック時間の計算処理
	void LockTimeProcess(void);

protected:

	//ノード
	NodesData nodes_;
	//エッジ
	EdgesData edges_;


	//AIのロック時間
	//計測中はAIが一切思考しない
	Time lockThinkTime_;
	// ロックする時間
	float lockTime_;

	// AIの完全ロック
	// 関数で解除しない限りずっとロック
	bool isPerfectLock_;


	//ノード探索中の現在地インデックス
	NODE_INDEX nowIndex_;



	//内部でのみ使用する関数
	void InitNode(void);
	void InitEdge(void);

	// 特別性の高い更新
	virtual void UpdateSpecial(void) = 0;

	//ノード作成
	virtual void CreateNode(void) = 0;
	//ノードをエッジで結合
	virtual void ConnectEdge(void) = 0;


	// ノードの追加
	void AddNode(int index, std::function<void(void)> func);
	// エッジの追加
	void AddEdge(const std::string& edgeName,
		int fromIndex, 
		const std::vector<int>& end,
		const std::vector<std::function<bool(void)>>& condition, 
		const std::vector<float>& weight);


	//条件を満たした条件ノードを
	//ルールにのとって取捨選択し、次のノードのインデックスを返す
	int TransitionNextNode(int fromIdx);
	void NodeTransition(void);



	//アクションを実行
	void NodeAction(void);


	// 指定した時間だけAIの思考ループをロック
	void LockThink(const float time);

	// 完全ロックしているか
	bool IsPerfectLock(void);

};