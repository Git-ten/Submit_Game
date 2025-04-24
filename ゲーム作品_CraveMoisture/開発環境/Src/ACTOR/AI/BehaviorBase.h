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

	// �R���X�g���N�^
	BehaviorBase(void);
	// ���z�f�X�g���N�^
	virtual ~BehaviorBase(void);

	// ������
	void Init(void);
	// �X�V
	void Update(void);


	// �A�N�V�����I����A��������AI�̎v�l���[�v�����b�N
	void LockThinkShortTime(void);
	// �A�N�V�����I����A����AI�̎v�l���[�v�����b�N
	void LockThinkLongTime(void);

	// ���S���b�N
	void PerfectLock(void);
	// ���S���b�N������
	void UnPerfectLock(void);
	
	// �A�N�V�������I������
	void EndAIAction(void);
	// �A�N�V�����̏I����Ԃ��擾
	bool IsEndAIAction(void)const;

private:

	// ���̃N���X�ł̂ݎg�p����f�[�^�^
	// �m�[�h���̘̂A�z�z��(key:�C���f�b�N�X)
	using NodesData = std::map<int, std::shared_ptr<Node>>;
	// �G�b�W���̘̂A�z�z��(key:�G�b�W��)
	using EdgesData = std::map<std::string, std::unique_ptr<Edge>>;

private:

	typedef int NODE_INDEX;

	//AI�̃A�N�V�������I��������
	bool isEndAIAction_;


	//AI�����b�N����
	bool IsLockThink(void)const;
	//���b�N���Ԃ̌v�Z����
	void LockTimeProcess(void);

protected:

	//�m�[�h
	NodesData nodes_;
	//�G�b�W
	EdgesData edges_;


	//AI�̃��b�N����
	//�v������AI����؎v�l���Ȃ�
	Time lockThinkTime_;
	// ���b�N���鎞��
	float lockTime_;

	// AI�̊��S���b�N
	// �֐��ŉ������Ȃ����肸���ƃ��b�N
	bool isPerfectLock_;


	//�m�[�h�T�����̌��ݒn�C���f�b�N�X
	NODE_INDEX nowIndex_;



	//�����ł̂ݎg�p����֐�
	void InitNode(void);
	void InitEdge(void);

	// ���ʐ��̍����X�V
	virtual void UpdateSpecial(void) = 0;

	//�m�[�h�쐬
	virtual void CreateNode(void) = 0;
	//�m�[�h���G�b�W�Ō���
	virtual void ConnectEdge(void) = 0;


	// �m�[�h�̒ǉ�
	void AddNode(int index, std::function<void(void)> func);
	// �G�b�W�̒ǉ�
	void AddEdge(const std::string& edgeName,
		int fromIndex, 
		const std::vector<int>& end,
		const std::vector<std::function<bool(void)>>& condition, 
		const std::vector<float>& weight);


	//�����𖞂����������m�[�h��
	//���[���ɂ̂Ƃ��Ď�̑I�����A���̃m�[�h�̃C���f�b�N�X��Ԃ�
	int TransitionNextNode(int fromIdx);
	void NodeTransition(void);



	//�A�N�V���������s
	void NodeAction(void);


	// �w�肵�����Ԃ���AI�̎v�l���[�v�����b�N
	void LockThink(const float time);

	// ���S���b�N���Ă��邩
	bool IsPerfectLock(void);

};