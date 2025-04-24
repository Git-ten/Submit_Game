#pragma once
class NodeBase
{
public:

	//コンストラクタ
	NodeBase(int index);

	//仮想デストラクタ
	virtual ~NodeBase(void);

	//仮想更新
	virtual void Update(void);

	//条件ノードで使用する
	virtual int IsCondition(void)const;
	//アクションノードで使用する
	virtual bool IsEndAction(void)const;

	virtual void SetIsEndAction(bool flg);

	//継承先で共通して使用する関数
	int GetIndex(void)const;
	void SetIndex(int index);
	bool IsEndNode(void)const;

protected:

	//自身のノード番号
	int NodeIndex_;

	//自身が終端ノードか
	bool IsEndNode_;

};