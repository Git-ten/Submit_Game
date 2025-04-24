#pragma once

class Edge
{
public:

	//�R���X�g���N�^
	Edge(int from, int to, float cost);

	//�f�X�g���N�^
	~Edge(void);

	//�R�X�g���擾
	float GetCost(void)const;
	//�O�m�[�h�̃C���f�b�N�X���擾
	int GetPrevIndex(void)const;
	//���m�[�h�̃C���f�b�N�X���擾
	int GetNextIndex(void)const;

private:

	int idxFrom_;
	int idxTo_;

	float passCost_;

};