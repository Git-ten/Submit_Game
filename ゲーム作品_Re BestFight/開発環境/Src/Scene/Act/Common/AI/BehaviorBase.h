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

	//�ő�m�[�h�T����
	static constexpr int MAX_NODE_SEARCH = 3;
	//�R�X�g����(���݃R�X�g�̏������p)
	static constexpr float NONE_COST = 0.0f;
	//�f�t�H���g�̃m�[�h�R�X�g
	static constexpr float DEFAULT_NODE_COST = 0.1f;
	//�C���f�b�N�X�Ȃ�
	static constexpr int NONE_INDEX = -1;

	//�R���X�g���N�^
	BehaviorBase(void);

	//���z�f�X�g���N�^
	virtual ~BehaviorBase(void);

	//������
	void Init(void);

	//���z�X�V
	virtual void Update(void) = 0;

	//�f�o�b�O�p�`��
	virtual void DebugDraw(void);

	void SetNowIndex(int index);
	int GetNowIndex(void)const;

	//�m�[�h�̏I������
	void EndNodeAction(int num);

protected:

	using NodesData = std::vector<std::unique_ptr<NodeBase>>;
	using EdgesData = std::vector<std::unique_ptr<Edge>>;

	//�m�[�h
	NodesData nodes_;
	//�G�b�W
	EdgesData edges_;

	//�m�[�h�T�����̌��ݒn
	int nowIndex_;
	//���݂���m�[�h�̃R�X�g
	float nowCost_;



	//�����ł̂ݎg�p����֐�
	void InitNode(void);
	void InitEdge(void);
	//���z�m�[�h�쐬
	virtual void CreateNode(void);
	//���z�m�[�h���G�b�W�Ō���
	virtual void ConnectNode(void);
	//�p����ŋ��ʂ��Ďg�p����֐�
	bool CheckNode(int index);
	bool CheckEdgeAll(void);
	void AddConditionNode(int index, std::function<int(void)> func);
	void AddActionNode(int index, std::function<void(void)> func);
	void AddConnectEdge(int fromIdx, int toIdx, float cost);
	int SearchNodeIdx(int index);
	int SearchEdgeIdx(int fromIdx, int toIdx);
	//�����𖞂����������m�[�h��
	//���[���ɂ̂Ƃ��Ď�̑I�����A���̃m�[�h�̃C���f�b�N�X��Ԃ�
	virtual int ConditionBranch(void);
	void NodeTransition(int index, int& Idx);
};