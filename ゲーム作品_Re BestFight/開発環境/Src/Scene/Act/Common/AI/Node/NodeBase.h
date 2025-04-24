#pragma once
class NodeBase
{
public:

	//�R���X�g���N�^
	NodeBase(int index);

	//���z�f�X�g���N�^
	virtual ~NodeBase(void);

	//���z�X�V
	virtual void Update(void);

	//�����m�[�h�Ŏg�p����
	virtual int IsCondition(void)const;
	//�A�N�V�����m�[�h�Ŏg�p����
	virtual bool IsEndAction(void)const;

	virtual void SetIsEndAction(bool flg);

	//�p����ŋ��ʂ��Ďg�p����֐�
	int GetIndex(void)const;
	void SetIndex(int index);
	bool IsEndNode(void)const;

protected:

	//���g�̃m�[�h�ԍ�
	int NodeIndex_;

	//���g���I�[�m�[�h��
	bool IsEndNode_;

};